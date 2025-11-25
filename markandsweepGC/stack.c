#include "stack.h"
#include <stddef.h>
#include <stdlib.h>

stackType *stackNew(size_t capacity) {
  stackType *newStack = malloc(capacity);
  if (newStack == NULL) {
    return NULL;
  }

  newStack->count = 0;
  newStack->capacity = capacity;
  newStack->data = malloc(capacity * sizeof(void *));
  if (newStack->data == NULL) {
    free(newStack);
    return NULL;
  }
  return newStack;
};

void stackPush(stackType *stack, void *obj) {
  if (stack->count == stack->capacity) {
    stack->capacity *= 2;
    stack->data = realloc(stack->data, stack->capacity * sizeof(void *));
    if (stack->data == NULL) {
      exit(1);
    }
  }
  stack->data[stack->count] = obj;
  stack->count++;
};

void *stackPop(stackType *stack) {
  if (stack->count == 0) {
    return NULL;
  }
  stack->count--;
  return stack->data[stack->count];
};

void stackFree(stackType *stack) {
  if (stack == NULL) {
    return;
  }
  if (stack->data != NULL) {
    free(stack->data);
  }
  free(stack);
};

void stackRemoveNulls(stackType *stack) {
  size_t newCount = 0;

  for (size_t i = 0; i < stack->count; i++) {
    if (stack->data[i] != NULL) {
      stack->data[newCount++] = stack->data[i];
    }
  }

  stack->count = newCount;

  for (size_t i = newCount; i < stack->capacity; i++) {
    stack->data[i] = NULL;
  }
};
