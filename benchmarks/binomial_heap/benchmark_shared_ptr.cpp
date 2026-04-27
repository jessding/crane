#include <benchmark/benchmark.h>

#include "binomial_heap_shared_ptr.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Build a heap by inserting n elements (values 0..n-1).
static BinomialHeap::priqueue build_heap(int n) {
    auto q = BinomialHeap::empty;
    for (int i = 0; i < n; ++i)
        q = BinomialHeap::insert(static_cast<unsigned int>(i), q);
    return q;
}

// ---------------------------------------------------------------------------
// Benchmarks for BinomialHeap (shared_ptr / baseline generated code)
// ---------------------------------------------------------------------------

// Insert n elements one by one into an initially empty heap.
static void BM_Insert(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        auto q = build_heap(n);
        benchmark::DoNotOptimize(q);
    }
    state.SetItemsProcessed(state.iterations() * n);
}

// Merge two heaps of n/2 elements each.
static void BM_Merge(benchmark::State& state) {
    const int n = state.range(0);
    auto p = build_heap(n / 2);
    auto q = build_heap(n / 2);
    for (auto _ : state) {
        auto merged = BinomialHeap::merge(p, q);
        benchmark::DoNotOptimize(merged);
    }
    state.SetItemsProcessed(state.iterations() * n);
}

// find_max on a heap of n elements.
static void BM_FindMax(benchmark::State& state) {
    const int n = state.range(0);
    auto q = build_heap(n);
    for (auto _ : state) {
        auto result = BinomialHeap::find_max(q);
        benchmark::DoNotOptimize(result);
    }
    state.SetItemsProcessed(state.iterations());
}

// Repeatedly delete_max until the heap is empty (measures full drain).
static void BM_DeleteMax(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        auto q = build_heap(n);
        state.ResumeTiming();

        while (true) {
            auto result = BinomialHeap::delete_max(q);
            if (!result.has_value()) break;
            q = std::move(result->second);
        }
        benchmark::DoNotOptimize(q);
    }
    state.SetItemsProcessed(state.iterations() * n);
}

// Full cycle: insert n elements then drain via delete_max.
static void BM_InsertDeleteCycle(benchmark::State& state) {
    const int n = state.range(0);
    for (auto _ : state) {
        auto q = build_heap(n);
        while (true) {
            auto result = BinomialHeap::delete_max(q);
            if (!result.has_value()) break;
            q = std::move(result->second);
        }
        benchmark::DoNotOptimize(q);
    }
    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_Insert)->Range(8, 1 << 12);
BENCHMARK(BM_Merge)->Range(8, 1 << 12);
BENCHMARK(BM_FindMax)->Range(8, 1 << 12);
// Capped at 1<<10 to match the redesign's range limit, keeping comparison apples-to-apples.
BENCHMARK(BM_DeleteMax)->Range(8, 1 << 10);
BENCHMARK(BM_InsertDeleteCycle)->Range(8, 1 << 10);

BENCHMARK_MAIN();
