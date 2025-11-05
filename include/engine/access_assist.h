#pragma once
#include <string>
#include <cassert>
#include <optional>

#include "engine_to_cpp.h"
#include "engine_from_cpp.h"

namespace yb::assist {

template <Storage_concept_to_cpp Storage>
decltype(auto) find_storage(const Storage& storage, std::string_view key) {
	return storage.interface_get_storage_by_key(key);
}

template <typename T, Storage_concept_to_cpp Storage>
bool deserialize_to(const Storage& storage, T& to_value) {
	auto deser = yb::to_cpp::storage_to_cpp_instance(to_value, storage);
	return deser.read_from();
}

template <typename T, Storage_concept_to_cpp Storage>
std::optional<T> deserialize(const Storage& storage) {
	std::optional<T> to_value;
	to_value.emplace();
	if(!deserialize_to(storage, to_value.value())) {
		to_value.reset();
	}
	return to_value;
}

template <typename T, Storage_concept_to_cpp Storage>
T deserialize(const Storage& storage, const T& def) {
	T to_value;
	if(!deserialize_to(storage, to_value)) {
		to_value = def;
	}
	return to_value;
}

template <typename T, Storage_concept_to_cpp Storage>
T deserialize(const Storage& storage, T&& def) {
	std::decay_t<T> to_value;
	if(!deserialize_to(storage, to_value)) {
		to_value = std::forward<T>(def);
	}
	return to_value;
}


template <Storage_concept_from_cpp Storage>
decltype(auto) create_storage(Storage&& storage, std::string_view key) {
	assert(!key.empty());
	return storage.interface_append_map_item(key);
}

#if __cplusplus >= 202002L //C++ 20

template <Storage_concept_from_cpp Storage, typename T>
void serialize_impl(Storage&& storage, t1_arg_t<T>& from_value) {
	auto inst = yb::from_cpp::cpp_to_storage_instance<T>(from_value, storage);
	inst.write_to();
}

template <Storage_concept_from_cpp Storage, typename T>
requires has_non_const_begin_v<T>
void serialize(Storage&& storage, T&& from_value) {
	serialize_impl<Storage, T>(std::forward<Storage>(storage), from_value);
}

template <Storage_concept_from_cpp Storage, typename T>
requires (!has_begin<T> || has_const_begin_v<T>)
void serialize(Storage&& storage, const T& from_value) {
	serialize_impl<Storage, T>(std::forward<Storage>(storage), from_value);
}


#else
template <Storage_concept_from_cpp Storage, typename T>
void serialize(Storage&& storage, const T& from_value) {
	auto inst = yb::from_cpp::cpp_to_storage_instance(from_value, storage);
	inst.write_to();
}
#endif
} // end of namespace yb::assist
