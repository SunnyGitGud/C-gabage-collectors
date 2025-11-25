#include "vm.h"
#include "stack.h"
#include <stdlib.h>

void vmTrackObject(vmType *vm, object_t *obj) { stackPush(vm->objects, obj); }

void vmFramePush(vmType *vm, frameType *frame) { stackPush(vm->frames, frame); }

frameType *vmNewFrame(vmType *vm) {
  frameType *frame = malloc(sizeof(frameType));
  frame->references = stackNew(8);
  vmFramePush(vm, frame);
  stackPush(vm->frames, frame);
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
  for (int i = 0; i < vm->frames->count; i++) {
    frameFree(vm->frames->data[i]);
  }
  stackFree(vm->frames);
  stackFree(vm->objects);
  free(vm);
};
