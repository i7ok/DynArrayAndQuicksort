#include <gtest/gtest.h>
#include <string>
#include "Array.h"

TEST(ArrayInt, InsertAndIndex) {
    Array<int> a;
    for (int i = 0; i < 100; ++i) a.insert(i);
    ASSERT_EQ(a.size(), 100);
    for (int i = 0; i < 100; ++i) ASSERT_EQ(a[i], i);
}

TEST(ArrayInt, InsertAtIndexShifts) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    int pos = a.insert(1, 99);
    ASSERT_EQ(pos, 1);
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(a[0], 1);
    ASSERT_EQ(a[1], 99);
    ASSERT_EQ(a[2], 2);
    ASSERT_EQ(a[3], 3);
}

TEST(ArrayInt, RemoveShiftsLeft) {
    Array<int> a;
    for (int i = 0; i < 10; ++i) a.insert(i);
    a.remove(5);
    ASSERT_EQ(a.size(), 9);
    for (int i = 0; i < 5; ++i) ASSERT_EQ(a[i], i);
    for (int i = 5; i < 9; ++i) ASSERT_EQ(a[i], i + 1);
}

TEST(ArrayInt, CopyCtorDeepCopy) {
    Array<int> a;
    for (int i = 0; i < 20; ++i) a.insert(i);

    Array<int> b(a); 
    ASSERT_EQ(b.size(), a.size());
    b[0] = 999;
    ASSERT_EQ(a[0], 0);
    ASSERT_EQ(b[0], 999);
}

TEST(ArrayInt, CopyAssignCopySwapSingleOperator) {
    Array<int> a;
    for (int i = 0; i < 20; ++i) a.insert(i);

    Array<int> b;
    b.insert(777);

    b = a; 
    ASSERT_EQ(b.size(), a.size());
    for (int i = 0; i < a.size(); ++i) ASSERT_EQ(b[i], a[i]);
}

TEST(ArrayInt, MoveAssignSingleOperator) {
    Array<int> a;
    for (int i = 0; i < 10; ++i) a.insert(i);

    Array<int> b;
    b = std::move(a);
    ASSERT_EQ(b.size(), 10);
    for (int i = 0; i < 10; ++i) ASSERT_EQ(b[i], i);
}

TEST(ArrayString, InsertRemoveShift) {
    Array<std::string> a;
    a.insert("a");
    a.insert("b");
    a.insert("c");
    a.insert(1, "X");
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(a[0], "a");
    ASSERT_EQ(a[1], "X");
    ASSERT_EQ(a[2], "b");
    ASSERT_EQ(a[3], "c");

    a.remove(2);
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(a[0], "a");
    ASSERT_EQ(a[1], "X");
    ASSERT_EQ(a[2], "c");
}

TEST(ArrayIterators, ForwardAndReverse) {
    Array<int> a;
    for (int i = 1; i <= 5; ++i) a.insert(i);

    int sumF = 0;
    for (auto it = a.iterator(); it.hasNext(); it.next()) sumF += it.get();
    ASSERT_EQ(sumF, 15);

    int sumR = 0;
    for (auto it = a.reverseIterator(); it.hasNext(); it.next()) sumR += it.get();
    ASSERT_EQ(sumR, 15);
}
