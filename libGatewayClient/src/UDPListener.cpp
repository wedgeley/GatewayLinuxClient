#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "UDPListener.h"

UDPListener::UDPListener()
{
    _listening = false;
}

//
//  Listen on the specified UDP port and calls the supplied function when a message is received
//  Returns FALSE if any error
//
GatewayReturnCodes UDPListener::Listen(int port, void (*functionPtr)())
{
    sockaddr_in si_me, si_other;
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0)
    {
        fprintf(stderr, "Error: Failed to create UDP socket\n");
        return GWAY_UDP_ERROR;
    }

    int broadcast = 1;
    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) < 0)
    {
        fprintf(stderr, "Error: Failed to set UDP broadcast option\n");
        return GWAY_UDP_ERROR;
    }

    // Time out the UDP listen after 5 seconds.  This allows us to respond to STOP requests
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        fprintf(stderr, "Error: Failed to set UDP timeout option\n");
        return GWAY_UDP_ERROR;
    }

    memset(&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = INADDR_ANY;

    if (::bind(s, (sockaddr *)&si_me, sizeof(sockaddr)) < 0)
    {
        fprintf(stderr, "Error: Failed to bind UDP socket\n");
        return GWAY_UDP_ERROR;
    }

    _listening = true;
    char buf[100];
    while(_listening)
    {
        unsigned slen = sizeof(sockaddr);
        if (recvfrom(s, buf, sizeof(buf)-1, 0, (sockaddr *)&si_other, &slen) > 0)
        {
            (*functionPtr)();
        }
    }

    return GWAY_SUCCESS;
}

//
//  Stop listening
//
void UDPListener::Stop()
{
    _listening = false;
}

UDPListener::~UDPListener()
{
    //dtor
}
