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
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
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
Json::Value JsonBuffer::AsJson()
{
    Json::Value jsonRoot;

    // Decipher the block of json returned
    Json::CharReaderBuilder builder;
    Json::CharReader* pReader = builder.newCharReader();
    bool parseOk = pReader->parse(_pMemory, _pMemory + _size, &jsonRoot, NULL);
    delete pReader;

    if (!parseOk) return NULL;

    return jsonRoot;
}

//
// Returns the buffer as a string
//
std::string JsonBuffer::AsString()
{
    std::string text = _pMemory;
    return text;
}

JsonBuffer::~JsonBuffer()
{
    free(_pMemory);
    _pMemory = NULL;
    _size = 0;
}
