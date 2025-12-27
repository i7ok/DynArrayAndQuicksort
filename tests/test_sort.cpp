#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "sort.h"
#include <algorithm>

TEST(SortInt, AlreadySorted) {
    std::vector<int> v{ 1,2,3,4,5,6 };
    sort(v.data(), v.data() + v.size(), [](int a, int b) { return a < b; }, 32);
    ASSERT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(SortInt, ReverseSorted) {
    std::vector<int> v{ 6,5,4,3,2,1 };
    sort(v.data(), v.data() + v.size(), [](int a, int b) { return a < b; }, 32);
    ASSERT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(SortInt, WithDuplicates) {
    std::vector<int> v{ 3,1,2,2,5,3,0,0,9 };
    sort(v.data(), v.data() + v.size(), [](int a, int b) { return a < b; }, 32);
    ASSERT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(SortInt, SmallSizes) {
    for (int n = 0; n <= 40; ++n) {
        std::vector<int> v;
        for (int i = n; i >= 1; --i) v.push_back(i); 
        sort(v.data(), v.data() + v.size(), [](int a, int b) { return a < b; }, 16);
        ASSERT_TRUE(std::is_sorted(v.begin(), v.end()));
    }
}

struct Person {
    std::string name;
    int age;
};

TEST(SortNonTrivial, StructByAgeThenName) {
    std::vector<Person> v{
        {"bob", 30}, {"ann", 20}, {"zoe", 20}, {"kim", 25}
    };

    sort(v.data(), v.data() + v.size(),
        [](const Person& a, const Person& b) {
            if (a.age != b.age) return a.age < b.age;
            return a.name < b.name;
        }, 8);

    for (size_t i = 1; i < v.size(); ++i) {
        auto& p = v[i - 1];
        auto& q = v[i];
        ASSERT_TRUE((p.age < q.age) || (p.age == q.age && p.name <= q.name));
    }
}
