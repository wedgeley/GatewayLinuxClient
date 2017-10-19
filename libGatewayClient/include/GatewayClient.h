#ifndef GATEWAYAPI_H
#define GATEWAYAPI_H

#include <string>
#include <curl/curl.h>
#include "JsonBuffer.h"
#include "gway_errors.h"

#define TIMEOUT_CONNECT_SECS    10         // Timeout for initial connection
#define TIMEOUT_DATA_SECS       10 * 60    // Timeout for data lookup

class GatewayClient
{
    public:
        GatewayClient(const char* url);
        virtual ~GatewayClient();

        GatewayReturnCodes LookupGatewaySerialNumber(std::string& serialNumber);

    protected:

    private:
        GatewayReturnCodes PerformLookup(long timeoutSecs, const char* controller, const char* action, Json::Value& jsonRoot);

        CURL* _pCurlHandle;
        JsonBuffer _buffer;
        const char* _url;

};

#endif // GATEWAYAPI_H
