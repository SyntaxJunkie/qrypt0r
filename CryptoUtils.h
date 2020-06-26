#ifndef HEADER_CRYPTO_UTILS
#define HEADER_CRYPTO_UTILS
#include <zdeflate.h>
#include <zinflate.h>

class CryptoUtils {
public:
	static std::string deflate(const std::string & orig, CryptoPP::Deflator & decoder);
	static std::string inflate(const std::string & orig, CryptoPP::Inflator & encoder);
	static std::string base64_decode(const std::string & orig);
	static std::string base64_encode(const std::string & orig);
	static std::string aes_decode(const std::string & orig, std::string key);
	static std::string aes_encode(const std::string & orig, std::string key);
	static std::string unreal_aes_decode(const std::string & orig, std::string key);
	static std::string unreal_aes_encode(const std::string & orig, std::string key);
	static std::string zlib_compress(const std::string & orig);
	static std::string zlib_decompress(const std::string & orig);
};

#endif // HEADER_CRYPTO_UTILS