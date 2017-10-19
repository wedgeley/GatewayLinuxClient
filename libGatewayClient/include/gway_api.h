#ifndef EXTERNALAPI_H_INCLUDED
#define EXTERNALAPI_H_INCLUDED

#include "gway_errors.h"

#ifdef __cplusplus
extern "C"
#endif // __cplusplus
GatewayReturnCodes LookupGatewaySerialNumber(const char* url, char* buffer, uint length);

#endif // EXTERNALAPI_H_INCLUDED