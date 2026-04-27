CXX      = clang++
CXXFLAGS = -std=c++23 -O3 -Wall -Wextra
INCLUDES = -I.

# Locate google-benchmark via brew; override BENCHMARK_PREFIX if installed elsewhere.
BENCHMARK_PREFIX := $(shell brew --prefix google-benchmark 2>/dev/null)
ifeq ($(BENCHMARK_PREFIX),)
  $(error google-benchmark not found. Run: brew install google-benchmark)
endif
INCLUDES += -I$(BENCHMARK_PREFIX)/include
LDFLAGS   = -L$(BENCHMARK_PREFIX)/lib -lbenchmark -lbenchmark_main

SHARED_PTR_BIN = run_benchmarks_shared_ptr
SHARED_PTR_SRC = benchmark_shared_ptr.cpp binomial_heap_shared_ptr.cpp

REDESIGN_BIN = run_benchmarks_redesign
REDESIGN_SRC = benchmark_redesign.cpp binomial_heap_redesign.cpp

COMPARE_PY = $(BENCHMARK_PREFIX)/share/googlebenchmark/tools/compare.py

.PHONY: all clean compare

all: $(SHARED_PTR_BIN) $(REDESIGN_BIN)

$(SHARED_PTR_BIN): $(SHARED_PTR_SRC) binomial_heap_shared_ptr.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(SHARED_PTR_SRC) $(LDFLAGS)

$(REDESIGN_BIN): $(REDESIGN_SRC) binomial_heap_redesign.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(REDESIGN_SRC) $(LDFLAGS)

compare: $(SHARED_PTR_BIN) $(REDESIGN_BIN)
	./$(SHARED_PTR_BIN) --benchmark_out=shared_ptr.json --benchmark_out_format=json
	./$(REDESIGN_BIN)     --benchmark_out=redesign.json     --benchmark_out_format=json
	python3 $(COMPARE_PY) benchmarks shared_ptr.json redesign.json

clean:
	rm -f $(SHARED_PTR_BIN) $(REDESIGN_BIN) *.o shared_ptr.json redesign.json
