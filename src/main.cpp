#include <iostream>
#include <fstream>

#include "serialization.hpp"

struct POS {
    DEFINE_SERIALIZE(x, y)

    int x = 0;
    int y = 0;
    POS() = default;
    POS(int x, int y): x(x), y(y) {}
};

class MyClass {
public:
    DEFINE_SERIALIZE(_id, _name, _none, _boo, _ch, _test)

    MyClass() = default;

    MyClass(float id, const std::string& name) {
        _id = id;
        _name = name;
        _none = POS(69, 187);
        _boo = true;
        _ch = 89;
        _test = {1, 2, 3, 4, 5, 6};
    }

    void print() {
        std::cout << "ID: [" << _id << "], Name: [" << _name << "], None: [" << _none.x << " " << _none.y << "]\n";
        std::cout << _boo << " [" << _ch << "]\n";
        for (auto& i: _test) std::cout << i << "#";
    }

private:
    double _id = 0;
    std::vector<int> _test;
    std::string _name;
    POS _none;
    bool _boo = false;
    char _ch = 0;
};

int main() {
    MyClass original(13.123, "Boost Serialization");
    serialization::serialize(original, "data.txt");

    MyClass loaded;
    serialization::deserialize(loaded, "data.txt");

    loaded.print();

    return 0;
}
