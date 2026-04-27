CXX      = clang++
CXXFLAGS = -std=c++23 -O3 -Wall -Wextra
INCLUDES = -I.

# Locate google-benchmark via brew; override BENCHMARK_PREFIX if installed elsewhere.
BENCHMARK_PREFIX := $(shell brew --prefix google-benchmark 2>/dev/null)
ifeq ($(BENCHMARK_PREFIX),)
  $(error google-benchmark not found. Run: brew install google-benchmark)
endif
INCLUDES  += -I$(BENCHMARK_PREFIX)/include
LDFLAGS    = -L$(BENCHMARK_PREFIX)/lib -lbenchmark -lbenchmark_main

BENCHMARK_BIN = run_benchmarks
BENCHMARK_SRC = benchmark_driver.cpp binomial_heap_semantic_ptr.cpp

.PHONY: all clean

all: $(BENCHMARK_BIN)

$(BENCHMARK_BIN): $(BENCHMARK_SRC) binomial_heap_semantic_ptr.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(BENCHMARK_SRC) $(LDFLAGS)

clean:
	rm -f $(BENCHMARK_BIN) *.o
