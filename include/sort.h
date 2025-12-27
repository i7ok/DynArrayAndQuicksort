#pragma once
#include <utility>  
#include <cstddef>   

template <typename T>
inline void move_swap(T& a, T& b) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

template <typename T, typename Compare>
void insertion_sort(T* first, T* last, Compare comp) {
    for (T* i = first + 1; i < last; ++i) {
        T key = std::move(*i);
        T* j = i;
        while (j > first && comp(key, *(j - 1))) {
            *j = std::move(*(j - 1));
            --j;
        }
        *j = std::move(key);
    }
}


template <typename T, typename Compare>
T* median_of_three(T* a, T* b, T* c, Compare comp) {
   
    if (comp(*a, *b)) {
        if (comp(*b, *c)) return b;      
        if (comp(*a, *c)) return c;      
        return a;                        
    }
    else {
        if (comp(*a, *c)) return a;     
        if (comp(*b, *c)) return c;      
        return b;                         
    }
}


template <typename T, typename Compare>
T* hoare_partition(T* first, T* last, Compare comp) {
    T* mid = first + (last - first) / 2;
    T* piv_ptr = median_of_three(first, mid, last - 1, comp);

    
    move_swap(*first, *piv_ptr);
    T& pivot = *first;

    T* i = first - 1;
    T* j = last;

    while (true) {
        do { ++i; } while (comp(*i, pivot));
        do { --j; } while (comp(pivot, *j));
        if (i >= j) return j;
        move_swap(*i, *j);
    }
}


template <typename T, typename Compare>
void sort(T* first, T* last, Compare comp, std::ptrdiff_t cutoff = 32) {
    while (last - first > 1) {
        const std::ptrdiff_t n = last - first;
        if (n <= cutoff) {
            insertion_sort(first, last, comp);
            return;
        }

        T* p = hoare_partition(first, last, comp); 
        T* left_first = first;
        T* left_last = p + 1;
        T* right_first = p + 1;
        T* right_last = last;

  
        if (left_last - left_first < right_last - right_first) {
            sort(left_first, left_last, comp, cutoff);
            first = right_first;
            last = right_last;
        }
        else {
            sort(right_first, right_last, comp, cutoff);
            first = left_first;
            last = left_last;
        }
    }
}


template <typename T, typename Compare>
void quick_sort_no_cutoff(T* first, T* last, Compare comp) {
    while (last - first > 1) {
        T* p = hoare_partition(first, last, comp);

        T* left_first = first;
        T* left_last = p + 1;
        T* right_first = p + 1;
        T* right_last = last;

        if (left_last - left_first < right_last - right_first) {
            quick_sort_no_cutoff(left_first, left_last, comp);
            first = right_first;
            last = right_last;
        }
        else {
            quick_sort_no_cutoff(right_first, right_last, comp);
            first = left_first;
            last = left_last;
        }
    }
}
