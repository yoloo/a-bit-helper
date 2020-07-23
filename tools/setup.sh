#!/bin/bash

git submodule init
git submodule update

# google-benchmark编译不过需要手动处理

#1.3party/gbenchmark/test/cxx03_test.cc:8:2: error: #error C++11 or greater detected. Should be C++03.
#  #error C++11 or greater detected. Should be C++03.

#2.3party/gbenchmark/test/cxx03_test.cc:12:2: error: #error C++11 or greater detected by the library. BENCHMARK_HAS_CXX11 is defined.
#  #error C++11 or greater detected by the library. BENCHMARK_HAS_CXX11 is defined.
