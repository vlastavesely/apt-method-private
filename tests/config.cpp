#include <stdexcept>
#include "test.h"
#include "../config-file.h"

START_TEST(test_config)
{
	Config config;
	std::string str;

	config.load_from_file("tests/files/config");

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

START_TEST(test_config_failures)
{
	Config config;
	std::string message = "";

	try {
		config.load_from_file("tests/does-not-exist");

	} catch (const std::invalid_argument &e) {
		message = e.what();
	}

	ck_assert_str_eq("Failed to read the config.", message.data());
}
END_TEST

void register_config_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_config);
	tcase_add_test(test_case, test_config_failures);
}
