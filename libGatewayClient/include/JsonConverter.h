#ifndef JSONCONVERTER_H_INCLUDED
#define JSONCONVERTER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gway_errors.h"
#include "../json/json.h"

class JsonConverter
{
    public:
        static GatewayReturnCodes ToJson(std::string& buffer, Json::Value& jsonRoot);

};

#endif // JSONCONVERTER_H_INCLUDED
