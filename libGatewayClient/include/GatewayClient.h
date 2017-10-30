#ifndef GATEWAYAPI_H
#define GATEWAYAPI_H

#include <string>
#include <curl/curl.h>
#include "MessageBuffer.h"
#include "gway_errors.h"
#include "Encryptor.h"

#define TIMEOUT_CONNECT_SECS    10         // Timeout for initial connection
#define TIMEOUT_DATA_SECS       10 * 60    // Timeout for data lookup

// String comparison function used for parameter map
struct cmp_str
{
   bool operator()(const char *a, const char *b)
   {
      return std::strcmp(a, b) < 0;
   }
};

class GatewayClient
{
    public:
        GatewayClient(const char* url);
        virtual ~GatewayClient();

        GatewayReturnCodes LookupGatewaySerialNumber(std::string& serialNumber);
        GatewayReturnCodes FetchPageOfKeys(const char* controllerSerialNumber, const char* lastKeycodeOnPreviousPage, int pageSize, std::vector<std::string>& keycodes);

    protected:

    private:
        GatewayReturnCodes PerformLookup(long timeoutSecs, const char* controller, const char* action, std::map<const char*, const char*, cmp_str> parameters, Json::Value& jsonValue);
        GatewayReturnCodes PerformLookup(long timeoutSecs, const char* controller, const char* action, Json::Value& jsonRoot);
        GatewayReturnCodes PerformLookup(long timeoutSecs, const char* url, Json::Value& jsonRoot);
        CURL* _pCurlHandle;
        MessageBuffer _buffer;
        Encryptor _encryptor;
        const char* _url;

};

#endif // GATEWAYAPI_H
