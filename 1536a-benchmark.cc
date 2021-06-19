#include <benchmark/benchmark.h>

#include "1536a-possible-solutions.c"

static AvlTree *GlobalAvlTree = NULL;

static void CustomArguments(benchmark::internal::Benchmark *b) {
  int i;
  for (i = 1; i <= (1 << 29); i <<= 1) {
    AvlTree_insert(&GlobalAvlTree, i);
    b->Args({(long int)GlobalAvlTree});
  }
  for (i = 1; i <= (1 << 29); i <<= 1) {
    AvlTree_insert(&GlobalAvlTree, -i);
    b->Args({(long int)GlobalAvlTree});
  }

  AvlTree_destroy(GlobalAvlTree);
  GlobalAvlTree = NULL;
}

static void BM_AvlTree_in_order(benchmark::State &state) {
  for (auto _ : state) {
    AvlTree_in_order((AvlTree *)state.range(0));
  }
}

BENCHMARK(BM_AvlTree_in_order)->Apply(CustomArguments);

static void BM_AvlTree_iterative_in_order(benchmark::State &state) {
  for (auto _ : state) {
    AvlTree_iterative_in_order((AvlTree *)state.range(0));
  }
}

BENCHMARK(BM_AvlTree_iterative_in_order)->Apply(CustomArguments);

BENCHMARK_MAIN();
