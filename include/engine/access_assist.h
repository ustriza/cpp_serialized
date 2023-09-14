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
bool get_value_to(const Storage& storage, T& outMyVar) {
	auto deser = yb::to_cpp::storage_to_cpp_instance(outMyVar, storage);
	return deser.read_from();
}

template <typename T, Storage_concept_to_cpp Storage>
std::optional<T> get_value(const Storage& storage) {
	std::optional<T> ret_value;
	ret_value.emplace();
	if(!get_value_to(storage, ret_value.value())) {
		ret_value.reset();
	}
	return ret_value;
}

template <typename T, Storage_concept_to_cpp Storage>
T get_value(const Storage& storage, const T& def) {
	T ret_value;
	if(!get_value_to(storage, ret_value)) {
		ret_value = def;
	}
	return ret_value;
}

template <typename T, Storage_concept_to_cpp Storage>
T get_value(const Storage& storage, T&& def) {
	std::decay_t<T> ret_value;
	if(!get_value_to(storage, ret_value)) {
		ret_value = std::forward<T>(def);
	}
	return ret_value;
}


template <Storage_concept_from_cpp Storage>
decltype(auto) create_storage(Storage&& storage, std::string_view key) {
	assert(!key.empty());
	return storage.interface_append_map_item(key);
}


template <Storage_concept_from_cpp Storage, typename T>
void set_value(Storage&& storage, const T& myVar) {
	auto inst = yb::from_cpp::cpp_to_storage_instance(myVar, storage);
	inst.write_to();
}

} // end of namespace yb::assist
