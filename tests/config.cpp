#include "test.h"
#include "../config.h"

START_TEST(test_config)
{
	Config config;
	std::string str;

	config.loadFromFile("tests/files/config");

	str = config["Private::httpserver::Origin"];
	ck_assert_str_eq("httpserver", str.c_str());

	str = config["Private::httpserver::URI"];
	ck_assert_str_eq("https://localhost/apt-test", str.c_str());

	str = config["Private::ftpserver::Origin"];
	ck_assert_str_eq("ftpserver", str.c_str());

	str = config["Private::ftpserver::URI"];
	ck_assert_str_eq("ftpes://ftpuser:secret@localhost/apt-test/", str.c_str());
}
END_TEST

void register_config_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_config);
}
