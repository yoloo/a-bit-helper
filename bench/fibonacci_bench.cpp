//
// Created by lecky on 2020/5/20.
//

#include "benchmark/benchmark.h"

#include <cmath>
#include <algorithm>
#include <ostream>

uint64_t fibonacci_1(uint64_t limit)
{
    if (!limit)
        return 0;

    uint64_t fir = 1;
    uint64_t sec = 1;
    for (int i = 0; i < limit; ++i) {
        std::swap(fir, sec);

        sec += fir;
    }

    return sec;
}

uint64_t finonacci_2(uint64_t limit)
{
    if (0 == limit)
        return 0;

    if (1 == limit)
        return 1;

    return finonacci_2(limit - 1) + finonacci_2(limit - 2);
}

static void BM_Fibonacci_1(benchmark::State& state)
{
    uint64_t ret = 0;
    while (state.KeepRunning())
    {
        ret |= fibonacci_1(state.range(0));
    }
}
BENCHMARK(BM_Fibonacci_1)->RangeMultiplier(2)->Range(1, 1 << 5);

static void BM_Finonacci_2(benchmark::State& state)
{
    uint64_t ret = 0;
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(ret = finonacci_2(state.range(0)));
    }

    std::ostream cnull(nullptr);
    cnull << ret;
}
BENCHMARK(BM_Finonacci_2)->RangeMultiplier(2)->Range(1, 1 << 5);

BENCHMARK_MAIN();