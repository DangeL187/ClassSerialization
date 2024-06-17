#ifndef CLASSSERIALIZATION_SERIALIZATION_HPP
#define CLASSSERIALIZATION_SERIALIZATION_HPP

#include <stdexcept>
#include <utility>

#include "Archive/map.h"

#include "Archive/Archive.hpp"

namespace serialization {
    template <typename T>
    void serialize(T& object, const std::string& file_path) {
        std::ofstream ofs(file_path);
        if (!ofs) throw std::runtime_error("Deserialization failed: could not open file for writing");
        Archive ar(ofs, std::string(TypeUtils::Type::name(object)));
        object.serializeBases(ar, 1);
        object.serialize(ar, 1);
    }

    template <typename T>
    void deserialize(T& object, const std::string& file_path) {
        Archive ar(file_path, std::string(TypeUtils::Type::name(object)));
        object.serializeBases(ar, 1);
        object.serialize(ar, 1);
    }
}

#define ADD_TO_ARCHIVE(arg) ar.connect(arg, #arg);
#define MAP_ADD_TO_ARCHIVE(...) MAP(ADD_TO_ARCHIVE, __VA_ARGS__)

#define DEFINE_SERIALIZE(...) \
public: \
    void serialize(Archive& ar, const unsigned int version, bool is_parent = true) { \
        if (is_parent) ar.setParent(std::string(TypeUtils::Type::name(*this))); \
        MAP_ADD_TO_ARCHIVE(__VA_ARGS__) \
        if (is_parent) ar.setParent(); \
    }

#define DEFINE_SERIALIZE_BASES(...) \
public: \
    void serializeBases(Archive& ar, const unsigned int version) { \
        __serializeBases<__VA_ARGS__>(ar, version); \
    } \
private: \
    template<typename... Bases> \
    void __serializeBases(Archive& ar, const unsigned int version) { \
        (Bases::serialize(ar, version, true), ...); \
    }

#endif //CLASSSERIALIZATION_SERIALIZATION_HPP
