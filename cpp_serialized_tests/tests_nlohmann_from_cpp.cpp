//
//  tests_nlohmann_from_cpp.cpp
//
//  Created by Yuri Barmin on 22.03.2025.
//

#include "cpp_serialized.h"
#include "nlohmann_storage.h"
#include "tests_engine.h"
#include "enum_string.h"


TEST(stlToNlohmann, writeNumbersInt) {
	constexpr std::array values {0, -1, -100, 1, 100, 123456789, -123456789};
	for(const int value: values) {
		nlohmann::json json;
		yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isInt(), true);
		EXPECT_EQ(storage.asValue<int>(), value);
	}
}

TEST(stlToNlohmann, writeNumbersFloat) {
	constexpr std::array values {0.f, -1.f, -100.f, 1.f, 100.f, 123456789.f, -123456789.f};
	for(const float value: values) {
		nlohmann::json json;
		yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isFloat(), true);
		EXPECT_EQ(storage.asValue<float>(), value);
	}
}

TEST(stlToNlohmann, writeNumbersBool) {
	constexpr std::array values {true, false};
	for(const bool value: values) {
		nlohmann::json json;
		yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isBool(), true);
		EXPECT_EQ(storage.asValue<bool>(), value);
	}
}

TEST(stlToNlohmann, writeStrings) {
	constexpr std::array values {"", "str123456", "87879879879osklsnlsdnksjdksdbsdmn,smdns,dbsmndsmndbsmndbsm"};
	for(const std::string value: values) {
		nlohmann::json json;
		yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isString(), true);
		EXPECT_EQ(storage.asValue<std::string>(), value);
	}
}


template<class T>
void toNLohmannTestArrays(const T& values) {
	for(const auto& value: values) {
		nlohmann::json json;
		yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		ASSERT_EQ(json.is_array(), true);
		ASSERT_EQ(json.size(), value.size());
		for(size_t i = 0; i< json.size(); i++) {
			auto it = value.begin();
			std::advance(it, i);
			const auto& v = (*it);
			
			yb::nlohmann::Json_storage_adapter_to_cpp storage_item{json[i]};
			const auto result = yb::assist::deserialize<typename T::value_type::value_type>(storage_item);
			
			ASSERT_EQ(result.has_value(), true);
			ASSERT_EQ(result.value(), v);
		}
	}
}

TEST(stlToNlohmann, writeVectorsInt) {
	using usedContainer = std::vector<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeDequesInt) {
	using usedContainer = std::deque<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeListsInt) {
	using usedContainer = std::list<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeSetsInt) {
	using usedContainer = std::set<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeUnorderedSetsInt) {
	using usedContainer = std::unordered_set<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeVectorsString) {
	using usedContainer = std::vector<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeDequesString) {
	using usedContainer = std::deque<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeListsString) {
	using usedContainer = std::list<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeSetsString) {
	using usedContainer = std::set<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toNLohmannTestArrays(values);
}

TEST(stlToNlohmann, writeUnorderedSetString) {
	using usedContainer = std::unordered_set<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toNLohmannTestArrays(values);
}


template<class T>
void testNLohmannPairs(const T& values) {
	for(const auto& value: values) {
		nlohmann::json json;
		yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		ASSERT_EQ(json.is_array(), true);
		
		ASSERT_EQ(json.size(), 2);
		
		auto it = json.begin();
		EXPECT_EQ(*it, value.first);
		
		++it;
		EXPECT_EQ(*it, value.second);
	}
}



TEST(stlToNlohmann, writePairsIntInt) {
	using usedContainer = std::pair<int, int>;
	const std::array values {usedContainer{}, usedContainer{10, 20}};
	testNLohmannPairs(values);
}

TEST(stlToNlohmann, writePairsIntString) {
	using usedContainer = std::pair<int, std::string>;
	const std::array values {usedContainer{}, usedContainer{10, "20bbb"}};
	testNLohmannPairs(values);
}

TEST(stlToNlohmann, writePairsStringInt) {
	using usedContainer = std::pair<std::string, int>;
	const std::array values {usedContainer{}, usedContainer{"10dsss", 20}};
	testNLohmannPairs(values);
}

TEST(stlToNlohmann, writePairsFloatInt) {
	using usedContainer = std::pair<float, int>;
	const std::array values {usedContainer{}, usedContainer{10.5f, 20}};
	testNLohmannPairs(values);
}


