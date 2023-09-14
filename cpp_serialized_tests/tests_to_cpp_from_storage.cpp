//
//  tests_to_cpp_from_storage.cpp
//
//  Created by Yuri Barmin on 01.08.2023.
//

//#include <iostream>
#include <variant>
#include <optional>
#include <array>
#include <map>

#include "cpp_serialized.h"
#include "string_utils.h"
#include "test_storage_to_cpp.h"
#include "tests_engine.h"
#include "enum_string.h"
#include "access_assist.h"

TEST(TestsToStl, toInt_Test) {
	int value{};
	
	yb::to_cpp::TestStorage storage;
	storage = 12345;
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	
	const bool success {deser.read_from()};
	ASSERT_EQ(success, true);
	EXPECT_EQ(value, 12345);
}

TEST(TestsToStl, toFloat_Test) {
	float value{};
	
	yb::to_cpp::TestStorage storage;
	storage = 12345.55f;
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	
	const bool success {deser.read_from()};
	ASSERT_EQ(success, true);
	EXPECT_EQ(yb::numbers_equal(value, 12345.55f), true);
}

TEST(TestsToStl, toFloatMax_Test) {
	float value{};
	
	yb::to_cpp::TestStorage storage;
	storage = std::numeric_limits<float>::max();
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	
	const bool success {deser.read_from()};
	ASSERT_EQ(success, true);
	EXPECT_EQ(yb::numbers_equal(value, std::numeric_limits<float>::max()), true);
}

TEST(TestsToStl, toDouble_Test) {
	double value{};

	yb::to_cpp::TestStorage storage;
	storage = 12345.55;

	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);

	const bool success {deser.read_from()};
	ASSERT_EQ(success, true);
	EXPECT_EQ(yb::numbers_equal(value, 12345.55), true);
}

TEST(TestsToStl, toDoubleMax_Test) {
	double value{};
	
	yb::to_cpp::TestStorage storage;
	storage = std::numeric_limits<double>::max();
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	
	const bool success {deser.read_from()};
	ASSERT_EQ(success, true);
	EXPECT_EQ(yb::numbers_equal(value, std::numeric_limits<double>::max()), true);
}


TEST(TestsToStl, toVectorInt_Test) {
	std::vector<int> vec;
	
	yb::to_cpp::TestStorage storage;
	storage.append(1);
	storage.append(2);
	storage.append(3);
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(vec, storage);
	
	const bool success {deser.read_from()};
	EXPECT_EQ(success, true);
	EXPECT_EQ(vec.size(), 3);
	EXPECT_EQ(vec[0], 1);
	EXPECT_EQ(vec[1], 2);
	EXPECT_EQ(vec[2], 3);
}

TEST(TestsToStl, toVectorUInt_Test) {
	std::vector<unsigned int> vec;

	yb::to_cpp::TestStorage storage;
	storage.append(1u);
	storage.append(2u);
	storage.append(3u);

	auto deser = yb::to_cpp::storage_to_cpp_instance(vec, storage);

	const bool success {deser.read_from()};
	EXPECT_EQ(success, true);
	EXPECT_EQ(vec.size(), 3);
	EXPECT_EQ(vec[0], 1u);
	EXPECT_EQ(vec[1], 2u);
	EXPECT_EQ(vec[2], 3u);
}

TEST(TestsToStl, toVectorFloat_Test) {
	std::vector<float> vec;

	yb::to_cpp::TestStorage storage;
	storage.append(1.f);
	storage.append(2.f);
	storage.append(3.f);

	auto deser = yb::to_cpp::storage_to_cpp_instance(vec, storage);

	const bool success {deser.read_from()};
	EXPECT_EQ(success, true);
	EXPECT_EQ(vec.size(), 3);
	EXPECT_EQ(vec[0], 1.f);
	EXPECT_EQ(vec[1], 2.f);
	EXPECT_EQ(vec[2], 3.f);
}

