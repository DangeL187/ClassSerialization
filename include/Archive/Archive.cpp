#include "Archive/Archive.hpp"

Archive::Archive(std::ostream& os, const std::string& class_name): _os(&os) {
    *_os << R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>)" << '\n';

    setParent(class_name);
    _class_name = class_name;
}

Archive::Archive(const std::string& file_path, const std::string& class_name): _os(nullptr) {
    _doc.LoadFile(file_path.c_str());

    setParent(class_name);
    _class_name = class_name;
}