/*
 *  engine_to_cpp.h
 *
 *  Created by Yuri Barmin on 19.07.2020.
 *
 */

#pragma once

#include <memory>
#include <deque>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <tuple>
#include <set>
#include <list>

#include <cassert>

#include <stddef.h>

#include "string_utils.h"

#include "engine_common.h"
#include "engine_addons.h"

#define SER_META_TABLE_LAMBDA_ADAPTER(name, adapterFunc)                        \
[adapter = adapterFunc](auto ptr) {                                             \
	return adapter(ptr->name);                                                  \
}

#define DESER_META_TABLE_LAMBDA_ADAPTER(name, adapterType, adapterFunc)             \
[adapter = std::tuple{(adapterType*)nullptr, adapterFunc}](auto ptr, auto data) {   \
	using type = std::remove_pointer_t<std::tuple_element_t<0, decltype(adapter)>>;	\
	if constexpr(std::is_same_v<decltype(ptr), nullptr_t>) { \
		return static_cast<type*>(nullptr);                  \
	}                                                        \
	else if constexpr(std::is_move_constructible_v<type>) {  \
		ptr->name = std::get<1>(adapter)(std::move(*data));  \
	}                                                        \
	else {                                                   \
		ptr->name = std::get<1>(adapter)(*data);             \
	}                                                        \
}


