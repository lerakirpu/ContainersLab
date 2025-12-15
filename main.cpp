#include "include/vector.h"
#include "include/list.h"
#include "include/forward_list.h"
#include <iostream>
#include <string>

template<typename Container>
void demonstrate_container(const std::string& container_name) {
    std::cout << "Демонстрация "  << container_name << std::endl;
    
    Container container;
    
    // Добавление 10 элементов
    for (int i = 0; i < 10; ++i) {
        container.push_back(i);
    }
    
    std::cout << "Содержимое после добавления 0-9: ";
    for (auto it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Вывод размера
    std::cout << "Размер контейнера: " << container.size() << std::endl;
    
    // Удаление 3-го, 5-го и 7-го элементов
    container.erase(6); 
    container.erase(4);  
    container.erase(2); 
    
    // Вывод после удаления
    std::cout << "Содержимое после удаления: ";
    for (auto it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Добавление элемента 10 в начало
    container.insert(0, 10);
    
    std::cout << "Содержимое после добавления 10 в начало: ";
    for (auto it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Вычисление середины контейнера
    size_t middle = container.size() / 2;
    container.insert(middle, 20);
    
    std::cout << "Содержимое после добавления 20 в середину: ";
    for (auto it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Добавление элемента 30 в конец
    container.push_back(30);
    
    std::cout << "Содержимое после добавления 30 в конец: ";
    for (auto it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Итоговый размер: " << container.size() << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "Тестирование пользовательских контейнеров " << std::endl;
    std::cout << std::endl;
    demonstrate_container<Vector<int>>("Vector (последовательный контейнер)");
    demonstrate_container<List<int>>("List (двунаправленный список)");
    demonstrate_container<ForwardList<int>>("ForwardList (однонаправленный список)");
    
    std::cout << "Все тесты завершены успешно!" << std::endl;
    return 0;
}