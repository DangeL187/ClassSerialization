#include <iostream>
#include <memory>

#include <serialization.hpp>

struct POS {
DEFINE_SERIALIZE(x, y)

    int x = 0;
    int y = 0;
    POS() = default;
    POS(int x, int y): x(x), y(y) {}
};

class Parent {
DEFINE_SERIALIZE(_pos)

protected:
    POS _pos;
};

class Parent2 {
DEFINE_SERIALIZE(_parent2_int)

protected:
    int _parent2_int = 45;
};

class MyClass: public Parent, public Parent2 {
DEFINE_SERIALIZE(_int_protected, _bool, _char, _double, _float, _long, _long_long, _short, _string,
                 _ptr_int, _ptr_string, _q_int, _q_string, _s_ptr_int, _s_ptr_string, _stack_int, _stack_string,
                 _v_int, _v_string)
DEFINE_SERIALIZE_BASES(Parent, Parent2)

public:
    MyClass() = default;

    explicit MyClass(bool is_default) {
        _int_protected = 123;

        _bool = true;
        _char = 'Y';
        _double = 3.14;
        _float = 1.87;
        _long = 1337;
        _long_long = 13378901;
        _short = 255;
        _string = "string";

        _ptr_int = new int(13);
        _ptr_string = new std::string("ptr string");

        _s_ptr_int = std::make_shared<int>(14);
        _s_ptr_string = std::make_shared<std::string>("shared ptr string");

        _q_int.push(1);
        _q_int.push(2);
        _q_int.push(3);

        _q_string.emplace("a");
        _q_string.emplace("b");
        _q_string.emplace("c");

        _stack_int.push(11);
        _stack_int.push(22);
        _stack_int.push(33);

        _stack_string.emplace("aa");
        _stack_string.emplace("bb");
        _stack_string.emplace("cc");

        _v_int = {1, 2, 3, 4, 5, 6};
        _v_string = {"a", "b", "c", "d", "e", "f"};

        _pos = POS(69, 96);
    }

    friend bool compare(const MyClass& objA, const MyClass& objB);

    void print() {
        std::cout << "Bool: " << _bool << "\n";
        std::cout << "Char: " << _char << "\n";
        std::cout << "Double: " << _double << "\n";
        std::cout << "Float: " << _float << "\n";
        std::cout << "Long: " << _long << "\n";
        std::cout << "Long long: " << _long_long << "\n";
        std::cout << "Short: " << _short << "\n";
        std::cout << "String: " << _string << "\n";
        std::cout << "Ptr int: " << *_ptr_int << "\n";
        std::cout << "Ptr string: " << *_ptr_string << "\n";
        std::cout << "Shared ptr int: " << *_s_ptr_int << "\n";
        std::cout << "Shared ptr string: " << *_s_ptr_string << "\n";

        std::cout << "Queue int:\n";
        printQueue(_q_int);
        std::cout << "Queue string:\n";
        printQueue(_q_string);

        std::cout << "Stack int:\n";
        printStack(_stack_int);
        std::cout << "Stack string:\n";
        printStack(_stack_string);

        std::cout << "Vector int:\n";
        printVector(_v_int);
        std::cout << "Vector string:\n";
        printVector(_v_string);
    }

protected:
    int                             _int_protected = 0;

private:
    bool                            _bool = false;
    char                            _char = 0;
    double                          _double = 0;
    float                           _float = 0;
    long                            _long = 0;
    long long                       _long_long = 0;
    short                           _short = 0;
    std::string                     _string;

    int*                            _ptr_int = nullptr;
    std::string*                    _ptr_string = nullptr;

    std::queue<int>                 _q_int;
    std::queue<std::string>         _q_string;

    std::shared_ptr<int>            _s_ptr_int = nullptr;
    std::shared_ptr<std::string>    _s_ptr_string = nullptr;

    std::stack<int>                 _stack_int;
    std::stack<std::string>         _stack_string;

    std::vector<int>                _v_int;
    std::vector<std::string>        _v_string;

    template <typename T>
    void printQueue(std::queue<T> queue) {
        while (!queue.empty()) {
            std::cout << queue.front() << " ";
            queue.pop();
        }
        std::cout << std::endl;
    }

    template <typename T>
    void printStack(std::stack<T> stack) {
        while (!stack.empty()) {
            std::cout << stack.top() << " ";
            stack.pop();
        }
        std::cout << std::endl;
    }

    template <typename T>
    void printVector(std::vector<T> vector) {
        for (auto& i: vector) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
};

bool compare(const MyClass& a, const MyClass& b) {
    if (a._bool != b._bool) return false;
    if (a._char != b._char) return false;
    if (a._int_protected != b._int_protected) return false;
    if (a._double != b._double) return false;
    if (a._float != b._float) return false;
    if (a._long != b._long) return false;
    if (a._long_long != b._long_long) return false;
    if (a._short != b._short) return false;
    if (a._string != b._string) return false;
    if (*a._ptr_int != *b._ptr_int) return false;
    if (*a._ptr_string != *b._ptr_string) return false;
    if (a._q_int != b._q_int) return false;
    if (a._q_string != b._q_string) return false;
    if (*a._s_ptr_int != *b._s_ptr_int) return false;
    if (*a._s_ptr_string != *b._s_ptr_string) return false;
    if (a._stack_int != b._stack_int) return false;
    if (a._stack_string != b._stack_string) return false;
    if (a._v_int != b._v_int) return false;
    if (a._v_string != b._v_string) return false;
    return true;
}

int main() {
    MyClass original(false);
    serialization::serialize(original, "data.xml");

    MyClass loaded;
    serialization::deserialize(loaded, "data.xml");

    loaded.print();

    if (compare(original, loaded)) std::cout << "\nClass fields are the same!\n";
    else std::cout << "\nClass fields are NOT the same :(\n";

    return 0;
}
