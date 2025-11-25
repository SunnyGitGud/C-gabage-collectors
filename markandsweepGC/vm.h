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

void vmCollectGarbage(vmType *vm);
void sweep(vmType *vm);
void trace(vmType *vm);
void traceBlackenObjects(stackType *grayObjects, object_t *obj);
void traceMarkObject(stackType *grayObjects, object_t *obj);
void mark(vmType *vm);

void frameReferenceObject(frameType *frame, object_t *obj);
frameType *vmNewFrame(vmType *vm);
void frameFree(frameType *frame);
void vmFramePush(vmType *vm, frameType *frame);
vmType *vmNew();
void vmFree(vmType *vm);
void vmTrackObject(vmType *vm, object_t *obj);
