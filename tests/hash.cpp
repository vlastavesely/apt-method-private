#include "test.h"
#include "../hash.h"

START_TEST(test_hash_sha1)
{
	std::string hash;

	hash = hashSha1("The quick brown fox", " jumps over the lazy dog");
	ck_assert_str_eq("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12", hash.data());
}
END_TEST

void register_hash_tests(struct TCase *test_case)
{
	tcase_add_test(test_case, test_hash_sha1);
}
