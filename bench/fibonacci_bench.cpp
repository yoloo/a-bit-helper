//
// Created by lecky on 2020/5/20.
//

#include "benchmark/benchmark.h"

#include <algorithm>
#include <ostream>

uint64_t finonacci(uint64_t limit)
{
    if (0 == limit)
        return 0;

    if (1 == limit)
        return 1;

    return finonacci(limit - 1) + finonacci(limit - 2);
}

static void BM_Finonacci_2(benchmark::State& state)
{
    uint64_t ret = 0;
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(ret = finonacci(state.range(0)));
    }

    std::ostream cnull(nullptr);
    cnull << ret;
}
BENCHMARK(BM_Finonacci_2)->RangeMultiplier(2)->Range(1, 1 << 5);

BENCHMARK_MAIN();