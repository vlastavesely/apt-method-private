#ifndef __CIPHER_H
#define __CIPHER_H

#include <openssl/evp.h>
#include <string>

class Cipher {
private:
	EVP_CIPHER_CTX *ctx_enc;
	EVP_CIPHER_CTX *ctx_dec;
public:
	Cipher(const std::string &cipher_name, const unsigned char *key);
	~Cipher();
	unsigned int get_blocksize();
	void encrypt_block(unsigned char *out, const unsigned char *in);
	void decrypt_block(unsigned char *out, const unsigned char *in);
};

#endif /* __CIPHER_H */
