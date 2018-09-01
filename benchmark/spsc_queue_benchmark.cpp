#include <benchmark/benchmark.h>

#include "spsc_queue.h"

static void BM_BaseCase(benchmark::State& state) {
    rutils::spsc_queue<int, 10> test_queue(NULL);
    int                         x = 0;
    for (auto _ : state) { test_queue.enqueue(x); }
}

BENCHMARK(BM_BaseCase);
