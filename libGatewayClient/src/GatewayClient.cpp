#include "GatewayClient.h"
#include "JsonConverter.h"


GatewayClient::GatewayClient(const char* url)
{
    _url = url;

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    _pCurlHandle = curl_easy_init();
}

//
//  Fetches serial number for the gateway
//
GatewayReturnCodes GatewayClient::LookupGatewaySerialNumber(std::string& serialNumber)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    Json::Value jsonRoot;
    status = PerformLookup(TIMEOUT_CONNECT_SECS, "Gateway", "Identify", jsonRoot);
    if (status == GWAY_SUCCESS)
    {
        serialNumber = jsonRoot.get("SerialNumber", "").asString();
    }

    return status;
}

//
//  Fetches all keys for the controller with the specified serial number
//
GatewayReturnCodes GatewayClient::FetchPageOfKeys(const char* controllerSerialNumber, const char* lastKeycodeOnPreviousPage, int pageSize, std::vector<std::string>& keycodes)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    char pageSizeStr[6];
    sprintf(pageSizeStr, "%d", pageSize);

    std::vector<ParameterValue> parameters;
    ParameterValue parameter1("ControllerSerialNumber", controllerSerialNumber);
    parameters.push_back(parameter1);
    ParameterValue parameter2("LastKeycodeOnPreviousPage", lastKeycodeOnPreviousPage);
    parameters.push_back(parameter2);
    ParameterValue parameter3("PageSize", pageSizeStr);
    parameters.push_back(parameter3);

    Json::Value jsonRoot;
    status = PerformLookup(TIMEOUT_DATA_SECS, "Key", "Page", parameters, jsonRoot);
    if (status == GWAY_SUCCESS)
    {
        if (jsonRoot.isArray())
        {
            for (uint i=0 ; i < jsonRoot.size(); i++)
            {
                keycodes.push_back(jsonRoot[i].get("Keycode", "").asString());
            }
        }
    }

    return status;
}


//
//  Fetches key updates for the controller with the specified serial number
//  Provide the time of the last update received in timeOfLastUpdate.  This is UTC ticks
//
GatewayReturnCodes GatewayClient::FetchKeyUpdates(const char* controllerSerialNumber, unsigned long long timeOfLastUpdate, int pageSize, std::vector<std::string>& keycodes)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    char utcTicksStr[24];
    sprintf(utcTicksStr, "%llu", timeOfLastUpdate);

    char pageSizeStr[6];
    sprintf(pageSizeStr, "%d", pageSize);

    std::vector<ParameterValue> parameters;
    ParameterValue parameter1("ControllerSerialNumber", controllerSerialNumber);
    parameters.push_back(parameter1);
    ParameterValue parameter2("UtcTicks", utcTicksStr);
    parameters.push_back(parameter2);
    ParameterValue parameter3("PageSize", pageSizeStr);
    parameters.push_back(parameter3);

    Json::Value jsonRoot;
    status = PerformLookup(TIMEOUT_DATA_SECS, "Key", "Updates", parameters, jsonRoot);
    if (status == GWAY_SUCCESS)
    {
        if (jsonRoot.isArray())
        {
            for (uint i=0 ; i < jsonRoot.size(); i++)
            {
                keycodes.push_back(jsonRoot[i].get("Keycode", "").asString());
            }
        }
    }

    return status;
}


//
//  Performs a lookup and returns the resulting JSON value
//  This overload takes a map of parameter
//
GatewayReturnCodes GatewayClient::PerformLookup(
    long timeoutSecs, const char* controller, const char* action, std::vector<ParameterValue> parameters, Json::Value& jsonValue)
{
    // Use std::string to build up the full url
    std::string fullUrl = _url;
    fullUrl += "/api/";
    fullUrl += controller;
    fullUrl += "/";
    fullUrl += action;
    fullUrl += "?";

    // Parameters
    std::vector<ParameterValue>::iterator it;
    for ( it = parameters.begin(); it != parameters.end(); it++ )
    {
        fullUrl += it->Name;
        fullUrl += "=";
        fullUrl += it->Value;
        fullUrl += "&";
    }
    fullUrl = fullUrl.substr(0, fullUrl.length() - 1);      // Remove trailing "&"

    // Specify URL to get
    return PerformLookup(timeoutSecs, fullUrl.c_str(), jsonValue);
}

//
//  Performs a lookup and returns the resulting JSON value
//  This overload takes no parameters
//
GatewayReturnCodes GatewayClient::PerformLookup(long timeoutSecs, const char* controller, const char* action, Json::Value& jsonValue)
{
    // Specify URL to get
    char fullUrl[180];
    snprintf(fullUrl, 180, "%s/api/%s/%s", _url, controller, action);
    return PerformLookup(timeoutSecs, fullUrl, jsonValue);
}


//
//  Performs a lookup and returns the resulting JSON value
//  This overload takes the request url
//
GatewayReturnCodes GatewayClient::PerformLookup(long timeoutSecs, const char* url, Json::Value& jsonValue)
{
    GatewayReturnCodes status = GWAY_SUCCESS;
    CURLcode res;

    // Error text
    char errorMessage[CURL_ERROR_SIZE];
    curl_easy_setopt(_pCurlHandle, CURLOPT_ERRORBUFFER, errorMessage);

    res = curl_easy_setopt(_pCurlHandle, CURLOPT_URL, url);

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
        // Verify that the buffer is valid
        status = _buffer.CheckBuffer();
    }

    std::string messageBody;
    if (IsSuccess(status))
    {
        // Decrypt the data
        status = _encryptor.Decrypt(_buffer.BufferPtr, _buffer.BufferSize, messageBody);
    }

    if (IsSuccess(status))
    {
        // Decipher the block of json returned
        status = JsonConverter::ToJson(messageBody, jsonValue);
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
