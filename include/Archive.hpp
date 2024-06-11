#ifndef CLASSSERIALIZATION_ARCHIVE_HPP
#define CLASSSERIALIZATION_ARCHIVE_HPP

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

class Archive {
public:
    explicit Archive(std::ostream& os): _os(&os), _is(nullptr) {}
    explicit Archive(std::istream& is): _os(nullptr), _is(&is) {}

    template<typename T>
    void operator&(std::vector<T>& other) {
        if (_os) {
            size_t size = other.size();
            *_os << size << '\n';
            for (auto& element: other) {
                (*this) & element;
            }
        } else if (_is) {
            size_t size;
            *_is >> size;
            _is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            other.resize(size);
            for (auto& element: other) {
                (*this) & element;
            }
        }
    }

    template<typename T>
    void operator&(T& other) {
        if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, std::string> || std::is_same_v<T, char>) {
            serialize_basic(other);
        } else {
            other.serialize(*this, 1);
        }
    }

private:
    template<typename T>
    void serialize_basic(T& other) {
        if (_os) {
            *_os << other << '\n';
        } else if (_is) {
            *_is >> other;
            _is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void serialize_basic(std::string& other) {
        if (_os) {
            *_os << other << '\n';
        } else if (_is) {
            std::getline(*_is, other);
        }
    }

    std::ostream* _os;
    std::istream* _is;
};

#endif //CLASSSERIALIZATION_ARCHIVE_HPP