template<class T>
void toNLohmannTestMaps(const T& values) {
	for(const auto& value: values) {
		nlohmann::json json;
		yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		ASSERT_EQ(json.is_object(), true);
		ASSERT_EQ(json.size(), value.size());
		for(auto it = begin(value); it != end(value); ++it) {
			const auto result = yb::assist::deserialize<typename T::value_type::mapped_type>(yb::nlohmann::Json_storage_adapter_to_cpp{json[yb::string_utils::val_to_string(it->first)]});
			
			ASSERT_EQ(result.has_value(), true);
			EXPECT_EQ(result.value(), it->second);
		}
	}
}


TEST(stlToNlohmann, writeMapsStrInt) {
	using usedContainer = std::map<std::string, int>;
	const std::array values {usedContainer{}, usedContainer{{"01234", 1234}, {"056789",
		56789}}};
	
	toNLohmannTestMaps(values);
}

TEST(stlToNlohmann, writeMapsStrStr) {
	using usedContainer = std::map<std::string, std::string>;
	const std::array values {usedContainer{}, usedContainer{{"01234", "1234sdsds"}, {"056789",
		"asdfdKKl56789"}}};
	
	toNLohmannTestMaps(values);
}

TEST(stlToNlohmann, writeMapsIntInt) {
	using usedContainer = std::map<int, int>;
	const std::array values {usedContainer{}, usedContainer{{1234, 123478}, {56789,
		56789343}}};
	
	toNLohmannTestMaps(values);
}

TEST(stlToNlohmann, writeMapsIntStr) {
	using usedContainer = std::map<int, std::string>;
	const std::array values {usedContainer{}, usedContainer{{1234, "01234"}, {56789, "056789"}}};
	
	toNLohmannTestMaps(values);
}

struct testNLohmanSerializeStruct1ContainerItem {
	DEFINE_CONTAINER_DATA(Item, int)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_CONTAINER_ITEM(Item)
	END_META_TABLE
	
};


TEST(stlToNlohmann, writeMapsIntStruct) {
	using usedContainer = std::map<int, testNLohmanSerializeStruct1ContainerItem>;
	const std::array values {usedContainer{}, usedContainer{{1234, testNLohmanSerializeStruct1ContainerItem{12345}}, {56789, testNLohmanSerializeStruct1ContainerItem{567890}}}};
	
	toNLohmannTestMaps(values);
}


TEST(stlToNlohmann, writeUnorderedMapsStrInt) {
	using usedContainer = std::unordered_map<std::string, int>;
	const std::array values {usedContainer{}, usedContainer{{"01234", 1234}, {"056789",
		56789}}};
	
	toNLohmannTestMaps(values);
}

TEST(stlToNlohmann, writeUnorderedMapsStrStr) {
	using usedContainer = std::unordered_map<std::string, std::string>;
	const std::array values {usedContainer{}, usedContainer{{"01234", "1234sdsds"}, {"056789",
		"asdfdKKl56789"}}};
	
	toNLohmannTestMaps(values);
}

TEST(stlToNlohmann, writeUnorderedMapsIntInt) {
	using usedContainer = std::unordered_map<int, int>;
	const std::array values {usedContainer{}, usedContainer{{1234, 123478}, {56789,
		56789343}}};
	
	toNLohmannTestMaps(values);
}

TEST(stlToNlohmann, writeUnorderedMapsIntStr) {
	using usedContainer = std::unordered_map<int, std::string>;
	const std::array values {usedContainer{}, usedContainer{{1234, "01234"}, {56789, "056789"}}};
	
	toNLohmannTestMaps(values);
}

