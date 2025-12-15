#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iostream>
#include <stdexcept>
#include <utility>

// Класс ForwardList
template<typename T>
class ForwardList {
private:
    struct Node {
        T data;         // Данные, хранящиеся в узле
        Node* next;     // Указатель на следующий узел
        
        // Конструктор для l-value ссылок
        Node(const T& value) : data(value), next(nullptr) {}
        
        // Конструктор для r-value ссылок с перемещением
        Node(T&& value) : data(std::move(value)), next(nullptr) {}
    };
    
    Node* head;     // Указатель на первый узел списка
    size_t size_;   // Количество элементов в списке
    
public:
    ForwardList() : head(nullptr), size_(0) {}
    
    // Копирующий конструктор
    ForwardList(const ForwardList& other) : head(nullptr), size_(0) {
        Node* current = other.head;  // Указатель для обхода исходного списка
        Node* prev = nullptr;        // Указатель на последний созданный узел
        
        // Поэлементное копирование всех узлов
        while (current) {
            Node* new_node = new Node(current->data);  // Создаем копию узла
            
            // Устанавливаем head для первого узла
            if (!head) {
                head = new_node;
            } else {
                prev->next = new_node;  // Связываем предыдущий узел с новым
            }
            
            prev = new_node;           // Обновляем указатель на последний узел
            current = current->next;   // Переходим к следующему узлу
            ++size_;                   // Увеличиваем счетчик размера
        }
    }
    
    // Перемещающий конструктор
    ForwardList(ForwardList&& other) noexcept 
        : head(other.head), size_(other.size_) {
        // Обнуляем указатели исходного списка, чтобы избежать двойного удаления
        other.head = nullptr;
        other.size_ = 0;
    }
    
    // Копирующий оператор присваивания
    ForwardList& operator=(const ForwardList& other) {
        // Проверка на самоприсваивание
        if (this != &other) {
            clear();  // Освобождаем текущие ресурсы
            
            // Копируем элементы как в копирующем конструкторе
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
        return *this;  // Возвращаем ссылку на текущий объект
    }
    
    // Перемещающий оператор присваивания
    ForwardList& operator=(ForwardList&& other) noexcept {
        if (this != &other) {
            clear();  // Освобождаем текущие ресурсы
            
            // Перехватываем ресурсы другого списка
            head = other.head;
            size_ = other.size_;
            
            // Обнуляем указатели исходного списка
            other.head = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    ~ForwardList() {
        clear();
    }
    
    // Очистка списка 
    void clear() {
        while (head) {
            Node* temp = head;    // Сохраняем указатель на текущий узел
            head = head->next;    // Перемещаем head на следующий узел
            delete temp;          // Освобождаем память текущего узла
        }
        size_ = 0; 
    }
    
    // Добавление элемента в конец списка (для l-value)
    void push_back(const T& value) {
        Node* new_node = new Node(value);  // Создаем новый узел
        
        if (!head) {
            // Если список пуст, новый узел становится головой
            head = new_node;
        } else {
            // Ищем последний узел
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;  // Присоединяем новый узел в конец
        }
        ++size_;
    }
    
    // Добавление элемента в конец списка (для r-value - с перемещением)
    void push_back(T&& value) {
        Node* new_node = new Node(std::move(value));  // Перемещаем значение
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
    
    // Добавление элемента в начало списка (для l-value)
    void push_front(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head;  // Новый узел указывает на старую голову
        head = new_node;        // Новый узел становится новой головой
        ++size_;
    }
    
    // Добавление элемента в начало списка (для r-value)
    void push_front(T&& value) {
        Node* new_node = new Node(std::move(value));
        new_node->next = head;
        head = new_node;
        ++size_;
    }
    
    // Вставка элемента в произвольную позицию (для l-value)
    void insert(size_t pos, const T& value) {
        // Проверка корректности позиции
        if (pos > size_) {
            throw std::out_of_range("Недопустимая позиция для вставки");
        }
        
        // Если вставка в начало, используем push_front
        if (pos == 0) {
            push_front(value);
            return;
        }
        
        // Находим узел, предшествующий позиции вставки
        Node* current = head;
        for (size_t i = 0; i < pos - 1; ++i) {
            current = current->next;
        }
        
        // Создаем новый узел и вставляем его после current
        Node* new_node = new Node(value);
        new_node->next = current->next;  // Новый узел указывает на следующий за current
        current->next = new_node;        // current теперь указывает на новый узел
        ++size_;
    }
    
    // Вставка элемента в произвольную позицию (для r-value)
    void insert(size_t pos, T&& value) {
        if (pos > size_) {
            throw std::out_of_range("Недопустимая позиция для вставки");
        }
        
        if (pos == 0) {
            push_front(std::move(value));
            return;
        }
        
        // Находим узел перед позицией вставки
        Node* current = head;
        for (size_t i = 0; i < pos - 1; ++i) {
            current = current->next;
        }
        
        Node* new_node = new Node(std::move(value));
        new_node->next = current->next;
        current->next = new_node;
        ++size_;
    }
    
    // Удаление элемента по позиции
    void erase(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Недопустимая позиция для удаления");
        }
        
        if (pos == 0) {
            // Удаление первого элемента
            Node* temp = head;       // Сохраняем указатель на удаляемый узел
            head = head->next;       // Перемещаем голову на следующий узел
            delete temp;             // Освобождаем память
        } else {
            // Удаление элемента в середине или конце
            Node* current = head;
            // Находим узел перед удаляемым
            for (size_t i = 0; i < pos - 1; ++i) {
                current = current->next;
            }
            
            Node* temp = current->next;    // Удаляемый узел
            current->next = temp->next;    // Пропускаем удаляемый узел
            delete temp;                   // Освобождаем память
        }
        --size_;
    }
    
    // Получение количества элементов в списке
    size_t size() const {
        return size_;
    }
    
    // Вложенный класс итератора для обхода списка
    class iterator {
    private:
        Node* current;  // Указатель на текущий узел
        
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
        
        // Постфиксный инкремент
        iterator operator++(int) {
            iterator temp = *this;  // Сохраняем текущее состояние
            if (current) {
                current = current->next;
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
    
    // Метод для получения итератора на конец списка
    iterator end() {
        return iterator(nullptr);
    }
};

#endif 