//
// Created by zhaoyongle on 2020/5/20.
//

#include "benchmark/benchmark.h"
#include <array>
#include <iostream>

constexpr int len = 1000;

auto args = [](benchmark::internal::Benchmark* bm) {
    for (int i = 10; i <= 20; ++i) {
        //bm->Args({i});
        bm->Arg(i);
    }
};

static void bench_array_operator(benchmark::State& state)
{
    std::array<int, len> arr{};
    //uint64_t counter = 0;
    while (state.KeepRunning()) {
        //++counter;
        for (int i = 0; i < state.range(0); ++i) {
            arr[i] = i + 1;
        }
    }

    //std::cout << "Output: " << counter << " Arg: " << state.range(0) << std::endl;
}
BENCHMARK(bench_array_operator)->Args({500})->Iterations(1000);
BENCHMARK(bench_array_operator)->Args({10})->Repetitions(10);
BENCHMARK(bench_array_operator)->Arg(100)->Arg(1000);
BENCHMARK(bench_array_operator)->Range(1, 1000);
BENCHMARK(bench_array_operator)->RangeMultiplier(2)->Range(1, 1000);
BENCHMARK(bench_array_operator)->Apply(args);

static void bench_array_at(benchmark::State& state)
{
    std::array<int, len> arr{};
    for (auto _: state) {
        for (int i = 0; i < state.range(0); ++i) {
            arr.at(0) = i + 1;
        }
    }
}
/*BENCHMARK(bench_array_operator)->Args({500})->Unit(benchmark::kNanosecond);
BENCHMARK(bench_array_operator)->Arg(100)->Arg(1000)->Unit(benchmark::kMicrosecond);
BENCHMARK(bench_array_operator)->Range(1, 1000)->Unit(benchmark::kMillisecond);
BENCHMARK(bench_array_operator)->RangeMultiplier(2)->Range(1, 1000);
BENCHMARK(bench_array_operator)->Apply(args);*/

BENCHMARK_MAIN();