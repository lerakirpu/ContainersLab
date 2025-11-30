// forward_list.h
#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class ForwardList {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& value) : data(value), next(nullptr) {}
        Node(T&& value) : data(std::move(value)), next(nullptr) {}
    };
    
    Node* head;
    size_t size_;
    
public:
    ForwardList() : head(nullptr), size_(0) {}
    
    // Копирующий конструктор
    ForwardList(const ForwardList& other) : head(nullptr), size_(0) {
        Node* current = other.head;
        Node* prev = nullptr;
        while (current) {
            Node* new_node = new Node(current->data);
            if (!head) {
                head = new_node;
            } else {
                prev->next = new_node;
            }
            prev = new_node;
            current = current->next;
            ++size_;
        }
    }
    
    // Перемещающий конструктор
    ForwardList(ForwardList&& other) noexcept 
        : head(other.head), size_(other.size_) {
        other.head = nullptr;
        other.size_ = 0;
    }
    
    // Копирующее присваивание
    ForwardList& operator=(const ForwardList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            Node* prev = nullptr;
            while (current) {
                Node* new_node = new Node(current->data);
                if (!head) {
                    head = new_node;
                } else {
                    prev->next = new_node;
                }
                prev = new_node;
                current = current->next;
                ++size_;
            }
        }
        return *this;
    }
    
    // Перемещающее присваивание
    ForwardList& operator=(ForwardList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            size_ = other.size_;
            other.head = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Деструктор
    ~ForwardList() {
        clear();
    }
    
    // Очистка списка
    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size_ = 0;
    }
    
    // Добавление в конец
    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = new_node;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
        ++size_;
    }
    
    void push_back(T&& value) {
        Node* new_node = new Node(std::move(value));
        if (!head) {
            head = new_node;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
        ++size_;
    }
    
    // Добавление в начало
    void push_front(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head;
        head = new_node;
        ++size_;
    }
    
    void push_front(T&& value) {
        Node* new_node = new Node(std::move(value));
        new_node->next = head;
        head = new_node;
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
        
        Node* current = head;
        for (size_t i = 0; i < pos - 1; ++i) {
            current = current->next;
        }
        
        Node* new_node = new Node(value);
        new_node->next = current->next;
        current->next = new_node;
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
        
        Node* current = head;
        for (size_t i = 0; i < pos - 1; ++i) {
            current = current->next;
        }
        
        Node* new_node = new Node(std::move(value));
        new_node->next = current->next;
        current->next = new_node;
        ++size_;
    }
    
    // Удаление элемента
    void erase(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Erase position out of range");
        }
        
        if (pos == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        } else {
            Node* current = head;
            for (size_t i = 0; i < pos - 1; ++i) {
                current = current->next;
            }
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
        }
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