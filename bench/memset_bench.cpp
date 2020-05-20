//
// Created by zhaoyongle on 2019/8/13.
//

#include "benchmark/benchmark.h"

#include <iostream>
#include <cstring>

static void BM_memset(benchmark::State& state) {
    char* buffer = (char*)malloc(state.range(0));
    for (auto _ : state)
        memset(buffer, 0x00, state.range(0));
}
BENCHMARK(BM_memset)->Arg(128*1024)->Arg(1024*1024);
BENCHMARK_MAIN();