TEST(TestsToStl, toDequeBool_Test) {
	std::deque<bool> vec;

	yb::to_cpp::TestStorage storage;
	storage.append(true);
	storage.append(false);
	storage.append(true);

	auto deser = yb::to_cpp::storage_to_cpp_instance(vec, storage);

	const bool success {deser.read_from()};
	EXPECT_EQ(success, true);
	EXPECT_EQ(vec.size(), 3);
	EXPECT_EQ(vec[0], true);
	EXPECT_EQ(vec[1], false);
	EXPECT_EQ(vec[2], true);
}

TEST(TestsToStl, toVectorString_Test) {
	{
		std::vector<std::string> vec;
		
		yb::to_cpp::TestStorage storage;
		storage.append("one");
		storage.append("two");
		storage.append("three");
		
		auto deser = yb::to_cpp::storage_to_cpp_instance(vec, storage);
		
		const bool success {deser.read_from()};
		EXPECT_EQ(success, true);
		EXPECT_EQ(vec.size(), 3);
		EXPECT_EQ(vec[0], "one");
		EXPECT_EQ(vec[1], "two");
		EXPECT_EQ(vec[2], "three");
	}
	{
		std::vector<std::string> vec;
		
		yb::to_cpp::TestStorage storage;
		storage.append(std::string("one"));
		storage.append(std::string("two"));
		storage.append(std::string("three"));
		
		auto deser = yb::to_cpp::storage_to_cpp_instance(vec, storage);
		
		const bool success {deser.read_from()};
		EXPECT_EQ(success, true);
		EXPECT_EQ(vec.size(), 3);
		EXPECT_EQ(vec[0], "one");
		EXPECT_EQ(vec[1], "two");
		EXPECT_EQ(vec[2], "three");
	}
}

TEST(TestsToStl, toMapIntInt_Test) {
	std::map<int, int> map;
	
	yb::to_cpp::TestStorage storage;
	storage.append(1, 10);
	storage.append(2, 20);
	storage.append(3, 30);
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(map, storage);
	const bool success{deser.read_from()};

	EXPECT_EQ(success, true);
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map[1], 10);
	EXPECT_EQ(map[2], 20);
	EXPECT_EQ(map[3], 30);
}

TEST(TestsToStl, toMapStringString_Test) {
	std::map<std::string, std::string> map;
	
	yb::to_cpp::TestStorage storage;
	storage.append("1", "10");
	storage.append("2", "20");
	storage.append("3", "30");
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(map, storage);
	const bool success{deser.read_from()};

	EXPECT_EQ(success, true);
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["1"], "10");
	EXPECT_EQ(map["2"], "20");
	EXPECT_EQ(map["3"], "30");
}


struct autoStorageDeserContainerItem {
	DEFINE_DESER_CONTAINER_GETTER(Item, int)
	
	BEGIN_DESER_META_TABLE
	DEFINE_DESER_META_TABLE_CONTAINER_ITEM(Item)
	END_DESER_META_TABLE
	
};

struct autoStorageDeserContainerAdapterItem {
	DEFINE_DESER_CONTAINER_GETTER(Item, int)
	
	BEGIN_DESER_META_TABLE
	DEFINE_DESER_META_TABLE_CONTAINER_ITEM_ADAPTER(Item, std::string, [](const auto& item){
		if(item.length() < 1) {
			return -1;
		}
		const auto s = item.substr(1);
		return yb::string_utils::string_to_val<int>(s);
	})
	END_DESER_META_TABLE
	
};

	//Unit tests
struct autoStorageDeserUnitTestStruct {
		//Getters
	DEFINE_DESER_GETTER(IntProp,int)
	DEFINE_DESER_GETTER(BoolPropTrue, bool)
	DEFINE_DESER_GETTER(BoolPropFalse, bool)
	DEFINE_DESER_GETTER(FloatProp, float)
	DEFINE_DESER_GETTER(StringProp, std::string)
	DEFINE_DESER_GETTER(VectorOfIntsProp, std::vector<int>)
	DEFINE_DESER_GETTER(VectorOfStringsProp, std::vector<std::string>)
	using mapOfInts = std::map<int, int>;
	DEFINE_DESER_GETTER(MapOfIntsProp, mapOfInts)
	using mapOfStrings = std::map<std::string, std::string>;
	DEFINE_DESER_GETTER(MapOfStringsProp, mapOfStrings)
	DEFINE_DESER_GETTER(AdapterProp, int)

