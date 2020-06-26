#ifndef HEADER_AES_UTILS
#define HEADER_AES_UTILS

void FAES_EncryptData(unsigned char *Contents, unsigned int NumBytes, const unsigned char* Key);
void FAES_DecryptData(unsigned char *Contents, unsigned int NumBytes, const unsigned char* Key);

#endif // HEADER_AES_UTILS