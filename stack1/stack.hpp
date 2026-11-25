#pragma once
#include <cstring>

struct Node {
  char* data;
  Node* next;
};

struct Stack {
  Node* head;
  std::size_t size;

  Stack() {
    head = nullptr;
    size = 0;
  }

  ~Stack() {
    while (head != nullptr) {
      Node* temp = head;
      head = head->next;
      delete[] temp->data;
      delete temp;
    }
  }
};

inline void Push(Stack* stack, const char* str) {
  if (stack == nullptr || str == nullptr) {
    return;
  }
  Node* newNode = new Node;
  newNode->data = new char[std::strlen(str) + 1];
  std::strcpy(newNode->data, str);
  newNode->next = stack->head;
  stack->head = newNode;
  stack->size++;
}

inline void Pop(Stack* stack) {
  if (stack != nullptr && stack->head != nullptr) {
    Node* temp = stack->head;
    stack->head = stack->head->next;
    delete[] temp->data;
    delete temp;
    stack->size--; 
  }
}

inline const char* Top(const Stack* stack) {
  if (stack == nullptr || stack->head == nullptr) {
    return nullptr;
  }
  return stack->head->data;
}

inline std::size_t Size(const Stack* stack) {
  if (stack == nullptr) {
    return 0;
  }
  return stack->size;
}

inline void Clear(Stack* stack) {
  while (stack->head != nullptr) {
    Pop(stack); 
  }
}
