#include "GatewayAPI.h"

GatewayAPI::GatewayAPI()
{
    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    _pCurlHandle = curl_easy_init();
}

//
//  Returns serial number for the specified gateway
//
GatewayReturnCodes GatewayAPI::LookupGatewaySerialNumber(const char* url, std::string& serialNumber)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    Json::Value jsonRoot;
    status = PerformLookup(url, "Gateway", "Identify", jsonRoot);
    if (status == GWAY_SUCCESS)
    {
        serialNumber = jsonRoot.get("SerialNumber", "").asCString();
    }

    return status;
}

//
//  Performs a lookup and returns the resulting JSON value
//
GatewayReturnCodes GatewayAPI::PerformLookup(const char* url, const char* controller, const char* action, Json::Value& jsonValue)
{
    CURLcode res;

    /* Specify URL to get */
    char fullUrl[180];
    snprintf(fullUrl, 180, "%s/api/%s/%s", url, controller, action);
    printf("url is [%s]\n", fullUrl);


    curl_easy_setopt(_pCurlHandle, CURLOPT_URL, fullUrl);
    /* Send all data to this function and pass it our buffer  */
    curl_easy_setopt(_pCurlHandle, CURLOPT_WRITEFUNCTION, _buffer.WriteMemoryCallback);
    curl_easy_setopt(_pCurlHandle, CURLOPT_WRITEDATA, (void *)&_buffer);

    /* Some servers don't like requests that are made without a user-agent field, so we provide one */
    curl_easy_setopt(_pCurlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* Perform the lookup */
    res = curl_easy_perform(_pCurlHandle);

    /* Check for errors */
    if (res != CURLE_OK) return GWAY_SERVER_UNAVAILABLE;

    // Decipher the block of json returned
    // printf("Received:\n%s\n", _buffer.AsString().c_str());
    jsonValue = _buffer.AsJson();
    return GWAY_SUCCESS;
}

GatewayAPI::~GatewayAPI()
{
    /* cleanup curl stuff */
    curl_easy_cleanup(_pCurlHandle);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();
}
