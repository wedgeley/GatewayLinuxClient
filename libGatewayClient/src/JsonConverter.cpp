#include "JsonConverter.h"

//
// Returns the buffer as a JSON object
// (static)
//
GatewayReturnCodes JsonConverter::ToJson(std::string& buffer, Json::Value& jsonRoot)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    // Decipher the block of json returned
    Json::CharReaderBuilder builder;
    Json::CharReader* pReader = builder.newCharReader();
    bool parseOk = pReader->parse(buffer.c_str(), buffer.c_str() + buffer.length(), &jsonRoot, NULL);
    delete pReader;

    if (!parseOk)
    {
        status = GWAY_JSON_PARSE_ERROR;
        fprintf(stderr, "Error: Failed to parse JSON response\n");
    }

    return status;
}
