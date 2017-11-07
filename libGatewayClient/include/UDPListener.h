#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include "gway_errors.h"


class UDPListener
{
    public:
        UDPListener();
        GatewayReturnCodes Listen(int port, void (*functionPtr)());
        void Stop();
        virtual ~UDPListener();

    protected:

    private:
        bool _listening;
};

#endif // UDPLISTENER_H
