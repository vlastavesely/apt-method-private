#include <iostream>
#include "test.h"
#include "stanza.h"
#include "config.h"
#include "uri.h"
#include "hash.h"
#include "hex.h"
#include "fetch.h"
#include "origin.h"

static struct Suite *create_test_suite()
{
	Suite *suite;
	TCase *test_case;

	suite = suite_create(NULL);
	test_case = tcase_create(NULL);
	suite_add_tcase(suite, test_case);

	register_stanza_tests(test_case);
	register_config_tests(test_case);
	register_uri_tests(test_case);
	register_hash_tests(test_case);
	register_hex_tests(test_case);
	register_fetch_tests(test_case);
	register_origin_tests(test_case);

	return suite;
}

static int run_test_suites(struct SRunner *runner)
{
	int retval;

	std::cout << "-----------------------------------------" << std::endl;
	srunner_run_all(runner, CK_NORMAL);
	retval = srunner_ntests_failed(runner);
	std::cout << "-----------------------------------------" << std::endl;

	if (retval == 0)
		std::cout << "\033[32mpassed\033[0m" << std::endl;
	else
		std::cout << "\033[31mfailed\033[0m" << std::endl;

	return retval;
}

int main(int argc, const char **argv)
{
	struct SRunner *runner;
	int retval;

	runner = srunner_create(NULL);
	srunner_add_suite(runner, create_test_suite());
	retval = run_test_suites(runner);
	srunner_free(runner);

	return retval;
}
