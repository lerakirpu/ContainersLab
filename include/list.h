#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class List {
private:
    struct Node {
        T data;         // Данные, хранящиеся в узле
        Node* prev;     // Указатель на предыдущий узел 
        Node* next;     // Указатель на следующий узел
        
        // Конструктор для l-value ссылок (обычных объектов)
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
        
        // Конструктор для r-value ссылок (временных объектов) с перемещением
        Node(T&& value) : data(std::move(value)), prev(nullptr), next(nullptr) {}
    };
    
    Node* head;     // Указатель на первый узел списка 
    Node* tail;     // Указатель на последний узел списка 
    size_t size_;   // Количество элементов в списке
    
public:
    List() : head(nullptr), tail(nullptr), size_(0) {}
    
    // Копирующий конструктор 
    List(const List& other) : head(nullptr), tail(nullptr), size_(0) {
        // Используем push_back для добавления каждого элемента
        Node* current = other.head;
        while (current) {
            push_back(current->data);  // Создаем копию каждого элемента
            current = current->next;
        }
    }
    
    // Перемещающий конструктор 
    List(List&& other) noexcept 
        : head(other.head), tail(other.tail), size_(other.size_) {
        // Обнуляем указатели исходного списка, чтобы избежать двойного удаления
        other.head = nullptr;
        other.tail = nullptr;
        other.size_ = 0;
    }
    
    // Копирующий оператор присваивания
    List& operator=(const List& other) {
        // Проверка на самоприсваивание
        if (this != &other) {
            clear();  // Освобождаем текущие ресурсы
            
            // Копируем элементы из другого списка
            Node* current = other.head;
            while (current) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;  // Возвращаем ссылку на текущий объект
    }
    
    // Перемещающий оператор присваивания
    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();  // Освобождаем текущие ресурсы
            
            // Перехватываем ресурсы другого списка
            head = other.head;
            tail = other.tail;
            size_ = other.size_;
            
            // Обнуляем указатели исходного списка
            other.head = nullptr;
            other.tail = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    ~List() {
        clear();
    }
    
    // Очистка списка 
    void clear() {
        while (head) {
            Node* temp = head;    // Сохраняем указатель на текущий узел
            head = head->next;    // Перемещаем head на следующий узел
            delete temp;          // Освобождаем память текущего узла
        }
        tail = nullptr;  // Обнуляем tail
        size_ = 0;       // Сбрасываем счетчик размера
    }
    
    // Добавление элемента в конец списка (для l-value)
    void push_back(const T& value) {
        Node* new_node = new Node(value);  // Создаем новый узел
        
        if (!head) {
            // Если список пуст, новый узел становится и head, и tail
            head = tail = new_node;
        } else {
            // Добавляем новый узел после текущего tail
            tail->next = new_node;    // Текущий tail указывает на новый узел
            new_node->prev = tail;    // Новый узел указывает назад на старый tail
            tail = new_node;          // Новый узел становится новым tail
        }
        ++size_; 
    }
    
    // Добавление элемента в конец списка (для r-value - с перемещением)
    void push_back(T&& value) {
        Node* new_node = new Node(std::move(value));  // Перемещаем значение
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        ++size_;
    }
    
    // Добавление элемента в начало списка (для l-value)
    void push_front(const T& value) {
        Node* new_node = new Node(value);
        
        if (!head) {
            // Если список пуст, новый узел становится и head, и tail
            head = tail = new_node;
        } else {
            // Добавляем новый узел перед текущим head
            new_node->next = head;    // Новый узел указывает на старый head
            head->prev = new_node;    // Старый head указывает назад на новый узел
            head = new_node;          // Новый узел становится новым head
        }
        ++size_;
    }
    
    // Добавление элемента в начало списка (для r-value)
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
    
    // Вставка элемента в произвольную позицию (для l-value)
    void insert(size_t pos, const T& value) {
        // Проверка корректности позиции
        if (pos > size_) {
            throw std::out_of_range("Позиция вставки вне диапазона");
        }
        
        // вставка в начало или конец
        if (pos == 0) {
            push_front(value);
            return;
        }
        
        if (pos == size_) {
            push_back(value);
            return;
        }
        
        // Находим узел, который будет находиться после нового узла
        Node* current = head;
        for (size_t i = 0; i < pos; ++i) {
            current = current->next;
        }
        
        // Создаем новый узел и вставляем его перед current
        Node* new_node = new Node(value);
        new_node->prev = current->prev;  
        
        current->prev->next = new_node;
        current->prev = new_node;
        
        ++size_;
    }
    
    // Вставка элемента в произвольную позицию (для r-value)
    void insert(size_t pos, T&& value) {
        if (pos > size_) {
            throw std::out_of_range("Позиция вставки вне диапазона");
        }
        
        if (pos == 0) {
            push_front(std::move(value));
            return;
        }
        
        if (pos == size_) {
            push_back(std::move(value));
            return;
        }
        
        // Находим узел, который будет находиться после нового узла
        Node* current = head;
        for (size_t i = 0; i < pos; ++i) {
            current = current->next;
        }
        
        // Создаем новый узел и вставляем его перед current
        Node* new_node = new Node(std::move(value));
        new_node->prev = current->prev;
        new_node->next = current;
        current->prev->next = new_node;
        current->prev = new_node;
        
        ++size_;
    }
    
    // Удаление элемента по позиции
    void erase(size_t pos) {
        // Проверка корректности позиции
        if (pos >= size_) {
            throw std::out_of_range("Позиция удаления вне диапазона");
        }
        
        // Находим удаляемый узел
        Node* current = head;
        for (size_t i = 0; i < pos; ++i) {
            current = current->next;
        }
        
        // Обновляем связи соседних узлов
        if (current->prev) {
            // Если есть предыдущий узел, его next указывает на следующий узел
            current->prev->next = current->next;
        } else {
            // Если это первый узел, обновляем head
            head = current->next;
        }
        
        if (current->next) {
            // Если есть следующий узел, его prev указывает на предыдущий узел
            current->next->prev = current->prev;
        } else {
            // Если это последний узел, обновляем tail
            tail = current->prev;
        }
        
        delete current;  // Освобождаем память удаляемого узла
        --size_;
    }
    
    // Получение количества элементов в списке
    size_t size() const {
        return size_;
    }
    
    // Вложенный класс итератора для обхода списка
    class iterator {
    private:
        Node* current;
        
    public:
        // Конструктор итератора
        iterator(Node* node) : current(node) {}
        
        // Оператор разыменования 
        T& operator*() {
            return current->data;
        }
        
        // Префиксный инкремент
        iterator& operator++() {
            if (current) {
                current = current->next;  // Переходим к следующему узлу
            }
            return *this;
        }
        
        // Постфиксный инкремент - движение вперед
        iterator operator++(int) {
            iterator temp = *this;  // Сохраняем текущее состояние
            if (current) {
                current = current->next;
            }
            return temp;  // Возвращаем старое состояние
        }
        
        // Префиксный декремент
        iterator& operator--() {
            if (current) {
                current = current->prev;  // Переходим к предыдущему узлу
            }
            return *this;
        }
        
        // Постфиксный декремент
        iterator operator--(int) {
            iterator temp = *this;  // Сохраняем текущее состояние
            if (current) {
                current = current->prev;
            }
            return temp;  // Возвращаем старое состояние
        }
        
        // Операторы сравнения для использования в циклах
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
        
        bool operator==(const iterator& other) const {
            return current == other.current;
        }
    };
    
    // Метод для получения итератора на начало списка
    iterator begin() {
        return iterator(head);
    }
    
    iterator end() {
        return iterator(nullptr);
    }
};

#endif 