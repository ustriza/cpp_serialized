//
//  engine_common.h
//  to
//
//  Created by Yuri Barmin on 08.08.2023.
//

#pragma once

#include <string>
#include <array>

#define YB_STATIC_STRING(X) yb::string_utils::Static_string<sizeof(X)>{X}

#if __cplusplus >= 202002L //C++ 20
#define CONSTEVAL consteval
#else
#define CONSTEVAL constexpr
#endif

#define BEGIN_META_TABLE \
	private: \
		template<typename, Storage_concept_to_cpp Storage> friend class yb::to_cpp::Engine_to_cpp; \
		template<typename, Storage_concept_from_cpp Storage> friend class yb::from_cpp::Engine_from_cpp; \
		friend struct yb::find_prop_class; \
		static constexpr auto meta_table = std::make_tuple(

//Items
#define DEFINE_META_TABLE_ITEM(name) std::tuple{                                  \
	yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE   |                                    \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE,                                    \
	YB_STATIC_STRING(#name),          \
	DESER_META_TABLE_LAMBDA(name),                                                \
	[](auto, auto){},                                                             \
	},

#define DEFINE_META_TABLE_ITEM_EMPTY(name) std::tuple{                            \
	yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE   |                                    \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE |                                    \
	yb::ANY_STL_METATABLE_ITEM_TYPE_EMPTY,                                           \
	YB_STATIC_STRING(#name),          \
	DESER_META_TABLE_LAMBDA(name),                                                \
	[](auto, auto){},                                                             \
	},

#define DEFINE_META_TABLE_ITEM_DEFAULT(name, def) std::tuple{                     \
	yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE   |                                    \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE |                                    \
	yb::ANY_STL_METATABLE_ITEM_TYPE_DEFAULT,                                         \
	YB_STATIC_STRING(#name),          \
	DESER_META_TABLE_LAMBDA(name),                                                \
	[](auto, auto){},                                                             \
	[]{return def;}                                                               \
	},

#define DEFINE_DATA(name, type)  public: type name{}; \
	void set_##name(const type& __value__) {name = __value__;} \
	constexpr const auto& get_##name() const noexcept { \
	constexpr auto arrName = YB_STATIC_STRING(#name); \
	constexpr bool result = yb::find_prop_class::find_prop<std::decay_t<std::remove_pointer_t<decltype(this)>>, yb::ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE | yb::ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE>(arrName);                                        \
	static_assert(result, "Could not find property name in the meta table");                       \
	return name;}  public:

#define DESER_META_TABLE_LAMBDA(name)                        \
[](auto ptr, auto) {                                         \
	using type = decltype(name);	                         \
	if constexpr(std::is_same_v<decltype(ptr), nullptr_t>) { \
		return static_cast<type*>(nullptr);                  \
	}                                                        \
	else {                                                   \
		return &ptr->name;                                   \
	}                                                        \
}

#define END_META_TABLE std::tuple{}); public:

#define DEFINE_MEMBER_CHECKER(member) \
template<typename TType, typename V = bool> \
struct has_ ## member : std::false_type { }; \
template<typename TType> \
struct has_ ## member<TType, \
typename std::enable_if< \
!std::is_same<decltype(std::declval<TType>().member), void>::value, \
bool \
>::type \
> : std::true_type { };


#define HAS_MEMBER(C, member) \
has_ ## member<C>::value


namespace yb {
enum class Type {null_value, int_value, uint_value, float_value, string_value, boolean_value, array_container, object_container};

template<typename T>
constexpr inline bool is_allowed_type() {
	return
	std::is_same_v<T, std::string> ||
	
	std::is_same_v<T, int> ||
	std::is_same_v<T, unsigned> ||
	std::is_same_v<T, long> ||
	std::is_same_v<T, long long> ||
	std::is_same_v<T, unsigned long> ||
	
	std::is_same_v<T, float> ||
	std::is_same_v<T, double> ||
	
	std::is_same_v<T, bool>;
}

constexpr unsigned ANY_STL_METATABLE_ITEM_INDEX_FLAGS    = 0;
constexpr unsigned ANY_STL_METATABLE_ITEM_INDEX_NAME     = 1;
constexpr unsigned ANY_STL_METATABLE_ITEM_INDEX_LAMBDA   = 2;
constexpr unsigned ANY_STL_METATABLE_ITEM_INDEX_ADAPTER  = 3;
constexpr unsigned ANY_STL_METATABLE_ITEM_INDEX_DEFAULT  = 4;

constexpr unsigned ANY_STL_METATABLE_ITEM_TYPE_DESERIALIZE  = 0b1;
constexpr unsigned ANY_STL_METATABLE_ITEM_TYPE_SERIALIZE    = 0b10;
constexpr unsigned ANY_STL_METATABLE_ITEM_TYPE_ADAPTER      = 0b1000;
constexpr unsigned ANY_STL_METATABLE_ITEM_TYPE_CONTAINER_ITEM    = 0b10000;
constexpr unsigned ANY_STL_METATABLE_ITEM_TYPE_EMPTY        = 0b100000;
constexpr unsigned ANY_STL_METATABLE_ITEM_TYPE_DEFAULT      = 0b1000000;

struct find_prop_class {
private:
	
	template<typename TArray, typename TArray2>
	static constexpr bool compare_strings(const TArray& ar, const TArray2& a) {
		if(ar.size() != a.size()) {
			return false;
		}
		for(size_t i = 0; i < ar.size() - 1; i++) {
			if(ar[i] != a[i]) {
				return false;
			}
		}
		return true;
	}
	
	
	template<typename T1, typename TString, size_t tupleIndex, unsigned flag>
	static constexpr bool find_prop_each_tuple_item(const TString& a) {
		constexpr auto& item = std::get<tupleIndex>(T1::meta_table);
		using tupleType = std::decay_t<std::remove_reference_t<decltype(item)>>;
		constexpr auto tupleSize = std::tuple_size_v<tupleType>;
		
		if constexpr(tupleSize == 0) {
			return false;
		}
		else {
			constexpr unsigned flags = std::get<ANY_STL_METATABLE_ITEM_INDEX_FLAGS>(item);
			constexpr bool isFlag = (flag == 0u || (flags & flag) != 0u);
			if(!isFlag) {
				return false;
			}
			else {
				const auto& name = std::get<ANY_STL_METATABLE_ITEM_INDEX_NAME>(item);
				if(compare_strings(name, a)) {
					return true;
				}
			}
		}
		return false;
	}
	
	template<class T1, typename TString, size_t Index = 0, size_t Size, unsigned flag>
	static constexpr bool find_prop_for_each_static_index(const TString& a) {
		if(find_prop_each_tuple_item<T1, TString, Index, flag>(a)) {
			return true;
		}
		
		if constexpr(Index + 1 != Size) {
			return find_prop_for_each_static_index<T1, TString, Index + 1, Size, flag>(a);
		}
		return false;
	}
	
public:
	
	template<typename TTuple, unsigned flag, typename TString>
	static constexpr bool find_prop(const TString& a) {
		const bool found = find_prop_for_each_static_index<TTuple, TString, 0, std::tuple_size_v<decltype(TTuple::meta_table)>, flag>(a);
		
		return found;
	}
};

}

#if __cplusplus >= 202002L

namespace yb::to_cpp {

struct Value_model {};
struct Key_model {};

template<class T>
concept Const_iterator_concept =
requires(T a, const T ca) {
	++a;
	{ca != ca} -> std::convertible_to<bool>;
	{ca.template interface_get_key<Key_model>()} -> std::convertible_to<Key_model>;
};

template<class T>
concept Storage_concept =
requires(const T a, const Value_model value_model, const Key_model key_model) {
	{a.template interface_get_value<Value_model>()} -> std::convertible_to<Value_model>;
	{a.interface_get_storage_by_key(key_model)} -> std::convertible_to<T>;
	
	{T::interface_get_storage_from_iterator(a.interface_begin())} -> std::convertible_to<T>;

	{a.interface_get_type()} -> std::same_as<Type>;
	
	//container support
	{a.interface_size()} -> std::same_as<size_t>;
	{a.interface_begin()} -> Const_iterator_concept;
	{a.interface_end()} -> Const_iterator_concept;
};
} // end of yb::to_cpp


namespace yb::from_cpp {

struct Value_model {};
struct Key_model {};

template<class T>
concept Storage_concept =
requires(T a, const Value_model value_model, const Key_model key_model) {
	requires (std::is_convertible_v<std::decay_t<decltype(a.interface_append_array_item())>, std::decay_t<T>>);
	requires (std::is_convertible_v<std::decay_t<decltype(a.interface_append_map_item(key_model))>, std::decay_t<T>>);
	{a.interface_init_container(yb::Type{})} -> std::same_as<void>;
	{a.interface_deinit_container(yb::Type{})} -> std::same_as<void>;
	{a.interface_assign_from(value_model)} -> std::same_as<void>;
};
} //end of yb::from_cpp


template<typename T>
concept Storage_concept_from_cpp = yb::from_cpp::Storage_concept<T>;

template<typename T>
concept Storage_concept_to_cpp = yb::to_cpp::Storage_concept<T>;

template<typename T>
concept Storage_concept_both = (Storage_concept_to_cpp<T> && Storage_concept_from_cpp<T>);


#else
#define Const_iterator_concept typename
#define Storage_concept_from_cpp typename
#define Storage_concept_to_cpp typename
#endif
