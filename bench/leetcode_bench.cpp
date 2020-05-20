//
// Created by zhaoyongle on 2020/5/20.
//
#include "benchmark/benchmark.h"

#include "leetcode/leetcode.h"

static void BM_Array_twoSum1(benchmark::State& state) {
    for (auto _ : state)
        LeetCode::Array::TwoSum::one({2, 7, 11, 15}, 9);
}
BENCHMARK(BM_Array_twoSum1);

static void BM_Array_twoSum2(benchmark::State& state) {
    for (auto _ : state)
        LeetCode::Array::TwoSum::two({2, 7, 11, 15}, 9);
}
BENCHMARK(BM_Array_twoSum2);

BENCHMARK_MAIN();
