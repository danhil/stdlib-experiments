#pragma once

#include <iostream>

struct BaseClass {
  virtual ~BaseClass() = default;

  auto *as_BaseClass() { return this; }

  virtual void call() { std::cout << "baseclass" << std::endl; }
};

struct Derived1 : virtual BaseClass {
  auto *as_Derived1() { return this; }

  void call() override { std::cout << "Derived1" << std::endl; }
};

// TODO -> why do all members _need_ to have data? Otherwise the cast fails.
struct Derived2 : virtual BaseClass {
  auto *as_Derived2() { return this; }

  int32_t Derived2data;
};

struct Derived1Derived2 : Derived1, Derived2 {
  auto *as_Derived1Derived2() { return this; }

  int32_t Derived1Derived2data;
};

struct Derived3 : protected BaseClass {
  auto *as_Derived3() { return this; }

  BaseClass *as_BaseClass() { return this; }

  int32_t Derived3data;
};

struct Derived4 : BaseClass {
  auto *as_Derived4() { return this; }

  int32_t Derived4data;
};

struct Derived5 : Derived4, Derived3 {
  auto *as_Derived5() { return this; }

  int32_t Derived5data;
};
