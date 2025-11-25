#pragma once
#include <stdbool.h>
#include <stddef.h>

struct VirtualMachine;

typedef struct Object object_t;

typedef struct {
  size_t size;
  object_t **elements;
} array_t;

typedef struct {
  object_t *x;
  object_t *y;
  object_t *z;
} vector_t;

typedef enum ObjectKind {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY
} object_kind_t;

typedef struct ObjectData {
  int v_int;
  float v_float;
  char *v_string;
  vector_t v_vector3;
  array_t v_array;
} object_data_t;

typedef struct Object {
  int refcount;
  object_kind_t kind;
  object_data_t data;
} object_t;

object_t *new_object_integer(struct VirtualMachine *vm, int value);
object_t *new_object_float(struct VirtualMachine *vm, float value);
object_t *new_object_string(struct VirtualMachine *vm, char *value);
object_t *new_object_vector3(struct VirtualMachine *vm, object_t *x,
                             object_t *y, object_t *z);
object_t *new_object_array(struct VirtualMachine *vm, size_t size);
bool array_set(object_t *obj, size_t index, object_t *value);
object_t *array_get(object_t *obj, size_t index);
void refcount_free(object_t *obj);
void refcount_dec(object_t *obj);
void refcount_inc(object_t *obj);
