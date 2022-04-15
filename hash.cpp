#include <openssl/sha.h>
#include "hash.h"
#include "hex.h"

std::string hashSha1(const std::string &salt, const std::string &path)
{
	unsigned char digest[20];
	struct SHAstate_st state;

	SHA1_Init(&state);
	SHA1_Update(&state, salt.data(), salt.length());
	SHA1_Update(&state, path.data(), path.length());
	SHA1_Final(digest, &state);

	return hexEncode(digest, sizeof(digest));
}