struct testNLohmannSerializeStruct {
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

TEST(stlToNlohmann, writeStruct0) {
	const testNLohmannSerializeStruct value{12345, 56789, 89010};
	
	const auto check = [](const auto& storage){
		ASSERT_EQ(storage.size(), 3);
		
		EXPECT_EQ(storage["item"].is_number_integer(), true);
		EXPECT_EQ(storage["item"].template get<int>(), 12345);
		
		EXPECT_EQ(storage["item2"].is_number_integer(), true);
		EXPECT_EQ(storage["item2"].template get<int>(), 56789);
		
		EXPECT_EQ(storage["adapterProp"].is_string(), true);
		EXPECT_EQ(storage["adapterProp"].template get<std::string>(), "s89010");
	};
	
	{
		nlohmann::json json;
		yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		check(json);
	}
	
	{
		nlohmann::json json;
		yb::assist::serialize(yb::nlohmann::Json_storage_adapter_from_cpp{json}, value);
		
		check(json);
	}
	
	{
		nlohmann::json json;
		yb::assist::serialize(yb::assist::create_storage(yb::nlohmann::Json_storage_adapter_from_cpp{json}, "key"), value);
		
		check(json["key"]);
	}
	
}

struct testNLohmannSerializeStruct1 {
	DEFINE_DATA(vectorOfIntsContItems, std::vector<testNLohmanSerializeStruct1ContainerItem>)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(vectorOfIntsContItems)
	END_META_TABLE
};

TEST(stlToNlohmann, writeArrayContainerItem) {
	const testNLohmannSerializeStruct1 value = []{
		testNLohmannSerializeStruct1 value;
		value.vectorOfIntsContItems.push_back(testNLohmanSerializeStruct1ContainerItem{12345});
		value.vectorOfIntsContItems.push_back(testNLohmanSerializeStruct1ContainerItem{67890});
		return value;
	}();
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	ASSERT_EQ(json.size(), 1);
	
	const auto vecJson = json["vectorOfIntsContItems"];
	
	ASSERT_EQ(vecJson.is_array(), true);
	ASSERT_EQ(vecJson.size(), 2);
	
	EXPECT_EQ(vecJson[0u].is_number_integer(), true);
	EXPECT_EQ(vecJson[0u].get<int>(), 12345);
	
	EXPECT_EQ(vecJson[1u].is_number_integer(), true);
	EXPECT_EQ(vecJson[1u].get<int>(), 67890);
}

TEST(stlToNlohmann, readStruct1) {
	const auto srcJson = "{"
	"\"vectorOfIntsContItems\": ["
	"12345, 56789]}";
	
	nlohmann::json autoJsonDeserUnitTestJson;
	try {
		autoJsonDeserUnitTestJson = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{autoJsonDeserUnitTestJson};
	
	testNLohmannSerializeStruct1 value;
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto inst = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool getResult = inst.read_from();
	
	ASSERT_EQ(getResult, true);
	
	ASSERT_EQ(value.vectorOfIntsContItems.size(), 2);
	EXPECT_EQ(value.vectorOfIntsContItems[0], 12345);
	EXPECT_EQ(value.vectorOfIntsContItems[1], 56789);
}


struct testNLohmannSerializeStruct2ContainerAdapter {
	DEFINE_CONTAINER_DATA(Item, int)
	
	BEGIN_META_TABLE
	DEFINE_SER_META_TABLE_CONTAINER_ITEM_ADAPTER(Item, [](const int item){
		return "s" + yb::string_utils::val_to_string(item);
	})
	END_META_TABLE
	
};

struct testNLohmannSerializeStruct2 {
	DEFINE_DATA(vectorOfIntsContItems, std::vector<testNLohmannSerializeStruct2ContainerAdapter>)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(vectorOfIntsContItems)
	END_META_TABLE
};

TEST(stlToNlohmann, writeStruct2) {
	const testNLohmannSerializeStruct2 value = []{
		testNLohmannSerializeStruct2 value;
		value.vectorOfIntsContItems.push_back(testNLohmannSerializeStruct2ContainerAdapter{12345});
		value.vectorOfIntsContItems.push_back(testNLohmannSerializeStruct2ContainerAdapter{67890});
		return value;
	}();
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	ASSERT_EQ(json.size(), 1);
	
	const auto vecJson = json["vectorOfIntsContItems"];
	
	ASSERT_EQ(vecJson.is_array(), true);
	ASSERT_EQ(vecJson.size(), 2);
	
	EXPECT_EQ(vecJson[0u].is_string(), true);
	EXPECT_EQ(vecJson[0u].get<std::string>(), "s12345");
	
	EXPECT_EQ(vecJson[1u].is_string(), true);
	EXPECT_EQ(vecJson[1u].get<std::string>(), "s67890");
}

DEFINE_ENUM_STRING(myWriteEnum, ME_1, ME_2)

TEST(stlToNlohmann, writeEnum) {
	const myWriteEnum value = myWriteEnum::ME_2;
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json.is_string(), true);
	EXPECT_EQ(json.get<std::string>(), "ME_2");
}

TEST(stlToNlohmann, writeEnumFail) {
	const myWriteEnum value = myWriteEnum(123456);
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json.is_string(), true);
	EXPECT_EQ(json.get<std::string>(), "");
}