#define BEGIN_DESER_META_TABLE \
	private: \
		template<typename, Storage_concept_to_cpp Storage> friend class yb::to_cpp::Engine_to_cpp; \
		template<typename, Storage_concept_from_cpp Storage> friend class yb::from_cpp::Engine_from_cpp; \
		friend struct yb::find_prop_class; \
		static constexpr auto meta_table = std::make_tuple(


//Items

#define DEFINE_DESER_META_TABLE_ITEM(name) DEFINE_META_TABLE_ITEM(name)

#define DEFINE_DESER_META_TABLE_ITEM_DEFAULT(name, def) std::tuple{                \
	yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE   |                                    \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_DEFAULT, \
	YB_STATIC_STRING(#name),                          \
	DESER_META_TABLE_LAMBDA(name),                                                 \
	[](auto, auto){},                                                              \
	[]{return def;}},

#define DEFINE_DESER_META_TABLE_ITEM_EMPTY(name) std::tuple{                     \
	yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE   |                                    \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_EMPTY, \
	YB_STATIC_STRING(#name),                        \
	DESER_META_TABLE_LAMBDA(name),                                               \
	[](auto, auto){},                                                            \
	[]() -> void {}},


//Items with adapter
#define DEFINE_DESER_META_TABLE_ITEM_ADAPTER(name, adapterType, adapterFunc) std::tuple{ \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_ADAPTER,       \
	YB_STATIC_STRING(#name),                                \
	DESER_META_TABLE_LAMBDA(name),                                                       \
	DESER_META_TABLE_LAMBDA_ADAPTER(name, adapterType, adapterFunc) },

//Items with adapter
#define DEFINE_SER_META_TABLE_ITEM_ADAPTER(name, adapterFunc) std::tuple{ \
	yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_ADAPTER,       \
	YB_STATIC_STRING(#name),                                \
	DESER_META_TABLE_LAMBDA(name),                                                       \
	SER_META_TABLE_LAMBDA_ADAPTER(name, adapterFunc) },

#define DEFINE_DESER_META_TABLE_ITEM_DEFAULT_ADAPTER(name, def, adapterType, adapterFunc) std::tuple{ \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_ADAPTER | yb::ANY_STL_METATABLE_ITEM_TYPE_DEFAULT,                      \
	YB_STATIC_STRING(#name),                                             \
	DESER_META_TABLE_LAMBDA(name),                                                                    \
	DESER_META_TABLE_LAMBDA_ADAPTER(name, adapterType, adapterFunc),                                  \
	[]() {return def;} },

#define DEFINE_DESER_META_TABLE_ITEM_EMPTY_ADAPTER(name, adapterType, adapterFunc) std::tuple{         \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_ADAPTER | yb::ANY_STL_METATABLE_ITEM_TYPE_EMPTY,                         \
	YB_STATIC_STRING(#name),                                              \
	DESER_META_TABLE_LAMBDA(name),                                                                     \
	DESER_META_TABLE_LAMBDA_ADAPTER(name, adapterType, adapterFunc),                                   \
	[]() -> adapterType {return {};}},

//Container items
#define DEFINE_DESER_META_TABLE_CONTAINER_ITEM(name) std::tuple{   \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_CONTAINER_ITEM,                        \
	YB_STATIC_STRING(#name),          \
	DESER_META_TABLE_LAMBDA(name),                                 \
	[](auto, auto){}},


#define DEFINE_META_TABLE_CONTAINER_ITEM(name) std::tuple{   \
	yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_CONTAINER_ITEM,                        \
	YB_STATIC_STRING(#name),          \
	DESER_META_TABLE_LAMBDA(name),                                 \
	[](auto, auto){}},

#define DEFINE_DESER_META_TABLE_CONTAINER_ITEM_ADAPTER(name, adapterType, adapterFunc) std::tuple{          \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_CONTAINER_ITEM | yb::ANY_STL_METATABLE_ITEM_TYPE_ADAPTER,                          \
	YB_STATIC_STRING(#name),                                                   \
	DESER_META_TABLE_LAMBDA(name),                                                                          \
	DESER_META_TABLE_LAMBDA_ADAPTER(name, adapterType, adapterFunc)},

#define DEFINE_SER_META_TABLE_CONTAINER_ITEM_ADAPTER(name, adapterFunc) std::tuple{          \
	yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_CONTAINER_ITEM | yb::ANY_STL_METATABLE_ITEM_TYPE_ADAPTER,                          \
	YB_STATIC_STRING(#name),                                                   \
	DESER_META_TABLE_LAMBDA(name),                                                                          \
	SER_META_TABLE_LAMBDA_ADAPTER(name, adapterFunc)},

#define END_DESER_META_TABLE END_META_TABLE

#define DEFINE_DESER_GETTER(name, type) private: type name{}; public: \
	const auto& get_##name() const noexcept { \
	constexpr auto arrName = YB_STATIC_STRING(#name); \
	constexpr bool result = yb::find_prop_class::find_prop<std::decay_t<std::remove_pointer_t<decltype(this)>>, yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE>(arrName);                                        \
	static_assert(result, "Could not find property name in the meta table");                       \
	return name;}  public:


#if defined(NDEBUG)
#define PRIVATE_IN_NDEBUG private:
#else
#define PRIVATE_IN_NDEBUG public: 																	
#endif

#define DEFINE_DESER_GETTER_UNITTEST(name, type) PRIVATE_IN_NDEBUG type name{}; public: \
	const auto& get_##name() const noexcept { \
	constexpr auto arrName = YB_STATIC_STRING(#name); \
	constexpr bool result = yb::find_prop_class::find_prop<std::decay_t<std::remove_pointer_t<decltype(this)>>, yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE>(arrName);                                        \
	static_assert(result, "Could not find property name in the meta table");                       \
	return name;}  public:


#define DEFINE_DESER_CONTAINER_GETTER(name, type) private: type name{}; public: \
	operator const type&() const noexcept {return name;}                        \
	bool operator == (const type& other)  const noexcept {return name == other;} \
	bool operator != (const type& other)  const noexcept {return name != other;} \
	const auto& get_##name() const noexcept {                                      \
	constexpr auto arrName = YB_STATIC_STRING(#name);  \
	constexpr bool result = yb::find_prop_class::find_prop<std::decay_t<std::remove_pointer_t<decltype(this)>>,           yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE>(arrName);                                        \
	static_assert(result, "Could not find property name in the meta table"); \
	return name;}  public:

#define DEFINE_CONTAINER_DATA(name, type) type name{}; public: \
	operator const type&() const noexcept {return name;}                        \
	bool operator != (const type& other)  const noexcept {return name != other;} \
	private: void check##name() const noexcept {                                      \
	constexpr auto arrName = YB_STATIC_STRING(#name);  \
	constexpr bool result = yb::find_prop_class::find_prop<std::decay_t<std::remove_pointer_t<decltype(this)>>,           yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE>(arrName);                                        \
	static_assert(result, "Could not find property name in the meta table"); }                     \
public:

#define DEFINE_DESER_COMPLETE_HANDLER(handler) static constexpr auto deserCompleteHandler {handler};

namespace yb::to_cpp {

template<typename T, Storage_concept_to_cpp Storage>
T get_storage_value(const Storage& storage) {
	return storage.template interface_get_value<T>();
};

template<typename T, Storage_concept_to_cpp Storage>
inline bool is_right_type(const Storage& storage) {
	bool rightType = false;
	if constexpr (std::is_same_v<T, std::string>) {
		rightType = storage.interface_get_type() == Type::string_value;
	}
	else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned> || std::is_same_v<T, long> || std::is_same_v<T, long long> || std::is_same_v<T, unsigned long>) {
		rightType = storage.interface_get_type() == Type::int_value || storage.interface_get_type() == Type::uint_value;
	}
	else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
		rightType = storage.interface_get_type() == Type::float_value || storage.interface_get_type() == Type::int_value || storage.interface_get_type() == Type::uint_value;
	}
	else if constexpr (std::is_same_v<T, bool>) {
		rightType = storage.interface_get_type() == Type::boolean_value;
	}
	return rightType;
}