	DEFINE_DESER_GETTER(IntPropEmpty, int)
	DEFINE_DESER_GETTER(IntPropEmpty2, int)
	DEFINE_DESER_GETTER(IntPropEmpty3, int)
	DEFINE_DESER_GETTER(IntPropDefault, int)
	DEFINE_DESER_GETTER(AdapterPropEmpty, int)
	DEFINE_DESER_GETTER(AdapterPropDefault, int)

	DEFINE_DESER_GETTER(IntPropNotEmpty, int)
	DEFINE_DESER_GETTER(IntPropNotDefault, int)
	DEFINE_DESER_GETTER(AdapterPropNotEmpty, int)
	DEFINE_DESER_GETTER(AdapterPropNotDefault, int)

	DEFINE_DESER_GETTER(VectorOfIntsContItems, std::vector<autoStorageDeserContainerItem>)
	DEFINE_DESER_GETTER(VectorOfIntsContAdaptItems, std::vector<autoStorageDeserContainerAdapterItem>)


//		//Meta table
	BEGIN_DESER_META_TABLE
	DEFINE_DESER_META_TABLE_ITEM(IntProp)
	DEFINE_DESER_META_TABLE_ITEM(BoolPropTrue)
	DEFINE_DESER_META_TABLE_ITEM(BoolPropFalse)
	DEFINE_DESER_META_TABLE_ITEM(FloatProp)
	DEFINE_DESER_META_TABLE_ITEM(StringProp)
	DEFINE_DESER_META_TABLE_ITEM(VectorOfIntsProp)
	DEFINE_DESER_META_TABLE_ITEM(VectorOfStringsProp)
	DEFINE_DESER_META_TABLE_ITEM(MapOfIntsProp)
	DEFINE_DESER_META_TABLE_ITEM(MapOfStringsProp)

	DEFINE_DESER_META_TABLE_ITEM_ADAPTER(AdapterProp, std::string, [](const std::string& item){
		if(item.length() < 1) {
			return -1;
		}
		const auto s = item.substr(1);
		return yb::string_utils::string_to_val<int>(s);
	})

	DEFINE_DESER_META_TABLE_ITEM_EMPTY(IntPropEmpty)
	DEFINE_DESER_META_TABLE_ITEM_EMPTY(IntPropEmpty2)
	DEFINE_DESER_META_TABLE_ITEM(IntPropEmpty3)
	DEFINE_DESER_META_TABLE_ITEM_DEFAULT(IntPropDefault, 100)

	DEFINE_DESER_META_TABLE_ITEM_EMPTY_ADAPTER(AdapterPropEmpty, std::string, [](const std::string& item){
		if(item.empty()) {
			return 1111;
		}
		return 2222;
	})
	DEFINE_DESER_META_TABLE_ITEM_DEFAULT_ADAPTER(AdapterPropDefault, "5555", std::string, [](const std::string& item){
		return yb::string_utils::string_to_val<int>(item);
	})

	DEFINE_DESER_META_TABLE_ITEM_EMPTY(IntPropNotEmpty)
	DEFINE_DESER_META_TABLE_ITEM_DEFAULT(IntPropNotDefault, 987654)
	DEFINE_DESER_META_TABLE_ITEM_EMPTY_ADAPTER(AdapterPropNotEmpty, std::string, [](const std::string& item){
		if(item.empty()) {
			return 1111;
		}
		return 2222;
	})
	DEFINE_DESER_META_TABLE_ITEM_DEFAULT_ADAPTER(AdapterPropNotDefault, "344", std::string, [](const std::string& item){
		if(item.length() < 1) {
			return -1;
		}
		const auto s = item.substr(1);
		return yb::string_utils::string_to_val<int>(s);
	})