struct testNLohmannSerializeDeserializeEmptyStruct {
	DEFINE_DATA(item, int)
	
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM_EMPTY(item)
	END_META_TABLE
};

TEST(stlToNlohmann, readStructEmpty) {
	const auto srcJson = R"({})";
	
	nlohmann::json autoJsonDeserUnitTestJson;
	try {
		autoJsonDeserUnitTestJson = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	testNLohmannSerializeDeserializeEmptyStruct value;
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto inst = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool getResult = inst.read_from();
	
	ASSERT_EQ(getResult, true);
	ASSERT_EQ(value.item, 0);
}

TEST(stlToNlohmann, writeStructEmpty) {
	testNLohmannSerializeDeserializeEmptyStruct value;
	value.item = 10;
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json["item"].is_number_integer(), true);
	EXPECT_EQ(json["item"].get<int>(), 10);
}

struct testNLohmannSerializeDeserializeDefaultStruct {
	DEFINE_DATA(item, int)
	
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM_DEFAULT(item, 100)
	END_META_TABLE
};

TEST(stlToNlohmann, readStructDefault) {
	const auto srcJson = R"({})";
	
	nlohmann::json autoJsonDeserUnitTestJson;
	try {
		autoJsonDeserUnitTestJson = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	testNLohmannSerializeDeserializeDefaultStruct value;
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto inst = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool getResult = inst.read_from();
	
	ASSERT_EQ(getResult, true);
	ASSERT_EQ(value.item, 100);
}

TEST(stlToNlohmann, writeStructDefault) {
	testNLohmannSerializeDeserializeDefaultStruct value;
	value.item = 10;
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json["item"].is_number_integer(), true);
	EXPECT_EQ(json["item"].get<int>(), 10);
}

struct testNLohmannSerializeDeserializeStruct {
	DEFINE_DATA(item, nlohmann::json)
	
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(item)
	END_META_TABLE
};

TEST(stlToNlohmann, readJsonStruct) {
	const auto srcJson = R"({"item": ["abcd", "xyz" ]})";
	
	nlohmann::json autoJsonDeserUnitTestJson;
	try {
		autoJsonDeserUnitTestJson = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}

	testNLohmannSerializeDeserializeStruct value;
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto inst = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool getResult = inst.read_from();
	
	ASSERT_EQ(getResult, true);
	ASSERT_EQ(value.item.type(), nlohmann::json::value_t::array);
	ASSERT_EQ(value.item.size(), 2);
	ASSERT_EQ(value.item[0].get<std::string>(), "abcd");
	ASSERT_EQ(value.item[1].get<std::string>(), "xyz");
}

TEST(stlToNlohmann, writeJsonStruct) {
	testNLohmannSerializeDeserializeStruct value;
	value.item.push_back("abcd");
	value.item.push_back("xyz");
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json["item"].is_array(), true);
	ASSERT_EQ(json["item"].size(), 2);
	ASSERT_EQ(json["item"][0u].get<std::string>(), "abcd");
	ASSERT_EQ(json["item"][1u].get<std::string>(), "xyz");
}

struct testNLohmannSerializeDeserializeEmptyArrayMapStruct {
	DEFINE_DATA(itemArray, std::vector<int>)
	
	using mapType = std::map<std::string, int>;
	DEFINE_DATA(itemMap, mapType)
	
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(itemArray)
	DEFINE_META_TABLE_ITEM(itemMap)
	END_META_TABLE
};

TEST(stlToNlohmann, writeEmptyArrayMapStruct) {
	testNLohmannSerializeDeserializeEmptyArrayMapStruct value;
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json["itemArray"].is_null(), false);
	EXPECT_EQ(json["itemArray"].is_array(), true);
	EXPECT_EQ(json["itemMap"].is_null(), false);
	EXPECT_EQ(json["itemMap"].is_object(), true);
}


TEST(stlToNlohmann, writePairKeyString) {
	std::pair<std::string, int> value {"key", 12345};
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json.is_array(), true);
	ASSERT_EQ(json.size(), 2);
	EXPECT_EQ(json[0u].get<std::string>(), "key");
	EXPECT_EQ(json[1u].get<int>(), 12345);
}