template<typename T, Storage_concept_to_cpp Storage>
inline T get_storage_value_no_key(const Storage& storage, bool& success, const T& defaultValue = {}) {
	if constexpr (std::is_same_v<T, std::string>) {
		success = true;
		switch(storage.interface_get_type()) {
			case Type::int_value:
				return std::to_string(get_storage_value<int>(storage));
			case Type::uint_value:
				return std::to_string(get_storage_value<unsigned>(storage));
			case Type::float_value:
				return std::to_string(get_storage_value<float>(storage));
			case Type::string_value:
				return get_storage_value<std::string>(storage);
			case Type::boolean_value:
				return std::to_string(get_storage_value<bool>(storage));
			default:
				break;
		}
	}
	else if(is_right_type<T>(storage)) {
		success = true;
		return get_storage_value<T>(storage);
	}
	success = false;
	return defaultValue;
}

template<class T, Storage_concept_to_cpp Storage>
inline bool value_storage_output(T &value,  const Storage& storage) {
	static_assert(!std::is_pointer<T>::value);
	
	const auto type = storage.interface_get_type();
	switch(type) {
		case Type::string_value:
			[[fallthrough]];
		case Type::int_value:
			[[fallthrough]];
		case Type::uint_value:
			[[fallthrough]];
		case Type::float_value:
			[[fallthrough]];
		case Type::boolean_value:
			break;
		default:
			return false;
	};
	
	bool success = false;
	value = get_storage_value_no_key<T>(storage, success);
	
	return success;
}

template <class T, Storage_concept_to_cpp Storage>
class Engine_to_cpp {
	template <class T1, Storage_concept_to_cpp Storage1>
	friend Engine_to_cpp<T1, Storage1> storage_to_cpp_instance(T1 &cont, const Storage1& storage);
	
	T &m_cont;
	const Storage& m_storage;
	
	//constructors
	Engine_to_cpp(T &cont, const Storage& storage): m_cont(cont), m_storage(storage){}
	
	DEFINE_MEMBER_CHECKER(meta_table)
	DEFINE_MEMBER_CHECKER(deserCompleteHandler)
	
	//parse other types
	template<class T1>
	bool read(T1 &value, const Storage& cur_storage) {
		if constexpr(HAS_MEMBER(T1, meta_table)) {
			return read_meta_table(value, cur_storage);
		}
		else if constexpr(std::is_enum_v<T1>) {
			return read_enum(value, cur_storage);
		}
		else if constexpr(!is_allowed_type<T1>()) {
			return meta_table_to_cpp(value, cur_storage);
		}
		else {
			const bool result = value_storage_output(value, cur_storage);
			return result;
		}
	}
	
	
	template<class T1>
	bool read_array(T1 &value, const Storage &cur_storage) {
		assert(cur_storage.interface_get_type() == Type::array_container || cur_storage.interface_get_type() == Type::null_value);
		if(cur_storage.interface_get_type() != Type::array_container) {
			return false;
		}
		
		if constexpr (std::is_same_v<T1, std::vector<typename T1::value_type>>) {
			value.reserve(cur_storage.interface_size());
		}
		value.clear();
		for(auto node_it = cur_storage.interface_begin(); node_it.interface_not_equal_to(cur_storage.interface_end()); node_it.interface_increment()) {
			const auto& nodeItem = std::decay_t<decltype(m_storage)>::interface_get_storage_from_iterator(node_it);
			if constexpr (std::is_same_v<T1, std::set<typename T1::value_type>> || std::is_same_v<T1, std::unordered_set<typename T1::value_type>>) {
				typename T1::value_type item;
				
				if(!read(item, nodeItem)) {
					return false;
				}
				[[maybe_unused]] const auto result = value.insert(std::move(item));
				assert(result.second);
			}
			else {
				typename T1::value_type *const item = &(value.emplace_back());
				
				if(!read(*item, nodeItem)) {
					return false;
				}

			}
		}
		
		return true;
	}
	
