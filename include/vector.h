#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class Vector {
private:
    T* data;        // Указатель на динамически выделенный массив элементов
    size_t size_;   // Текущее количество элементов в векторе
    size_t capacity_; // Максимальное количество элементов, которое может храниться
    
    // Приватный метод для перевыделения памяти с новым capacity
    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];  // Выделяем новый массив
        
        // Перемещаем элементы из старого массива в новый
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data[i]); 
        }
        
        delete[] data;      // Освобождаем старый массив
        data = new_data;    // Обновляем указатель
        capacity_ = new_capacity;  // Обновляем емкость
    }
    
public:
    Vector() : data(nullptr), size_(0), capacity_(0) {}
    
    // Конструктор с указанием начального размера
    Vector(size_t initial_size) : size_(initial_size), capacity_(initial_size) {
        data = new T[capacity_];  // Выделяем память под указанное количество элементов
    }
    
    // Копирующий конструктор
    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_) {
        data = new T[capacity_];  // Выделяем память такого же размера
        for (size_t i = 0; i < size_; ++i) {
            data[i] = other.data[i];  // Копируем каждый элемент
        }
    }
    
    // Перемещающий конструктор
    Vector(Vector&& other) noexcept 
        : data(other.data), size_(other.size_), capacity_(other.capacity_) {
        // Обнуляем указатели исходного вектора, чтобы избежать двойного удаления
        other.data = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    // Копирующий оператор присваивания
    Vector& operator=(const Vector& other) {
        // Проверка на самоприсваивание
        if (this != &other) {
            delete[] data;  // Освобождаем текущую память
            
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = new T[capacity_];  // Выделяем новую память
            
            // Копируем элементы
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;  // Возвращаем ссылку на текущий объект
    }
    
    // Перемещающий оператор присваивания
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;  // Освобождаем текущую память
            
            // Перехватываем ресурсы другого вектора
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            // Обнуляем указатели исходного вектора
            other.data = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    ~Vector() {
        delete[] data;
    }
    
    // Методы доступа к элементам 
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        return data[index];  // Возвращаем ссылку на элемент
    }
    
    // Оператор для константного доступа 
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        return data[index];  // Возвращаем константную ссылку на элемент
    }
    
    // Добавление элемента в конец (для l-value)
    void push_back(const T& value) {
        // Проверяем, нужно ли увеличить емкость
        if (size_ >= capacity_) {
            // Стратегия увеличения: если capacity == 0, устанавливаем в 2,
            // иначе удваиваем 
            size_t new_capacity = capacity_ == 0 ? 2 : capacity_ * 2;
            reallocate(new_capacity);  // Перевыделяем память с новой емкостью
        }
        // Добавляем элемент в конец и увеличиваем размер
        data[size_++] = value;
    }
    
    // Добавление элемента в конец (для r-value - с перемещением)
    void push_back(T&& value) {
        if (size_ >= capacity_) {
            size_t new_capacity = capacity_ == 0 ? 2 : capacity_ * 2;
            reallocate(new_capacity);
        }
        // Используем перемещение вместо копирования для эффективности
        data[size_++] = std::move(value);
    }
    
    // Вставка элемента в произвольную позицию (для l-value)
    void insert(size_t pos, const T& value) {
        // Проверка корректности позиции
        if (pos > size_) {
            throw std::out_of_range("Позиция вставки вне диапазона");
        }
        
        // Проверяем, нужно ли увеличить емкость
        if (size_ >= capacity_) {
            size_t new_capacity = capacity_ == 0 ? 2 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        // Сдвигаем элементы вправо, начиная с конца, чтобы освободить место
        for (size_t i = size_; i > pos; --i) {
            data[i] = std::move(data[i - 1]);
        }
        
        // Вставляем новый элемент на освободившееся место
        data[pos] = value;
        ++size_;  // Увеличиваем размер
    }
    
    // Вставка элемента в произвольную позицию (для r-value)
    void insert(size_t pos, T&& value) {
        if (pos > size_) {
            throw std::out_of_range("Позиция вставки вне диапазона");
        }
        
        if (size_ >= capacity_) {
            size_t new_capacity = capacity_ == 0 ? 2 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        // Сдвигаем элементы вправо
        for (size_t i = size_; i > pos; --i) {
            data[i] = std::move(data[i - 1]);
        }
        
        // Вставляем новый элемент с перемещением
        data[pos] = std::move(value);
        ++size_;
    }
    
    // Удаление элемента по позиции
    void erase(size_t pos) {
        // Проверка корректности позиции
        if (pos >= size_) {
            throw std::out_of_range("Позиция удаления вне диапазона");
        }
        
        // Сдвигаем элементы влево, начиная с позиции после удаляемой
        for (size_t i = pos; i < size_ - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        
        --size_;
    }
    
    // Получение текущего количества элементов
    size_t size() const {
        return size_;
    }
    
    // Получение текущей емкости
    size_t capacity() const {
        return capacity_;
    }
    
    class iterator {
    private:
        T* ptr;  // Указатель на текущий элемент
        
    public:
        // Конструктор итератора
        iterator(T* p) : ptr(p) {}
        
        // Оператор разыменования - доступ к текущему элементу
        T& operator*() {
            return *ptr;
        }
        
        // Префиксный инкремент
        iterator& operator++() {
            ++ptr;  // Просто перемещаем указатель на следующий элемент
            return *this;
        }
        
        // Постфиксный инкремент 
        iterator operator++(int) {
            iterator temp = *this;  // Сохраняем текущее состояние
            ++ptr;                  // Перемещаем указатель
            return temp;            // Возвращаем старое состояние
        }
        
        // Операторы сравнения для использования в циклах
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
        
        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
    };
    
    // Метод для получения итератора на начало вектора
    iterator begin() {
        return iterator(data);
    }
    
    iterator end() {
        return iterator(data + size_);
    }
};

#endif