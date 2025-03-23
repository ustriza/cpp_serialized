//
//  test_storage_to_cpp.h
//
//  Created by Yuri Barmin on 08.08.2023.
//

#pragma once

#include "engine_to_cpp.h"
#include "test_storage_common.h"

namespace yb::to_cpp {

class TestStorage {
	using Value_type = std::variant<int, unsigned int, float, double, std::string, bool>;
	using Map = std::map<tests::MapKey, TestStorage>;
	
	std::optional<Value_type> m_value;
	Map m_map;
public:
	
	//----Methods for Const_iterator_concept

	template<OptionsForEngine option>
	static constexpr auto get_options_for_engine(){}

	//Iterators
	class Const_value_iterator {
	public:
		Const_value_iterator(TestStorage::Map::const_iterator iter): m_iter(iter){}

		//Methods for Const_iterator_concept

		void interface_increment() {++m_iter;}
		
		bool interface_not_equal_to(const Const_value_iterator& other) const {return m_iter != other.m_iter;}
		
		template<typename T1>
		const T1& interface_get_key() const {
			return std::get<T1>(m_iter->first.deserialize());
		}
		//----------------------------------------\
		
		const TestStorage& get_storage() const {return m_iter->second;}
	private:
		TestStorage::Map::const_iterator m_iter;
	};
#if __cplusplus >= 202002L
	static_assert(yb::to_cpp::Const_iterator_concept<Const_value_iterator>);
#endif
	// end of Iterators

	//----Methods for Storage_concept_to_cpp
	template<typename T1>
	T1 interface_get_value() const {
		if constexpr(std::is_same_v<T1, int>) {
			return std::get<int>(m_value.value());
		}
		else if constexpr(std::is_same_v<T1, unsigned int>) {
			return std::get<unsigned int>(m_value.value());
		}
		else if constexpr(std::is_same_v<T1, float> || std::is_same_v<T1, double>) {
			if(isInt()) {
				return static_cast<T1>(std::get<int>(m_value.value()));
			}
			if(isUInt()) {
				return static_cast<T1>(std::get<unsigned int>(m_value.value()));
			}
			return std::get<T1>(m_value.value());
		}
		else if constexpr(std::is_same_v<T1, bool>) {
			return std::get<bool>(m_value.value());
		}
		else {
			return std::get<std::string>(m_value.value());
		}
	}

	yb::Type interface_get_type() const {
		if(isString()) {
			return yb::Type::string_value;
		}
		if(isInt()) {
			return yb::Type::int_value;
		}
		if(isUInt()) {
			return yb::Type::uint_value;
		}
		if(isFloat()) {
			return yb::Type::float_value;
		}
		if(isBool()) {
			return yb::Type::boolean_value;
		}
		if(isObject()) {
			return yb::Type::object_container;
		}
		if(isArray()) {
			return yb::Type::array_container;
		}
		if(isNull()) {
			return yb::Type::null_value;
		}
		assert(false && "Unknown type");
		return yb::Type::null_value;
	}
	
	//container support
	size_t interface_size() const {
		return m_map.size();
	}
	Const_value_iterator interface_begin() const {
		return {m_map.begin()};
	}
	Const_value_iterator interface_end() const {
		return {m_map.end()};
	}

	template<typename T1>
	const TestStorage& interface_get_storage_by_key(const T1& key) const {
		const static TestStorage empty_storage;
		if(const auto find_it = m_map.find(tests::MapKey{key}); find_it != m_map.end()) {
			return find_it->second;
		}
		return empty_storage;
	}

	static const TestStorage& interface_get_storage_from_iterator(const Const_value_iterator& iter) {
		return iter.get_storage();
	}

	//----End of Methods for Storage_concept_to_cpp
	
	//For tests
	TestStorage() = default;
	
	TestStorage(int value): m_value(value), m_storage_type(Value_subtype::SCALAR){};
	TestStorage(unsigned int value): m_value(value), m_storage_type(Value_subtype::SCALAR){};
	TestStorage(float value): m_value(value), m_storage_type(Value_subtype::SCALAR){};
	TestStorage(double value): m_value(value), m_storage_type(Value_subtype::SCALAR){};
	TestStorage(bool value): m_value(value), m_storage_type(Value_subtype::SCALAR){};
	TestStorage(const std::string& value): m_value(value), m_storage_type(Value_subtype::SCALAR){};
	TestStorage(std::string_view value): m_value(std::string(value)), m_storage_type(Value_subtype::SCALAR){};
	TestStorage(const char* value): m_value(std::string(value)), m_storage_type(Value_subtype::SCALAR){};
	
	TestStorage(const TestStorage& other): m_value(other.m_value), m_map(other.m_map), m_storage_type(other.m_storage_type){}
	TestStorage(TestStorage&& other): m_value(std::move(other.m_value)), m_map(std::move(other.m_map)), m_storage_type(other.m_storage_type){other.m_storage_type = Value_subtype::NONE;}
	
	TestStorage& operator=(const TestStorage& other) {
		m_value = other.m_value;
		m_map = other.m_map;
		m_storage_type = other.m_storage_type;
		return *this;
	}
	TestStorage& operator=(TestStorage&& other) {
		m_value = std::move(other.m_value);
		m_map = std::move(other.m_map);
		m_storage_type = other.m_storage_type;
		other.m_storage_type = Value_subtype::NONE;
		return *this;
	}
	
	template<typename T1>
	TestStorage& append(const T1& value) {
		m_storage_type = Value_subtype::ARRAY;
		const int max_elm{getMaxArrayIndex()};
		m_map[static_cast<size_t>(max_elm + 1)] = value;
		return *this;
	}
	
	template<typename TKey, typename TValue>
	TestStorage& append(const TKey& key, const TValue& value) {
		m_storage_type = Value_subtype::OBJECT;
		m_map.emplace(key, value);
		return *this;
	}
	
	TestStorage& operator[](const std::string& key) {
		m_storage_type = Value_subtype::OBJECT;
		return m_map[tests::MapKey{key}];
	}

private:
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
	Value_subtype m_storage_type{Value_subtype::NONE};
};
#if __cplusplus >= 202002L
static_assert(Storage_concept_to_cpp<TestStorage>);
#endif
} //end of namespace yb::to


