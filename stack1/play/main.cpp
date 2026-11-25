#include "../stack.hpp"

#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const Stack& stack) {
  for (const Node* curr = stack.head; curr != nullptr; curr = curr->next) {
    os << curr->data << " ";
  }
  return os;
}

int main() {
  Stack stack;
  Push(&stack, "hello");
  Push(&stack, "world");

  std::cout << stack << std::endl;
  std::cout << Top(&stack) << std::endl;
  std::cout << Size(&stack) << std::endl;
  Pop(&stack);
  std::cout << stack << std::endl;
  Clear(&stack);
  std::cout << stack << std::endl;
  return 0;
}