	DEFINE_DESER_META_TABLE_ITEM(VectorOfIntsContItems)
	DEFINE_DESER_META_TABLE_ITEM(VectorOfIntsContAdaptItems)
	
	END_DESER_META_TABLE
};

TEST(TestsToStl, storageToStl_Read_Test) {
	
	yb::to_cpp::TestStorage storage;
	storage["IntProp"] = 15;
	storage["IntPropEmpty3"] = 28;
	storage["BoolPropTrue"] = true;
	storage["BoolPropFalse"] = false;
	storage["FloatProp"] = 15.5f;
	storage["StringProp"] = "The Persuaders";
	
	auto& vectorOfIntsProp = storage["VectorOfIntsProp"];
	vectorOfIntsProp.append(1);
	vectorOfIntsProp.append(2);
	vectorOfIntsProp.append(10);
	vectorOfIntsProp.append(20);

	
	auto& vectorOfStringsProp = storage["VectorOfStringsProp"];
	vectorOfStringsProp.append("s1");
	vectorOfStringsProp.append("s2");
	vectorOfStringsProp.append("s10");
	vectorOfStringsProp.append("s20");

	auto& mapOfIntsProp = storage["MapOfIntsProp"];
	mapOfIntsProp.append(1, 86400);
	mapOfIntsProp.append(2, 172800);
	mapOfIntsProp.append(3, 259200);

	auto& mapOfStringsProp = storage["MapOfStringsProp"];
	mapOfStringsProp.append("1", "s86400");
	mapOfStringsProp.append("2", "s172800");
	mapOfStringsProp.append("3", "s259200");

	storage["AdapterProp"] = "s12345";
	storage["IntPropNotEmpty"] = 1000;
	storage["IntPropNotDefault"] = 2000;
	storage["AdapterPropNotEmpty"] = "s6000";
	storage["AdapterPropNotDefault"] = "s7777";
	
	auto& vectorOfIntsContItems = storage["VectorOfIntsContItems"];
	vectorOfIntsContItems.append(10);
	vectorOfIntsContItems.append(20);
	vectorOfIntsContItems.append(100);
	vectorOfIntsContItems.append(200);

	
	auto& vectorOfIntsContAdaptItems = storage["VectorOfIntsContAdaptItems"];
	vectorOfIntsContAdaptItems.append("s100");
	vectorOfIntsContAdaptItems.append("s200");
	vectorOfIntsContAdaptItems.append("s1000");
	vectorOfIntsContAdaptItems.append("s2000");

	autoStorageDeserUnitTestStruct autoStorageDeserUnitTestData;

	auto deser = yb::to_cpp::storage_to_cpp_instance(autoStorageDeserUnitTestData, storage);
	const bool success{deser.read_from()};

	EXPECT_EQ(success, true);

	EXPECT_EQ(autoStorageDeserUnitTestData.get_IntProp(), 15);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_BoolPropTrue(), true);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_BoolPropFalse(), false);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_FloatProp(), 15.5);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_StringProp(), "The Persuaders");

	const auto testVectorOfInts = std::vector {1, 2, 10, 20};
	EXPECT_EQ(autoStorageDeserUnitTestData.get_VectorOfIntsProp(), testVectorOfInts);

	const auto testVectorOfStrings = std::vector<std::string> {"s1", "s2", "s10", "s20"};
	EXPECT_EQ(autoStorageDeserUnitTestData.get_VectorOfStringsProp(), testVectorOfStrings);

	const auto testMapOfIntsProp = std::map<int, int> { {1,86400}, {2, 172800}, {3, 259200}};
	EXPECT_EQ(autoStorageDeserUnitTestData.get_MapOfIntsProp(), testMapOfIntsProp);

	const auto testMapOfStringsProp = std::map<std::string, std::string> {{"1", "s86400"}, {"2", "s172800"}, {"3", "s259200"}};
	EXPECT_EQ(autoStorageDeserUnitTestData.get_MapOfStringsProp(), testMapOfStringsProp);

	EXPECT_EQ(autoStorageDeserUnitTestData.get_AdapterProp(), 12345);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_IntPropEmpty(), 0);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_IntPropEmpty2(), 0);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_IntPropEmpty3(), 28);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_IntPropDefault(), 100);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_AdapterPropEmpty(), 1111);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_AdapterPropDefault(), 5555);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_IntPropNotEmpty(), 1000);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_IntPropNotDefault(), 2000);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_AdapterPropNotEmpty(), 2222);
	EXPECT_EQ(autoStorageDeserUnitTestData.get_AdapterPropNotDefault(), 7777);
	{
		const auto testVectorOfIntsContItems = std::vector {10, 20, 100, 200};
		EXPECT_EQ(testVectorOfIntsContItems.size(), autoStorageDeserUnitTestData.get_VectorOfIntsContItems().size());
		if(testVectorOfIntsContItems.size() == autoStorageDeserUnitTestData.get_VectorOfIntsContItems().size()) {
			for(size_t i = 0; i < autoStorageDeserUnitTestData.get_VectorOfIntsContItems().size();i++) {
				EXPECT_EQ(autoStorageDeserUnitTestData.get_VectorOfIntsContItems()[i].get_Item(), testVectorOfIntsContItems[i]);
				if(autoStorageDeserUnitTestData.get_VectorOfIntsContItems()[i].get_Item() != testVectorOfIntsContItems[i]) {
					break;
				}
			}
		}
	}

	{
		const auto testVectorOfIntsContAdaptItems = std::vector {100, 200, 1000, 2000};
		EXPECT_EQ(testVectorOfIntsContAdaptItems.size(), autoStorageDeserUnitTestData.get_VectorOfIntsContAdaptItems().size());
		if(testVectorOfIntsContAdaptItems.size() == autoStorageDeserUnitTestData.get_VectorOfIntsContAdaptItems().size()) {
			for(size_t i = 0; i < autoStorageDeserUnitTestData.get_VectorOfIntsContAdaptItems().size();i++) {
				EXPECT_EQ(autoStorageDeserUnitTestData.get_VectorOfIntsContAdaptItems()[i].get_Item(), testVectorOfIntsContAdaptItems[i]);
				if(autoStorageDeserUnitTestData.get_VectorOfIntsContAdaptItems()[i].get_Item() != testVectorOfIntsContAdaptItems[i]) {
					break;
				}
			}
		}
	}
}

