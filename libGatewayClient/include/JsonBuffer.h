#ifndef JSONBUFFER_H
#define JSONBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../json/json.h"

class JsonBuffer
{
    public:
        JsonBuffer();
        virtual ~JsonBuffer();

        static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *jsonBuffer);

        Json::Value AsJson();
        std::string AsString();

    protected:

    private:
        char *_pMemory;
        size_t _size;
};

#endif // JSONBUFFER_H
