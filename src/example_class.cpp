#include "example.h"
#include <pybind11/operators.h>

class Animal {
public:
    virtual ~Animal() { }
    virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i = 0; i < n_times; ++i)
            result += "woof! ";
        return result;
    }
};


std::string call_go(Animal* animal) {
    return animal->go(3);
}

class PyAnimal : public Animal {
public:
    /* Inherit the constructors */
    using Animal::Animal;

    /* Trampoline (need one for each virtual function) */
    std::string go(int n_times) override {
        PYBIND11_OVERRIDE_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            go,          /* Name of function in C++ (must match Python name) */
            n_times      /* Argument(s) */
        );
    }

};


class Animal2 {
public:
    virtual std::string go(int n_times) = 0;
    virtual std::string name() { return "unknown"; }
};
class Dog2 : public Animal2 {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i = 0; i < n_times; ++i)
            result += bark() + " ";
        return result;
    }
    virtual std::string bark() { return "woof!"; }
};
/*
class PyAnimal2 : public Animal2 {
public:
    using Animal2::Animal2; // Inherit constructors
    std::string go(int n_times) override { PYBIND11_OVERRIDE_PURE(std::string, Animal2, go, n_times); }
    std::string name() override { PYBIND11_OVERRIDE(std::string, Animal2, name, ); }
};
class PyDog : public Dog2 {
public:
    using Dog2::Dog2; // Inherit constructors
    std::string go(int n_times) override { PYBIND11_OVERRIDE(std::string, Dog2, go, n_times); }
    std::string name() override { PYBIND11_OVERRIDE(std::string, Dog2, name, ); }
    std::string bark() override { PYBIND11_OVERRIDE(std::string, Dog2, bark, ); }
};
*/
template <class AnimalBase = Animal2> class PyAnimal3 : public AnimalBase {
public:
    using AnimalBase::AnimalBase; // Inherit constructors
    std::string go(int n_times) override { PYBIND11_OVERRIDE_PURE(std::string, AnimalBase, go, n_times); }
    std::string name() override { PYBIND11_OVERRIDE(std::string, AnimalBase, name, ); }
};

template <class DogBase = Dog2> class PyDog2 : public PyAnimal3<DogBase> {
public:
    using PyAnimal3<DogBase>::PyAnimal3; // Inherit constructors
    // Override PyAnimal's pure virtual go() with a non-pure one:
    std::string go(int n_times) override { PYBIND11_OVERRIDE(std::string, DogBase, go, n_times); }
    std::string bark() override { PYBIND11_OVERRIDE(std::string, DogBase, bark, ); }
};


/*
class Husky : public Dog{};
class PyHusky : public Husky {
public:
    using Husky::Husky; // Inherit constructors
    std::string go(int n_times) override { PYBIND11_OVERRIDE_PURE(std::string, Husky, go, n_times); }
    std::string name() override { PYBIND11_OVERRIDE(std::string, Husky, name, ); }
    std::string bark() override { PYBIND11_OVERRIDE(std::string, Husky, bark, ); }
};
*/

class Husky : public Dog2 {};

class Example {
public:
    // ...
    Example() = default;
    virtual ~Example() = default;
};
class PyExample : public Example {
public:
    using Example::Example;//移动构造函数和拷贝构造函数
    PyExample(Example&& base) : Example(std::move(base)) {} //
};

class Example2 {
private:
    Example2(int) {}; // private constructor
public:
    // Factory function - returned by value:
    static Example2 create(int a) { return Example2(a); }

    // These constructors are publicly callable:
    Example2(double) {};
    Example2(int, int) {};
    Example2(std::string) {};
};

struct Aggregate {
    int a;
    std::string b;
};

class Vector2 {
public:
    Vector2(float x, float y) : x(x), y(y) { }

    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator*(float value) const { return Vector2(x * value, y * value); }
    Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
    Vector2& operator*=(float v) { x *= v; y *= v; return *this; }

    friend Vector2 operator*(float f, const Vector2& v) {
        return Vector2(f * v.x, f * v.y);
    }

    std::string toString() const {
        return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
    }
private:
    float x, y;
};


class Pickleable {
public:
    Pickleable(const std::string& value) : m_value(value) { }
    const std::string& value() const { return m_value; }

