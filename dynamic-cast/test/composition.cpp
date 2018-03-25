
#include "composition.h"
#include "dynamicast.h"

#include <cassert>
#include <typeinfo>

//////////////////////////////////////////////////////////////////// BaseClass
static void* BaseClass_convertToBase(char* p, const std::type_info& to) {
  assert(false);  // I have no bases.
  return nullptr;
}

static void* BaseClass_maybeFromHasAPublicChildOfTypeTo(
    char* p, int32_t offset, const std::type_info& from,
    const std::type_info& to) {
  return nullptr;
}

static bool BaseClass_isPublicBaseOfYourself(int32_t offset,
                                             const std::type_info& from) {
  assert(false);  // I have no bases.
  return true;
}

CustomTypeInfo BaseClass_typeinfo{
    BaseClass_convertToBase,
    BaseClass_maybeFromHasAPublicChildOfTypeTo,
    BaseClass_isPublicBaseOfYourself,
};

/////////////////////////////////////////////////////////////////////// Derived1
static void* Derived1_convertToBase(char* p, const std::type_info& to) {
  if (to == typeid(BaseClass)) return p + 16;
  return nullptr;
}
static void* Derived1_maybeFromHasAPublicChildOfTypeTo(
    char* p, int32_t offset, const std::type_info& from,
    const std::type_info& to) {
  return nullptr;
}

static bool Derived1_isPublicBaseOfYourself(int32_t offset,
                                            const std::type_info& from) {
  return true;  // I have only public bases.
}

CustomTypeInfo Derived1_typeinfo{
    Derived1_convertToBase,
    Derived1_maybeFromHasAPublicChildOfTypeTo,
    Derived1_isPublicBaseOfYourself,
};

/////////////////////////////////////////////////////////////////////// Derived2
static void* Derived2_convertToBase(char* p, const std::type_info& to) {
  if (to == typeid(BaseClass)) return p + 16;
  return nullptr;
}

static void* Derived2_maybeFromHasAPublicChildOfTypeTo(
    char* p, int32_t offset, const std::type_info& from,
    const std::type_info& to) {
  return nullptr;
}

static bool Derived2_isPublicBaseOfYourself(int32_t offset,
                                            const std::type_info& from) {
  return true;  // I have only public bases.
}

CustomTypeInfo Derived2_typeinfo{
    Derived2_convertToBase,
    Derived2_maybeFromHasAPublicChildOfTypeTo,
    Derived2_isPublicBaseOfYourself,
};

////////////////////////////////////////////////////////////////////
/// Derived1Derived2
static void* Derived1Derived2_convertToBase(char* p, const std::type_info& to) {
  if (to == typeid(Derived1)) return p + 0;
  if (to == typeid(Derived2)) return p + 16;
  if (to == typeid(BaseClass)) return p + 32;
  return nullptr;
}

static void* Derived1Derived2_maybeFromHasAPublicChildOfTypeTo(
    char* p, int32_t offset, const std::type_info& from,
    const std::type_info& to) {
  if (from == typeid(Derived1) && to == typeid(BaseClass)) return p + 32;
  if (from == typeid(Derived2) && to == typeid(BaseClass)) return p + 32;
  return nullptr;
}

static bool Derived1Derived2_isPublicBaseOfYourself(
    int32_t offset, const std::type_info& from) {
  return true;  // I have only public bases.
}

CustomTypeInfo Derived1Derived2_typeinfo{
    Derived1Derived2_convertToBase,
    Derived1Derived2_maybeFromHasAPublicChildOfTypeTo,
    Derived1Derived2_isPublicBaseOfYourself,
};

///////////////////////////////////////////////////////////////////// Derived2
static void* Derived3_convertToBase(char* p, const std::type_info& to) {
  // BaseClass is a protected base of Derived2, so it does not appear here.
  return nullptr;
}
static void* Derived3_maybeFromHasAPublicChildOfTypeTo(
    char* p, int32_t offset, const std::type_info& from,
    const std::type_info& to) {
  return nullptr;
}

static bool Derived3_isPublicBaseOfYourself(int32_t offset,
                                            const std::type_info& from) {
  return false;  // I have only non-public bases.
}

CustomTypeInfo Derived3_typeinfo{
    Derived3_convertToBase,
    Derived3_maybeFromHasAPublicChildOfTypeTo,
    Derived3_isPublicBaseOfYourself,
};

////////////////////////////////////////////////////////////////////// Derived4
static void* Derived4_convertToBase(char* p, const std::type_info& to) {
  if (to == typeid(BaseClass)) return p + 0;
  return nullptr;
}

static void* Derived4_maybeFromHasAPublicChildOfTypeTo(
    char* p, int32_t offset, const std::type_info& from,
    const std::type_info& to) {
  return nullptr;
}

static bool Derived4_isPublicBaseOfYourself(int32_t offset,
                                            const std::type_info& from) {
  return true;  // I have only public bases.
}

CustomTypeInfo Derived4_typeinfo{
    Derived4_convertToBase,
    Derived4_maybeFromHasAPublicChildOfTypeTo,
    Derived4_isPublicBaseOfYourself,
};

////////////////////////////////////////////////////////////////////// Derived5
static void* Derived5_convertToBase(char* p, const std::type_info& to) {
  if (to == typeid(Derived4)) return p + 0;
  if (to == typeid(Derived3)) return p + 16;
  // BaseClass is an ambiguous base, so it is not listed here.
  return nullptr;
}

static void* Derived5_maybeFromHasAPublicChildOfTypeTo(
    char* p, int32_t offset, const std::type_info& from,
    const std::type_info& to) {
  if (from == typeid(Derived4) && to == typeid(BaseClass) && offset == 0)
    return p + 0;
  return nullptr;
}

static bool Derived5_isPublicBaseOfYourself(int32_t offset,
                                            const std::type_info& from) {
  if (from == typeid(BaseClass) && offset == 0) return true;
  if (from == typeid(Derived4) && offset == 0) return true;
  if (from == typeid(BaseClass) && offset == 16) return false;
  if (from == typeid(Derived3) && offset == 16) return true;
  printf("unexpectedly %d %s\n", offset, from.name());
  assert(false);
  return false;
}

CustomTypeInfo Derived5_typeinfo{
    Derived5_convertToBase,
    Derived5_maybeFromHasAPublicChildOfTypeTo,
    Derived5_isPublicBaseOfYourself,
};

// This would be compiler generated if the dynamic-cast implementation looked
// like the one proposed here. For now we just manually implement it.
const CustomTypeInfo& awkward_typeinfo_conversion(const std::type_info& ti) {
#define CASE(Derived1) \
  if (ti == typeid(Derived1)) return Derived1##_typeinfo
  CASE(BaseClass);
  CASE(Derived1);
  CASE(Derived2);
  CASE(Derived1Derived2);
  CASE(Derived3);
  CASE(Derived4);
  CASE(Derived5);
  assert(false);
}
