#include "gway_errors.h"

//
//  Determines if the error code is a success code
//
extern "C"
bool IsSuccess(GatewayReturnCodes code)
{
    return code == GWAY_SUCCESS;
}
