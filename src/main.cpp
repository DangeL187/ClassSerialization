#include <iostream>
#include <fstream>

#include "serialization.hpp"

struct POS {
    DEFINE_SERIALIZE(x, y)

    int x = 69;
    int y = 187;
    POS() = default;
};

class MyClass {
public:
    DEFINE_SERIALIZE(_id, _name, _none)

    MyClass() = default;

    MyClass(int id, const std::string& name) {
        _id = id;
        _name = name;
    }

    void print() {
        std::cout << "ID: [" << _id << "], Name: [" << _name << "], None: [" << _none.x << " " << _none.y << "]" << std::endl;
    }

private:
    int _id = 0;
    std::string _name;
    POS _none;
};

int main() {
    MyClass original(13, "Boost Serialization");
    serialization::serialize(original, "data.txt");

    MyClass loaded;
    serialization::deserialize(loaded, "data.txt");

    loaded.print();

    return 0;
}
