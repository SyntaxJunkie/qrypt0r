// qrypt0r.cpp : Defines the exported functions for the DLL application.
//
#include <string>
#include <windows.h>
#include "CryptoUtils.h"
#include "CompressUtils.h"
#include "StrUtils.h"

static std::string base64_key = "lEQWeCt51ET+MIuxdTs7Ig/gzVZP2vdkVZA1BDfz+L0=";
static std::string aes_key = "6EF35759-454D-4EBC-8041-9A94CB99FD5D";
static std::string magic_inner = "DbdDAQEB";
static std::string magic_outer = "DbdDAgAC";
static std::string final_key = CryptoUtils::unreal_aes_decode(CryptoUtils::base64_decode(base64_key), aes_key); //get real key

std::string decrypt_data(std::string json_data) {
	std::string payload(json_data.begin() + 0x8, json_data.end()); //remove magic number
	std::string raw_payload = CryptoUtils::base64_decode(payload); //base64 decode the result
	std::string decrypted_payload = CryptoUtils::unreal_aes_decode(raw_payload, final_key); //aes decode the result
	for (std::size_t i = 0; i < decrypted_payload.size(); ++i) {
		decrypted_payload[i] += 0x1; //shift all bytes by + 0x1
	}
	std::string payload_inner(decrypted_payload.begin() + 0x8, decrypted_payload.end()); //remove second magic number
	std::string raw_inner = CryptoUtils::base64_decode(payload_inner); //base64 decode the result
	std::string gzipped_json(raw_inner.begin() + 4, raw_inner.end()); //remove size header
	std::string unzipped_json = CryptoUtils::zlib_decompress(gzipped_json); //unzip json
	std::wstring final_json = StrUtils::interpret_str_as_wstr(unzipped_json); //wide json
	return StrUtils::wstr_to_str(final_json); //convert to utf8
}

std::string encrypt_data(std::string json_data) {
	std::string json_wide = StrUtils::interpret_wstr_as_str(StrUtils::str_to_wstr(json_data)); //convert to utf16
	std::string zipped_json = UE_CompressZlib(reinterpret_cast<unsigned char*>(&json_wide[0]), static_cast<unsigned int>(json_wide.size())); //zip json
	std::string size_adjusted_zipped = StrUtils::pad_with_number(zipped_json, static_cast<unsigned int>(json_wide.size())); //prepend file size
	std::string enc_inner = CryptoUtils::base64_encode(size_adjusted_zipped); //base64 encode the result
	std::string before_crypt = magic_inner;
	std::size_t enc_size = enc_inner.size() + before_crypt.size(); //size of data to be encrypted
	std::size_t pad_size = 0;
	if (enc_size & 0xF) //least significant bit
	{
		pad_size = 0x10 - (enc_size & 0xF); //get size to pad
	}
	enc_inner.append(pad_size, 0x01); //fix up padding
	before_crypt.append(enc_inner);
	for (std::size_t i = 0; i < before_crypt.size(); ++i) {
		before_crypt[i] -= 0x1; //shift all bytes by - 0x1
	}
	std::string encrypted_json = CryptoUtils::unreal_aes_encode(before_crypt, final_key); //aes encode the result
	std::string enc_payload = CryptoUtils::base64_encode(encrypted_json); //base64 encode the result
	std::string json_file = magic_outer;
	json_file.append(enc_payload); //append data to magic number
	return json_file;
}

extern "C" __declspec(dllexport)  void  __cdecl readData(char* jsonBytes, char* buffer)
{
	std::string json(jsonBytes);
	if (json.size() <= 20) {
		MessageBoxA(0, "Unable to parse (file too small)", "Error", MB_ICONERROR);
		return;
	}
	std::string result = decrypt_data(json);
	if (result.size() >= 0x2000000) {
		MessageBoxA(0, "Unable to parse (file too big)", "Error", MB_ICONERROR);
		return;
	}
	memcpy(buffer, &result[0], result.size());
}

extern "C" __declspec(dllexport)  void  __cdecl writeData(char* jsonBytes, char* buffer)
{
	std::string json(jsonBytes);
	if (json.size() <= 20) {
		MessageBoxA(0, "Unable to parse (file too small)", "Error", MB_ICONERROR);
		return;
	}
	std::string result = encrypt_data(json);
	if (result.size() >= 0x2000000) {
		MessageBoxA(0, "Unable to parse (file too big)", "Error", MB_ICONERROR);
		return;
	}
	memcpy(buffer, &result[0], result.size());
}