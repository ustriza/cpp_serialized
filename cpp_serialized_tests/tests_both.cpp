//
//  tests_both.cpp
//
//  Created by Yuri Barmin on 18.08.2023.
//

#include "test_storage.h"
#include "access_assist.h"
#include "tests_engine.h"

struct testSerializeStruct {
	DEFINE_DATA(item, int)
	DEFINE_DATA(item2, int)
	DEFINE_DATA(adapterProp, int)
	
	
	BEGIN_META_TABLE
	DEFINE_SER_META_TABLE_ITEM_ADAPTER(adapterProp, [](const int item){
		return "s" + yb::string_utils::val_to_string(item);
	})
	DEFINE_META_TABLE_ITEM(item)
	DEFINE_META_TABLE_ITEM(item2)
	END_META_TABLE
};

TEST(TestsBoth, writeStruct0) {
	const testSerializeStruct value{12345, 56789, 89010};
	
	const auto check = [](const auto& storage){
		EXPECT_EQ(storage.interface_size(), 3);
		
		EXPECT_EQ(storage.interface_get_storage_by_key("item").interface_get_type() == yb::Type::int_value, true);
		EXPECT_EQ(storage.interface_get_storage_by_key("item").template interface_get_value<int>(), 12345);
		
		EXPECT_EQ(storage.interface_get_storage_by_key("item2").interface_get_type() == yb::Type::int_value, true);
		EXPECT_EQ(storage.interface_get_storage_by_key("item2").template interface_get_value<int>(), 56789);
		
		EXPECT_EQ(storage.interface_get_storage_by_key("adapterProp").interface_get_type() == yb::Type::string_value, true);
		EXPECT_EQ(storage.interface_get_storage_by_key("adapterProp").template interface_get_value<std::string>(), "s89010");
	};
	
	{
		yb::both::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		check(storage);
	}
	
	{
		yb::both::TestStorage storage;
		yb::assist::set_value(storage, value);
		check(storage);
	}
	
	{
		yb::both::TestStorage storage;
		yb::assist::set_value(yb::assist::create_storage(storage, "key"), value);
		check(storage.test_get_by_key("key"));
	}
	
}

struct testDeserializeStruct {
	DEFINE_DATA(item, int)
	DEFINE_DATA(item2, int)
	
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(item)
	DEFINE_META_TABLE_ITEM(item2)
	END_META_TABLE
};

TEST(TestsBoth, readStruct0) {
	yb::both::TestStorage storage;
	storage.append("item", 12345);
	storage.append("item2", 56789);

	testDeserializeStruct value;

	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	const bool read_result = deser.read_from();

	EXPECT_EQ(read_result, true);

	EXPECT_EQ(value.item, 12345);
	EXPECT_EQ(value.item2, 56789);
}

struct testSerializeStruct1ContainerItem {
	DEFINE_CONTAINER_DATA(Item, int)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_CONTAINER_ITEM(Item)
	END_META_TABLE
	
};

template<class T>
void testMaps(const T& values) {
	for(const auto& value: values) {
		yb::both::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.interface_get_type() == yb::Type::object_container, true);
		EXPECT_EQ(storage.interface_size(), value.size());
		for(auto it = begin(value); it != end(value); ++it) {
			const auto& resultStorage = storage.interface_get_storage_by_key(it->first);
			EXPECT_EQ(resultStorage.template interface_get_value<typename T::value_type::mapped_type>(), it->second);
		}
	}
}

TEST(TestsBoth, writeMapsIntStruct) {
	using usedContainer = std::map<int, testSerializeStruct1ContainerItem>;
	const std::array values {usedContainer{}, usedContainer{{1234, testSerializeStruct1ContainerItem{12345}}, {56789, testSerializeStruct1ContainerItem{567890}}}};
	
	for(const auto& value: values) {
		yb::both::TestStorage storage_write;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_write);
		inst.write_to();
		
		EXPECT_EQ(storage_write.interface_get_type() == yb::Type::object_container, true);
		EXPECT_EQ(storage_write.interface_size(), value.size());
		for(auto it = begin(value); it != end(value); ++it) {
			const auto& resultStorage = storage_write.interface_get_storage_by_key(it->first);
			
			testSerializeStruct1ContainerItem read_value;
			auto deser = yb::to_cpp::storage_to_cpp_instance(read_value, resultStorage);
			const bool read_result = deser.read_from();

			EXPECT_EQ(read_result, true);
			
			EXPECT_EQ(read_value, it->second);
		}
	}
}

struct testSerializeStruct1 {
	DEFINE_DATA(vectorOfIntsContItems, std::vector<testSerializeStruct1ContainerItem>)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(vectorOfIntsContItems)
	END_META_TABLE
};

