#include <fstream>
#include "test.h"
#include "../filter.h"

#define MAX_FILE_SIZE   128
#define CBC_PAD(n, bs)	(bs + n + (bs - (n % bs)))	/* IV + n + pad */
#define CTR_PAD(n, bs)	((bs / 2) + n)			/* IV + n */

// A random key
static const unsigned char key[32] = {
	0xdb, 0x42, 0xa5, 0x6c, 0x3b, 0x4f, 0xe1, 0x8f,
	0xd6, 0x14, 0x0a, 0xe0, 0xd9, 0x67, 0x92, 0xb6,
	0x6c, 0x06, 0x8d, 0x84, 0x60, 0x93, 0x78, 0xc6,
	0x6b, 0x9c, 0xc5, 0x25, 0x69, 0xe8, 0xfc, 0x1a
};

static int read_file(unsigned char *out, const std::string &filename)
{
	std::ifstream in(filename);
	return in.readsome((char *) out, MAX_FILE_SIZE);
}

static void test_encode(const std::string &cipher, const std::string &file,
			const std::string &enc_file, const std::string &dec_file,
			unsigned int enc_len, unsigned int dec_len)
{
	unsigned char a[MAX_FILE_SIZE], b[MAX_FILE_SIZE];
	int n;

	encode_file(file, enc_file, cipher, key);
	n = read_file(a, enc_file);
	ck_assert_int_eq(enc_len, n);

	decode_file(enc_file, dec_file, cipher, key);
	n = read_file(b, dec_file);
	ck_assert_int_eq(dec_len, n);

	n = read_file(a, file);
	ck_assert_int_eq(dec_len, n);

	for (auto i = 0; i < n; i++) {
		ck_assert_int_eq(a[i], b[i]);
	}

	remove(enc_file.data());
	remove(dec_file.data());
}

START_TEST(test_cbc_filter)
{
	test_encode("cbc(aes)", "tests/files/repo/a",
		    "/tmp/apt-transport-private.a.aes-cbc.enc",
		    "/tmp/apt-transport-private.a.aes-cbc.dec", CBC_PAD(6, 16), 6);

	test_encode("cbc(aes)", "tests/files/repo/b",
		    "/tmp/apt-transport-private.b.aes-cbc.enc",
		    "/tmp/apt-transport-private.b.aes-cbc.dec", CBC_PAD(16, 16), 16);

	test_encode("cbc(aes)", "tests/files/repo/c",
		    "/tmp/apt-transport-private.c.aes-cbc.enc",
		    "/tmp/apt-transport-private.c.aes-cbc.dec", CBC_PAD(21, 16), 21);

	test_encode("cbc(aes)", "tests/files/repo/d",
		    "/tmp/apt-transport-private.d.aes-cbc.enc",
		    "/tmp/apt-transport-private.d.aes-cbc.dec", CBC_PAD(0, 16), 0);

	test_encode("cbc(aes)", "tests/files/repo/e",
		    "/tmp/apt-transport-private.e.aes-cbc.enc",
		    "/tmp/apt-transport-private.e.aes-cbc.dec", CBC_PAD(39, 16), 39);

	test_encode("cbc(aes)", "tests/files/repo/f",
		    "/tmp/apt-transport-private.f.aes-cbc.enc",
		    "/tmp/apt-transport-private.f.aes-cbc.dec", CBC_PAD(102, 16), 102);
}
END_TEST

START_TEST(test_ctr_filter)
{
	test_encode("ctr(aes)", "tests/files/repo/a",
		    "/tmp/apt-transport-private.a.aes-ctr.enc",
		    "/tmp/apt-transport-private.a.aes-ctr.dec", CTR_PAD(6, 16), 6);

	test_encode("ctr(aes)", "tests/files/repo/b",
		    "/tmp/apt-transport-private.b.aes-ctr.enc",
		    "/tmp/apt-transport-private.b.aes-ctr.dec", CTR_PAD(16, 16), 16);

	test_encode("ctr(aes)", "tests/files/repo/c",
		    "/tmp/apt-transport-private.c.aes-ctr.enc",
		    "/tmp/apt-transport-private.c.aes-ctr.dec", CTR_PAD(21, 16), 21);

	test_encode("ctr(aes)", "tests/files/repo/d",
		    "/tmp/apt-transport-private.d.aes-ctr.enc",
		    "/tmp/apt-transport-private.d.aes-ctr.dec", CTR_PAD(0, 16), 0);

	test_encode("ctr(aes)", "tests/files/repo/e",
		    "/tmp/apt-transport-private.e.aes-ctr.enc",
		    "/tmp/apt-transport-private.e.aes-ctr.dec", CTR_PAD(39, 16), 39);

	test_encode("ctr(aes)", "tests/files/repo/f",
		    "/tmp/apt-transport-private.f.aes-ctr.enc",
		    "/tmp/apt-transport-private.f.aes-ctr.dec", CTR_PAD(102, 16), 102);
}
END_TEST

void register_filter_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_cbc_filter);
	tcase_add_test(test_case, test_ctr_filter);
}
