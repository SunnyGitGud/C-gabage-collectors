#pragma once

#include <stdlib.h>

typedef struct {
  size_t count;
  size_t capacity;
  void **data;
} stackType;

stackType *stackNew(size_t capacity);
void stackPush(stackType *stack, void *obj);
void *stackPop(stackType *stack);
void stackFree(stackType *stack);
void stackRemoveNulls(stackType *stack);
