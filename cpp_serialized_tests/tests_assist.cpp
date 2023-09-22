//
//  tests_assist.cpp
//
//  Created by Yuri Barmin on 31.08.2023.
//

#include "cpp_serialized.h"
#include "tests_engine.h"
#include "test_storage_to_cpp.h"
#include "test_storage_from_cpp.h"
#include "enum_string.h"

template <class Storage, class T1, class T2>
void test_assign_to_storage(Storage& storage, const std::pair<T1, T2> &value) {
	storage.append(value.first);
	storage.append(value.second);
}

template<class Storage, class T1>
void test_assign_to_storage(Storage& storage, const std::vector<T1> &value) {
	for(const auto& item: value) {
		storage.append(item);
	}
}

template<class Storage, class T1>
void test_assign_to_storage(Storage& storage, const std::deque<T1> &value) {
	for(const auto& item: value) {
		storage.append(item);
	}
}


template <class Storage, class _Key, class _Tp, class _Allocator>
void test_assign_to_storage(Storage& storage, const std::map<_Key, _Tp, _Allocator> &value) {
	for(const auto& item: value) {
		storage.append(item.first, item.second);
	}
}


template<class Storage, class T1>
void test_assign_to_storage(Storage& storage, const T1 &value) {
	storage = value;
}


template <typename T>
bool test_check_get_value(const T& value) {
	using Clear_type = std::conditional_t<yb::string_utils::is_string<T>(), std::string, T> ;
	
	{
		yb::to_cpp::TestStorage storage;
		test_assign_to_storage(storage, value);

		const auto result = yb::assist::deserialize<Clear_type>(storage);
		if(!result.has_value()) {
			return false;
		}
		if(result.value() != value) {
			return false;
		}
	}
	
	{
		yb::to_cpp::TestStorage storage;
		test_assign_to_storage(storage, value);

		Clear_type result{};
		const bool success{yb::assist::deserialize_to<Clear_type>(storage, result)};
		
		if(!success) {
			return false;
		}
		if(result != value) {
			return false;
		}
	}
	
	return true;
}

TEST(TestsAssist, getValue) {
	EXPECT_EQ(test_check_get_value(123456), true);
	EXPECT_EQ(test_check_get_value(123456u), true);
	EXPECT_EQ(test_check_get_value(123456.f), true);

	EXPECT_EQ(test_check_get_value(true), true);
	EXPECT_EQ(test_check_get_value(false), true);
	
	EXPECT_EQ(test_check_get_value("123456"), true);
	
	EXPECT_EQ(test_check_get_value(std::pair{123456, 5678.f}), true);
	EXPECT_EQ(test_check_get_value(std::pair<std::string, unsigned>{"123456", 5678u}), true);

	EXPECT_EQ(test_check_get_value(std::map{std::pair{123456, 5678.f}}), true);
	EXPECT_EQ(test_check_get_value(std::map{std::pair{std::string{"123456"}, 5678}}), true);
	EXPECT_EQ(test_check_get_value(std::map{std::pair{123456, std::string{"5678"}}}), true);

	EXPECT_EQ(test_check_get_value(std::vector{123456, 5678, 54345}), true);
	EXPECT_EQ(test_check_get_value(std::vector{123456u, 5678u, 54345u}), true);
	EXPECT_EQ(test_check_get_value(std::vector{123456.f, 5678.f, 54345.f}), true);
	EXPECT_EQ(test_check_get_value(std::deque{true, false, true, false}), true);
	EXPECT_EQ(test_check_get_value(std::vector<std::string>{"123456", "5678", "54345"}), true);
}


TEST(TestsAssist, getValueFail) {
	{
		yb::to_cpp::TestStorage storage;
		
		const auto result0 = yb::assist::deserialize<int>(storage);
		EXPECT_EQ(result0.has_value(), false);
	}
	{
		yb::to_cpp::TestStorage storage;
		storage = "123456";
		
		const auto result0 = yb::assist::deserialize<int>(storage);
		EXPECT_EQ(result0.has_value(), false);
	}
}

