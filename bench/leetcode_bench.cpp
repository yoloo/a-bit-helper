//
// Created by zhaoyongle on 2020/5/20.
//
#include "benchmark/benchmark.h"

#include "leetcode/leetcode.h"

static void BM_Array_twoSum1(benchmark::State& state) {
    for (auto _ : state)
        LeetCode::Array::twoSum_1({2, 7, 11, 15}, 9);
}
BENCHMARK(BM_Array_twoSum1)->Iterations(1000);

static void BM_Array_twoSum2(benchmark::State& state) {
    for (auto _ : state)
        LeetCode::Array::twoSum_2({2, 7, 11, 15}, 9);
}
BENCHMARK(BM_Array_twoSum2)->Iterations(1000);

BENCHMARK_MAIN();
