#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class Vector {
private:
    T* data;
    size_t size_;
    size_t capacity_;
    
    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        capacity_ = new_capacity;
    }
    
public:
    Vector() : data(nullptr), size_(0), capacity_(0) {}
    
    Vector(size_t initial_size) : size_(initial_size), capacity_(initial_size) {
        data = new T[capacity_];
    }
    
    // Копирующий конструктор
    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_) {
        data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // Перемещающий конструктор
    Vector(Vector&& other) noexcept 
        : data(other.data), size_(other.size_), capacity_(other.capacity_) {
        other.data = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    // Копирующее присваивание
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // Перемещающее присваивание
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    // Деструктор
    ~Vector() {
        delete[] data;
    }
    
    // Методы доступа
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    // Добавление в конец
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            size_t new_capacity = capacity_ == 0 ? 2 : capacity_ * 2;
            reallocate(new_capacity);
        }
        data[size_++] = value;
    }
    
    void push_back(T&& value) {
        if (size_ >= capacity_) {
            size_t new_capacity = capacity_ == 0 ? 2 : capacity_ * 2;
            reallocate(new_capacity);
        }
        data[size_++] = std::move(value);
    }
    
    // Вставка в произвольную позицию
    void insert(size_t pos, const T& value) {
        if (pos > size_) {
            throw std::out_of_range("Insert position out of range");
        }
        
        if (size_ >= capacity_) {
            size_t new_capacity = capacity_ == 0 ? 2 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        for (size_t i = size_; i > pos; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[pos] = value;
        ++size_;
    }
    
    void insert(size_t pos, T&& value) {
        if (pos > size_) {
            throw std::out_of_range("Insert position out of range");
        }
        
        if (size_ >= capacity_) {
            size_t new_capacity = capacity_ == 0 ? 2 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        for (size_t i = size_; i > pos; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[pos] = std::move(value);
        ++size_;
    }
    
    // Удаление элемента
    void erase(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Erase position out of range");
        }
        
        for (size_t i = pos; i < size_ - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --size_;
    }
    
    // Размер контейнера
    size_t size() const {
        return size_;
    }
    
    // Емкость контейнера
    size_t capacity() const {
        return capacity_;
    }
    
    // Итератор
    class iterator {
    private:
        T* ptr;
        
    public:
        iterator(T* p) : ptr(p) {}
        
        T& operator*() {
            return *ptr;
        }
        
        iterator& operator++() {
            ++ptr;
            return *this;
        }
        
        iterator operator++(int) {
            iterator temp = *this;
            ++ptr;
            return temp;
        }
        
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
        
        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
    };
    
    iterator begin() {
        return iterator(data);
    }
    
    iterator end() {
        return iterator(data + size_);
    }
};

#endif