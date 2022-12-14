#include <fstream>
#include "test.h"
#include "../origin.h"

START_TEST(test_origin)
{
	Config config;
	config.load_from_file("tests/files/config");

	Origin origin(config, "httpserver");
	ck_assert_str_eq("https://localhost/apt-test", origin.uri.data());
	ck_assert_str_eq("salt1", origin.salt.data());

	for (int i = 0; i < 32; i++) {
		ck_assert_int_eq(0xaa, origin.key[i]);
	}
}
END_TEST

START_TEST(test_origin_bad_key)
{
	Config config;
	config.load_from_file("tests/files/config");
	std::string msg = "";

	try {
		Origin origin(config, "ftpserver");
	} catch (const std::invalid_argument &e) {
		msg = e.what();
	}

	ck_assert_str_eq("Failed to parse the encryption key.", msg.data());
}
END_TEST

void register_origin_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_origin);
	tcase_add_test(test_case, test_origin_bad_key);
}
