#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <random>
#include <algorithm>
#include "sort.h"

using SteadyClock = std::chrono::steady_clock;

enum class DataKind {
    Descending,
    Ascending,
    Random,
    AlmostSorted,
    FewUnique
};

static const char* kind_name(DataKind k) {
    switch (k) {
    case DataKind::Descending:   return "descending";
    case DataKind::Ascending:    return "ascending";
    case DataKind::Random:       return "random";
    case DataKind::AlmostSorted: return "almost_sorted";
    case DataKind::FewUnique:    return "few_unique";
    default: return "unknown";
    }
}

static std::vector<int> make_data(int n, DataKind kind, std::mt19937& rng) {
    std::vector<int> v(n);

    if (kind == DataKind::Descending) {
        for (int i = 0; i < n; ++i) v[i] = n - i;
        return v;
    }

    if (kind == DataKind::Ascending) {
        for (int i = 0; i < n; ++i) v[i] = i + 1;
        return v;
    }

    if (kind == DataKind::Random) {
        std::uniform_int_distribution<int> dist(0, n * 10);
        for (int i = 0; i < n; ++i) v[i] = dist(rng);
        return v;
    }

    if (kind == DataKind::AlmostSorted) {
       
        for (int i = 0; i < n; ++i) v[i] = i + 1;
        if (n >= 2) {
            std::uniform_int_distribution<int> idx(0, n - 1);
            int swaps = std::max(1, n / 20); 
            for (int s = 0; s < swaps; ++s) {
                int a = idx(rng), b = idx(rng);
                std::swap(v[a], v[b]);
            }
        }
        return v;
    }

    if (kind == DataKind::FewUnique) {
        
        std::uniform_int_distribution<int> dist(0, 4); 
        for (int i = 0; i < n; ++i) v[i] = dist(rng);
        return v;
    }

    return v;
}

template <typename Func>
static double mean_ms_10(Func f) {
    std::vector<double> samples;
    samples.reserve(10);
    for (int k = 0; k < 10; ++k) {
        auto t0 = SteadyClock::now();
        f();
        auto t1 = SteadyClock::now();
        samples.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
    }
    double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
    return sum / samples.size();
}

static bool is_sorted_vec(const std::vector<int>& v) {
    return std::is_sorted(v.begin(), v.end());
}

int main() {
    std::mt19937 rng(123456); 

    std::vector<DataKind> kinds = {
        DataKind::Descending,
        DataKind::Random,
        DataKind::Ascending,
        DataKind::AlmostSorted,
        DataKind::FewUnique
    };

    std::cout << "Benchmark: compare insertion sort vs quicksort (no cutoff)\n";
    std::cout << "Each measurement = mean of 10 runs. Arrays generated per kind.\n\n";

    for (auto kind : kinds) {
        std::cout << "=== Data: " << kind_name(kind) << " ===\n";
        std::cout << "  N | insertion(ms) | quick(ms) | insertion_better\n";
        std::cout << "----+--------------+----------+-----------------\n";

        int first_quick_faster = -1;

        for (int n = 5; n <= 200; ++n) {
            auto base = make_data(n, kind, rng);

            double t_ins = mean_ms_10([&] {
                auto v = base;
                insertion_sort(v.data(), v.data() + v.size(), [](int a, int b) { return a < b; });
                if (!is_sorted_vec(v)) std::abort();
                });

            double t_qs = mean_ms_10([&] {
                auto v = base;
                quick_sort_no_cutoff(v.data(), v.data() + v.size(), [](int a, int b) { return a < b; });
                if (!is_sorted_vec(v)) std::abort();
                });

            bool ins_better = (t_ins <= t_qs);
            if (first_quick_faster < 0 && t_qs < t_ins) first_quick_faster = n;

            std::cout << std::setw(3) << n
                << " | " << std::setw(12) << t_ins
                << " | " << std::setw(8) << t_qs
                << " | " << (ins_better ? "YES" : "no")
                << "\n";
        }

        std::cout << "\nFirst N where quick becomes faster: " << first_quick_faster
            << " (insertion tends to be better for smaller N)\n\n";
    }

   
    std::cout << "=== Hybrid quicksort (with cutoff) tuning on DESCENDING, N=200 ===\n";
    int N = 200;
    auto base = make_data(N, DataKind::Descending, rng);

    double best = 1e100;
    int best_cut = 0;

    for (int cut = 4; cut <= 64; cut += 2) {
        double t = mean_ms_10([&] {
            auto v = base;
            sort(v.data(), v.data() + v.size(), [](int a, int b) { return a < b; }, cut);
            if (!is_sorted_vec(v)) std::abort();
            });
        if (t < best) { best = t; best_cut = cut; }
        std::cout << "cutoff=" << std::setw(2) << cut << " => " << t << " ms\n";
    }

    std::cout << "\nBest cutoff: " << best_cut << " (ms=" << best << ")\n";
    return 0;
}
