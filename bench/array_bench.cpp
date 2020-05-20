//
// Created by zhaoyongle on 2020/5/20.
//

#include "benchmark/benchmark.h"
#include <array>

constexpr int len = 6;

static void bench_array_operator(benchmark::State& state)
{
    std::array<int, 6> arr;
    constexpr int i = 1;
    for (auto _: state) {
        arr[0] = i;
        arr[1] = i + 1;
        arr[2] = i + 2;
        arr[3] = i + 3;
        arr[4] = i + 4;
        arr[5] = i + 5;
    }
}
BENCHMARK(bench_array_operator);

static void bench_array_at(benchmark::State& state)
{
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _: state) {
        arr.at(0) = i;
        arr.at(1) = i + 1;
        arr.at(2) = i + 2;
        arr.at(3) = i + 3;
        arr.at(4) = i + 4;
        arr.at(5) = i + 5;
    }
}
BENCHMARK(bench_array_at);

static void bench_array_get(benchmark::State& state)
{
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _: state) {
        std::get<0>(arr) = i;
        std::get<1>(arr) = i + 1;
        std::get<2>(arr) = i + 2;
        std::get<3>(arr) = i + 3;
        std::get<4>(arr) = i + 4;
        std::get<5>(arr) = i + 5;
    }
}
BENCHMARK(bench_array_get)->Iterations(1000000000);

BENCHMARK_MAIN();