#include <fstream>
#include "test.h"
#include "../stanza.h"

START_TEST(test_stanza)
{
	std::ifstream in("tests/files/stanzas");

	Stanza s1(in);
	ck_assert_str_eq("001", s1["one"].c_str());
	ck_assert_str_eq("002", s1["two"].c_str());
	ck_assert_str_eq("", s1["three"].c_str());

	Stanza s2(in);
	ck_assert_str_eq("003", s2["three"].c_str());
	ck_assert_str_eq("004", s2["four"].c_str());
	ck_assert_str_eq("", s2["one"].c_str());
}
END_TEST

START_TEST(test_stanza_failures)
{
	std::ifstream in("tests/does-not-exist");
	std::string message;

	try {
		Stanza s(in);

	} catch (const std::invalid_argument &e) {
		message = e.what();
	}

	ck_assert_str_eq("Failed to read a stanza.", message.data());
}
END_TEST

void register_stanza_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_stanza);
	tcase_add_test(test_case, test_stanza_failures);
}
