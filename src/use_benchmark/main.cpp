#include <benchmark/benchmark.h>

int main(int argc, char* argv[]) {
	// google benchmark
	::benchmark::Initialize(&argc, argv);
	::benchmark::RunSpecifiedBenchmarks();

	return 0;
}

static void benchmark_test(benchmark::State& state) {
	for (auto _ : state) {
		std::string str{ "hello world" };
	}
}

BENCHMARK(benchmark_test);
