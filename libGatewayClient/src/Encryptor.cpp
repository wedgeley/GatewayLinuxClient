#include "Encryptor.h"
#include "../aes/Rijndael.h"
#include "../base64/base64.h"

//
//  Decrypts a text string
//
GatewayReturnCodes Encryptor::Decrypt(const char* encrypted, size_t encryptedSize, std::string& decryptedStr)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    string encryptedStr = base64_decode(encrypted, encryptedSize);

    CRijndael oRijndael;
    oRijndael.MakeKey(Key, IV, 16, 16);

    // Buffer size must be a multiple of 16
    size_t decryptedSize = (1 + encryptedSize/16) * 16;

    // Allocate characters for the output buffer
    char* pDecrypted = new char[decryptedSize + 1];
    memset(pDecrypted, 0, decryptedSize + 1);

    oRijndael.Decrypt(encryptedStr.c_str(), pDecrypted, decryptedSize, CRijndael::CBC);

    decryptedStr = pDecrypted;
    delete pDecrypted;

    return status;
}
