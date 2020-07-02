//
// Created by lecky on 2020/5/20.
//

#include "benchmark/benchmark.h"

#include <algorithm>

size_t fibonacci(size_t n)
{
    if (!n)
        return 0;

    size_t a = 1;
    size_t b = 1;
    for (int i = 0; i < n; ++i) {
        std::swap(a, b);

        b += a;
    }

    return b;
}

static void BM_Fibonacci(benchmark::State& state)
{
    size_t ret = 0;
    while (state.KeepRunning())
    {
        ret |= fibonacci(state.range(0));
    }
}
BENCHMARK(BM_Fibonacci)->Arg(5)->Arg(42)->Arg(87)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();