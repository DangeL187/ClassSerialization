#include <iostream>

#include <serialization.hpp>

struct POS {
    DEFINE_SERIALIZE(x, y)

    int x = 0;
    int y = 0;
    POS() = default;
    POS(int x, int y): x(x), y(y) {}
};

class Parent {
public:
    DEFINE_SERIALIZE(_pos)

protected:
    POS _pos;
};

class Parent2 {
public:
    DEFINE_SERIALIZE(_int)

protected:
    int _int = 45;

    void printPrivate() const {
        std::cout << _private << " - private\n";
    }

private:
    int _private = 45;
};

class MyClass: public Parent, public Parent2 {
DEFINE_SERIALIZE(_double, _string, _vector, _char)
DEFINE_SERIALIZE_BASES(Parent, Parent2)
public:
    double _double = 0;

    MyClass() {
        _string = std::make_shared<std::string>("Default");
    }

    MyClass(double id, const std::string& name) {
        _double = id;
        _string = std::make_shared<std::string>(name);
        //_string = nullptr;
        _pos = POS(69, 187);
        _vector = {1, 2, 3, 4, 5, 6};
        _char = 89;
    }

    void print() {
        std::cout << "double: [" << _double << "], None: [" << _pos.x << " " << _pos.y << "]\n";
        std::cout << "Shared Ptr String: [";
        if (_string == nullptr) std::cout << "NULL";
        else std::cout << *_string;
        std::cout << "]\n";
        std::cout << _char << " " << _int << "\n";
        printPrivate();
        for (auto& i: _vector) std::cout << i << "#";
    }

protected:
    std::vector<int> _vector;

private:
    std::shared_ptr<std::string> _string;
    char _char = 0;
};

int main() {
    MyClass original(13.123, "STRING");
    serialization::serialize(original, "data.xml");

    MyClass loaded;
    serialization::deserialize(loaded, "data.xml");

    loaded.print();

    return 0;
}
