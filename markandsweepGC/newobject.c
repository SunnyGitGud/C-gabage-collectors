
#include "vm.h"
#include <string.h>

object_t *_new_object(vmType *vm) {
  object_t *obj = calloc(1, sizeof(object_t));
  if (!obj)
    return NULL;
  obj->is_Marked = false;
  vmTrackObject(vm, obj);
  return obj;
}

object_t *new_object_integer(vmType *vm, int value) {
  object_t *obj = _new_object(vm);
  if (!obj)
    return NULL;
  obj->kind = INTEGER;
  obj->data.v_int = value;
  return obj;
}

object_t *new_object_float(vmType *vm, float value) {
  object_t *obj = _new_object(vm);
  if (!obj)
    return NULL;
  obj->kind = FLOAT;
  obj->data.v_float = value;
  return obj;
}

object_t *new_object_string(vmType *vm, char *value) {
  object_t *obj = _new_object(vm);
  if (!obj)
    return NULL;
  obj->kind = STRING;
  char *temp = malloc(strlen(value) + 1);
  strcpy(temp, value);
  obj->data.v_string = temp;
  return obj;
}

object_t *new_object_vector3(vmType *vm, object_t *x, object_t *y,
                             object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  object_t *obj = _new_object(vm);
  obj->kind = VECTOR3;
  obj->data.v_vector3 = (vector_t){.x = x, .y = y, .z = z};
  return obj;
};

object_t *new_object_array(vmType *vm, size_t size) {
  object_t *obj = _new_object(vm);
  object_t **elements = calloc(size, sizeof(object_t));
  obj->kind = ARRAY;
  obj->data.v_array = (array_t){.size = size, .elements = elements};
  return obj;
};
