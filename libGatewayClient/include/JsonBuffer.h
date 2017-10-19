#ifndef JSONBUFFER_H
#define JSONBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gway_errors.h"
#include "../json/json.h"

class JsonBuffer
{
    public:
        JsonBuffer();
        virtual ~JsonBuffer();

        static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *jsonBuffer);

        GatewayReturnCodes ToJson(Json::Value& jsonRoot);
        GatewayReturnCodes ToString(std::string& text);

    protected:

    private:
        char *_pMemory;
        size_t _size;

        GatewayReturnCodes CheckBuffer();
};

#endif // JSONBUFFER_H
