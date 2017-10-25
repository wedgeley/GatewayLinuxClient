#ifndef MESSAGEBUFFER_H
#define MESSAGEBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gway_errors.h"
#include "../json/json.h"

class MessageBuffer
{
    public:
        MessageBuffer();
        virtual ~MessageBuffer();

        static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *jsonBuffer);

        GatewayReturnCodes CheckBuffer();
        GatewayReturnCodes ToString(std::string& text);

        char *BufferPtr;
        size_t BufferSize;

    protected:

    private:
};

#endif // MESSAGEBUFFER_H
