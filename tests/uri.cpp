#include <stdexcept>
#include "test.h"
#include "../uri.h"

START_TEST(test_uri)
{
	URI uri("https://server/path");
	ck_assert_str_eq("https", uri.protocol.c_str());
	ck_assert_str_eq("server", uri.origin.c_str());
	ck_assert_str_eq("/path", uri.path.c_str());

	std::string str = uri;
	ck_assert_str_eq("https://server/path", str.c_str());
}
END_TEST

START_TEST(test_uri_failures)
{
	std::string message = "";
	try {
		URI uri("xxx");
	} catch (const std::invalid_argument &e) {
		message = e.what();
	}

	ck_assert_str_eq("Invalid URI.", message.c_str());

}
END_TEST

void register_uri_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_uri);
	tcase_add_test(test_case, test_uri_failures);
}
