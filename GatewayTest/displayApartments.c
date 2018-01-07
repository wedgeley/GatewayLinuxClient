#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "../libGatewayClient/include/gway_api.h"
#include "constants.h"

//
//  Retrieves apartments for the specified entrance panel and outputs the list to stdout
//
bool DisplayApartments(const char* url, const char* entrancePanel)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    fprintf(stdout, "Fetching apartments for %s...\n", entrancePanel);

    // Allocate memory for a page of apartment entries from the stack
    int i;
    struct ApartmentEntry apartmententries[MAX_APARTMENTS];                // Allocates memory for the apartment entry structures

    // To pass into the function, convert to an array of pointers
    struct ApartmentEntry *buffer[MAX_APARTMENTS];
    for (i = 0 ; i < MAX_APARTMENTS ; i++) buffer[i] = &apartmententries[i];

    int apartmentCount = 0;

    status = FetchApartments(url, entrancePanel, buffer, MAX_APARTMENTS, &apartmentCount);

    if (IsSuccess(status))
    {
        fprintf(stdout, "%d apartment(s) returned\n", apartmentCount);
        for (i = 0 ; i < apartmentCount ; i++)
        {
            fprintf(stdout, "\t%d", buffer[i]->ApartmentNumber);
            if (buffer[i]->DivertToConcierge) fprintf(stdout, " (DivertToConcierge)");
            fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
    }

    if (!IsSuccess(status))
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "*** Failed to fetch apartments ***\n");
        fprintf(stderr, "\tError code %d - %s\n", status, ErrorDescription(status));
        return false;
    }

    return true;
}
