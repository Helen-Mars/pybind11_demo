#include "example.h"

#include <pybind11/embed.h>

int main() {
    py::scoped_interpreter guard{}; // start the interpreter and keep it alive

    py::print("Hello, World!");// use the Python API

    return 0;
}