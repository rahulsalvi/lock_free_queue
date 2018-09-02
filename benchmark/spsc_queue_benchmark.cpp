#include <benchmark/benchmark.h>

#include "spsc_queue.h"

// clang-format off
#define ENQUEUE_ITERATIONS  1000000
#define ENQUEUE_REPETITIONS 50
#define DEQUEUE_ITERATIONS  1000000
#define DEQUEUE_REPETITIONS 50
// clang-format on

static void BM_Enqueue(benchmark::State& state) {
    int                                         buf[ENQUEUE_ITERATIONS];
    int                                         x = 0;
    rutils::spsc_queue<int, ENQUEUE_ITERATIONS> test_queue(buf);
    for (auto _ : state) { benchmark::DoNotOptimize(test_queue.enqueue(x)); }
}

BENCHMARK(BM_Enqueue)
    ->Iterations(ENQUEUE_ITERATIONS)
    ->Repetitions(ENQUEUE_REPETITIONS)
    ->ReportAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
        return *(std::max_element(std::begin(v), std::end(v)));
    });

static void BM_Dequeue(benchmark::State& state) {
    int                                         buf[DEQUEUE_ITERATIONS];
    int                                         x = 0;
    rutils::spsc_queue<int, DEQUEUE_ITERATIONS> test_queue(buf);
    for (int i = 0; i < DEQUEUE_ITERATIONS; i++) { test_queue.enqueue(x); }
    for (auto _ : state) {
        int x;
        benchmark::DoNotOptimize(test_queue.dequeue(x));
    }
}

BENCHMARK(BM_Dequeue)
    ->Iterations(DEQUEUE_ITERATIONS)
    ->Repetitions(DEQUEUE_REPETITIONS)
    ->ReportAggregatesOnly(true)
    ->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
        return *(std::max_element(std::begin(v), std::end(v)));
    });
