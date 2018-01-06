#ifndef GATEWAYAPI_H
#define GATEWAYAPI_H

#include <string>
#include <curl/curl.h>
#include "MessageBuffer.h"
#include "gway_errors.h"
#include "Encryptor.h"
#include "Packets.h"

#define TIMEOUT_CONNECT_SECS    10         // Timeout for initial connection
#define TIMEOUT_DATA_SECS       10 * 60    // Timeout for data lookup

// Parameter definition
struct ParameterValue
{
    ParameterValue(const char* name, const char* value)
    {
        Name = name;
        Value = value;
    }

    const char* Name;
    const char* Value;
};

class GatewayClient
{
    public:
        GatewayClient(const char* url);
        virtual ~GatewayClient();

        GatewayReturnCodes LookupGatewaySerialNumber(std::string& serialNumber);
        GatewayReturnCodes LookupGatewayLastSyncTime(unsigned long long* utcTicks);
        GatewayReturnCodes FetchPageOfKeys(const char* controllerSerialNumber, const char* inPageMarker, int pageSize, std::vector<KeyEntryItem>& keyentries, std::string& outPageMarker);
        GatewayReturnCodes FetchKeyUpdates(const char* controllerSerialNumber, unsigned long long timeOfLastUpdate, int pageSize, std::vector<KeyUpdateItem>& updates);

    protected:

    private:
        GatewayReturnCodes PerformLookup(long timeoutSecs, const char* controller, const char* action, std::vector<ParameterValue> parameters, Json::Value& jsonValue);
        GatewayReturnCodes PerformLookup(long timeoutSecs, const char* controller, const char* action, Json::Value& jsonRoot);
        GatewayReturnCodes PerformLookup(long timeoutSecs, const char* url, Json::Value& jsonRoot);
        void ExtractError();

        CURL* _pCurlHandle;
        MessageBuffer _buffer;
        Encryptor _encryptor;
        const char* _url;

};

#endif // GATEWAYAPI_H
