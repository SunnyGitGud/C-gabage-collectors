#include "object.h"
#include <stdlib.h>
#include <string.h>

object_t *_new_object() {
  object_t *obj = calloc(1, sizeof(object_t));
  if (!obj)
    return NULL;
  obj->refcount = 1;
  return obj;
}

object_t *new_object_integer(int value) {
  object_t *obj = _new_object();
  if (!obj)
    return NULL;
  obj->kind = INTEGER;
  obj->data.v_int = value;
  return obj;
}

object_t *new_object_float(float value) {
  object_t *obj = _new_object();
  if (!obj)
    return NULL;
  obj->kind = FLOAT;
  obj->data.v_float = value;
  return obj;
}

object_t *new_object_string(char *value) {
  object_t *obj = _new_object();
  if (!obj)
    return NULL;
  obj->kind = STRING;
  char *temp = malloc(strlen(value) + 1);
  strcpy(temp, value);
  obj->data.v_string = temp;
  return obj;
}

object_t *new_object_vector3(object_t *x, object_t *y, object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  object_t *obj = _new_object();
  obj->kind = VECTOR3;
  obj->data.v_vector3 = (vector_t){.x = x, .y = y, .z = z};
  refcount_inc(x);
  refcount_inc(y);
  refcount_inc(z);
  return obj;
};

object_t *new_object_array(size_t size) {
  object_t *obj = _new_object();
  object_t **elements = calloc(size, sizeof(object_t));
  obj->kind = ARRAY;
  obj->data.v_array = (array_t){.size = size, .elements = elements};
  return obj;
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
