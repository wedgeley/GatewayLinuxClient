#include "MessageBuffer.h"
#include "gway_errors.h"

MessageBuffer::MessageBuffer()
{
    /* will be grown as needed */
    BufferPtr = (char*)malloc(1);
    BufferSize = 0;
}

//
//  Callback function to load this buffer
//
size_t MessageBuffer::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *jsonBuffer)
{
    size_t realsize = size * nmemb;
    MessageBuffer* pBuffer = (MessageBuffer*)jsonBuffer;

    pBuffer->BufferPtr = (char*)realloc(pBuffer->BufferPtr, pBuffer->BufferSize + realsize + 1);
    if(pBuffer->BufferPtr == NULL)
    {
        // Out of memory
        fprintf( stderr, "Error: Failed to allocate %d bytes for JSON buffer\n", realsize);
        return 0;
    }

    memcpy(&(pBuffer->BufferPtr[pBuffer->BufferSize]), contents, realsize);
    pBuffer->BufferSize += realsize;
    pBuffer->BufferPtr[pBuffer->BufferSize] = 0;

    return realsize;
}

//
// Returns the buffer as a string
//
GatewayReturnCodes MessageBuffer::ToString(std::string& text)
{
    GatewayReturnCodes status = CheckBuffer();

    if (IsSuccess(status))
    {
        text = BufferPtr;
    }

    return status;
}

//
// Checks status of memory buffer and returns appropriate status code
//
GatewayReturnCodes MessageBuffer::CheckBuffer()
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    if (BufferPtr == NULL)
    {
        status = GWAY_OUT_OF_MEMORY;
    }

    if (IsSuccess(status) && BufferSize == 0)
    {
        status = GWAY_NO_DATA_RETURNED;
    }

    return status;
}

MessageBuffer::~MessageBuffer()
{
    free(BufferPtr);
    BufferPtr = NULL;
    BufferSize = 0;
}
