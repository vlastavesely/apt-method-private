#include <stdexcept>
#include <openssl/evp.h>
#include "hash.h"
#include "hex.h"

std::string hash_sha1(const std::string &salt, const std::string &path)
{
	EVP_MD_CTX *ctx;
	unsigned char digest[20];

	ctx = EVP_MD_CTX_new();

	EVP_DigestInit_ex(ctx, EVP_sha1(), NULL);
	EVP_DigestUpdate(ctx, salt.data(), salt.length());
	EVP_DigestUpdate(ctx, path.data(), path.length());
	EVP_DigestFinal_ex(ctx, digest, NULL);

	EVP_MD_CTX_free(ctx);

	return hex_encode(digest, sizeof(digest));
}
