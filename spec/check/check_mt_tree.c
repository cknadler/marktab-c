#include <stdlib.h>
#include <check.h>
#include <assert.h>

// TODO: remove relative links
#include <../../src/mt_tree.h>
#include <../../src/mt_string.h>

START_TEST(test_tree_insert)
{
  MtTree* tree = mt_tree_new();
  MtString* key1 = mt_string_new_from_utf8("str1");
  MtString* key2 = mt_string_new_from_utf8("str2");
  int value1 = 8;
  int value2 = 4;

  mt_tree_insert(tree, key1, &value1);
  mt_tree_insert(tree, key2, &value2);
  MtPair* pair1 = mt_tree_search(tree, key1);
  MtPair* pair2 = mt_tree_search(tree, key2);

  fail_unless(*(int*) pair1->value == 8, "invalid tree data");
  fail_unless(*(int*) pair2->value == 4, "invalid tree data");

  mt_tree_free(tree);
} END_TEST

START_TEST(test_tree_remove)
{
  MtTree* tree = mt_tree_new();
  MtString* key = mt_string_new_from_utf8("str");
  int value = 8;

  mt_tree_insert(tree, key, &value);
  fail_unless(tree->size == 1, "invalid tree size");

  mt_tree_remove(tree, key);
  fail_unless(tree->size == 0, "invalid tree size");

  MtPair* pair = mt_tree_search(tree, key);
  fail_unless(pair == NULL, "value not properly removed from tree");

  mt_tree_free(tree);
} END_TEST

Suite* hash_suite()
{
  Suite* s = suite_create("tree");

  TCase* tc_insert = tcase_create("insert");
  //TCase* tc_remove = tcase_create("remove");

  tcase_add_test(tc_insert, test_tree_insert);
  //tcase_add_test(tc_remove, test_tree_remove);

  suite_add_tcase(s, tc_insert);
  //suite_add_tcase(s, tc_remove);

  return s;
}

int main()
{
  int number_failed;

  Suite* s = hash_suite();
  SRunner* sr = srunner_create(s);

  srunner_set_log(sr, "check_mt_tree.log");

  srunner_run_all(sr, CK_NORMAL);

  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (number_failed == 0)? EXIT_SUCCESS : EXIT_FAILURE;
}
