#include <stdlib.h>
#include <check.h>
#include <assert.h>

// TODO: remove relative links
#include <../../src/mt_hash.h>
#include <../../src/mt_string.h>

// test hash insert
START_TEST(test_hash_insert)
{
  MtHash* hash = mt_hash_new();

  MtString* key = mt_string_new_from_utf8("str");
  int value = 8;

  mt_hash_insert(hash, key, &value);
  fail_unless(hash->length == 1, "invalid hash length");

  MtPair* pair = mt_hash_search(hash, key);
  fail_unless(*((int*) pair->value) == 8, "invalid hash data");

  mt_hash_free(hash);
} END_TEST

// test hash remove
START_TEST(test_hash_remove)
{
  MtHash* hash = mt_hash_new();

  MtString* key = mt_string_new_from_utf8("str");
  int value = 8;

  mt_hash_insert(hash, key, &value);
  fail_unless(hash->length == 1, "invalid hash length");

  mt_hash_remove(hash, key);
  fail_unless(hash->length == 0, "invalid hash length");

  mt_hash_free(hash);
} END_TEST

// test hash clear

// test hash free

Suite* hash_suite()
{
  Suite* s = suite_create("hash");

  TCase* tc_insert = tcase_create("insert");
  TCase* tc_remove = tcase_create("remove");

  tcase_add_test(tc_insert, test_hash_insert);
  tcase_add_test(tc_remove, test_hash_remove);

  suite_add_tcase(s, tc_insert);
  suite_add_tcase(s, tc_remove);

  return s;
}

int main()
{
  int number_failed;

  Suite* s = hash_suite();
  SRunner* sr = srunner_create(s);

  srunner_set_log(sr, "check_mt_hash.log");

  srunner_run_all(sr, CK_NORMAL);

  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (number_failed == 0)? EXIT_SUCCESS : EXIT_FAILURE;
}