	template<class T1>
	bool read_map(T1 &value, const Storage &cur_storage) {
		assert(cur_storage.interface_get_type() == Type::object_container || cur_storage.interface_get_type() == Type::null_value);
		if(cur_storage.interface_get_type() != Type::object_container) {
			return false;
		}
		
		auto item_it = cur_storage.interface_begin();
		const auto end_it = cur_storage.interface_end();
		for (; item_it.interface_not_equal_to(end_it); item_it.interface_increment()) {
			const auto& j_value = std::decay_t<decltype(m_storage)>::interface_get_storage_from_iterator(item_it);
			const typename T1::key_type& key = item_it.template interface_get_key<typename T1::key_type>();
			
			const auto result = value.emplace(key, typename T1::mapped_type());
			assert(result.second);
			
			if(result.second) {
				auto& item = (*result.first).second;
				if(!read(item, j_value)) {
					return false;
				}
			}
		}
		
		return true;
	}

	template<class T1>
	bool read_pair(T1 &value, const Storage &cur_storage) {
		assert(cur_storage.interface_get_type() == Type::array_container || cur_storage.interface_get_type() == Type::null_value);
		if(cur_storage.interface_get_type() != Type::array_container) {
			return false;
		}

		assert(cur_storage.interface_size() == 2);
		if(cur_storage.interface_size() != 2) {
			return false;
		}
		
		auto itemIt = cur_storage.interface_begin();

		{
			const auto& jValue = std::decay_t<decltype(m_storage)>::interface_get_storage_from_iterator(itemIt);
			if(!read(value.first, jValue)) {
				return false;
			}
		}

		{
			itemIt.interface_increment();
			const auto& jValue = std::decay_t<decltype(m_storage)>::interface_get_storage_from_iterator(itemIt);
			if(!read(value.second, jValue)) {
				return false;
			}
		}

		return true;
	}

	template<typename T1, size_t tupleIndex, Const_iterator_concept Iterator>
	constexpr bool each_read_tuple_item(T1& value, const Iterator& curNodeIterator, bool& success){
		auto& tupleItem = std::get<tupleIndex>(value);
		
		const auto& jValue = std::decay_t<decltype(m_storage)>::interface_get_storage_from_iterator(curNodeIterator);
		if(!read(tupleItem, jValue)) {
			success = false;
			return false;
		}
		
		return true;
	}
	
	template<class T1, size_t Index = 0, size_t Size, Const_iterator_concept Iterator>
	constexpr void for_each_read_tuple_static_index(T1& value, Iterator& curNodeIterator, bool& success) {
		if(!each_read_tuple_item<T1, Index>(value, curNodeIterator, success)) {
			return;
		}
		
		curNodeIterator.interface_increment();
		
		if constexpr(Index + 1 != Size) {
			for_each_read_tuple_static_index<T1, Index + 1, Size>(value, curNodeIterator, success);
		}
	}

	
	template<class T1>
	bool read_tuple(T1 &value, const Storage &cur_storage) {
		assert(cur_storage.interface_get_type() == Type::array_container || cur_storage.interface_get_type() == Type::null_value);
		if(cur_storage.interface_get_type() != Type::array_container) {
			return false;
		}
		
		assert(std::tuple_size_v<T1> == cur_storage.interface_size());
		if(std::tuple_size_v<T1> != cur_storage.interface_size()) {
			return false;
		}

		bool success{true};
		auto curNodeIterator = cur_storage.interface_begin();
		for_each_read_tuple_static_index<T1, 0, std::tuple_size_v<T1>>(value, curNodeIterator, success);

		return success;
	}

