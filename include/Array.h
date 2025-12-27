#pragma once
#include <cstdlib>   
#include <new>       
#include <utility>   
#include <cassert>   
#include <type_traits>

template <typename T>
class Array final {
public:
    class Iterator {
    public:
        Iterator(T* data, int size, int pos, int step)
            : data_(data), size_(size), pos_(pos), step_(step) {
        }

        const T& get() const {
            assert(pos_ >= 0 && pos_ < size_);
            return data_[pos_];
        }

        void set(const T& value) {
            assert(pos_ >= 0 && pos_ < size_);
            data_[pos_] = value;
        }

        void next() { pos_ += step_; }

        bool hasNext() const {
            return (step_ > 0) ? (pos_ < size_) : (pos_ >= 0);
        }

    private:
        T* data_;
        int size_;
        int pos_;
        int step_;
    };

    class ConstIterator {
    public:
        ConstIterator(const T* data, int size, int pos, int step)
            : data_(data), size_(size), pos_(pos), step_(step) {
        }

        const T& get() const {
            assert(pos_ >= 0 && pos_ < size_);
            return data_[pos_];
        }

        void next() { pos_ += step_; }

        bool hasNext() const {
            return (step_ > 0) ? (pos_ < size_) : (pos_ >= 0);
        }

    private:
        const T* data_;
        int size_;
        int pos_;
        int step_;
    };

public:
    Array() : Array(kDefaultCapacity) {}

    explicit Array(int capacity)
        : data_(nullptr), size_(0), capacity_(capacity > 0 ? capacity : kDefaultCapacity) {
        data_ = static_cast<T*>(std::malloc(sizeof(T) * capacity_));
        if (!data_) throw std::bad_alloc();
    }

    
    Array(const Array& other) : Array(other.capacity_) {
        
        for (int i = 0; i < other.size_; ++i) {
            ::new (static_cast<void*>(data_ + i)) T(other.data_[i]);
        }
        size_ = other.size_;
    }

    
    Array(Array&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    
    Array& operator=(Array other) noexcept(std::is_nothrow_move_constructible_v<T>) {
        swap(other);
        return *this;
    }

    ~Array() {
        destroy_range(0, size_);
        std::free(data_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    int insert(const T& value) {
        return insert(size_, value);
    }

    int insert(int index, const T& value) {
        assert(index >= 0 && index <= size_);
        if (size_ >= capacity_) {
            reallocate(grow_capacity(capacity_));
        }

       
        
        for (int i = size_; i > index; --i) {
            
            ::new (static_cast<void*>(data_ + i)) T(std::move(data_[i - 1]));
            (data_ + (i - 1))->~T();
        }

        ::new (static_cast<void*>(data_ + index)) T(value);
        ++size_;
        return index;
    }

    void remove(int index) {
        assert(index >= 0 && index < size_);

        
        (data_ + index)->~T();

        
        for (int i = index; i < size_ - 1; ++i) {
            ::new (static_cast<void*>(data_ + i)) T(std::move(data_[i + 1]));
            (data_ + (i + 1))->~T();
        }
        --size_;
    }

    const T& operator[](int index) const {
        assert(index >= 0 && index < size_);
        return data_[index];
    }

    T& operator[](int index) {
        assert(index >= 0 && index < size_);
        return data_[index];
    }

    int size() const { return size_; }
    int capacity() const { return capacity_; }

    Iterator iterator() { return Iterator(data_, size_, 0, +1); }
    ConstIterator iterator() const { return ConstIterator(data_, size_, 0, +1); }

    Iterator reverseIterator() { return Iterator(data_, size_, size_ - 1, -1); }
    ConstIterator reverseIterator() const { return ConstIterator(data_, size_, size_ - 1, -1); }


    T* data() { return data_; }
    const T* data() const { return data_; }

    void swap(Array& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:
    static constexpr int kDefaultCapacity = 16;

    static int grow_capacity(int current) {
     
        int next = current > 0 ? current * 2 : kDefaultCapacity;
        if (next < kDefaultCapacity) next = kDefaultCapacity;
        return next;
    }

    void destroy_range(int from, int to) noexcept {
        for (int i = from; i < to; ++i) {
            (data_ + i)->~T();
        }
    }

    void reallocate(int new_capacity) {
        assert(new_capacity >= size_);
        T* new_data = static_cast<T*>(std::malloc(sizeof(T) * new_capacity));
        if (!new_data) throw std::bad_alloc();

        
        for (int i = 0; i < size_; ++i) {
            ::new (static_cast<void*>(new_data + i)) T(std::move(data_[i]));
        }

       
        destroy_range(0, size_);
        std::free(data_);

        data_ = new_data;
        capacity_ = new_capacity;
    }

private:
    T* data_;
    int size_;
    int capacity_;
};
