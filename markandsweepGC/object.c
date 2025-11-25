#include "vm.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void object_free(object_t *obj) {
  switch (obj->kind) {
  case INTEGER:
  case FLOAT:
    break;
  case STRING:
    free(obj->data.v_string);
    break;
  case VECTOR3:
    break;
  case ARRAY: {
    array_t *array = &obj->data.v_array;

    free(array->elements);
    break;
  }
  }
  free(obj);
};

bool array_set(object_t *obj, size_t index, object_t *value) {
  if (obj == NULL || value == NULL) {
    return false;
  }
  if (obj->kind != ARRAY) {
    return false;
  }
  if (index >= obj->data.v_array.size) {
    return false;
  }
  if (index < obj->data.v_array.size) {
    obj->data.v_array.elements[index] = value;
    return true;
  };
  return false;
};

object_t *array_get(object_t *obj, size_t index) {
  if (obj == NULL) {
    return NULL;
  }
  if (obj->kind != ARRAY) {
    return NULL;
  }

  if (index >= obj->data.v_array.size) {
    return NULL;
  }
  return obj->data.v_array.elements[index];
}
