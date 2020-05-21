//
// Created by zhaoyongle on 2020/5/20.
//
#include "benchmark/benchmark.h"
#include "leetcode/leetcode.h"

#include <iostream>

constexpr int from   = 1;
constexpr int to     = 1000;
constexpr int target = (from + to) / 2;

std::vector<int> nums{};

auto random_nums = []() {
    if (nums.empty()) {
        for (int i = from; i <= to; ++i) {
            nums.push_back(i);
        }
    }

    std::random_shuffle(nums.begin(), nums.end());
};

static void BM_Array_twoSum1(benchmark::State& state) {
    random_nums();

    /*std::cout << "target: " << target << std::endl;
    std::for_each(nums.begin(), nums.end(), [](int item) {
       std::cout << item << std::endl;
    });*/

    while (state.KeepRunning()) {
        LeetCode::Array::TwoSum::one(nums, target);
    }
}
//BENCHMARK(BM_Array_twoSum1)->Repetitions(3)->Iterations(2)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Array_twoSum1);

static void BM_Array_twoSum2(benchmark::State& state) {
    random_nums();

    while (state.KeepRunning()) {
        LeetCode::Array::TwoSum::two(nums, target);
    }
}
BENCHMARK(BM_Array_twoSum2);

BENCHMARK_MAIN();
