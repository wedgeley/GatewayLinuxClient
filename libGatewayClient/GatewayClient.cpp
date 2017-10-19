#include "GatewayClient.h"

GatewayClient::GatewayClient(const char* url)
{
    _url = url;

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    _pCurlHandle = curl_easy_init();
}

//
//  Returns serial number for the specified gateway
//
GatewayReturnCodes GatewayClient::LookupGatewaySerialNumber(std::string& serialNumber)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    Json::Value jsonRoot;
    status = PerformLookup(TIMEOUT_CONNECT_SECS, "Gateway", "Identify", jsonRoot);
    if (status == GWAY_SUCCESS)
    {
        serialNumber = jsonRoot.get("SerialNumber", "").asCString();
    }

    return status;
}

//
//  Performs a lookup and returns the resulting JSON value
//
GatewayReturnCodes GatewayClient::PerformLookup(long timeoutSecs, const char* controller, const char* action, Json::Value& jsonValue)
{
    GatewayReturnCodes status = GWAY_SUCCESS;
    CURLcode res;

    // Error text
    char errorMessage[CURL_ERROR_SIZE];
    curl_easy_setopt(_pCurlHandle, CURLOPT_ERRORBUFFER, errorMessage);

    // Specify URL to get
    char fullUrl[180];
    snprintf(fullUrl, 180, "%s/api/%s/%s", _url, controller, action);
    res = curl_easy_setopt(_pCurlHandle, CURLOPT_URL, fullUrl);

    // Timeout for this lookup
    if (res == CURLE_OK) res = curl_easy_setopt(_pCurlHandle, CURLOPT_TIMEOUT, timeoutSecs);

    // Send all data to the buffer function
    if (res == CURLE_OK) res = curl_easy_setopt(_pCurlHandle, CURLOPT_WRITEFUNCTION, _buffer.WriteMemoryCallback);
    if (res == CURLE_OK) res = curl_easy_setopt(_pCurlHandle, CURLOPT_WRITEDATA, (void *)&_buffer);

    // Some servers don't like requests that are made without a user-agent field, so we provide one
    if (res == CURLE_OK) res = curl_easy_setopt(_pCurlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // Perform the lookup
    if (res == CURLE_OK) res = curl_easy_perform(_pCurlHandle);

    // Check for errors
    if (res != CURLE_OK)
    {
        fprintf(stderr, "Error: Curl error code %d:\n", res);
        fprintf(stderr, "       \"%s\"\n", errorMessage);
        status = GWAY_SERVER_UNAVAILABLE;
    }

    if (IsSuccess(status))
    {
        // Decipher the block of json returned
        status = _buffer.ToJson(jsonValue);
    }

    return status;
}

GatewayClient::~GatewayClient()
{
    /* cleanup curl stuff */
    curl_easy_cleanup(_pCurlHandle);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();
}