	template<typename T1, size_t tuple_index>
	constexpr bool each_meta_table_item_pay_load(const Storage& storage_by_key, T1& value) {
		constexpr auto& item = std::get<tuple_index>(T1::meta_table);
		constexpr unsigned flags = std::get<yb::ANY_STL_METATABLE_ITEM_INDEX_FLAGS>(item);
		constexpr bool is_container = (flags & yb::ANY_STL_METATABLE_ITEM_TYPE_CONTAINER_ITEM) != 0u;
		constexpr bool is_adapter = (flags & yb::ANY_STL_METATABLE_ITEM_TYPE_ADAPTER) != 0u;
		constexpr bool is_default = (flags & yb::ANY_STL_METATABLE_ITEM_TYPE_DEFAULT) != 0u;
		constexpr bool is_def_empty = (flags & yb::ANY_STL_METATABLE_ITEM_TYPE_EMPTY) != 0u;

		constexpr auto& lambda = std::get<yb::ANY_STL_METATABLE_ITEM_INDEX_LAMBDA>(item);
		constexpr auto& lambda_adapter = std::get<yb::ANY_STL_METATABLE_ITEM_INDEX_ADAPTER>(item);

		using Type_data = std::remove_pointer_t<std::invoke_result_t<decltype(lambda), nullptr_t, nullptr_t>>;
		using Type_data_for_adapter = std::remove_pointer_t<std::invoke_result_t<decltype(lambda_adapter), nullptr_t, nullptr_t>>;

		if (storage_by_key.interface_get_type() == Type::null_value) {
			if (is_container) {
				assert(false && "[Engine_to_cpp::read_meta_table] Could not read property from storage: node not found");
				return false;
			}
			else if constexpr (!is_container && (is_default || is_def_empty)) { //set default value
				if constexpr (!is_adapter && is_def_empty) {
					return true;
				}
				else {
					if constexpr (is_adapter) {
						Type_data_for_adapter defData{ std::get<yb::ANY_STL_METATABLE_ITEM_INDEX_DEFAULT>(item)() };
						lambda_adapter(&value, &defData);
					}
					else {
						Type_data def_data{ std::get<yb::ANY_STL_METATABLE_ITEM_INDEX_DEFAULT>(item)() };
						auto data = lambda(&value, nullptr);
						*data = std::move(def_data);
					}
					return true;
				}
			}
			else {
//						assert(false && "[Engine_to_cpp::read_meta_table] Could not read property from storage: node not found");
				return false;
			}
		}

		bool success{};
		if constexpr (is_adapter) {
			Type_data_for_adapter data{};
			success = read(data, storage_by_key);
			if (success) {
				lambda_adapter(&value, &data);
			}
		}
		else {
			auto data = lambda(&value, nullptr);
			success = read(*data, storage_by_key);
		}
		assert(success && "[Engine_to_cpp::read_meta_table] Could not read property from storage");

		return success;
	}

	template<typename T1, size_t tuple_index>
	constexpr bool each_meta_table_item(T1& value, const Storage &cur_storage, bool& success){
		constexpr auto& item = std::get<tuple_index>(T1::meta_table);
		using Tuple_type = std::decay_t<decltype(item)>;
		constexpr size_t tuple_size{std::tuple_size_v<Tuple_type>};
		
		if constexpr(tuple_size == 0) {//last empty item
			return false;
		}
		else {
			constexpr unsigned flags = std::get<yb::ANY_STL_METATABLE_ITEM_INDEX_FLAGS>(item);
			
			constexpr auto& arr_name = std::get<yb::ANY_STL_METATABLE_ITEM_INDEX_NAME>(item);
			constexpr bool result = yb::find_prop_class::find_prop<T1, yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE>(arr_name);
			static_assert(result, "Could not find apropriate meta table item for deseralize");
			
			constexpr bool is_deserialize = (flags & yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE) != 0u;
			
			if constexpr(!is_deserialize) {
				return true;
			}
			else {
				constexpr bool is_container = (flags & yb::ANY_STL_METATABLE_ITEM_TYPE_CONTAINER_ITEM) != 0u;
				
				if constexpr(is_container) {//if is an container item
					success = each_meta_table_item_pay_load<T1, tuple_index>(cur_storage, value);
				}
				else  {
					//cur_storage.interface_get_storage_by_key can return 'const Storage&' or 'Storage'. In the second case, NRVO will work.
					
					constexpr auto name = std::get<yb::ANY_STL_METATABLE_ITEM_INDEX_NAME>(item).data();
					
					const Storage& storage_by_key = cur_storage.interface_get_storage_by_key(name);
					success = each_meta_table_item_pay_load<T1, tuple_index>(storage_by_key, value);
				}

				
				
				return success;
			}
		}
	}