TEST(TestsBoth, writeArrayContainerItem) {
	const testSerializeStruct1 value = []{
		testSerializeStruct1 value;
		value.vectorOfIntsContItems.push_back(testSerializeStruct1ContainerItem{12345});
		value.vectorOfIntsContItems.push_back(testSerializeStruct1ContainerItem{67890});
		return value;
	}();

	yb::both::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.interface_size(), 1);

	const auto vecJson = storage.test_get_by_key("vectorOfIntsContItems");

	EXPECT_EQ(vecJson.interface_get_type() == yb::Type::array_container, true);
	EXPECT_EQ(vecJson.interface_size(), 2);

	EXPECT_EQ(vecJson[0u].interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(vecJson[0u].interface_get_value<int>(), 12345);

	EXPECT_EQ(vecJson[1u].interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(vecJson[1u].interface_get_value<int>(), 67890);
}

TEST(TestsBoth, readStruct1) {
	yb::both::TestStorage storage;
	auto& new_storage = storage.interface_append_map_item("vectorOfIntsContItems");
	new_storage.append(12345);
	new_storage.append(56789);

	
	testSerializeStruct1 value;
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	const auto read_result = deser.read_from();
	
	EXPECT_EQ(read_result, true);
	
	EXPECT_EQ(value.vectorOfIntsContItems.size(), 2);
	EXPECT_EQ(value.vectorOfIntsContItems[0], 12345);
	EXPECT_EQ(value.vectorOfIntsContItems[1], 56789);
}

struct testSerializeStruct2ContainerAdapter {
	DEFINE_CONTAINER_DATA(Item, int)
	
	BEGIN_META_TABLE
	DEFINE_SER_META_TABLE_CONTAINER_ITEM_ADAPTER(Item, [](const int item){
		return "s" + yb::string_utils::val_to_string(item);
	})
	END_META_TABLE
	
};

struct testSerializeStruct2 {
	DEFINE_DATA(vectorOfIntsContItems, std::vector<testSerializeStruct2ContainerAdapter>)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(vectorOfIntsContItems)
	END_META_TABLE
};

TEST(TestsBoth, writeStruct2) {
	const testSerializeStruct2 value = []{
		testSerializeStruct2 value;
		value.vectorOfIntsContItems.push_back(testSerializeStruct2ContainerAdapter{12345});
		value.vectorOfIntsContItems.push_back(testSerializeStruct2ContainerAdapter{67890});
		return value;
	}();
	
	yb::both::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	ASSERT_EQ(storage.interface_size(), 1);
	
	const auto vecJson = storage.test_get_by_key("vectorOfIntsContItems");
	
	ASSERT_EQ(vecJson.interface_get_type() == yb::Type::array_container, true);
	ASSERT_EQ(vecJson.interface_size(), 2);
	
	EXPECT_EQ(vecJson[0u].interface_get_type() == yb::Type::string_value, true);
	EXPECT_EQ(vecJson[0u].interface_get_value<std::string>(), "s12345");
	
	EXPECT_EQ(vecJson[1u].interface_get_type() == yb::Type::string_value, true);
	EXPECT_EQ(vecJson[1u].interface_get_value<std::string>(), "s67890");
}

struct testSerializeDeserializeEmptyStruct {
	DEFINE_DATA(item, int)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM_EMPTY(item)
	END_META_TABLE
};

TEST(TestsBoth, readStructEmpty) {
	yb::both::TestStorage storage;

	testSerializeDeserializeEmptyStruct value;
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	const auto read_result = deser.read_from();

	ASSERT_EQ(read_result, true);
	ASSERT_EQ(value.item, 0);
}

TEST(TestsBoth, writeStructEmpty) {
	testSerializeDeserializeEmptyStruct value;
	value.item = 10;
	
	yb::both::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.interface_get_storage_by_key("item").interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage.interface_get_storage_by_key("item").interface_get_value<int>(), 10);
}

struct testSerializeDeserializeDefaultStruct {
	DEFINE_DATA(item, int)
	
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM_DEFAULT(item, 100)
	END_META_TABLE
};

TEST(TestsBoth, readStructDefault) {
	yb::both::TestStorage storage;

	testSerializeDeserializeDefaultStruct value;
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	const auto read_result = deser.read_from();

	ASSERT_EQ(read_result, true);
	ASSERT_EQ(value.item, 100);
}

TEST(TestsBoth, writeStructDefault) {
	testSerializeDeserializeDefaultStruct value;
	value.item = 10;
	
	yb::both::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.interface_get_storage_by_key("item").interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage.interface_get_storage_by_key("item").interface_get_value<int>(), 10);
}

