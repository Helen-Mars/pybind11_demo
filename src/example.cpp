#include "example.h"

struct Pet {
    enum Kind {
        Dog = 0,
        Cat
    };

    struct Attributes {
        float age = 0;
    };

    Pet(const std::string& name, Kind type) : name(name), type(type) { }

    std::string name;
    Kind type;
    Attributes attr;
};

class SomeType {
public:
    SomeType(int value) : value_(value) {}

    int getValue() const { return value_; }
private:
    int value_;
};

void print_dict(const py::dict& dict) {
    for (auto item : dict)
        std::cout << "key=" << std::string(py::str(item.first)) << ", "
        << "value=" << std::string(py::str(item.second)) << std::endl;
}

class MyClass {
public:
    void myFunction(const SomeType& arg) {
        std::cout << "myFunction called with arg value: " << arg.getValue() << std::endl;
    }
};

// 定义一个模板函数
template <typename T>
T add(T a, T b) {
    return a + b;
}

void init_ex1(py::module& m) {
    py::class_<Pet> pet(m, "Pet");

    pet.def(py::init<const std::string&, Pet::Kind>())
        .def_readwrite("name", &Pet::name)
        .def_readwrite("type", &Pet::type)
        .def_readwrite("attr", &Pet::attr);

    py::enum_<Pet::Kind>(pet, "Kind")
        .value("Dog", Pet::Kind::Dog)
        .value("Cat", Pet::Kind::Cat)
        .export_values();

    py::class_<Pet::Attributes>(pet, "Attributes")
        .def(py::init<>())
        .def_readwrite("age", &Pet::Attributes::age);

    m.def("print_dict", &print_dict);

    m.def("add", &add<int>);

}

PYBIND11_MODULE(example, m) {
    init_ex1(m);
    init_ex2(m);
    init_ex3(m);
    /* ... */
}