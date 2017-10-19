#include "gway_errors.h"

//
//  Determines if the error code is a success code
//
extern "C"
bool IsSuccess(GatewayReturnCodes code)
{
    return code == GWAY_SUCCESS;
}

//
//  Returns text description for an error code
//
extern "C"
const char* ErrorDescription(GatewayReturnCodes code)
{
    switch(code)
    {
        case GWAY_SUCCESS:              return "Success";
        case GWAY_BUFFER_TOO_SMALL:     return "The buffer passed into the function was not big enough for the result";
        case GWAY_SERVER_UNAVAILABLE:   return "Failed to contact Gateway";
        case GWAY_OUT_OF_MEMORY:        return "Memory allocation failure";
        case GWAY_JSON_PARSE_ERROR:     return "Failed to parse JSON returned from lookup";
        case GWAY_NO_DATA_RETURNED:     return "No data returned from lookup";
        default:                        return "Unknown error";
    }
}
