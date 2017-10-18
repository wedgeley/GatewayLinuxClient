#include <stdio.h>
#include <stdlib.h>

#include "../libGatewayClient/include/libGateway_C_API.h"

int main()
{
    int i1 = 12;
    int i2 = 13;
    int sum = SampleAddInt(i1, i2);

    printf("%d + %d = %d\n", i1, i2, sum);

    char serialNumber[80];
    LookupGatewaySerialNumber("http://192.168.2.3:8080", serialNumber, 80);
    printf("Serial number is %s\n", serialNumber);

    return 0;
}
