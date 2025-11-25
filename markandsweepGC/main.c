#include "newobject.h"
#include "stack.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

void print_vm_stats(vmType *vm, const char *label) {
  printf("[%s] Objects: %zu, Frames: %zu\n", label, vm->objects->count,
         vm->frames->count);
}

void test_basic_allocation() {
  printf("\n--- Test: Basic Allocation & Retention ---\n");
  vmType *vm = vmNew();
  frameType *frame = vmNewFrame(vm);

  printf("Allocating Integer...\n");
  object_t *intObj = new_object_integer(vm, 42);
  frameReferenceObject(frame, intObj);

  printf("Allocating String...\n");
  object_t *strObj = new_object_string(vm, "Hello GC");
  frameReferenceObject(frame, strObj);

  print_vm_stats(vm, "Before GC");
  vmCollectGarbage(vm);
  print_vm_stats(vm, "After GC");

  if (vm->objects->count == 2) {
    printf("PASS: Objects retained.\n");
  } else {
    printf("FAIL: Expected 2 objects, got %zu.\n", vm->objects->count);
  }

  vmFree(vm);
}

void test_basic_collection() {
  printf("\n--- Test: Basic Collection ---\n");
  vmType *vm = vmNew();
  // We create a frame but don't reference the objects in it
  vmNewFrame(vm);

  printf("Allocating Float (Unreferenced)...\n");
  new_object_float(vm, 3.14f);

  print_vm_stats(vm, "Before GC");
  vmCollectGarbage(vm);
  print_vm_stats(vm, "After GC");

  if (vm->objects->count == 0) {
    printf("PASS: Object collected.\n");
  } else {
    printf("FAIL: Expected 0 objects, got %zu.\n", vm->objects->count);
  }

  vmFree(vm);
}

void test_nested_vector() {
  printf("\n--- Test: Nested Objects (Vector3) ---\n");
  vmType *vm = vmNew();
  frameType *frame = vmNewFrame(vm);

  printf("Creating Vector3...\n");
  object_t *x = new_object_float(vm, 1.0f);
  object_t *y = new_object_float(vm, 2.0f);
  object_t *z = new_object_float(vm, 3.0f);
  object_t *vec = new_object_vector3(vm, x, y, z);

  // Only reference the vector itself
  frameReferenceObject(frame, vec);

  print_vm_stats(vm, "Before GC");
  // Should have 4 objects: 3 floats + 1 vector
  vmCollectGarbage(vm);
  print_vm_stats(vm, "After GC");

  if (vm->objects->count == 4) {
    printf("PASS: Vector and children retained.\n");
  } else {
    printf("FAIL: Expected 4 objects, got %zu.\n", vm->objects->count);
  }

  // Now remove reference
  printf("Dropping reference...\n");
  // To "drop" in this simple VM, we can just pop the frame or reset references.
  // Since we only have one frame with one ref, let's just clear the stack of
  // references in the frame. But frameReferenceObject pushes to
  // frame->references. Let's manually pop it to simulate dropping scope
  // variable.
  stackPop(frame->references);

  vmCollectGarbage(vm);
  print_vm_stats(vm, "After GC (Dropped)");

  if (vm->objects->count == 0) {
    printf("PASS: Vector and children collected.\n");
  } else {
    printf("FAIL: Expected 0 objects, got %zu.\n", vm->objects->count);
  }

  vmFree(vm);
}

void test_nested_array() {
  printf("\n--- Test: Nested Objects (Array) ---\n");
  vmType *vm = vmNew();
  frameType *frame = vmNewFrame(vm);

  printf("Creating Array of size 2...\n");
  object_t *arr = new_object_array(vm, 2);
  object_t *val1 = new_object_integer(vm, 100);
  object_t *val2 = new_object_integer(vm, 200);

  array_set(arr, 0, val1);
  array_set(arr, 1, val2);

  frameReferenceObject(frame, arr);

  print_vm_stats(vm, "Before GC");
  // Should have 3 objects: 1 array + 2 integers
  vmCollectGarbage(vm);
  print_vm_stats(vm, "After GC");

  if (vm->objects->count == 3) {
    printf("PASS: Array and elements retained.\n");
  } else {
    printf("FAIL: Expected 3 objects, got %zu.\n", vm->objects->count);
  }

  printf("Dropping reference...\n");
  stackPop(frame->references);

  vmCollectGarbage(vm);
  print_vm_stats(vm, "After GC (Dropped)");

  if (vm->objects->count == 0) {
    printf("PASS: Array and elements collected.\n");
  } else {
    printf("FAIL: Expected 0 objects, got %zu.\n", vm->objects->count);
  }

  vmFree(vm);
}

int main() {
  printf("Starting Mark and Sweep GC Tests...\n");

  test_basic_allocation();
  test_basic_collection();
  test_nested_vector();
  test_nested_array();

  printf("\nAll tests completed.\n");
  return 0;
}
