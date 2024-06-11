#ifndef CLASSSERIALIZATION_SERIALIZATION_HPP
#define CLASSSERIALIZATION_SERIALIZATION_HPP

#include "Archive.hpp"
#include <stdexcept>

namespace serialization {
    template <typename T>
    void serialize(T& object, const std::string& file_path) {
        std::ofstream ofs(file_path);
        if (!ofs) throw std::runtime_error("Deserialization failed: could not open file for writing");
        Archive ar(ofs);
        object.serialize(ar, 1);
    }

    template <typename T>
    void deserialize(T& object, const std::string& file_path) {
        std::ifstream ifs(file_path);
        if (!ifs) throw std::runtime_error("Deserialization failed: could not open file for reading");
        Archive ar(ifs);
        object.serialize(ar, 1);
    }
}

#define DEFINE_SERIALIZE(...) \
template<class Archive> \
void serialize(Archive& ar, const unsigned int version) { \
    std::apply([&ar](auto&&... args) { ((ar & args), ...); }, std::tie(__VA_ARGS__)); \
}

#endif //CLASSSERIALIZATION_SERIALIZATION_HPP
