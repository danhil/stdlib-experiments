#include <cstdio>
#include <type_traits>

#include "composition.h"
#include "dynamicast.h"

template <class F, class T, class = void>
struct can_dynamic_cast_impl : std::false_type {};
template <class F, class T>
struct can_dynamic_cast_impl<F, T,
                             decltype(void(dynamic_cast<T *>((F *)nullptr)))>
    : std::true_type {};
template <class F, class T>
struct can_dynamic_cast : can_dynamic_cast_impl<F, T, void> {};

template <class F, class T>
constexpr bool can_dynamic_cast_v = can_dynamic_cast<F, T>::value;

template <bool B>
using bool_if_t = std::enable_if_t<B, bool>;

template <class To, class From, bool_if_t<can_dynamic_cast_v<From, To>> = true>
void test(From *f) {
  printf("%s", __PRETTY_FUNCTION__);
  To *p = dynamic_cast<To *>(f);
  To *q = dynamicast<To *>(f);
  if (p == q) {
    printf("\rPASS: %s\n", __PRETTY_FUNCTION__);
  } else {
    printf("\rFAIL: %s\n", __PRETTY_FUNCTION__);
    printf("expected=%p, result=%p\n", p, q);
  }
}

template <class To, class From, bool_if_t<!can_dynamic_cast_v<From, To>> = true>
void test(From *) {
  printf("NOOP: %s\n", __PRETTY_FUNCTION__);
}

template <class T>
static T *instance = new T;

template <class To>
void test_to() {
  test<To>(instance<BaseClass>->as_BaseClass());

  test<To>(instance<Derived1>->as_BaseClass());
  test<To>(instance<Derived1>->as_Derived1());

  test<To>(instance<Derived2>->as_BaseClass());
  test<To>(instance<Derived2>->as_Derived2());

  test<To>(instance<Derived1Derived2>->as_BaseClass());
  test<To>(instance<Derived1Derived2>->as_Derived1());
  test<To>(instance<Derived1Derived2>->as_Derived2());

  test<To>(instance<Derived3>->as_BaseClass());
  test<To>(instance<Derived3>->as_Derived3());

  test<To>(instance<Derived4>->as_BaseClass());
  test<To>(instance<Derived4>->as_Derived4());

  test<To>(instance<Derived5>->as_Derived3()->as_BaseClass());
  test<To>(instance<Derived5>->as_Derived4()->as_BaseClass());
  test<To>(instance<Derived5>->as_Derived3());
  test<To>(instance<Derived5>->as_Derived4());
  test<To>(instance<Derived5>->as_Derived5());
}

int32_t main() {
  test_to<void>();
  test_to<BaseClass>();
  test_to<Derived1>();
  test_to<Derived2>();
  test_to<Derived1Derived2>();
  test_to<Derived3>();
  test_to<Derived4>();
  test_to<Derived5>();
}
