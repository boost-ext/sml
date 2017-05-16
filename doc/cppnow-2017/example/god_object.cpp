#include <fstream>
#include <iostream>

int main() {
  auto value = 0;
  {
    std::fstream file{"input.txt"};
    file >> value;
  }
  std::cout << value << '\n';
}
