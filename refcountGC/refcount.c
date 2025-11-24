#include "object.h"
#include <stdlib.h>

#include <stdio.h>

void refcount_free(object_t *obj) {
  printf("Freed object of kind %d\n", obj->kind);
  switch (obj->kind) {
  case INTEGER:
  case FLOAT:
    break;
  case STRING:
    free(obj->data.v_string);
    break;
  case VECTOR3: {
    vector_t vec = obj->data.v_vector3;
    refcount_dec(vec.x);
    refcount_dec(vec.y);
    refcount_dec(vec.z);
    break;
  };
  case ARRAY: {
    for (size_t i = 0; i < obj->data.v_array.size; i++) {
      if (obj->data.v_array.elements[i] != NULL) {
        refcount_dec(obj->data.v_array.elements[i]);
      }
    }
    free(obj->data.v_array.elements);
    break;
  };
  }
  free(obj);
}

void refcount_dec(object_t *obj) {
  if (obj == NULL) {
    return;
  }
  obj->refcount--;
  if (obj->refcount == 0) {
    refcount_free(obj);
  }
}

void refcount_inc(object_t *obj) {
  if (obj == NULL) {
    return;
  };

  obj->refcount++;
  return;
}
