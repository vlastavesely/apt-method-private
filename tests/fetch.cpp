#include <fstream>
#include <filesystem>
#include "test.h"
#include "../fetch.h"

#define MAX_TEST_FILE 512
#define TEST_FILE "/tmp/apt-transport-private.test"

static int load_file(char *out, const char *filename)
{
	std::ifstream in(filename);
	int n;

	n = in.readsome(out, MAX_TEST_FILE);
	out[n] = '\0';

	return 0;
}

START_TEST(test_fetch)
{
	std::string uri, cwd;
	char a[MAX_TEST_FILE], b[MAX_TEST_FILE];
	int ret;

	remove(TEST_FILE);

	cwd = std::filesystem::current_path();
	uri = "file://" + cwd + "/tests/files/stanzas";

	ret = fetch_uri(uri, TEST_FILE);
	ck_assert_int_eq(0, ret);

	ret = load_file(a, "tests/files/stanzas");
	ck_assert_int_eq(0, ret);

	ret = load_file(b, TEST_FILE);
	ck_assert_int_eq(0, ret);
	ck_assert_str_eq(a, b);

	remove(TEST_FILE);
}
END_TEST

void register_fetch_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_fetch);
}
