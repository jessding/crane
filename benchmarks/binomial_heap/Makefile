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

SEMANTIC_PTR_BIN = run_benchmarks_semantic_ptr
SEMANTIC_PTR_SRC = benchmark_semantic_ptr.cpp binomial_heap_semantic_ptr.cpp

REDESIGN_BIN = run_benchmarks_redesign
REDESIGN_SRC = benchmark_redesign.cpp binomial_heap_redesign.cpp

COMPARE_PY = $(BENCHMARK_PREFIX)/share/googlebenchmark/tools/compare.py

.PHONY: all clean compare

all: $(SEMANTIC_PTR_BIN) $(REDESIGN_BIN)

$(SEMANTIC_PTR_BIN): $(SEMANTIC_PTR_SRC) binomial_heap_semantic_ptr.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(SEMANTIC_PTR_SRC) $(LDFLAGS)

$(REDESIGN_BIN): $(REDESIGN_SRC) binomial_heap_redesign.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(REDESIGN_SRC) $(LDFLAGS)

compare: $(SEMANTIC_PTR_BIN) $(REDESIGN_BIN)
	./$(SEMANTIC_PTR_BIN) --benchmark_out=semantic_ptr.json --benchmark_out_format=json
	./$(REDESIGN_BIN)     --benchmark_out=redesign.json     --benchmark_out_format=json
	python3 $(COMPARE_PY) benchmarks semantic_ptr.json redesign.json

clean:
	rm -f $(SEMANTIC_PTR_BIN) $(REDESIGN_BIN) *.o semantic_ptr.json redesign.json
