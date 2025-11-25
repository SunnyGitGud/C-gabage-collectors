#pragma once

#include "vm.h"

object_t *new_object_integer(vmType *vm, int value);
object_t *new_object_float(vmType *vm, float value);
object_t *new_object_string(vmType *vm, char *value);
object_t *new_object_vector3(vmType *vm, object_t *x, object_t *y, object_t *z);
object_t *new_object_array(vmType *vm, size_t size);
