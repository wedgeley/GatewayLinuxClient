#ifndef GATEWAYAPI_H
#define GATEWAYAPI_H

#include <string>
#include <curl/curl.h>
#include "JsonBuffer.h"
#include "gway_errors.h"

class GatewayAPI
{
    public:
        GatewayAPI();
        virtual ~GatewayAPI();

        GatewayReturnCodes LookupGatewaySerialNumber(const char* url, std::string& serialNumber);

    protected:

    private:
        GatewayReturnCodes PerformLookup(const char* url, const char* controller, const char* action, Json::Value& jsonRoot);

        CURL* _pCurlHandle;
        JsonBuffer _buffer;

};

#endif // GATEWAYAPI_H
