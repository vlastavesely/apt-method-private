#include "test.h"
#include "../cipher.h"

static const unsigned char plaintext[] = {
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

static const unsigned char key_256[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

static const unsigned char ciphertext_256[] = {
	0x8e, 0xa2, 0xb7, 0xca, 0x51, 0x67, 0x45, 0xbf,
	0xea, 0xfc, 0x49, 0x90, 0x4b, 0x49, 0x60, 0x89
};

START_TEST(test_cipher)
{
	Cipher cipher("aes", key_256);
	unsigned char buf[16];
	int i;

	cipher.encrypt_block(buf, plaintext);
	for (i = 0; i < 16; i++) {
		ck_assert_int_eq(ciphertext_256[i], buf[i]);
	}

	cipher.decrypt_block(buf, buf);
	for (i = 0; i < 16; i++) {
		ck_assert_int_eq(plaintext[i], buf[i]);
	}
}
END_TEST

void register_cipher_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_cipher);
}
