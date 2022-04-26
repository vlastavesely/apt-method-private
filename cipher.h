#ifndef __CIPHER_H
#define __CIPHER_H

#include <string>

class Cipher {
private:
	void *subkeys;
public:
	Cipher(const std::string &cipher_name, const unsigned char *key);
	~Cipher();
	unsigned int get_blocksize();
	void encrypt_block(unsigned char *out, const unsigned char *in);
	void decrypt_block(unsigned char *out, const unsigned char *in);
};

#endif /* __CIPHER_H */
