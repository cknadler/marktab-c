#include <stdlib.h>
#include <check.h>
#include <assert.h>

// TODO: remove relative links
#include <../../src/mt_queue.h>

START_TEST(test_queue_enqueue)
{
  MtQueue* queue = mt_queue_new();
  int value = 8;

  mt_queue_enqueue(queue, &value);
  fail_unless(*(int*) mt_queue_dequeue(queue) == 8, "invalid queue contents");
  fail_unless(mt_queue_dequeue(queue) == NULL, "empty queue dequeue");

  mt_queue_free(queue);
} END_TEST

START_TEST(test_queue_clear)
{
  MtQueue* queue = mt_queue_new();
  int value = 8;

  mt_queue_enqueue(queue, &value);
  mt_queue_clear(queue);
  fail_unless(queue->length == 0, "invalid queue length");

  mt_queue_free(queue);
} END_TEST

START_TEST(test_queue_each_val)
{
  MtQueue* queue = mt_queue_new();
  int value = 8;

  // enqueue value 5 times
  int i;
  for (i = 0; i < 5; ++i)
    mt_queue_enqueue(queue, &value);

  fail_unless(queue->length == 5, "invalid queue length");

  int count = 0;
  mt_queue_each_val(queue, {
    fail_unless(*(int*)val == 8, "invalid value");
    ++count;
  });

  fail_unless(count == 5, "invalid queue length");

  mt_queue_free(queue);
} END_TEST

START_TEST(test_queue_dequeue_each_val)
{
  MtQueue* queue = mt_queue_new();
  int value = 8;

  // enqueue value 5 times
  int i;
  for (i = 0; i < 5; ++i)
    mt_queue_enqueue(queue, &value);

  int count = 0;
  mt_queue_dequeue_each_val(queue, {
    fail_unless(*(int*)val == 8, "invalid value");
    ++count;
  });

  fail_unless(count == 5, "invalid queue length");
  fail_unless(queue->length == 0, "invalid queue length");

  mt_queue_free(queue);
} END_TEST

Suite* hash_suite()
{
  Suite* s = suite_create("queue");

  TCase* tc_enqueue = tcase_create("enqueue");
  TCase* tc_clear = tcase_create("clear");
  TCase* tc_each_val = tcase_create("each_val");
  TCase* tc_dequeue_each_val = tcase_create("dequeue_each_val");

  tcase_add_test(tc_enqueue, test_queue_enqueue);
  tcase_add_test(tc_clear, test_queue_clear);
  tcase_add_test(tc_each_val, test_queue_each_val);
  tcase_add_test(tc_dequeue_each_val, test_queue_dequeue_each_val);

  suite_add_tcase(s, tc_enqueue);
  suite_add_tcase(s, tc_clear);
  suite_add_tcase(s, tc_each_val);
  suite_add_tcase(s, tc_dequeue_each_val);

  return s;
}

int main()
{
  int number_failed;

  Suite* s = hash_suite();
  SRunner* sr = srunner_create(s);

  srunner_set_log(sr, "check_mt_queue.log");

  srunner_run_all(sr, CK_NORMAL);

  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (number_failed == 0)? EXIT_SUCCESS : EXIT_FAILURE;
}
