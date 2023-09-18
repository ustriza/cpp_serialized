//
//  tests_jsoncpp_from_cpp.cpp
//
//  Created by Yuri Barmin on 14.05.2021.
//

#include "json_engine_addons.h"
#include "cpp_serialized.h"
#include "jsoncpp_storage.h"
#include "tests_engine.h"
#include <json/reader.h>
#include "enum_string.h"


TEST(stlToJson, writeNumbersInt) {
	constexpr std::array values {0, -1, -100, 1, 100, 123456789, -123456789};
	for(const int value: values) {
		Json::Value json;
		yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		EXPECT_EQ(storage.isInt(), true);
		EXPECT_EQ(storage.asValue<int>(), value);
	}
}

TEST(stlToJson, writeNumbersFloat) {
	constexpr std::array values {0.f, -1.f, -100.f, 1.f, 100.f, 123456789.f, -123456789.f};
	for(const float value: values) {
		Json::Value json;
		yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		EXPECT_EQ(storage.isFloat(), true);
		EXPECT_EQ(storage.asValue<float>(), value);
	}
}

TEST(stlToJson, writeNumbersBool) {
	constexpr std::array values {true, false};
	for(const bool value: values) {
		Json::Value json;
		yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		EXPECT_EQ(storage.isBool(), true);
		EXPECT_EQ(storage.asValue<bool>(), value);
	}
}

TEST(stlToJson, writeStrings) {
	constexpr std::array values {"", "str123456", "87879879879osklsnlsdnksjdksdbsdmn,smdns,dbsmndsmndbsmndbsm"};
	for(const std::string value: values) {
		Json::Value json;
		yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isString(), true);
		EXPECT_EQ(storage.asValue<std::string>(), value);
	}
}


template<class T>
void toJsonTestArrays(const T& values) {
	for(const auto& value: values) {
		Json::Value json;
		yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		ASSERT_EQ(json.isArray(), true);
		ASSERT_EQ(json.size(), value.size());
		for(size_t i = 0; i< json.size(); i++) {
			auto it = value.begin();
			std::advance(it, i);
			const auto& v = (*it);
			
			yb::jsoncpp::Json_storage_adapter_to_cpp storage_item{json[Json::Value::ArrayIndex{static_cast<Json::ArrayIndex>(i)}]};
			const auto result = yb::assist::get_value<typename T::value_type::value_type>(storage_item);

			ASSERT_EQ(result.has_value(), true);
			ASSERT_EQ(result.value(), v);
		}
	}
}

TEST(stlToJson, writeVectorsInt) {
	using usedContainer = std::vector<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeDequesInt) {
	using usedContainer = std::deque<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeListsInt) {
	using usedContainer = std::list<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeSetsInt) {
	using usedContainer = std::set<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeUnorderedSetsInt) {
	using usedContainer = std::unordered_set<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeVectorsString) {
	using usedContainer = std::vector<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeDequesString) {
	using usedContainer = std::deque<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeListsString) {
	using usedContainer = std::list<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeSetsString) {
	using usedContainer = std::set<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toJsonTestArrays(values);
}

TEST(stlToJson, writeUnorderedSetString) {
	using usedContainer = std::unordered_set<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toJsonTestArrays(values);
}


template<class T>
void testPairs(const T& values) {
	for(const auto& value: values) {
		Json::Value json;
		yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		ASSERT_EQ(json.isArray(), true);

		ASSERT_EQ(json.size(), 2);

		auto it = json.begin();
		EXPECT_EQ(*it, value.first);

		++it;
		EXPECT_EQ(*it, value.second);
	}
}



TEST(stlToJson, writePairsIntInt) {
	using usedContainer = std::pair<int, int>;
	const std::array values {usedContainer{}, usedContainer{10, 20}};
	testPairs(values);
}

TEST(stlToJson, writePairsIntString) {
	using usedContainer = std::pair<int, std::string>;
	const std::array values {usedContainer{}, usedContainer{10, "20bbb"}};
	testPairs(values);
}

TEST(stlToJson, writePairsStringInt) {
	using usedContainer = std::pair<std::string, int>;
	const std::array values {usedContainer{}, usedContainer{"10dsss", 20}};
	testPairs(values);
}

TEST(stlToJson, writePairsFloatInt) {
	using usedContainer = std::pair<float, int>;
	const std::array values {usedContainer{}, usedContainer{10.5f, 20}};
	testPairs(values);
}


