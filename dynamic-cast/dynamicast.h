#pragma once

#include <functional>
#include <type_traits>
#include <typeinfo>

struct CustomTypeInfo {
  std::function<void *(char *p, const std::type_info &to)> convertToBase_;

  std::function<void *(char *p, int32_t offset, const std::type_info &from,
                       const std::type_info &to)>
      maybeFromHasAPublicChildOfTypeTo_;

  std::function<bool(int32_t offset, const std::type_info &from)>
      isPublicBaseOfYourself;

  inline void *convertToBase(void *p, const std::type_info &to) const {
    return convertToBase_(reinterpret_cast<char *>(p), to);
  }

  inline void *maybeFromHasAPublicChildOfTypeTo(
      void *p, int32_t offset, const std::type_info &from,
      const std::type_info &to) const {
    return maybeFromHasAPublicChildOfTypeTo_(reinterpret_cast<char *>(p),
                                             offset, from, to);
  }
};

void *dynamicast_to_mdo(void *p);

const std::type_info &dynamicast_typeid(void *mdo);

const CustomTypeInfo &awkward_typeinfo_conversion(const std::type_info &);

#if defined(_MSC_VER)
#include "dynamicast-msvc.h"
#else
#include "dynamicast-itanium.h"
#endif