DEFINE_ENUM_STRING(myReadEnum, ME_1, ME_2)

TEST(TestsToStl, readEnum) {
	yb::to_cpp::TestStorage storage;
	storage = "ME_2";
	
	myReadEnum enum_var;

	auto deser = yb::to_cpp::storage_to_cpp_instance(enum_var, storage);
	const bool success{deser.read_from()};

	ASSERT_EQ(success, true);
	EXPECT_EQ(enum_var, myReadEnum::ME_2);
}

TEST(TestsToStl, readEnumFail) {
	yb::to_cpp::TestStorage storage;
	storage = "ME_234647647675";

	myReadEnum enum_var;

	auto deser = yb::to_cpp::storage_to_cpp_instance(enum_var, storage);
	const bool success{deser.read_from()};

	ASSERT_EQ(success, false);
}

TEST(TestsToStl, readIntToFloat) {
	yb::to_cpp::TestStorage storage;
	storage = 2;
	
	float my_var{};
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(my_var, storage);
	const bool success{deser.read_from()};
	
	ASSERT_EQ(success, true);
	EXPECT_EQ(my_var, 2.f);
}

TEST(TestsToStl, readFloatToInt) {
	yb::to_cpp::TestStorage storage;
	storage = 2.f;
	
	int my_var{};
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(my_var, storage);
	const bool success{deser.read_from()};

	ASSERT_EQ(success, false);
}

