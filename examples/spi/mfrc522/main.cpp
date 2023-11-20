
// make && ./app

#include <unistd.h>

#include "mfrc522.h"

// rst  -> 3v3 or 27
// sda  -> spi.cs1  (16)
// mosi -> spi.mosi (11)
// miso -> spi.miso (12)
// scl  -> spi.sclk (14)
// irq  -> x

int main()
{
    MFRC522 mfrc;

    mfrc.PCD_Init();

    while (1)
    {
        // Look for a card
        if (!mfrc.PICC_IsNewCardPresent())
        {
            continue;
        }

        if (!mfrc.PICC_ReadCardSerial())
        {
            continue;
        }

        // Print UID
        for (byte i = 0; i < mfrc.uid.size; ++i)
        {
            if (mfrc.uid.uidByte[i] < 0x10)
            {
                printf(" 0");
                printf("%X", mfrc.uid.uidByte[i]);
            }
            else
            {
                printf(" ");
                printf("%X", mfrc.uid.uidByte[i]);
            }
        }
        printf("\n");
        delay(1000);
    }
    return 0;
}
