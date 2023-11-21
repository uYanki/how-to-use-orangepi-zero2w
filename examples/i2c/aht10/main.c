
#include <stdio.h>
#include "aht10.h"

int main()
{
    aht10_init("/dev/i2c-2");
    float temp, hum;
    while (1)
    {
        temp = read_temperature();
        hum  = read_humidity();
        printf("\rT: %.2f C, H: %.2f %%", temp, hum);
        fflush(stdout);
        delay(1000);
    }

    return 0;
}
