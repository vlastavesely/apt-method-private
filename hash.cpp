#include <openssl/sha.h>
#include "hash.h"
#include "hex.h"

std::string hash_sha1(const std::string &salt, const std::string &path)
{
	struct SHAstate_st state;
	unsigned char digest[20];

	SHA1_Init(&state);
	SHA1_Update(&state, salt.data(), salt.length());
	SHA1_Update(&state, path.data(), path.length());
	SHA1_Final(digest, &state);

	return hex_encode(digest, sizeof(digest));
}
