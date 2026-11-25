#include "../stack.hpp"

#include <gtest/gtest.h>

class StackTest : public ::testing::Test {
 protected:
  void Push(const char* string) {
    ::Push(&stack_, string);
  }

  void Pop() {
    ::Pop(&stack_);
  }

  const char* Top() {
    return ::Top(&stack_);
  }

  size_t Size() {
    return ::Size(&stack_);
  }

  void Clear() {
    return ::Clear(&stack_);
  }

  void TearDown() override {
    Clear();
  }

 private:
  Stack stack_;
};