TEST(TestsAssist, getValueDef) {
	{
		yb::to_cpp::TestStorage storage;

		const int result{yb::assist::deserialize<int>(storage, 56789)};
		EXPECT_EQ(result, 56789);
	}
	{
		yb::to_cpp::TestStorage storage;
		storage = 12345;
		
		const int result{yb::assist::deserialize<int>(storage, 56789)};
		EXPECT_EQ(result, 12345);
	}
	{
		yb::to_cpp::TestStorage storage;
		storage = 12345;
		
		int def = 56789;
		const int result{yb::assist::deserialize<int>(storage, def)};
		EXPECT_EQ(result, 12345);
	}
	{
		yb::to_cpp::TestStorage storage;
		storage = 12345;
		
		const int def = 56789;
		const int result{yb::assist::deserialize<int>(storage, def)};
		EXPECT_EQ(result, 12345);
	}

	{ //'def' copied to the return value
		yb::to_cpp::TestStorage storage;
		
		std::vector<int> def{12345, 6789};
		
		const auto result = yb::assist::deserialize<std::vector<int>>(storage, def);
		EXPECT_EQ(def.size(), 2);
		EXPECT_EQ(result, def);
	}

	{ //'def' moved to the return value
		yb::to_cpp::TestStorage storage;
		
		std::vector<int> def{12345, 6789};
		
		const auto result = yb::assist::deserialize<std::vector<int>>(storage, std::move(def));
		
		EXPECT_EQ(def.empty(), true);
		EXPECT_EQ(result.size(), 2);
		EXPECT_EQ(result[0], 12345);
		EXPECT_EQ(result[1], 6789);
	}
	
	{
		yb::to_cpp::TestStorage storage;
		const auto value = std::vector{67890, 54321};
		test_assign_to_storage(storage, value);

		std::vector<int> def{12345, 6789};
		
		const auto result = yb::assist::deserialize<std::vector<int>>(storage, def);
		
		EXPECT_EQ(result, value);
	}
}

TEST(TestsAssist, getTupleValue) {
	yb::to_cpp::TestStorage storage;
	storage.append(12345);
	storage.append(12345.f);
	storage.append("s12345");
	
	std::tuple<int, float, std::string> value;
	
	const bool success{yb::assist::deserialize_to(storage, value)};
	
	EXPECT_EQ(success, true);
	EXPECT_EQ(std::get<0>(value), 12345);
	EXPECT_EQ(std::get<1>(value), 12345.f);
	EXPECT_EQ(std::get<2>(value), "s12345");
}

struct Struct_to_string_assist {
	DEFINE_DATA(intProp, int)
	DEFINE_DATA(floatProp, float)
	DEFINE_DATA(stringProp, std::string)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(intProp)
	DEFINE_META_TABLE_ITEM(floatProp)
	DEFINE_META_TABLE_ITEM(stringProp)
	END_META_TABLE
};

TEST(TestsAssist, getStructValue) {
	yb::to_cpp::TestStorage storage;

	storage.append("intProp", 12345);
	storage.append("floatProp", 12345.f);
	storage.append("stringProp", "s12345");
	
	Struct_to_string_assist value;
	
	const bool success{yb::assist::deserialize_to(storage, value)};

	EXPECT_EQ(success, true);
	EXPECT_EQ(value.intProp, 12345);
	EXPECT_EQ(value.floatProp, 12345.f);
	EXPECT_EQ(value.stringProp, "s12345");
}


template<class T>
bool test_empty_value() {
	yb::to_cpp::TestStorage storage;
	T value;
	const bool success{yb::assist::deserialize_to(storage, value)};
	return success == false;
}

DEFINE_ENUM_STRING(Test_empty_enum, ME_1, ME_2)

