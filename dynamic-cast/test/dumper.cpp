#include <cstdint>
#include <cstdio>
#include <string>
#include <type_traits>
#include <typeinfo>

template <class U>
void dump_bytes(const U *vp) {
  void *const *p = (void *const *)(void const *)vp;
  for (size_t i = 0; i < sizeof(U); i += 8) {
    void *x = p[i / 8];
    printf("%016llx", (uint64_t)x);
    printf("\n");
  }
}

std::string diagnose_typeinfo_pointer(const void *vp);

template <class U>
void dump_vtable(const U *vp, int32_t vptr_offset) {
  void *const *p = (void *const *)(void const *)vp;
  p = (void *const *)p[vptr_offset / 8];
  for (int32_t i = -10; i < 10; ++i) {
    void *x = p[i];
    printf("%016llx", (uint64_t)x);
    if (i == 0) printf(" *");
    if (i == -1)
      printf(" type_info pointer (%s)", diagnose_typeinfo_pointer(x).c_str());
    if (i == -2) printf(" offset to most-derived (%d)", (int)(uint64_t)x);
    if (i <= -3) printf(" offset to virtual base, or vcall offset");
    printf("\n");
  }
}

struct BaseClass {
  virtual ~BaseClass() {}
  int64_t BaseClassdata = 0xA1A2A3A4;
};

struct Derived1 : virtual BaseClass {
  int64_t Derived1data = 0xC1C2C3C4;
};

struct Derived2 : virtual BaseClass {
  int64_t Derived2data = 0xD1D2D3D4;
};

struct Derived1Derived2 : Derived1, Derived2 {
  int64_t cddata = 0xCDCDCDCD;
};

struct Rat : BaseClass {
  int64_t ratdata = 0xEEEEEEEE;
};

struct RatDerived1Derived2 : Rat, Derived1Derived2 {
  int64_t ratdata = 0xECDECDECD;
};

std::string diagnose_typeinfo_pointer(const void *vp) {
  const std::type_info *p = (const std::type_info *)vp;
  if (!dynamic_cast<const std::type_info *>(p)) {
    return "not a typeinfo pointer";
#define T(C)                  \
  }                           \
  else if (p == &typeid(C)) { \
    return #C;
    T(BaseClass)
    T(Derived1)
    T(Derived2)
    T(Derived1Derived2)
    T(Rat)
    T(RatDerived1Derived2)
  } else {
    return "unknown typeinfo pointer";
  }
}

int32_t main() {
  BaseClass a;
  dump_bytes(&a);
  printf("-------------\n");
  dump_vtable(&a, 0);
  printf("-------------\n");
  Derived1 c;
  dump_bytes(&c);
  printf("-------------\n");
  dump_vtable(&c, 0);
  printf("-------------\n");
  dump_vtable(&c, 16);
  printf("-------------\n");
}
