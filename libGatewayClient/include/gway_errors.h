#ifndef GWAY_ERRORS_H_INCLUDED
#define GWAY_ERRORS_H_INCLUDED

//
//  Return codes for all API functions
//
typedef enum
{
    GWAY_SUCCESS = 1,
    GWAY_BUFFER_TOO_SMALL = 2,          // The buffer passed into the function was not big enough for the result
    GWAY_SERVER_UNAVAILABLE = 3,        // Failed to contact Gateway
    GWAY_OUT_OF_MEMORY = 4,             // Memory allocation failure
    GWAY_JSON_PARSE_ERROR = 5,          // Failed to parse returned JSON
    GWAY_NO_DATA_RETURNED = 6           // No data returned from lookup
}
GatewayReturnCodes;

//
//  Determines if the error code is a success code
//
#ifdef __cplusplus
extern "C"
#endif // __cplusplus
bool IsSuccess(GatewayReturnCodes code);


#endif // GWAY_ERRORS_H_INCLUDED
