//
//  jsoncpp_storage.h
//  anyToStl
//
//  Created by Yuri Barmin on 01.09.2023.
//

#pragma once
#include <json/value.h>

namespace yb::jsoncpp {

class Const_value_iterator {
public:
	Const_value_iterator(const Json::Value::const_iterator& iter): m_iter(iter){}
	
	//Methods for Const_iterator_concept
	template<OptionsForEngine option>
	static constexpr auto get_options_for_engine(){
		if constexpr(option == OptionsForEngine::STORAGE_TYPE_FOR_ITEM) {
			return static_cast<std::add_pointer_t<Json::Value>>(nullptr);
		}
	}

	void interface_increment() {++m_iter;}
	
	bool interface_not_equal_to(const Const_value_iterator& other) const {return m_iter != other.m_iter;}
	
	template<typename T1>
	T1 interface_get_key() const {
		return yb::string_utils::string_to_val<T1>(m_iter.name());
	}
	//-------------------------------------
	
	const Json::Value& get_json() const {
		return *m_iter;
	}
private:
	Json::Value::const_iterator m_iter;
};
#if __cplusplus >= 202002L
static_assert(yb::to_cpp::Const_iterator_concept<Const_value_iterator>);
#endif
// end of Iterators

class Json_storage_adapter_to_cpp {
public:
	
	//----Methods for Storage_concept_to_cpp
	
	template<OptionsForEngine option>
	static constexpr auto get_options_for_engine(){
		if constexpr(option == OptionsForEngine::STORAGE_TYPE_FOR_ITEM) {
			return static_cast<std::add_pointer_t<Json::Value>>(nullptr);
		}
	}

	template<typename T1>
	auto interface_get_value() const -> std::conditional_t<std::is_same_v<T1, Json::Value>, const T1&, T1>{
		if constexpr(std::is_same_v<T1, Json::Value>) {
			return m_json_storage;
		}
		else {
			return m_json_storage.as<T1>();
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
		return m_json_storage.size();
	}
	Const_value_iterator interface_begin() const {
		return {m_json_storage.begin()};
	}
	Const_value_iterator interface_end() const {
		return {m_json_storage.end()};
	}

	template<typename T1>
	Json_storage_adapter_to_cpp interface_get_storage_by_key(const T1& key) const {
		return m_json_storage[std::string{key}];
	}
	
	static Json_storage_adapter_to_cpp interface_get_storage_from_iterator(const Const_value_iterator& iter) {
		return Json_storage_adapter_to_cpp{iter.get_json()};
	}
	//----End of Methods for Storage_concept_to_cpp
	
		//For tests
	Json_storage_adapter_to_cpp(const Json::Value& storage): m_json_storage{storage}{}
	
	const Json::Value& get_json_value() const {
		return m_json_storage;
	}

private:
	bool isString() const {
		return m_json_storage.isString();
	}
	bool isInt() const {
		return m_json_storage.type() == Json::ValueType::intValue;
	}
	bool isUInt() const {
		return m_json_storage.type() == Json::ValueType::uintValue;
	}
	bool isFloat() const {
		return m_json_storage.isDouble();
	}
	bool isBool() const {
		return m_json_storage.isBool();
	}
	bool isObject() const {
		return m_json_storage.isObject();
	}
	bool isArray() const {
		return m_json_storage.isArray();
	}
	bool isNull() const {
		return m_json_storage.isNull();
	}

	const Json::Value& m_json_storage;
};
#if __cplusplus >= 202002L
static_assert(Storage_concept_to_cpp<Json_storage_adapter_to_cpp>);
#endif

class Json_storage_adapter_from_cpp {
public:
	
	//Interface implementation. Methods for Storage_concept_from_cpp concept.
	template<OptionsForEngine option>
	static constexpr auto get_options_for_engine(){
		if constexpr(option == OptionsForEngine::STORAGE_TYPE_FOR_ITEM) {
			return static_cast<std::add_pointer_t<Json::Value>>(nullptr);
		}
	}

	void interface_init_container(yb::Type type) {
		switch (type) {
			case Type::array_container:
				m_json_storage = Json::Value(Json::ValueType::arrayValue);
				break;
			case Type::object_container:
				m_json_storage = Json::Value(Json::ValueType::objectValue);
				break;
			default:
				assert(false && "[Json_storage_wrapper_from_cpp::interface_init_container] Wrong type.");
				break;
		}
	}
	void interface_deinit_container(yb::Type type) {}
	
	template<typename T1>
	void interface_assign_from(const T1& value) {
		m_json_storage = value;
	}
	
	
	Json_storage_adapter_from_cpp interface_append_array_item() {
		assert(m_json_storage.type() == Json::ValueType::arrayValue);
		return Json_storage_adapter_from_cpp{m_json_storage.append({})};
	}
	
	template<typename TKey>
	Json_storage_adapter_from_cpp interface_append_map_item(const TKey& key) {
		assert(m_json_storage.type() == Json::ValueType::objectValue || m_json_storage.type() == Json::ValueType::nullValue);
		return Json_storage_adapter_from_cpp{m_json_storage[yb::string_utils::val_to_string(key)]};
	}
	//End if Interface implementation
	
	Json_storage_adapter_from_cpp(Json::Value& storage): m_json_storage{storage}{}
	
	bool isString() const {
		return m_json_storage.isString();
	}
	bool isInt() const {
		return m_json_storage.type() == Json::ValueType::intValue;
	}
	bool isUInt() const {
		return m_json_storage.type() == Json::ValueType::uintValue;
	}
	bool isFloat() const {
		return m_json_storage.isDouble();
	}
	bool isBool() const {
		return m_json_storage.isBool();
	}
	bool isObject() const {
		return m_json_storage.isObject();
	}
	bool isArray() const {
		return m_json_storage.isArray();
	}
	bool isNull() const {
		return m_json_storage.isNull();
	}

	template<typename T1>
	T1 asValue() const {
		return m_json_storage.as<T1>();
	}
private:
	Json::Value& m_json_storage;
};

} //end of namespace yb::jsoncpp