	template<class T1, size_t Index = 0, size_t Size>
	constexpr void for_each_static_index(T1& value, const Storage &cur_storage, bool& success) {
		if(!each_meta_table_item<T1, Index>(value, cur_storage, success)) {
			return;
		}
		
		if constexpr(Index + 1 != Size) {
			for_each_static_index<T1, Index + 1, Size>(value, cur_storage, success);
		}
	}

	template<class T1>
	bool read_meta_table(T1 &value, const Storage &cur_storage) {
		bool success{true};
		
		for_each_static_index<T1, 0, std::tuple_size_v<decltype(T1::meta_table)>>(value, cur_storage, success);
		
		if constexpr(HAS_MEMBER(T1, deserCompleteHandler)) {
			if(success) {
				T1::deserCompleteHandler(value);
			}
		}
		
		return success;
	}

	template<class T1>
	bool read_enum(T1 &value, const Storage &cur_storage) {
		assert(cur_storage.interface_get_type() == Type::string_value || cur_storage.interface_get_type() == Type::null_value);
		if(cur_storage.interface_get_type() != Type::string_value) {
			return false;
		}
		
		const auto str = cur_storage.template interface_get_value<std::string>();
		const auto result = yb_enum_from_string(str, T1());
//		assert(result && "Could not convert enum to string");
		if(result.has_value()) {
			value = result.value();
			return true;
		}
		return false;
	}
	
	template<class T1>
	bool read(std::vector<T1> &value, const Storage& cur_storage) {
		return read_array(value, cur_storage);
	}
	
	template<class T1>
	bool read(std::list<T1> &value, const Storage& cur_storage) {
		return read_array(value, cur_storage);
	}
	
	template<class _Key, class _Compare, class _Allocator>
	bool read(std::set<_Key, _Compare, _Allocator> &value, const Storage& cur_storage) {
		return read_array(value, cur_storage);
	}

	template<class T1>
	bool read(std::unordered_set<T1> &value, const Storage& cur_storage) {
		return read_array(value, cur_storage);
	}

	template<class T1>
	bool read(std::deque<T1> &value, const Storage& cur_storage) {
		return read_array(value, cur_storage);
	}
	
	template <class _Key, class _Tp, class _Allocator>
	bool read(std::map<_Key, _Tp, _Allocator> &value, const Storage& cur_storage) {
		return read_map(value, cur_storage);
	}
	
	template <class T1, class T2>
	bool read(std::unordered_map<T1, T2> &value, const Storage& cur_storage) {
		return read_map(value, cur_storage);
	}

	template <class T1, class T2>
	bool read(std::pair<T1, T2> &value, const Storage& cur_storage) {
		return read_pair(value, cur_storage);
	}

	template <class ...T1>
	bool read(std::tuple<T1...> &value, const Storage& cur_storage) {
		return read_tuple(value, cur_storage);
	}

	template<class T1, class T2>
	bool read(std::pair<T1, T2> &value, const std::pair<const char*, const Storage*>& cur_it) {
		value.first = yb::string_utils::string_to_val<T1>(cur_it.first);
		
		return read(value.second, *cur_it.second);
	}
public:
	[[nodiscard]] bool read_from() {
		const bool result = this->read(m_cont, m_storage);
		return result;
	}
};

template <class T, Storage_concept_to_cpp Storage>
Engine_to_cpp<T, Storage> storage_to_cpp_instance(T &cont, const Storage& storage) {
	Engine_to_cpp<T, Storage> d(cont, storage);
	return d;
}

} //end of namespace common::utilsModule

