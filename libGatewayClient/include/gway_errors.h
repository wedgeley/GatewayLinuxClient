#ifndef GWAY_ERRORS_H_INCLUDED
#define GWAY_ERRORS_H_INCLUDED

//
//  Return codes for all API functions
//
typedef enum
{
    GWAY_SUCCESS = 1,
    GWAY_BUFFER_TOO_SMALL = 2,
    GWAY_SERVER_UNAVAILABLE = 3
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