template<class T>
void toJsonTestMaps(const T& values) {
	for(const auto& value: values) {
		Json::Value json;
		yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		ASSERT_EQ(json.isObject(), true);
		ASSERT_EQ(json.size(), value.size());
		for(auto it = begin(value); it != end(value); ++it) {
			const auto result = yb::assist::get_value<typename T::value_type::mapped_type>(yb::jsoncpp::Json_storage_adapter_to_cpp{json[yb::string_utils::val_to_string(it->first)]});

			ASSERT_EQ(result.has_value(), true);
			EXPECT_EQ(result.value(), it->second);
		}
	}
}


TEST(stlToJson, writeMapsStrInt) {
	using usedContainer = std::map<std::string, int>;
	const std::array values {usedContainer{}, usedContainer{{"01234", 1234}, {"056789",
		56789}}};

	toJsonTestMaps(values);
}

TEST(stlToJson, writeMapsStrStr) {
	using usedContainer = std::map<std::string, std::string>;
	const std::array values {usedContainer{}, usedContainer{{"01234", "1234sdsds"}, {"056789",
		"asdfdKKl56789"}}};

	toJsonTestMaps(values);
}

TEST(stlToJson, writeMapsIntInt) {
	using usedContainer = std::map<int, int>;
	const std::array values {usedContainer{}, usedContainer{{1234, 123478}, {56789,
		56789343}}};

	toJsonTestMaps(values);
}

TEST(stlToJson, writeMapsIntStr) {
	using usedContainer = std::map<int, std::string>;
	const std::array values {usedContainer{}, usedContainer{{1234, "01234"}, {56789, "056789"}}};

	toJsonTestMaps(values);
}

struct testSerializeStruct1ContainerItem {
	DEFINE_CONTAINER_DATA(Item, int)

	BEGIN_META_TABLE
	DEFINE_META_TABLE_CONTAINER_ITEM(Item)
	END_META_TABLE

};


TEST(stlToJson, writeMapsIntStruct) {
	using usedContainer = std::map<int, testSerializeStruct1ContainerItem>;
	const std::array values {usedContainer{}, usedContainer{{1234, testSerializeStruct1ContainerItem{12345}}, {56789, testSerializeStruct1ContainerItem{567890}}}};

	toJsonTestMaps(values);
}


TEST(stlToJson, writeUnorderedMapsStrInt) {
	using usedContainer = std::unordered_map<std::string, int>;
	const std::array values {usedContainer{}, usedContainer{{"01234", 1234}, {"056789",
		56789}}};

	toJsonTestMaps(values);
}

TEST(stlToJson, writeUnorderedMapsStrStr) {
	using usedContainer = std::unordered_map<std::string, std::string>;
	const std::array values {usedContainer{}, usedContainer{{"01234", "1234sdsds"}, {"056789",
		"asdfdKKl56789"}}};

	toJsonTestMaps(values);
}

TEST(stlToJson, writeUnorderedMapsIntInt) {
	using usedContainer = std::unordered_map<int, int>;
	const std::array values {usedContainer{}, usedContainer{{1234, 123478}, {56789,
		56789343}}};

	toJsonTestMaps(values);
}

TEST(stlToJson, writeUnorderedMapsIntStr) {
	using usedContainer = std::unordered_map<int, std::string>;
	const std::array values {usedContainer{}, usedContainer{{1234, "01234"}, {56789, "056789"}}};

	toJsonTestMaps(values);
}

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

TEST(stlToJson, writeStruct0) {
	const testSerializeStruct value{12345, 56789, 89010};

	const auto check = [](const auto& storage){
		ASSERT_EQ(storage.size(), 3);

		EXPECT_EQ(storage["item"].isInt(), true);
		EXPECT_EQ(storage["item"].asInt(), 12345);

		EXPECT_EQ(storage["item2"].isInt(), true);
		EXPECT_EQ(storage["item2"].asInt(), 56789);

		EXPECT_EQ(storage["adapterProp"].isString(), true);
		EXPECT_EQ(storage["adapterProp"].asString(), "s89010");
	};

	{
		Json::Value json;
		yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		check(json);
	}

	{
		Json::Value json;
		yb::assist::set_value(yb::jsoncpp::Json_storage_adapter_from_cpp{json}, value);
		
		check(json);
	}

	{
		Json::Value json;
		yb::assist::set_value(yb::assist::create_storage(yb::jsoncpp::Json_storage_adapter_from_cpp{json}, "key"), value);

		check(json["key"]);
	}

}

struct testSerializeStruct1 {
	DEFINE_DATA(vectorOfIntsContItems, std::vector<testSerializeStruct1ContainerItem>)

	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(vectorOfIntsContItems)
	END_META_TABLE
};

