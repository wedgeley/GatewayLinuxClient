#ifndef GWAY_ERRORS_H_INCLUDED
#define GWAY_ERRORS_H_INCLUDED

//
//  Return codes for all API functions
//  Keep ErrorDescription() function up to date
//
typedef enum
{
    GWAY_SUCCESS = 1,                   // Success
    GWAY_BUFFER_TOO_SMALL = 2,          // The buffer passed into the function was not big enough for the result
    GWAY_ARRAY_TOO_SMALL = 3,           // The array passed into the function was not big enough for the result
    GWAY_SERVER_UNAVAILABLE = 4,        // Failed to contact Gateway
    GWAY_OUT_OF_MEMORY = 5,             // Memory allocation failure
    GWAY_JSON_PARSE_ERROR = 6,          // Failed to parse returned JSON
    GWAY_NO_DATA_RETURNED = 7,          // No data returned from lookup
    GWAY_LOOKUP_FAILED = 8,             // Server returned error response
    GWAY_UDP_ERROR = 9,                 // Error setting up UDP socket
    GWAY_PTHREAD_ERROR = 10,            // Error setting up pthreads
    GWAY_ERROR_CODES_END                // Do not use
}
GatewayReturnCodes;

//
//  Determines if the error code is a success code
//
#ifdef __cplusplus
extern "C"
#endif // __cplusplus
bool IsSuccess(GatewayReturnCodes code);

//
//  Returns text description for an error code
//
#ifdef __cplusplus
extern "C"
#endif // __cplusplus
const char* ErrorDescription(GatewayReturnCodes code);

#endif // GWAY_ERRORS_H_INCLUDED
