#ifndef EXTERNALAPI_H_INCLUDED
#define EXTERNALAPI_H_INCLUDED

#include "gway_errors.h"

#ifdef __cplusplus
extern "C"
#endif // __cplusplus
GatewayReturnCodes LookupGatewaySerialNumber(const char* url, char* buffer, size_t bufferLength);

#ifdef __cplusplus
extern "C"
#endif // __cplusplus
GatewayReturnCodes ListAllKeys(const char* url, const char* controllerSerialNumber, char* buffer[], size_t keycodeSize, uint numberOfKeycodes, int* returnedKeyCount);

#endif // EXTERNALAPI_H_INCLUDED