TEST(stlToJson, writeArrayContainerItem) {
	const testSerializeStruct1 value = []{
		testSerializeStruct1 value;
		value.vectorOfIntsContItems.push_back(testSerializeStruct1ContainerItem{12345});
		value.vectorOfIntsContItems.push_back(testSerializeStruct1ContainerItem{67890});
		return value;
	}();

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	ASSERT_EQ(json.size(), 1);

	const auto vecJson = json["vectorOfIntsContItems"];

	ASSERT_EQ(vecJson.isArray(), true);
	ASSERT_EQ(vecJson.size(), 2);

	EXPECT_EQ(vecJson[0u].isInt(), true);
	EXPECT_EQ(vecJson[0u].asInt(), 12345);

	EXPECT_EQ(vecJson[1u].isInt(), true);
	EXPECT_EQ(vecJson[1u].asInt(), 67890);
}

TEST(stlToJson, readStruct1) {
	const auto srcJson = "{"
	"\"vectorOfIntsContItems\": ["
		"12345, 56789]}";

	Json::Reader reader;
	Json::Value autoJsonDeserUnitTestJson;
	[[maybe_unused]] const auto parseJsonResult = reader.parse(srcJson, autoJsonDeserUnitTestJson);
	ASSERT_EQ(parseJsonResult, true);

	testSerializeStruct1 value;

	yb::jsoncpp::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto inst = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool getResult = inst.read_from();

	ASSERT_EQ(getResult, true);

	ASSERT_EQ(value.vectorOfIntsContItems.size(), 2);
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

TEST(stlToJson, writeStruct2) {
	const testSerializeStruct2 value = []{
		testSerializeStruct2 value;
		value.vectorOfIntsContItems.push_back(testSerializeStruct2ContainerAdapter{12345});
		value.vectorOfIntsContItems.push_back(testSerializeStruct2ContainerAdapter{67890});
		return value;
	}();

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	ASSERT_EQ(json.size(), 1);

	const auto vecJson = json["vectorOfIntsContItems"];

	ASSERT_EQ(vecJson.isArray(), true);
	ASSERT_EQ(vecJson.size(), 2);

	EXPECT_EQ(vecJson[0u].isString(), true);
	EXPECT_EQ(vecJson[0u].asString(), "s12345");

	EXPECT_EQ(vecJson[1u].isString(), true);
	EXPECT_EQ(vecJson[1u].asString(), "s67890");
}

DEFINE_ENUM_STRING(myWriteEnum, ME_1, ME_2)

TEST(stlToJson, writeEnum) {
	const myWriteEnum value = myWriteEnum::ME_2;

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json.isString(), true);
	EXPECT_EQ(json.asString(), "ME_2");
}

TEST(stlToJson, writeEnumFail) {
	const myWriteEnum value = myWriteEnum(123456);

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json.isString(), true);
	EXPECT_EQ(json.asString(), "");
}


struct testSerializeDeserializeEmptyStruct {
	DEFINE_DATA(item, int)


	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM_EMPTY(item)
	END_META_TABLE
};

TEST(stlToJson, readStructEmpty) {
	const auto srcJson = R"({})";

	Json::Reader reader;
	Json::Value autoJsonDeserUnitTestJson;
	[[maybe_unused]] const auto parseJsonResult = reader.parse(srcJson, autoJsonDeserUnitTestJson);
	ASSERT_EQ(parseJsonResult, true);

	testSerializeDeserializeEmptyStruct value;

	yb::jsoncpp::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto inst = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool getResult = inst.read_from();

	ASSERT_EQ(getResult, true);
	ASSERT_EQ(value.item, 0);
}

TEST(stlToJson, writeStructEmpty) {
	testSerializeDeserializeEmptyStruct value;
	value.item = 10;

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json["item"].isInt(), true);
	EXPECT_EQ(json["item"].asInt(), 10);
}

struct testSerializeDeserializeDefaultStruct {
	DEFINE_DATA(item, int)


	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM_DEFAULT(item, 100)
	END_META_TABLE
};

TEST(stlToJson, readStructDefault) {
	const auto srcJson = R"({})";

	Json::Reader reader;
	Json::Value autoJsonDeserUnitTestJson;
	[[maybe_unused]] const auto parseJsonResult = reader.parse(srcJson, autoJsonDeserUnitTestJson);
	ASSERT_EQ(parseJsonResult, true);

	testSerializeDeserializeDefaultStruct value;

	yb::jsoncpp::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto inst = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool getResult = inst.read_from();

	ASSERT_EQ(getResult, true);
	ASSERT_EQ(value.item, 100);
}

TEST(stlToJson, writeStructDefault) {
	testSerializeDeserializeDefaultStruct value;
	value.item = 10;

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json["item"].isInt(), true);
	EXPECT_EQ(json["item"].asInt(), 10);
}

struct testSerializeDeserializeJsonStruct {
	DEFINE_DATA(item, Json::Value)


	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(item)
	END_META_TABLE
};

