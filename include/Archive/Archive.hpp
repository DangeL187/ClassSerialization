#ifndef CLASSSERIALIZATION_ARCHIVE_HPP
#define CLASSSERIALIZATION_ARCHIVE_HPP

#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <regex>
#include <stack>
#include <string>
#include <vector>

#include "tinyxml2/tinyxml2.h"

#include "Archive/TypeName.hpp"

class Archive {
public:
    explicit Archive(std::ostream& os, const std::string& class_name);
    explicit Archive(const std::string& file_path, const std::string& class_name);

    template<typename T>
    void connect(T*& other, const std::string& name) {
        serialize_pointer(other, name);
    }

    template<typename T>
    void connect(std::queue<T>& other, const std::string& name) {
        serialize_queue(other, name);
    }

    template<typename T>
    void connect(std::shared_ptr<T>& other, const std::string& name) {
        serialize_shared_pointer(other, name);
    }

    template<typename T>
    void connect(std::stack<T>& other, const std::string& name) {
        serialize_stack(other, name);
    }

    void connect(std::string& other, const std::string& name) {
        serialize_string(other, name);
    }

    template<typename T>
    void connect(std::vector<T>& other, const std::string& name) {
        serialize_vector(other, name);
    }

    template<typename T>
    void connect(T& other, const std::string& name) {
        if constexpr (std::is_arithmetic_v<T>) {
            serialize_basic(other, name);
        } else {
            setParent(name);
            other.serialize(*this, 1, false);
            setParent();
        }
    }

    void setParent(const std::string& parent_name = "") {
        bool is_parent = !parent_name.empty();

        if (is_parent && parent_name != _class_name) {
            _tags.push(parent_name);

            if (_os) {
                *_os << std::string((_tags.size() - 1) * 4, ' ') << "<" << _tags.top() << ">\n";
                return;
            }

            if (_tags.size() == 1) {
                _nodes.push(_doc.FirstChildElement(parent_name.c_str()));
            } else {
                _nodes.push(_nodes.top()->FirstChildElement(_tags.top().c_str()));
            }
        } else if (!is_parent) {
            if (_os) {
                *_os << std::string((_tags.size() - 1) * 4, ' ') << "</" << _tags.top() << ">\n";
            }
            _tags.pop();
            _nodes.pop();
        }
    }

private:
    template<typename T>
    void serialize_basic(T& basic, const std::string& name) {
        if (_os) {
            *_os << std::string(_tags.size() * 4, ' ') << "<" << name << ">" << basic << "</" << name << ">\n";
            return;
        }

        std::string value = _nodes.top()->FirstChildElement(name.c_str())->GetText();
        if constexpr (std::is_same_v<T, bool>) {
            if (value != "0" && value != "1") {
                throw std::runtime_error("Conversion to 'bool' failed: invalid value");
            }
            basic = bool(std::stoi(value));
        } else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char>) {
            if (value.size() != 1) {
                throw std::runtime_error("Conversion to 'char' failed: invalid value");
            }
            basic = value[0];
        } else {
            std::from_chars(value.data(), value.data() + value.size(), basic);
        }
    }

    template<typename T>
    void serialize_pointer(T*& ptr, const std::string& name) {
        setParent(name);
        if (_os) {
            *_os << std::string(_tags.size() * 4, ' ') << "<is_null>" << !ptr << "</is_null>\n";
            if (ptr) connect(*ptr, "value");
        } else {
            if (std::stoi(_nodes.top()->FirstChildElement("is_null")->GetText())) {
                ptr = nullptr;
            } else {
                if (!ptr) ptr = new T();
                connect(*ptr, "value");
            }
        }
        setParent();
    }

    template<typename T>
    void serialize_queue(std::queue<T>& queue, const std::string& name) {
        if (_os) {
            std::vector<T> vector;
            std::queue<T> other_copy = queue;
            while (!other_copy.empty()) {
                vector.push_back(other_copy.front());
                other_copy.pop();
            }
            connect(vector, name);
            return;
        }
        std::vector<T> vector;
        connect(vector, name);
        queue = std::queue<T>();
        for (auto& element: vector) {
            queue.push(element);
        }
    }

    template<typename T>
    void serialize_shared_pointer(std::shared_ptr<T>& shared_ptr, const std::string& name) {
        if (!shared_ptr) shared_ptr = std::make_shared<T>();
        auto* ptr = shared_ptr.get();
        serialize_pointer(ptr, name);
        if (!ptr) shared_ptr = nullptr;
    }

    template<typename T>
    void serialize_stack(std::stack<T>& stack, const std::string& name) {
        if (_os) {
            std::vector<T> vector;
            std::stack<T> stack_copy = stack;
            while (!stack_copy.empty()) {
                vector.push_back(stack_copy.top());
                stack_copy.pop();
            }
            connect(vector, name);
            return;
        }
        std::vector<T> vector;
        connect(vector, name);
        stack = std::stack<T>();
        for (auto it = vector.rbegin(); it != vector.rend(); ++it) {
            stack.push(*it);
        }
    }

    void serialize_string(std::string& string, const std::string& name) {
        if (_os) {
            *_os << std::string(_tags.size() * 4, ' ') << "<" << name << ">" << string << "</" << name << ">\n";
            return;
        }
        string = _nodes.top()->FirstChildElement(name.c_str())->GetText();
    }

    template<typename T>
    void serialize_vector(std::vector<T>& vector, const std::string& name) {
        setParent(name);
        if (_os) {
            *_os << std::string(_tags.size() * 4, ' ') << "<size>" << vector.size() << "</size>\n";
            for (int i = 0; i < vector.size(); i++) {
                connect(vector[i], "item-" + std::to_string(i));
            }
        } else {
            size_t size = std::stoull(_nodes.top()->FirstChildElement("size")->GetText());
            vector.resize(size);
            for (int i = 0; i < vector.size(); i++) {
                connect(vector[i], "item-" + std::to_string(i));
            }
        }
        setParent();
    }

    std::string                         _class_name;
    tinyxml2::XMLDocument               _doc;
    std::stack<tinyxml2::XMLElement*>   _nodes;
    std::ostream*                       _os;
    std::stack<std::string>             _tags;
};

#endif //CLASSSERIALIZATION_ARCHIVE_HPP
