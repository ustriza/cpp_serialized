//
//  test_storage_from_cpp.h
//
//  Created by Yuri Barmin on 09.08.2023.
//

#pragma once

#include <optional>

#include "engine_from_cpp.h"
#include "test_storage_common.h"

namespace yb::from_cpp {

class TestStorage {
	using Value_type = std::variant<int, unsigned int, float, double, std::string, bool>;
	using Map = std::map<tests::MapKey, TestStorage>;
	
	std::optional<Value_type> m_value;
	Map m_map;
public:
	//Interface implementation
	void interface_init_container(yb::Type type) {
		m_storage_type = type_to_subtype(type);
	}
	void interface_deinit_container(yb::Type type) {}

	template<typename T1>
	void interface_assign_from(const T1& value) {
		m_value = value;
		m_map.clear();
		m_storage_type = Value_subtype::SCALAR;
	}

	
	TestStorage& interface_append_array_item() {
		assert(m_storage_type == Value_subtype::ARRAY);
		m_storage_type = Value_subtype::ARRAY;
		const int max_elm{getMaxArrayIndex()};
		return m_map[static_cast<size_t>(max_elm + 1)];
	}
	
	template<typename TKey>
	TestStorage& interface_append_map_item(const TKey& key) {
		assert(m_storage_type == Value_subtype::OBJECT);
		m_storage_type = Value_subtype::OBJECT;
		return m_map[tests::MapKey{key}];
	}
	//End if Interface implementation


	//For tests
	TestStorage() = default;
	TestStorage(const TestStorage& other): m_value(other.m_value), m_map(other.m_map), m_storage_type(other.m_storage_type){}

	std::string asString() const {
		return std::get<std::string>(m_value.value());
	}
	int asInt() const {
		return std::get<int>(m_value.value());
	}
	unsigned int asUInt() const {
		return std::get<unsigned int>(m_value.value());
	}
	bool asBool() const {
		return std::get<bool>(m_value.value());
	}
	float asFloat() const {
		return std::get<float>(m_value.value());
	}
	double asDouble() const {
		return std::get<double>(m_value.value());
	}

	template<typename T1>
	T1 asValue() const {
		if constexpr(std::is_same_v<T1, int>) {
			return asInt();
		}
		else if constexpr(std::is_same_v<T1, unsigned int>) {
			return asUInt();
		}
		else if constexpr(std::is_same_v<T1, float>) {
			return asFloat();
		}
		else if constexpr(std::is_same_v<T1, double>) {
			return asDouble();
		}
		else if constexpr(std::is_same_v<T1, bool>) {
			return asBool();
		}
		else {
			return asString();
		}
	}
	
	bool isString() const {
		return m_storage_type == Value_subtype::SCALAR && m_value.has_value() && std::holds_alternative<std::string>(m_value.value());
	}
	bool isInt() const {
		return m_storage_type == Value_subtype::SCALAR && m_value.has_value() && std::holds_alternative<int>(m_value.value());
	}
	bool isUInt() const {
		return m_storage_type == Value_subtype::SCALAR && m_value.has_value() && std::holds_alternative<unsigned int>(m_value.value());
	}
	bool isFloat() const {
		return m_storage_type == Value_subtype::SCALAR && m_value.has_value() && (std::holds_alternative<float>(m_value.value()) || std::holds_alternative<double>(m_value.value()));
	}
	bool isBool() const {
		return m_storage_type == Value_subtype::SCALAR && m_value.has_value() && std::holds_alternative<bool>(m_value.value());
	}
	bool isObject() const {
		return m_storage_type == Value_subtype::OBJECT;
	}
	bool isArray() const {
		return m_storage_type == Value_subtype::ARRAY;
	}
	bool isNull() const {
		return m_storage_type == Value_subtype::NONE && !m_value.has_value() && m_map.empty();
	}
	
	const auto& get_map() const {
		return m_map;
	}
	
	size_t size() const {
		return m_map.size();
	}
	TestStorage& operator[](size_t index) {
		return m_map[tests::MapKey{index}];
	}
	TestStorage& operator[](const tests::MapKey& key) {
		return m_map[key];
	}
	//--------------------------------------
private:
	int getMaxArrayIndex() const {
		if(m_map.empty()) {
			return -1;
		}
		const auto& max_elm = std::max_element(m_map.begin(), m_map.end(), [](const auto& item1, const auto& item2) -> bool {
			if(!std::holds_alternative<size_t>(item1.first.deserialize()) || !std::holds_alternative<size_t>(item2.first.deserialize())) {
				return false;
			}
			return std::get<size_t>(item1.first.deserialize()) < std::get<size_t>(item2.first.deserialize());
		});
		
		return static_cast<int>(std::get<size_t>(max_elm->first.deserialize()));
	}
	
	enum class Value_subtype {NONE, SCALAR, ARRAY, OBJECT};

	static Value_subtype type_to_subtype(yb::Type type) {
		return (type == Type::array_container ? Value_subtype::ARRAY : type == Type::object_container ? Value_subtype::OBJECT : type == Type::null_value ? Value_subtype::NONE : Value_subtype::SCALAR);
	}

	Value_subtype m_storage_type{Value_subtype::NONE};
};

#if __cplusplus >= 202002L
static_assert(Storage_concept_from_cpp<TestStorage>);
#endif

} //end of namespace yb::from_cpp


