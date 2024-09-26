#define UNICODE

#include "example.h"
#include <windows.h>

using namespace std;


void init_ex3(py::module& m) {

    m.def("pass_char", [](char c) { return c; });
    m.def("f", [](int a, int b) { /* ... */ },
        py::arg("a"), py::kw_only(), py::arg("b"));

    m.def("set_window_text",
        [](HWND hwnd, std::wstring s) {
            // Call SetWindowText with null-terminated UTF-16 string
            ::SetWindowText(hwnd, s.c_str());
        }
    );

    m.def("get_window_text",
        [](HWND hwnd) {
            const int buffer_size = ::GetWindowTextLength(hwnd) + 1;
            auto buffer = std::make_unique< wchar_t[] >(buffer_size);

            ::GetWindowText(hwnd, buffer.get(), buffer_size);

            std::wstring text(buffer.get());

            // wstring will be converted to Python str
            return text;
        }
    );

}