TEST(stlToJson, readJsonStruct) {
	const auto srcJson = R"({"item": ["abcd", "xyz" ]})";

	Json::Reader reader;
	Json::Value autoJsonDeserUnitTestJson;
	[[maybe_unused]] const auto parseJsonResult = reader.parse(srcJson, autoJsonDeserUnitTestJson);
	ASSERT_EQ(parseJsonResult, true);

	testSerializeDeserializeJsonStruct value;

	yb::jsoncpp::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto inst = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool getResult = inst.read_from();

	ASSERT_EQ(getResult, true);
	ASSERT_EQ(value.item.type(), Json::ValueType::arrayValue);
	ASSERT_EQ(value.item.size(), 2);
	ASSERT_EQ(value.item[0u].asString(), "abcd");
	ASSERT_EQ(value.item[1u].asString(), "xyz");
}

TEST(stlToJson, writeJsonStruct) {
	testSerializeDeserializeJsonStruct value;
	value.item.append("abcd");
	value.item.append("xyz");

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json["item"].isArray(), true);
	ASSERT_EQ(json["item"].size(), 2);
	ASSERT_EQ(json["item"][0u].asString(), "abcd");
	ASSERT_EQ(json["item"][1u].asString(), "xyz");
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

TEST(stlToJson, writeEmptyArrayMapStruct) {
	testSerializeDeserializeEmptyArrayMapStruct value;

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json["itemArray"].isNull(), false);
	EXPECT_EQ(json["itemArray"].isArray(), true);
	EXPECT_EQ(json["itemMap"].isNull(), false);
	EXPECT_EQ(json["itemMap"].isObject(), true);
}


TEST(stlToJson, writePairKeyString) {
	std::pair<std::string, int> value {"key", 12345};

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json.isArray(), true);
	ASSERT_EQ(json.size(), 2);
	EXPECT_EQ(json[0u].asString(), "key");
	EXPECT_EQ(json[1u].asInt(), 12345);
}

TEST(stlToJson, writePairKeyInt) {
	std::pair<int, int> value {98765, 12345};

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json.isArray(), true);
	ASSERT_EQ(json.size(), 2);
	EXPECT_EQ(json[0u].asInt(), 98765);
	EXPECT_EQ(json[1u].asInt(), 12345);
}

TEST(stlToJson, writeTupleStringInt) {
	std::tuple<std::string, int> value {"key", 12345};

	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json.isArray(), true);
	ASSERT_EQ(json.size(), 2);
	EXPECT_EQ(json[0u].asString(), "key");
	EXPECT_EQ(json[1u].asInt(), 12345);
}

struct tupleSerStructItem {
	DEFINE_DATA(item1, int)
	DEFINE_DATA(item2, std::string)

	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(item1)
	DEFINE_META_TABLE_ITEM(item2)
	END_META_TABLE
};

TEST(stlToJson, writeTupleComplex) {
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


	Json::Value json;
	yb::jsoncpp::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();

	EXPECT_EQ(json.isArray(), true);
	ASSERT_EQ(json.size(), 5);
	EXPECT_EQ(json[0u].asString(), "98765");
	EXPECT_EQ(json[1u].asInt(), 1234);

	EXPECT_EQ(json[2u].isArray(), true);
	EXPECT_EQ(json[2u].size(), 3);
	EXPECT_EQ(json[2u][0u].isInt(), true);
	EXPECT_EQ(json[2u][0u].asInt(), 1);
	EXPECT_EQ(json[2u][1u].isInt(), true);
	EXPECT_EQ(json[2u][1u].asInt(), 2);
	EXPECT_EQ(json[2u][2u].isInt(), true);
	EXPECT_EQ(json[2u][2u].asInt(), 3);

	EXPECT_EQ(json[3u].isArray(), true);
	EXPECT_EQ(json[3u].size(), 3);
	EXPECT_EQ(json[3u][0u].isInt(), true);
	EXPECT_EQ(json[3u][0u].asInt(), 100);
	EXPECT_EQ(json[3u][1u].isInt(), true);
	EXPECT_EQ(json[3u][1u].asInt(), 200);
	EXPECT_EQ(json[3u][2u].isInt(), true);
	EXPECT_EQ(json[3u][2u].asInt(), 300);

	EXPECT_EQ(json[4u].isObject(), true);
	ASSERT_EQ(json[4u].size(), 2);
	EXPECT_EQ(json[4u]["item1"].isInt(), true);
	EXPECT_EQ(json[4u]["item1"].asInt(), 100);
	EXPECT_EQ(json[4u]["item2"].isString(), true);
	EXPECT_EQ(json[4u]["item2"].asString(), "200");
}
