#include <stdexcept>
#include <string.h>
#include "cipher.h"

Cipher::Cipher(const std::string &cipher_name, const unsigned char *key)
{
	/* The cipher is AES-256. No other ciphers or keysizes are supported
	 * at this time. */
	if (cipher_name != "aes")
		throw std::invalid_argument("Invalid cipher name");

	ctx_enc = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx_enc, 0);
	EVP_EncryptInit_ex(ctx_enc, EVP_aes_256_ecb(), NULL, key, NULL);

	ctx_dec = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx_dec, 0);
	EVP_DecryptInit_ex(ctx_dec, EVP_aes_256_ecb(), NULL, key, NULL);
}

Cipher::~Cipher()
{
	EVP_CIPHER_CTX_free(ctx_enc);
	EVP_CIPHER_CTX_free(ctx_dec);
}

unsigned int Cipher::get_blocksize()
{
	return 16;
}

void Cipher::encrypt_block(unsigned char *out, const unsigned char *in)
{
	int outl;
	EVP_EncryptUpdate(ctx_enc, out, &outl, in, get_blocksize());
}

void Cipher::decrypt_block(unsigned char *out, const unsigned char *in)
{
	int outl;
	EVP_DecryptUpdate(ctx_dec, out, &outl, in, get_blocksize());
}