TEST(TestsAssist, getEmptyValue) {
	EXPECT_EQ(test_empty_value<int>(), true);
	EXPECT_EQ(test_empty_value<std::vector<int>>(), true);
	
	const bool test_map = test_empty_value<std::map<int, int>>();
	EXPECT_EQ(test_map, true);
	
	const bool test_pair = test_empty_value<std::pair<int, int>>();
	EXPECT_EQ(test_pair, true);

	const bool test_tuple = test_empty_value<std::tuple<int, int>>();
	EXPECT_EQ(test_tuple, true);
	
	EXPECT_EQ(test_empty_value<Test_empty_enum>(), true);
	
	EXPECT_EQ(test_empty_value<Struct_to_string_assist>(), true);
}

TEST(TestsAssist, findStorage) {
	yb::to_cpp::TestStorage storage;
	yb::to_cpp::TestStorage storage0;
	storage0 = "000";
	yb::to_cpp::TestStorage storage1;
	storage1 = 111;
	
	storage.append("key0", storage0);
	storage.append("key1", storage1);
	
	const auto& found_storage0 = yb::assist::find_storage(storage, "key0");
	const auto& found_storage1 = yb::assist::find_storage(storage, "key1");

	EXPECT_EQ(found_storage0.interface_get_type(), yb::Type::string_value);
	EXPECT_EQ(found_storage0.interface_get_value<std::string>(), "000");
	
	EXPECT_EQ(found_storage1.interface_get_type(), yb::Type::int_value);
	EXPECT_EQ(found_storage1.interface_get_value<int>(), 111);

	EXPECT_EQ(yb::assist::find_storage(storage, "wrong_key").interface_get_type(), yb::Type::null_value);
}

TEST(TestsAssist, createStorage) {
	{//returns the same storage for existing key
		yb::from_cpp::TestStorage storage;
		storage.interface_init_container(yb::Type::object_container);
		
		yb::from_cpp::TestStorage& new_storage = yb::assist::create_storage(storage, "key");
		
		EXPECT_EQ(new_storage.isNull(), true);
		
		new_storage.interface_assign_from(12345);

		EXPECT_EQ(new_storage.isInt(), true);
		EXPECT_EQ(new_storage.asInt(), 12345);

		yb::from_cpp::TestStorage& new_storage1 = yb::assist::create_storage(storage, "key");
		
		EXPECT_EQ(new_storage1.isInt(), true);
		EXPECT_EQ(new_storage1.asInt(), 12345);
	}
	
	{//returns new storages for different keys
		yb::from_cpp::TestStorage storage;
		storage.interface_init_container(yb::Type::object_container);

		yb::from_cpp::TestStorage& new_storage = yb::assist::create_storage(storage, "key0");

		EXPECT_EQ(new_storage.isNull(), true);
		
		new_storage.interface_assign_from(12345);

		EXPECT_EQ(new_storage.isInt(), true);
		EXPECT_EQ(new_storage.asInt(), 12345);

		yb::from_cpp::TestStorage& new_storage1 = yb::assist::create_storage(storage, "key1");
		
		EXPECT_EQ(new_storage1.isNull(), true);
	}
}

TEST(TestsAssist, setValue) {
	{
		yb::from_cpp::TestStorage storage;
		
		yb::assist::serialize(storage, 12345);
		
		ASSERT_EQ(storage.isInt(), true);
		EXPECT_EQ(storage.asInt(), 12345);
	}
	
	{
		const std::vector<int> value{12345, 67890, 9876, 54321};
		
		yb::from_cpp::TestStorage storage;
		
		yb::assist::serialize(storage, value);
		
		ASSERT_EQ(storage.isArray(), true);
		ASSERT_EQ(storage.size(), value.size());

		for(size_t index{}; index < value.size(); index++) {
			EXPECT_EQ(storage[index].asInt(), value[index]);
		}
	}
}
