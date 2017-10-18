#ifndef GATEWAYAPI_H
#define GATEWAYAPI_H

#include <string>
#include <curl/curl.h>
#include "JsonBuffer.h"

class GatewayAPI
{
    public:
        GatewayAPI();
        virtual ~GatewayAPI();

        std::string LookupGatewaySerialNumber(const char* url);

    protected:

    private:
        Json::Value PerformLookup(const char* url, const char* controller, const char* action);

        CURL* _pCurlHandle;
        JsonBuffer _buffer;

};

#endif // GATEWAYAPI_H
