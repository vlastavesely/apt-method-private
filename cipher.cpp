#include <stdexcept>
#include <string.h>
#include <openssl/aes.h>
#include "cipher.h"

struct aes_keys {
	AES_KEY ek;
	AES_KEY dk;
};

Cipher::Cipher(const std::string &cipher_name, const unsigned char *key)
{
	/* The cipher is AES-256. No other ciphers or keysizes are supported
	 * at this time. */
	if (cipher_name != "aes")
		throw std::invalid_argument("Invalid cipher name");

	subkeys = new struct aes_keys;
	AES_set_encrypt_key(key, 256, &((struct aes_keys *) subkeys)->ek);
	AES_set_decrypt_key(key, 256, &((struct aes_keys *) subkeys)->dk);
}

Cipher::~Cipher()
{
	struct aes_keys *keys = (struct aes_keys *) subkeys;

	memset(&keys->ek, '\0', sizeof(keys->ek));
	memset(&keys->dk, '\0', sizeof(keys->dk));

	delete keys;
}

unsigned int Cipher::get_blocksize()
{
	return 16;
}

void Cipher::encrypt_block(unsigned char *out, const unsigned char *in)
{
	AES_encrypt(in, out, &((struct aes_keys *) subkeys)->ek);
}

void Cipher::decrypt_block(unsigned char *out, const unsigned char *in)
{
	AES_decrypt(in, out, &((struct aes_keys *) subkeys)->dk);
}
