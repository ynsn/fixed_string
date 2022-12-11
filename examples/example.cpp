#include <fixed_string/fixed_string.h>
#include <iostream>

template<stdext::fixed_string Name>
struct name {};

int main(int argc, char **argv) {
  name<"lol"> isaname;

  std::string_view arg = argv[0];
  constexpr stdext::fixed_string m1 = "Hello";
  constexpr stdext::fixed_string m2 = m1.substr(0, 2);

}
