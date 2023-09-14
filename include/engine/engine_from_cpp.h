//
//  engine_from_cpp.h
//
//  Created by Yuri Barmin on 08.08.2023.
//

#pragma once
#include <type_traits>
#include <string>
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

#include "engine_common.h"
#include "string_utils.h"
#include "engine_addons.h"

namespace yb::from_cpp {

template<class T, Storage_concept_from_cpp Storage>
inline void value_storage_input(const T &value,  Storage& storage) {
	static_assert(!std::is_pointer<T>::value);
	storage.interface_assign_from(value);
}

template <class T, Storage_concept_from_cpp Storage>
class Engine_from_cpp {
	template <class T1, Storage_concept_from_cpp Storage1>
	friend Engine_from_cpp<T1, Storage1> cpp_to_storage_instance(const T1 &cont, Storage1& storage);

	const T &m_cont;
	Storage& m_storage;

		//constructors
	Engine_from_cpp(const T &cont, Storage& storage): m_cont{cont}, m_storage{storage}{}

	DEFINE_MEMBER_CHECKER(meta_table)

		//parse other types
	template<class T1>
	void write(const T1 &value, Storage& cur_storage) {
		if constexpr(HAS_MEMBER(T1, meta_table)) {
			write_meta_table(value, cur_storage);
		}
		else if constexpr(std::is_enum_v<T1>) {
			write_enum(value, cur_storage);
		}
		else if constexpr(!is_allowed_type<T1>()) {
			return meta_table_from_cpp(value, cur_storage);
		}
		else {
			value_storage_input(value, cur_storage);
		}
	}


	template<class T1>
	void write_array(const T1 &value, Storage &cur_storage) {
		cur_storage.interface_init_container(yb::Type::array_container);
		
		for(const auto& valueItem: value) {
			decltype(auto) new_storage = cur_storage.interface_append_array_item();
			write(valueItem, new_storage);
		}

		cur_storage.interface_deinit_container(yb::Type::array_container);
	}

	template<class T1>
	void write_map(const T1 &value, Storage &cur_storage) {
		cur_storage.interface_init_container(yb::Type::object_container);

		for(const auto& valueItem: value) {
			decltype(auto) new_storage = cur_storage.interface_append_map_item(valueItem.first);
			write(valueItem.second, new_storage);
		}
		cur_storage.interface_deinit_container(yb::Type::object_container);
	}

	template<class T1>
	void write_pair(const T1 &value, Storage &cur_storage) {
		cur_storage.interface_init_container(yb::Type::array_container);

		decltype(auto) first = cur_storage.interface_append_array_item();
		write(value.first, first);
		decltype(auto) second = cur_storage.interface_append_array_item();
		write(value.second, second);

		cur_storage.interface_deinit_container(yb::Type::array_container);
	}

	template<typename T1, size_t tupleIndex>
	constexpr void each_write_tuple_item(const T1& value, Storage &cur_storage) {
		const auto& tupleItem = std::get<tupleIndex>(value);
		decltype(auto) array_item = cur_storage.interface_append_array_item();
		write(tupleItem, array_item);
	}

	template<class T1, size_t Index = 0, size_t Size>
	constexpr void for_each_write_tuple_static_index(const T1& value, Storage &cur_storage) {
		each_write_tuple_item<T1, Index>(value, cur_storage);

		if constexpr(Index + 1 != Size) {
			for_each_write_tuple_static_index<T1, Index + 1, Size>(value, cur_storage);
		}
	}

	template<class T1>
	void write_tuple(const T1 &value, Storage &cur_storage) {
		cur_storage.interface_init_container(yb::Type::array_container);

		for_each_write_tuple_static_index<T1, 0, std::tuple_size_v<T1>>(value, cur_storage);

		cur_storage.interface_deinit_container(yb::Type::array_container);
	}

