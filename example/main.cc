#include <iostream>
#include <stddef.h>

template <typename R, typename... Args>
class Callback {
 public:
  using RunType = R(Args...);

  Callback() = default;
  Callback(std::nullptr_t) = delete;

  ~Callback() {}

 private:
};

int main() {
  std::cout << "Hello World!\n";
}