TEST(stlToNlohmann, writePairKeyInt) {
	std::pair<int, int> value {98765, 12345};
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json.is_array(), true);
	ASSERT_EQ(json.size(), 2);
	EXPECT_EQ(json[0u].get<int>(), 98765);
	EXPECT_EQ(json[1u].get<int>(), 12345);
}

TEST(stlToNlohmann, writeTupleStringInt) {
	std::tuple<std::string, int> value {"key", 12345};
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json.is_array(), true);
	ASSERT_EQ(json.size(), 2);
	EXPECT_EQ(json[0u].get<std::string>(), "key");
	EXPECT_EQ(json[1u].get<int>(), 12345);
}

struct tupleNLohmannSerStructItem {
	DEFINE_DATA(item1, int)
	DEFINE_DATA(item2, std::string)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(item1)
	DEFINE_META_TABLE_ITEM(item2)
	END_META_TABLE
};

TEST(stlToNlohmann, writeTupleComplex) {
	using testType = std::tuple<std::string, int, std::vector<int>, std::tuple<int, int, int>, tupleNLohmannSerStructItem>;
	
	testType value;
	std::get<0>(value) = "98765";
	std::get<1>(value) = 1234;
	
	const std::vector<int> vec {1, 2, 3};
	std::get<2>(value) = vec;
	
	std::tuple<int, int, int> tup {100, 200, 300};
	std::get<3>(value) = tup;
	
	tupleNLohmannSerStructItem struc;
	struc.item1 = 100;
	struc.item2 = "200";
	std::get<4>(value) = struc;
	
	
	nlohmann::json json;
	yb::nlohmann::Json_storage_adapter_from_cpp storage_adapter{json};
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage_adapter);
	inst.write_to();
	
	EXPECT_EQ(json.is_array(), true);
	ASSERT_EQ(json.size(), 5);
	EXPECT_EQ(json[0u].get<std::string>(), "98765");
	EXPECT_EQ(json[1u].get<int>(), 1234);
	
	EXPECT_EQ(json[2u].is_array(), true);
	EXPECT_EQ(json[2u].size(), 3);
	EXPECT_EQ(json[2u][0u].is_number_integer(), true);
	EXPECT_EQ(json[2u][0u].get<int>(), 1);
	EXPECT_EQ(json[2u][1u].is_number_integer(), true);
	EXPECT_EQ(json[2u][1u].get<int>(), 2);
	EXPECT_EQ(json[2u][2u].is_number_integer(), true);
	EXPECT_EQ(json[2u][2u].get<int>(), 3);
	
	EXPECT_EQ(json[3u].is_array(), true);
	EXPECT_EQ(json[3u].size(), 3);
	EXPECT_EQ(json[3u][0u].is_number_integer(), true);
	EXPECT_EQ(json[3u][0u].get<int>(), 100);
	EXPECT_EQ(json[3u][1u].is_number_integer(), true);
	EXPECT_EQ(json[3u][1u].get<int>(), 200);
	EXPECT_EQ(json[3u][2u].is_number_integer(), true);
	EXPECT_EQ(json[3u][2u].get<int>(), 300);
	
	EXPECT_EQ(json[4u].is_object(), true);
	ASSERT_EQ(json[4u].size(), 2);
	EXPECT_EQ(json[4u]["item1"].is_number_integer(), true);
	EXPECT_EQ(json[4u]["item1"].get<int>(), 100);
	EXPECT_EQ(json[4u]["item2"].is_string(), true);
	EXPECT_EQ(json[4u]["item2"].get<std::string>(), "200");
}

TEST(stlToNlohmann, write_AssistVectorMapStringInt) {
	const std::vector<std::map<int, std::string>> value{{{12345, "string_test0"}, {98765, "string_test1"}}, {{54321, "string_test11"}, {1234321, "string_test12"}}};
	nlohmann::json json;
	yb::assist::serialize(yb::nlohmann::Json_storage_adapter_from_cpp{json}, value);
	
	ASSERT_EQ(json.is_array(), true);
	ASSERT_EQ(json.size(), 2);
	
	EXPECT_EQ(json[0u]["12345"].get<std::string>(), "string_test0");
	EXPECT_EQ(json[0u]["98765"].get<std::string>(), "string_test1");
	
	EXPECT_EQ(json[1u]["54321"].get<std::string>(), "string_test11");
	EXPECT_EQ(json[1u]["1234321"].get<std::string>(), "string_test12");
}
