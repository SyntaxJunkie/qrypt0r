#include <Windows.h>
#include <base64.h>
#include <zlib.h>
#include <rijndael.h>
#include <aes.h>
#include <modes.h>
#include <filters.h>

#include "CryptoUtils.h"
#include "AESUtils.h"

#pragma comment(lib, "cryptlib.lib")

std::string CryptoUtils::deflate(const std::string & orig, CryptoPP::Deflator & decoder) {
	decoder.Put((byte*)orig.data(), orig.size());
	decoder.MessageEnd();
	CryptoPP::word64 size = decoder.MaxRetrievable();
	std::string decoded;
	if (size && size <= SIZE_MAX) {
		decoded.resize(size);
		decoder.Get((byte*)decoded.data(), decoded.size());
	}
	return decoded;
}

std::string CryptoUtils::inflate(const std::string & orig, CryptoPP::Inflator & encoder) {
	encoder.Put((byte*)orig.data(), orig.size());
	encoder.MessageEnd();
	CryptoPP::word64 size = encoder.MaxRetrievable();
	std::string encoded;
	if (size && size <= SIZE_MAX) {
		encoded.resize(size);
		encoder.Get((byte*)encoded.data(), encoded.size());
	}
	return encoded;
}

std::string CryptoUtils::base64_decode(const std::string & orig) {
	CryptoPP::Base64Decoder decoder;
	decoder.Put((byte*)orig.data(), orig.size());
	decoder.MessageEnd();
	CryptoPP::word64 size = decoder.MaxRetrievable();
	std::string decoded;
	if (size && size <= SIZE_MAX) {
		decoded.resize(size);
		decoder.Get((byte*)decoded.data(), decoded.size());
	}
	return decoded;
}

std::string CryptoUtils::base64_encode(const std::string & orig) {
	CryptoPP::Base64Encoder encoder(NULL, false);
	encoder.Put((byte*)orig.data(), orig.size());
	encoder.MessageEnd();
	CryptoPP::word64 size = encoder.MaxRetrievable();
	std::string encoded;
	if (size && size <= SIZE_MAX) {
		encoded.resize(size);
		encoder.Get((byte*)encoded.data(), encoded.size());
	}
	return encoded;
}

std::string CryptoUtils::zlib_compress(const std::string & orig) {
	unsigned int deflateLevel = CryptoPP::Deflator::MIN_DEFLATE_LEVEL;
	CryptoPP::ZlibCompressor encoder;
	return deflate(orig, encoder);
}

std::string CryptoUtils::zlib_decompress(const std::string & orig) {
	CryptoPP::ZlibDecompressor decoder;
	return inflate(orig, decoder);
}

std::string CryptoUtils::aes_decode(const std::string & orig, std::string key) {
	byte iv[CryptoPP::AES::BLOCKSIZE];
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
	CryptoPP::Rijndael::Decryption decrypter((byte*)key.c_str(), key.length());
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(decrypter, iv);

	std::string result;
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(result));
	stfDecryptor.Put((byte*)orig.data(), orig.size());
	stfDecryptor.MessageEnd();
	return result;
}

std::string CryptoUtils::aes_encode(const std::string & orig, std::string key) {
	byte iv[CryptoPP::AES::BLOCKSIZE];
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
	CryptoPP::Rijndael::Encryption encrypter((byte*)key.c_str(), key.length());
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(encrypter, iv);

	std::string result;
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(result));
	stfEncryptor.Put((byte*)orig.data(), orig.size());
	stfEncryptor.MessageEnd();
	return result;
}

std::string CryptoUtils::unreal_aes_decode(const std::string & orig, std::string key) {
	std::string res(orig.data(), orig.size());
	FAES_DecryptData((unsigned char*)res.data(), (unsigned int)res.size(), (unsigned char*)key.c_str());
	return res;
}

std::string CryptoUtils::unreal_aes_encode(const std::string & orig, std::string key) {
	std::string res(orig.data(), orig.size());
	FAES_EncryptData((unsigned char*)res.data(), (unsigned int)res.size(), (unsigned char*)key.c_str());
	return res;
}