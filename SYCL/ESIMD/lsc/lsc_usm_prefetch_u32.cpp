//==------------ lsc_usm_prefetch_u32.cpp - DPC++ ESIMD on-device test -----==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// REQUIRES: gpu-intel-pvc || esimd_emulator
// UNSUPPORTED: cuda || hip
// RUN: %clangxx -fsycl %s -o %t.out
// RUN: %GPU_RUN_PLACEHOLDER %t.out

#include "Inputs/lsc_usm_load.hpp"

constexpr uint32_t seed = 188;
constexpr lsc_data_size DS = lsc_data_size::u32;

constexpr cache_hint L1H = cache_hint::cached;
constexpr cache_hint L3H = cache_hint::uncached;

template <int TestCastNum, typename T> bool tests() {
  bool passed = true;
  // non transpose
  passed &=
      test<TestCastNum, T, 1, 4, 32, 1, false, DS, L1H, L3H, true>(rand());
  passed &=
      test<TestCastNum + 1, T, 1, 4, 32, 2, false, DS, L1H, L3H, true>(rand());
  passed &=
      test<TestCastNum + 2, T, 1, 4, 16, 2, false, DS, L1H, L3H, true>(rand());
  passed &=
      test<TestCastNum + 3, T, 1, 4, 4, 1, false, DS, L1H, L3H, true>(rand());
  passed &= test<TestCastNum + 4, T, 1, 1, 1, 1, false, DS, L1H, L3H, true>(1);
  passed &= test<TestCastNum + 5, T, 2, 1, 1, 1, false, DS, L1H, L3H, true>(1);

  // transpose
  passed &= test<TestCastNum + 8, T, 1, 4, 1, 32, true, DS, L1H, L3H, true>();
  passed &= test<TestCastNum + 9, T, 2, 2, 1, 16, true, DS, L1H, L3H, true>();
  passed &= test<TestCastNum + 10, T, 4, 4, 1, 4, true, DS, L1H, L3H, true>();

  return passed;
}

int main(void) {
  srand(seed);
  bool passed = true;

  passed &= tests<0, uint32_t>();
  passed &= tests<11, float>();
  passed &= tests<22, sycl::ext::intel::experimental::esimd::tfloat32>();

  std::cout << (passed ? "Passed\n" : "FAILED\n");
  return passed ? 0 : 1;
}
