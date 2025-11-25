#pragma once
#include "object.h"
#include "stack.h"

typedef struct VirtualMachine {
  stackType *frames;
  stackType *objects;
} vmType;

typedef struct StackFrame {
  stackType *references;
} frameType;

frameType *vmNewFrame(vmType *vm);
void frameFree(frameType *frame);
void vmFramePush(vmType *vm, frameType *frame);
vmType *vmNew();
void vmFree(vmType *vm);
void vmTrackObject(vmType *vm, object_t *obj);
