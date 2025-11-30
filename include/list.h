#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class List {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
        Node(T&& value) : data(std::move(value)), prev(nullptr), next(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    size_t size_;
    
public:
    List() : head(nullptr), tail(nullptr), size_(0) {}
    
    // Копирующий конструктор
    List(const List& other) : head(nullptr), tail(nullptr), size_(0) {
        Node* current = other.head;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
    }
    
    // Перемещающий конструктор
    List(List&& other) noexcept 
        : head(other.head), tail(other.tail), size_(other.size_) {
        other.head = nullptr;
        other.tail = nullptr;
        other.size_ = 0;
    }
    
    // Копирующее присваивание
    List& operator=(const List& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }
    
    // Перемещающее присваивание
    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            size_ = other.size_;
            other.head = nullptr;
            other.tail = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Деструктор
    ~List() {
        clear();
    }
    
    // Очистка списка
    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size_ = 0;
    }
    
    // Добавление в конец
    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        ++size_;
    }
    
    void push_back(T&& value) {
        Node* new_node = new Node(std::move(value));
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        ++size_;
    }
    
    // Добавление в начало
    void push_front(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        ++size_;
    }
    
    void push_front(T&& value) {
        Node* new_node = new Node(std::move(value));
        if (!head) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        ++size_;
    }
    
    // Вставка в произвольную позицию
    void insert(size_t pos, const T& value) {
        if (pos > size_) {
            throw std::out_of_range("Insert position out of range");
        }
        
        if (pos == 0) {
            push_front(value);
            return;
        }
        
        if (pos == size_) {
            push_back(value);
            return;
        }
        
        Node* current = head;
        for (size_t i = 0; i < pos; ++i) {
            current = current->next;
        }
        
        Node* new_node = new Node(value);
        new_node->prev = current->prev;
        new_node->next = current;
        current->prev->next = new_node;
        current->prev = new_node;
        ++size_;
    }
    
    void insert(size_t pos, T&& value) {
        if (pos > size_) {
            throw std::out_of_range("Insert position out of range");
        }
        
        if (pos == 0) {
            push_front(std::move(value));
            return;
        }
        
        if (pos == size_) {
            push_back(std::move(value));
            return;
        }
        
        Node* current = head;
        for (size_t i = 0; i < pos; ++i) {
            current = current->next;
        }
        
        Node* new_node = new Node(std::move(value));
        new_node->prev = current->prev;
        new_node->next = current;
        current->prev->next = new_node;
        current->prev = new_node;
        ++size_;
    }
    
    // Удаление элемента
    void erase(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Erase position out of range");
        }
        
        Node* current = head;
        for (size_t i = 0; i < pos; ++i) {
            current = current->next;
        }
        
        if (current->prev) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }
        
        if (current->next) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }
        
        delete current;
        --size_;
    }
    
    // Размер контейнера
    size_t size() const {
        return size_;
    }
    
    // Итератор
    class iterator {
    private:
        Node* current;
        
    public:
        iterator(Node* node) : current(node) {}
        
        T& operator*() {
            return current->data;
        }
        
        iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        iterator operator++(int) {
            iterator temp = *this;
            if (current) {
                current = current->next;
            }
            return temp;
        }
        
        iterator& operator--() {
            if (current) {
                current = current->prev;
            }
            return *this;
        }
        
        iterator operator--(int) {
            iterator temp = *this;
            if (current) {
                current = current->prev;
            }
            return temp;
        }
        
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
        
        bool operator==(const iterator& other) const {
            return current == other.current;
        }
    };
    
    iterator begin() {
        return iterator(head);
    }
    
    iterator end() {
        return iterator(nullptr);
    }
};

#endif