    void setExtra(int extra) { m_extra = extra; }
    int extra() const { return m_extra; }
private:
    std::string m_value;
    int m_extra = 0;
};

class A {
public:
    virtual ~A() = default;

protected:
    virtual int foo() const { return 42; }
};

class Trampoline : public A {
public:
    int foo() const override { PYBIND11_OVERRIDE(int, A, foo, ); }
};

class Publicist : public A {
public:
    using A::foo;
};

template <typename T>
struct MyClass {
    template <typename V>
    T fn(V v) {
        // 函数的实现
        T result = T{};
        // 这里可以根据参数v的类型进行相应的处理
        return result;
    }
};


struct Cat {};
//struct Dog;

template <typename PetType>
struct Cage {
    Cage() = default;  // 添加默认构造函数
    Cage(PetType& pet) {};
    PetType& get() { return pet_; };
private:
    PetType pet_;
};


std::unique_ptr<Example> create_example() { return std::unique_ptr<Example>(new Example()); }

enum class PetKind { Cat, Dog3, Zebra };
struct Pet {   // Not polymorphic: has no virtual methods
    const PetKind kind;
    int age = 0;
protected:
    Pet(PetKind _kind) : kind(_kind) {}
};

struct Dog3 : Pet {
    Dog3() : Pet(PetKind::Dog3) {}
    std::string sound = "woof!";
    std::string bark() const { return sound; }
};

class Child { };
class Parent {
public:
    Parent() : child(std::make_shared<Child>()) { }
    std::shared_ptr<Child> get_child() { return child; }  /* Hint: ** DON'T DO THIS ** */
private:
    std::shared_ptr<Child> child;
};


void init_ex2(py::module& m) {


    py::class_<Parent, std::shared_ptr<Parent>>(m, "Parent")
        .def(py::init<>())
        .def("get_child", &Parent::get_child);

    py::class_<Child, std::shared_ptr<Child>> childClass(m, "Child");

    m.def("create_example", &create_example);

    py::class_<Cage<Cat>>(m, "CatCage")
        .def("get", & Cage<Cat>::get);

    py::class_<MyClass<int>>(m, "MyClassT")
        .def("fn", &MyClass<int>::fn<std::string>);

    py::class_<Pickleable>(m, "Pickleable")
        .def(py::init<std::string>())
        .def("value", &Pickleable::value)
        .def("extra", &Pickleable::extra)
        .def("setExtra", &Pickleable::setExtra)
        .def(py::pickle(
            [](const Pickleable& p) { // __getstate__
                /* Return a tuple that fully encodes the state of the object */
                return py::make_tuple(p.value(), p.extra());
            },
            [](py::tuple t) { // __setstate__
                if (t.size() != 2)
                    throw std::runtime_error("Invalid state!");

                /* Create a new C++ instance */
                Pickleable p(t[0].cast<std::string>());

                /* Assign any additional state */
                p.setExtra(t[1].cast<int>());

                return p;
            }))
        ;

    m.def("sub", [](int a, int b) { return a - b; });
    
    py::class_<Animal, PyAnimal /* <--- trampoline*/>(m, "Animal")
        .def(py::init<>())
        .def("go", &Animal::go);

    py::class_<Dog, Animal>(m, "Dog")
        .def(py::init<>());
    
    py::class_<Animal2, PyAnimal3<>> animal(m, "Animal2");
    py::class_<Dog2, Animal2, PyDog2<>> dog(m, "Dog2");
    py::class_<Husky, Dog2, PyDog2<Husky>> husky(m, "Husky");
    

    py::class_<Example, PyExample>(m, "Example")
        .def(py::init([]() { return new Example(); } /* no alias needed */,
            []() { return new PyExample(); } /* alias needed */));

    py::class_<Example2>(m, "Example2")
        .def(py::init(&Example2::create))
        .def(py::init([](std::string arg) {
        return std::unique_ptr<Example2>(new Example2(arg));
            }))
        .def(py::init([](int a, int b) { return new Example2(a, b); }))
        .def(py::init<double>());

    py::class_<Aggregate>(m, "Aggregate")
        .def(py::init<int, const std::string&>());

    py::class_<Vector2>(m, "Vector2")
        .def(py::init<float, float>())
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(py::self *= float())
        .def(float() * py::self)
        .def(py::self * float())
        .def("__repr__", &Vector2::toString);


    m.def("call_go", &call_go);
}
