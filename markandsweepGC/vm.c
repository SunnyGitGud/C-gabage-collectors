#include "vm.h"
#include "stack.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

void vmCollectGarbage(vmType *vm) {
  mark(vm);
  trace(vm);
  sweep(vm);
}

void sweep(vmType *vm) {
  for (size_t i = 0; i < vm->objects->count; i++) {
    object_t *obj = vm->objects->data[i];
    if (!obj)
      continue;
    if (obj->is_Marked) {
      obj->is_Marked = false;
    } else {
      object_free(obj);
      vm->objects->data[i] = NULL;
    }
  }
  stackRemoveNulls(vm->objects);
}

void trace(vmType *vm) {
  stackType *grayObjects = stackNew(8);
  if (!grayObjects)
    return;
  for (size_t i = 0; i < vm->objects->count; i++) {
    object_t *obj = vm->objects->data[i];
    if (obj && obj->is_Marked) {
      stackPush(grayObjects, obj);
    }
  }

  while (grayObjects->count > 0) {
    object_t *obj = stackPop(grayObjects);
    traceBlackenObjects(grayObjects, obj);
  }
  stackFree(grayObjects);
}

void traceBlackenObjects(stackType *grayObjects, object_t *obj) {
  if (obj->kind == INTEGER || obj->kind == FLOAT || obj->kind == STRING)
    return;
  if (obj->kind == VECTOR3) {
    traceMarkObject(grayObjects, obj->data.v_vector3.x);
    traceMarkObject(grayObjects, obj->data.v_vector3.y);
    traceMarkObject(grayObjects, obj->data.v_vector3.z);
    return;
  }
  if (obj->kind == ARRAY) {
    for (size_t i = 0; i < obj->data.v_array.size; i++) {
      traceMarkObject(grayObjects, obj->data.v_array.elements[i]);
    }
    return;
  }
  return;
}

void traceMarkObject(stackType *grayObjects, object_t *obj) {
  if (!obj)
    return;
  if (obj->is_Marked)
    return;
  obj->is_Marked = true;
  stackPush(grayObjects, obj);
}

void mark(vmType *vm) {
  for (size_t i = 0; i < vm->frames->count; i++) {
    frameType *frame = vm->frames->data[i];
    for (size_t j = 0; j < frame->references->count; j++) {
      object_t *obj = frame->references->data[j];
      if (obj) {
        obj->is_Marked = true;
      }
    }
  }
}

void frameReferenceObject(frameType *frame, object_t *obj) {
  stackPush(frame->references, obj);
}

void vmTrackObject(vmType *vm, object_t *obj) { stackPush(vm->objects, obj); }

void vmFramePush(vmType *vm, frameType *frame) { stackPush(vm->frames, frame); }

frameType *vmNewFrame(vmType *vm) {
  frameType *frame = malloc(sizeof(frameType));
  frame->references = stackNew(8);
  vmFramePush(vm, frame);

  return frame;
}

void frameFree(frameType *frame) {
  if (!frame)
    return;
  if (frame->references) {
    stackFree(frame->references);
  }
  free(frame);
}

vmType *vmNew() {
  vmType *vm = malloc(sizeof(vmType));
  if (vm == NULL) {
    return NULL;
  }
  vm->frames = stackNew(8);
  vm->objects = stackNew(8);
  return vm;
};

void vmFree(vmType *vm) {
  if (!vm)
    return;

  while (vm->objects->count > 0) {
    object_t *obj = stackPop(vm->objects);
    object_free(obj);
  }

  stackFree(vm->objects);
  while (vm->frames->count > 0) {
    frameType *frame = stackPop(vm->frames);
    frameFree(frame);
  }
  stackFree(vm->frames);
  free(vm);
};
