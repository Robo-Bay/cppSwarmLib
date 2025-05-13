#include <iostream>
#include <string> // Пример компонента, использующего string
#include <tuple>

// ----------- Пример компонентных типов -----------
// Каждый компонент должен иметь конструктор, принимающий int

struct ComponentA {
  int data_a;
  ComponentA(int val) : data_a(val) {
    std::cout << "ComponentA создан с значением: " << data_a << std::endl;
  }
};

struct ComponentB {
  std::string data_b;
  // Конструктор принимает int и преобразует его в string
  ComponentB(int val) : data_b(std::to_string(val) + "_suffix") {
    std::cout << "ComponentB создан с значением: " << data_b << std::endl;
  }
};

struct ComponentC {
  double data_c;
  // Конструктор принимает int и преобразует его в double
  ComponentC(int val) : data_c(static_cast<double>(val) * 1.5) {
    std::cout << "ComponentC создан с значением: " << data_c << std::endl;
  }
};

// ----------- Класс, содержащий tuple -----------

template <typename... ComponentsT> class MyTupleContainer {
private:
  // Твой std::tuple, содержащий элементы типов из пакета ComponentsT...
  std::tuple<ComponentsT...> _components;

public:
  // Конструктор класса, который принимает int для инициализации tuple
  MyTupleContainer(int initial_value)
      // Список инициализации членов:
      // Здесь происходит магия разворачивания пакета параметров.
      // (ComponentsT(initial_value))... создает список аргументов:
      // ComponentsT1(initial_value), ComponentsT2(initial_value), ...
      // Этот список передается конструктору std::tuple.
      : _components(ComponentsT(initial_value)...) {
    std::cout << "MyTupleContainer создан." << std::endl;
  }

  // Дополнительно можно добавить методы для доступа к элементам tuple,
  // но для демонстрации инициализации это не обязательно.
  // Например:
  // template<size_t Index>
  // auto& get_component() {
  //     return std::get<Index>(_components);
  // }
};

// ----------- Пример использования -----------

int main() {
  std::cout << "Создаем контейнер со значением инициализации 42:" << std::endl;

  // Создаем экземпляр контейнера, указывая типы компонентов
  MyTupleContainer<ComponentA, ComponentB, ComponentC> container(42);

  // Когда объект 'container' создается, вызывается его конструктор
  // MyTupleContainer(42).
  // В списке инициализации членов, _components инициализируется как:
  // std::tuple<ComponentA, ComponentB, ComponentC>(ComponentA(42),
  // ComponentB(42), ComponentC(42)); Это вызовет соответствующие конструкторы
  // ComponentA, ComponentB, ComponentC с аргументом 42.

  std::cout << "\nСоздаем другой контейнер со значением инициализации 99:"
            << std::endl;
  MyTupleContainer<ComponentB, ComponentA> another_container(99);

  return 0;
}