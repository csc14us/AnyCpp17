#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include <utility/Any.h>

int main() {
  using Utility::Any;

  Any nothing;
  assert(nothing.empty());

  std::string hello{"Hello Curtis"};
  std::cout << hello << std::endl;

  Any helloAny{hello};
  assert(helloAny.as<int>() == nullptr);
  assert(Any::IsOfType<std::string>(helloAny));
  std::cout << *helloAny.as<std::string>() << std::endl;

  helloAny = Any::Make<std::string>("Yo Coop");
  std::cout << *helloAny.as<std::string>() << std::endl;

  Any helloAny2 = helloAny;
  assert(helloAny2.as<std::string>() != nullptr);
  std::cout << *helloAny2.as<std::string>() << std::endl;
  assert(*helloAny.as<std::string>() == "Yo Coop");

  Any helloAny3 = std::move(helloAny2);
  assert(helloAny2.empty());
  std::cout << *helloAny3.as<std::string>() << std::endl;

  Any helloAny4;
  assert(helloAny4.empty());
  helloAny4 = std::move(helloAny3);
  assert(helloAny3.empty());
  std::cout << *helloAny4.as<std::string>() << std::endl;

  Any helloAny5;
  assert(helloAny5.empty());
  helloAny5 = helloAny4;
  assert(!helloAny5.empty());
  std::cout << *helloAny5.as<std::string>() << std::endl;

  helloAny5 = {};
  assert(helloAny5.empty());

  using IntVec = std::vector<int>;
  const auto initList = { 0, 0, 0 };
  auto zeroes = Any::Make<IntVec>(initList);
  assert(*zeroes.as<IntVec>() == IntVec(initList));
  zeroes.as<IntVec>()->clear();
  assert(zeroes.as<IntVec>()->empty());

  std::cout << "Bye!" << std::endl;
  return 0;
}