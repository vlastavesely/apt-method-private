#include "test.h"
#include "../hex.h"

START_TEST(test_hex)
{
	std::string hex;
	unsigned char bin[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

	hex = hex_encode(bin, 8);
	ck_assert_str_eq("0011223344556677", hex.data());
}
END_TEST

void register_hex_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_hex);
}