	template<typename T1, size_t tuple_index>
	constexpr void each_tuple_item(const T1& value, Storage &cur_storage){
		constexpr auto& item = std::get<tuple_index>(T1::meta_table);
		using Tuple_type = std::decay_t<decltype(item)>;
		constexpr auto tuple_size = std::tuple_size_v<Tuple_type>;

		if constexpr(tuple_size == 0) {//last empty item
			return;
		}
		else {
			constexpr auto& arr_name = std::get<ANY_STL_METATABLE_ITEM_INDEX_NAME>(item);
			constexpr bool result = yb::find_prop_class::find_prop<T1, ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE>(arr_name);
			static_assert(result == true, "Could not find apropriate meta table item for seralize");


			constexpr unsigned flags = std::get<ANY_STL_METATABLE_ITEM_INDEX_FLAGS>(item);
			constexpr bool is_serialize = (flags & ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE) != 0u;

			if constexpr(!is_serialize) {
				return;
			}
			else {
				constexpr bool is_adapter = (flags & ANY_STL_METATABLE_ITEM_TYPE_ADAPTER) != 0u;
				constexpr bool is_container_item = (flags & ANY_STL_METATABLE_ITEM_TYPE_CONTAINER_ITEM) != 0u;

				if constexpr(is_adapter) {
					constexpr auto& lambda_adapter = std::get<ANY_STL_METATABLE_ITEM_INDEX_ADAPTER>(item);
					const auto data = lambda_adapter(&value);

					if constexpr(is_container_item) {
						write(data, cur_storage);
					}
					else {
						constexpr auto name = arr_name.data();
						decltype(auto) new_storage = cur_storage.interface_append_map_item(name);
						write(data, new_storage);
					}
				}
				else {
					constexpr auto& lambda = std::get<ANY_STL_METATABLE_ITEM_INDEX_LAMBDA>(item);
					const auto data = lambda(&value, nullptr);

					if constexpr(is_container_item) {
						write(*data, cur_storage);
					}
					else {
						constexpr auto name = arr_name.data();
						decltype(auto) new_storage = cur_storage.interface_append_map_item(name);
						write(*data, new_storage);
					}
				}
			}
		}
	}

	template<class T1, size_t Index = 0, size_t Size>
	constexpr void for_each_static_index(const T1& value, Storage &cur_storage) {
		each_tuple_item<T1, Index>(value, cur_storage);

		if constexpr(Index + 1 != Size) {
			for_each_static_index<T1, Index + 1, Size>(value, cur_storage);
		}
	}

	template<class T1>
	void write_meta_table(const T1 &value, Storage &cur_storage) {
		cur_storage.interface_init_container(yb::Type::object_container);
		for_each_static_index<T1, 0, std::tuple_size_v<decltype(T1::meta_table)>>(value, cur_storage);
		cur_storage.interface_deinit_container(yb::Type::object_container);
	}

	template<class T1>
	void write_enum(const T1 &value, Storage &cur_storage) {
		const auto str = yb_enum_to_string(value);
		cur_storage.interface_assign_from(std::string(str));
	}

	template<class T1>
	void write(const std::vector<T1> &value, Storage& cur_storage) {
		write_array(value, cur_storage);
	}

	template<class T1>
	void write(const std::list<T1> &value, Storage& cur_storage) {
		write_array(value, cur_storage);
	}

	template<class _Key, class _Compare, class _Allocator>
	void write(const std::set<_Key, _Compare, _Allocator> &value, Storage& cur_storage) {
		write_array(value, cur_storage);
	}

	template<class T1>
	void write(const std::unordered_set<T1> &value, Storage& cur_storage) {
		write_array(value, cur_storage);
	}

	template<class T1>
	void write(const std::deque<T1> &value, Storage& cur_storage) {
		write_array(value, cur_storage);
	}

	template <class _Key, class _Tp, class _Allocator>
	void write(const std::map<_Key, _Tp, _Allocator> &value, Storage& cur_storage) {
		write_map(value, cur_storage);
	}

	template <class T1, class T2>
	void write(const std::unordered_map<T1, T2> &value, Storage& cur_storage) {
		write_map(value, cur_storage);
	}

	template<class T1, class T2>
	void write(const std::pair<T1, T2> &value, Storage& cur_storage) {
		write_pair(value, cur_storage);
	}

	template<class ...T1>
	void write(const std::tuple<T1...> &value, Storage& cur_storage) {
		write_tuple(value, cur_storage);
	}
public:
	void write_to() {
		this->write(m_cont, m_storage);
	}
};

template <class T, Storage_concept_from_cpp Storage>
Engine_from_cpp<T, Storage> cpp_to_storage_instance(const T &cont, Storage& storage) {
	Engine_from_cpp<T, Storage> d(cont, storage);
	return d;
}

} //end of namespace yb::from
