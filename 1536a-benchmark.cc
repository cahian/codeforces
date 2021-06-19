#include <benchmark/benchmark.h>

#include <vector>

#include "1536a-possible-solutions.c"

static std::vector<AvlTree *> custom_args;
static int custom_args_size = 128;
static int custom_args_step = 128;

static void InitializeCustomArguments() {
  custom_args.reserve(custom_args_size);
  for (int i = 0; i < custom_args_size; ++i) {
    AvlTree *avl_tree = NULL;
    for (int j = 0; j < (custom_args_step * i); j += custom_args_step) {
      AvlTree_insert(&avl_tree, j);
    }
    custom_args.push_back(avl_tree);
  }
}

static void DestroyCustomArguments() {
  for (AvlTree *avl_tree : custom_args) {
    AvlTree_destroy(avl_tree);
  }
}

static void BM_AvlTree_in_order(benchmark::State &state) {
  for (auto _ : state) {
    AvlTree_in_order(custom_args[state.range(0)]);
  }
}

BENCHMARK(BM_AvlTree_in_order)->DenseRange(0, custom_args_size - 1, 1);

static void BM_AvlTree_iterative_in_order(benchmark::State &state) {
  for (auto _ : state) {
    AvlTree_iterative_in_order(custom_args[state.range(0)]);
  }
}

BENCHMARK(BM_AvlTree_iterative_in_order)
    ->DenseRange(0, custom_args_size - 1, 1);

int main(int argc, char **argv) {
  InitializeCustomArguments();
  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
    return 1;
  }
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  DestroyCustomArguments();
  return 0;
}
