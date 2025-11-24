#include "minunit.h"
#include "object.h"
#include <stdio.h>

int tests_run = 0;

static char *test_integer() {
  object_t *int_obj = new_object_integer(42);
  mu_assert("error, integer value != 42", int_obj->data.v_int == 42);
  mu_assert("error, refcount != 1", int_obj->refcount == 1);

  refcount_dec(int_obj);
  // Cannot verify freed easily without mock allocator, but we assume it works
  // if no crash
  return 0;
}

static char *test_vector3() {
  object_t *x = new_object_float(1.0f);
  object_t *y = new_object_float(2.0f);
  object_t *z = new_object_float(3.0f);

  object_t *vec = new_object_vector3(x, y, z);

  mu_assert("error, vector refcount != 1", vec->refcount == 1);

  // Check children refcounts.
  // Initial: 1.
  // Vector adds 1.
  // Total should be 2.
  mu_assert("error, x refcount != 2", x->refcount == 2);
  mu_assert("error, y refcount != 2", y->refcount == 2);
  mu_assert("error, z refcount != 2", z->refcount == 2);

  // We decrement our local references
  refcount_dec(x);
  refcount_dec(y);
  refcount_dec(z);

  // Now refcounts should be 1 (owned by vector)
  mu_assert("error, x refcount != 1", x->refcount == 1);
  mu_assert("error, y refcount != 1", y->refcount == 1);
  mu_assert("error, z refcount != 1", z->refcount == 1);

  refcount_dec(vec); // Should free vec and children
  return 0;
}

static char *test_array() {
  object_t *arr = new_object_array(2);
  object_t *str1 = new_object_string("Hello");
  object_t *str2 = new_object_string("World");

  array_set(arr, 0, str1);
  array_set(arr, 1, str2);

  mu_assert("error, array refcount != 1", arr->refcount == 1);

  // Array does NOT increment refcounts currently (based on my reading of
  // object.c) Wait, let's check array_set implementation in object.c It just
  // assigns: obj->data.v_array.elements[index] = value; So it does NOT
  // increment. But refcount_free for ARRAY decrements elements. This is
  // inconsistent with Vector3 now. If Vector3 increments, Array should probably
  // too? Or we should manually increment before adding to array? For now, I
  // will test existing behavior: refcount should be 1.

  mu_assert("error, str1 refcount != 1", str1->refcount == 1);
  mu_assert("error, str2 refcount != 1", str2->refcount == 1);

  refcount_dec(arr);
  // This will dec str1 and str2. 1 -> 0. They get freed.
  return 0;
}

static char *test_shared() {
  object_t *shared = new_object_integer(100);
  object_t *vec1 = new_object_vector3(shared, shared, shared);

  // shared refcount:
  // 1 (initial)
  // +1 (x)
  // +1 (y)
  // +1 (z)
  // = 4
  mu_assert("error, shared refcount != 4", shared->refcount == 4);

  refcount_dec(vec1);
  // vec1 freed. Decs x, y, z.
  // 4 - 3 = 1.

  mu_assert("error, shared refcount != 1", shared->refcount == 1);

  refcount_dec(shared); // 1 -> 0. Freed.
  return 0;
}

static char *all_tests() {
  mu_run_test(test_integer);
  mu_run_test(test_vector3);
  mu_run_test(test_array);
  mu_run_test(test_shared);
  return 0;
}

int main(int argc, char **argv) {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