TEST(TestsToStlUtils, readTimePoint) {
	const time_t data {1234567890L};
	
	yb::to_cpp::TestStorage storage;
	storage = std::to_string(data);

	std::chrono::time_point<std::chrono::system_clock> value;

	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	const bool success{deser.read_from()};

	ASSERT_EQ(success, true);
	ASSERT_EQ(std::chrono::system_clock::to_time_t(value), data);
}

TEST(TestsToStlUtils, readTimePointFail) {
	std::chrono::time_point<std::chrono::system_clock> value;
	yb::to_cpp::TestStorage storage;
	storage = 98989333; //Wrong data type. Must be string.
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage);
	const bool success{deser.read_from()};

	ASSERT_EQ(success, false);
}

TEST(TestsToStl, readPairKeyString) {
	yb::to_cpp::TestStorage storage;
	storage.append("key");
	storage.append(1234);
	
	std::pair<std::string, int> pairVar;
	const auto success = yb::assist::get_value_to(storage, pairVar);
	ASSERT_EQ(success, true);
	
	const std::pair<std::string, int> standardValue{"key", 1234};
	EXPECT_EQ(pairVar, standardValue);
}


TEST(TestsToStl, readPairKeyInt) {
	yb::to_cpp::TestStorage storage;
	storage.append(98765);
	storage.append(1234);
	
	std::pair<int, int> pairVar;
	const auto success = yb::assist::get_value_to(storage, pairVar);
	ASSERT_EQ(success, true);
	
	const std::pair<int, int> standardValue{98765, 1234};
	EXPECT_EQ(pairVar, standardValue);
}

TEST(TestsToStl, readTupleStringInt) {
	yb::to_cpp::TestStorage storage;
	storage.append(98765);
	storage.append(1234);

	std::tuple<std::string, int> tupleVar;
	const auto success = yb::assist::get_value_to(storage, tupleVar);
	ASSERT_EQ(success, true);

	const std::tuple<std::string, int> standardValue{"98765", 1234};
	EXPECT_EQ(tupleVar, standardValue);
}

struct tupleDeserStructItem {
	DEFINE_DATA(item1, int)
	DEFINE_DATA(item2, std::string)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(item1)
	DEFINE_META_TABLE_ITEM(item2)
	END_META_TABLE
};


TEST(TestsToStl, readTupleComplex) {
	yb::to_cpp::TestStorage storage;
	storage.append("98765").append(1234);

	{
		yb::to_cpp::TestStorage arr;
		arr.append(1).append(2).append(3);
		storage.append(arr);
	}
	{
		yb::to_cpp::TestStorage arr;
		arr.append(100).append(200).append(300);
		storage.append(arr);
	}
	
	{
		yb::to_cpp::TestStorage map;
		map.append("item1", 100).append("item2", "200");
		storage.append(map);
	}
	
	using testType = std::tuple<std::string, int, std::vector<int>, std::tuple<int, int, int>, tupleDeserStructItem>;
	
	
	testType tupleVar;
	const auto getResult = yb::assist::get_value_to(storage, tupleVar);
	ASSERT_EQ(getResult, true);
	
	testType standardValue;
	std::get<0>(standardValue) = "98765";
	std::get<1>(standardValue) = 1234;
	
	const std::vector<int> vec {1, 2, 3};
	std::get<2>(standardValue) = vec;
	
	std::tuple<int, int, int> tup {100, 200, 300};
	std::get<3>(standardValue) = tup;
	
	tupleDeserStructItem struc;
	struc.item1 = 100;
	struc.item2 = "200";
	std::get<4>(standardValue) = struc;
	
	
	EXPECT_EQ(std::get<0>(tupleVar), std::get<0>(standardValue));
	EXPECT_EQ(std::get<1>(tupleVar), std::get<1>(standardValue));
	EXPECT_EQ(std::get<2>(tupleVar), std::get<2>(standardValue));
	EXPECT_EQ(std::get<3>(tupleVar), std::get<3>(standardValue));
	EXPECT_EQ(std::get<4>(tupleVar).item1, std::get<4>(standardValue).item1);
	EXPECT_EQ(std::get<4>(tupleVar).item2, std::get<4>(standardValue).item2);
}
