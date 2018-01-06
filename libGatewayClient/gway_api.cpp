#include <string>
#include <pthread.h>
#include "gway_api.h"
#include "GatewayClient.h"
#include "UDPListener.h"


// Key update broadcasts come in on UDP
UDPListener keyUpdateUDPListener;

// UDP port to listen to for key update notifications
const int KEY_UPDATES_UDP_PORT = 62000;

// UDP listener thread and function
pthread_t udpThread;
void *OnRunUDPListener(void *ptr);

//
// Requests serial number from the Gateway
// Writes the serial number into the supplied buffer
//
extern "C"
GatewayReturnCodes LookupGatewaySerialNumber(const char* url, char* buffer, size_t bufferLength)
{
    GatewayReturnCodes status = GWAY_SUCCESS;
    GatewayClient client(url);

    std::string serialNumber;
    status = client.LookupGatewaySerialNumber(serialNumber);

    if (IsSuccess(status) && serialNumber.length() > (bufferLength - 1))
    {
        // Insufficient room for the serial number
        fprintf(stderr, "Error: Buffer of size %d characters too small for serial number of length %d\n", bufferLength, serialNumber.length());
        status = GWAY_BUFFER_TOO_SMALL;
    }

    if (IsSuccess(status))
    {
        strcpy(buffer, serialNumber.c_str());
    }

    return status;
}


//
// Requests the time that the Gateway last sync-ed up to the cloud
//
extern "C"
GatewayReturnCodes LookupGatewayLastCloudSyncTime(const char* url, unsigned long long* utcTicks)
{
    GatewayClient client(url);
    return client.LookupGatewayLastSyncTime(utcTicks);
}


//
// Requests all keys for the controller with the specified serial number
//
extern "C"
GatewayReturnCodes FetchPageOfKeys(
    const char* url,
    const char* controllerSerialNumber,
    const char* inPageMarker,
    char* outPageMarker,
    struct KeyEntry* buffer[],
    size_t keyCodeSize,
    uint pageSize,
    int* returnedKeyCount)
{
    GatewayReturnCodes status = GWAY_SUCCESS;
    *returnedKeyCount = 0;
    GatewayClient client(url);

    std::vector<KeyEntryItem> keyentries;
    std::string outPageMarkerStr;
    status = client.FetchPageOfKeys(controllerSerialNumber, inPageMarker, pageSize, keyentries, outPageMarkerStr);

    if (IsSuccess(status))
    {
        // Copy key codes into the supplied buffer
        *returnedKeyCount = keyentries.size();
        for (uint i=0 ; i < keyentries.size(); i++)
        {
            if (keyentries[i].KeyCode.length() > (keyCodeSize - 1))
            {
                status = GWAY_BUFFER_TOO_SMALL;
            }
            else
            {
                strcpy(buffer[i]->KeyCode, keyentries[i].KeyCode.c_str());
                buffer[i]->Tagged = keyentries[i].Tagged;
            }
        }
    }

    if (IsSuccess(status) && *returnedKeyCount > 0)
    {
        // Return the page marker for the next page
        strcpy(outPageMarker, outPageMarkerStr.substr(0, 39).c_str());
    }

    return status;
}


//
//  Fetches key updates for the controller with the specified serial number
//  Provide the time of the last update received in timeOfLastUpdate.  This is UTC ticks
//
extern "C"
GatewayReturnCodes FetchKeyUpdates(
    const char* url,
    const char* controllerSerialNumber,
    unsigned long long timeOfLastUpdate,
    struct KeyUpdate* buffer[],
    size_t keyCodeSize,
    uint pageSize,
    int* returnedKeyCount)
{
    GatewayReturnCodes status = GWAY_SUCCESS;
    *returnedKeyCount = 0;
    GatewayClient client(url);

    std::vector<KeyUpdateItem> updates;
    status = client.FetchKeyUpdates(controllerSerialNumber, timeOfLastUpdate, pageSize, updates);

    if (IsSuccess(status))
    {
        *returnedKeyCount = updates.size();
        for (uint i=0 ; i < updates.size(); i++)
        {
            if (updates[i].KeyCode.length() > (keyCodeSize - 1))
            {
                status = GWAY_BUFFER_TOO_SMALL;
            }
            else
            {
                strcpy(buffer[i]->KeyCode, updates[i].KeyCode.c_str());
                buffer[i]->Active = updates[i].Active;
                buffer[i]->Tagged = updates[i].Tagged;
                buffer[i]->UtcTicks = updates[i].UtcTicks;
            }
        }
    }

    return status;
}


//
//  Start listening for UDP key update messages
//  The supplied function will be called when key updates are available
//
extern "C"
GatewayReturnCodes RegisterForUpdateNotification(
    void (*functionPtr)())
{
    // Create a thread for the UDP listener
    int iret = pthread_create(&udpThread, NULL, OnRunUDPListener, (void*) functionPtr);
    if(iret)
    {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
        return GWAY_PTHREAD_ERROR;
    }

    return GWAY_SUCCESS;
}


//
//  Runs UDP listener on thread
//
void *OnRunUDPListener(void *ptr)
{
    void (*functionPtr)();
    functionPtr = (void (*)())ptr;
    GatewayReturnCodes status = keyUpdateUDPListener.Listen(KEY_UPDATES_UDP_PORT, functionPtr);

    if (!IsSuccess(status))
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "*** Failed to run UDP listener ***\n");
        fprintf(stderr, "\tError code %d - %s\n", status, ErrorDescription(status));
    }

    return NULL;
}


//
//  Stops listening for update notifications
//
extern "C"
GatewayReturnCodes CancelUpdateNotification()
{
    // Tel UDP listener to stop and wait for the thread to die
    keyUpdateUDPListener.Stop();
    pthread_join(udpThread, NULL);

    return GWAY_SUCCESS;
}
