#include "Encryptor.h"
#include "../aes/Rijndael.h"
#include "../base64/base64.h"

//
//  Decrypts a text string
//
GatewayReturnCodes Encryptor::Decrypt(const char* encrypted, int encryptedSize, std::string& decryptedStr)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    string encryptedStr = base64_decode(encrypted, encryptedSize);

    CRijndael oRijndael;
    oRijndael.MakeKey(Key, IV, 16, 16);

    // TODO: Change this buffer size to be dynamic
    char decrypted[257];
    memset(decrypted, 0, 257);
    oRijndael.Decrypt(encryptedStr.c_str(), decrypted, 256, CRijndael::CBC);

    decryptedStr = decrypted;

    return status;
}
