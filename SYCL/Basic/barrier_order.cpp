// UNSUPPORTED: hip
// RUN: %clangxx -fsycl -fsycl-targets=%sycl_triple %s -o %t.out
// RUN: %CPU_RUN_PLACEHOLDER %t.out
// RUN: %GPU_RUN_PLACEHOLDER %t.out

#include <iostream>
#include <stdlib.h>
#include <sycl/sycl.hpp>

int main() {
  sycl::device dev{sycl::default_selector_v};
  sycl::queue q{dev};

  int *x = sycl::malloc_shared<int>(1, q);
  int *y = sycl::malloc_shared<int>(1, q);
  *x = 0;
  *y = 0;

  q.single_task<class kernel1>([=] { *x = 1; });

  q.ext_oneapi_submit_barrier();

  q.single_task<class kernel2>([=] {
    if (*x == 1) {
      *y = 2;
    }
  });

  q.wait_and_throw();

  int error = (*x != 1 || *y != 2) ? 1 : 0;
  std::cout << (error ? "failed\n" : "passed\n");

  sycl::free(x, q);
  sycl::free(y, q);

  return error;
}
