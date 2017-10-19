#include "JsonBuffer.h"
#include "gway_errors.h"

JsonBuffer::JsonBuffer()
{
    /* will be grown as needed */
    _pMemory = (char*)malloc(1);
    _size = 0;
}

//
//  Callback function to load this buffer
//
size_t JsonBuffer::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *jsonBuffer)
{
    size_t realsize = size * nmemb;
    JsonBuffer* pBuffer = (JsonBuffer*)jsonBuffer;

    pBuffer->_pMemory = (char*)realloc(pBuffer->_pMemory, pBuffer->_size + realsize + 1);
    if(pBuffer->_pMemory == NULL)
    {
        // Out of memory
        fprintf( stderr, "Error: Failed to allocate %d bytes for JSON buffer\n", realsize);
        return 0;
    }

    memcpy(&(pBuffer->_pMemory[pBuffer->_size]), contents, realsize);
    pBuffer->_size += realsize;
    pBuffer->_pMemory[pBuffer->_size] = 0;

    return realsize;
}

//
// Returns the buffer as a JSON object
//
GatewayReturnCodes JsonBuffer::ToJson(Json::Value& jsonRoot)
{
    GatewayReturnCodes status = CheckBuffer();

    if (IsSuccess(status))
    {
        // Decipher the block of json returned
        Json::CharReaderBuilder builder;
        Json::CharReader* pReader = builder.newCharReader();
        bool parseOk = pReader->parse(_pMemory, _pMemory + _size, &jsonRoot, NULL);
        delete pReader;

        if (!parseOk)
        {
            status = GWAY_JSON_PARSE_ERROR;

            // Send response to stderr
            std::string text = "";
            ToString(text);
            fprintf(stderr, "Error: Failed to parse JSON response\n");
            fprintf(stderr, "%s\n", text.c_str());
        }
    }

    return status;
}

//
// Returns the buffer as a string
//
GatewayReturnCodes JsonBuffer::ToString(std::string& text)
{
    GatewayReturnCodes status = CheckBuffer();

    if (IsSuccess(status))
    {
        text = _pMemory;
    }

    return status;
}

//
// Checks status of memory buffer and returns appropriate status code
//
GatewayReturnCodes JsonBuffer::CheckBuffer()
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    if (_pMemory == NULL)
    {
        status = GWAY_OUT_OF_MEMORY;
    }

    if (IsSuccess(status) && _size == 0)
    {
        status = GWAY_NO_DATA_RETURNED;
    }

    return status;
}

JsonBuffer::~JsonBuffer()
{
    free(_pMemory);
    _pMemory = NULL;
    _size = 0;
}