struct testSerializeDeserializeEmptyArrayMapStruct {
	DEFINE_DATA(itemArray, std::vector<int>)
	
	using mapType = std::map<std::string, int>;
	DEFINE_DATA(itemMap, mapType)
	
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(itemArray)
	DEFINE_META_TABLE_ITEM(itemMap)
	END_META_TABLE
};

TEST(TestsBoth, writeEmptyArrayMapStruct) {
	testSerializeDeserializeEmptyArrayMapStruct value;
	
	yb::both::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.test_get_by_key("itemArray").interface_get_type(), yb::Type::array_container);
	EXPECT_EQ(storage.test_get_by_key("itemMap").interface_get_type(), yb::Type::object_container);
}

struct tupleSerStructItem {
	DEFINE_DATA(item1, int)
	DEFINE_DATA(item2, std::string)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(item1)
	DEFINE_META_TABLE_ITEM(item2)
	END_META_TABLE
};

TEST(TestsBoth, writeTupleComplex) {
	using testType = std::tuple<std::string, int, std::vector<int>, std::tuple<int, int, int>, tupleSerStructItem>;
	
	testType value;
	std::get<0>(value) = "98765";
	std::get<1>(value) = 1234;
	
	const std::vector<int> vec {1, 2, 3};
	std::get<2>(value) = vec;
	
	std::tuple<int, int, int> tup {100, 200, 300};
	std::get<3>(value) = tup;
	
	tupleSerStructItem struc;
	struc.item1 = 100;
	struc.item2 = "200";
	std::get<4>(value) = struc;
	
	
	yb::both::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.interface_get_type() == yb::Type::array_container, true);
	ASSERT_EQ(storage.interface_size(), 5);
	EXPECT_EQ(storage[0u].interface_get_value<std::string>(), "98765");
	EXPECT_EQ(storage[1u].interface_get_value<int>(), 1234);
	
	EXPECT_EQ(storage[2u].interface_get_type() == yb::Type::array_container, true);
	EXPECT_EQ(storage[2u].interface_size(), 3);
	EXPECT_EQ(storage[2u][0u].interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage[2u][0u].interface_get_value<int>(), 1);
	EXPECT_EQ(storage[2u][1u].interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage[2u][1u].interface_get_value<int>(), 2);
	EXPECT_EQ(storage[2u][2u].interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage[2u][2u].interface_get_value<int>(), 3);
	
	EXPECT_EQ(storage[3u].interface_get_type(), yb::Type::array_container);
	EXPECT_EQ(storage[3u].interface_size(), 3);
	EXPECT_EQ(storage[3u][0u].interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage[3u][0u].interface_get_value<int>(), 100);
	EXPECT_EQ(storage[3u][1u].interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage[3u][1u].interface_get_value<int>(), 200);
	EXPECT_EQ(storage[3u][2u].interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage[3u][2u].interface_get_value<int>(), 300);
	
	EXPECT_EQ(storage[4u].interface_get_type(), yb::Type::object_container);
	ASSERT_EQ(storage[4u].interface_size(), 2);
	EXPECT_EQ(storage[4u].interface_get_storage_by_key("item1").interface_get_type() == yb::Type::int_value, true);
	EXPECT_EQ(storage[4u].interface_get_storage_by_key("item1").interface_get_value<int>(), 100);
	EXPECT_EQ(storage[4u].interface_get_storage_by_key("item2").interface_get_type() == yb::Type::string_value, true);
	EXPECT_EQ(storage[4u].interface_get_storage_by_key("item2").interface_get_value<std::string>(), "200");
}

struct dataStructItem {
	DEFINE_DATA(Item1, int)
	DEFINE_DATA(Item2, std::string)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(Item1)
	DEFINE_META_TABLE_ITEM(Item2)
	END_META_TABLE
};


TEST(TestsBoth, readWriteDataGetterSetter) {
	{
		dataStructItem data;
		
		data.Item1 = 123456;
		data.Item2 = "7890abc";
		
		ASSERT_EQ(data.Item1, 123456);
		ASSERT_EQ(data.get_Item1(), 123456);
		
		ASSERT_EQ(data.Item2, "7890abc");
		ASSERT_EQ(data.get_Item2(), "7890abc");
	}
	
	{
		dataStructItem data;
		
		data.set_Item1(123456);
		data.set_Item2("7890abc");
		
		ASSERT_EQ(data.Item1, 123456);
		ASSERT_EQ(data.get_Item1(), 123456);
		
		ASSERT_EQ(data.Item2, "7890abc");
		ASSERT_EQ(data.get_Item2(), "7890abc");
	}
}
