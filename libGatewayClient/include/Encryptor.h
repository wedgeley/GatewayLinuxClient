#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <string>
#include "gway_errors.h"

class Encryptor
{
    public:
        Encryptor()
        {
            Key[0] = 97;
            Key[1] = 98;
            Key[2] = 51;
            Key[3] = 54;
            Key[4] = 110;
            Key[5] = 114;
            Key[6] = 55;
            Key[7] = 48;
            Key[8] = 117;
            Key[9] = 119;
            Key[10] = 110;
            Key[11] = 77;
            Key[12] = 104;
            Key[13] = 115;
            Key[14] = 71;
            Key[15] = 83;

            IV[0] = 85;
            IV[1] = 103;
            IV[2] = 110;
            IV[3] = 77;
            IV[4] = 104;
            IV[5] = 115;
            IV[6] = 71;
            IV[7] = 83;
            IV[8] = 86;
            IV[9] = 97;
            IV[10] = 112;
            IV[11] = 77;
            IV[12] = 83;
            IV[13] = 51;
            IV[14] = 51;
            IV[15] = 97;
        }

        GatewayReturnCodes Decrypt(const char* encrypted, int encryptedSize, std::string& decrypted);

    protected:

    private:
        char Key[16];
        char IV[16];
};

#endif // ENCRYPTOR_H
