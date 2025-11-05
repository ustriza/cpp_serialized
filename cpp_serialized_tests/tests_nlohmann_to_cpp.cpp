//
//  tests_jsoncpp_to_cpp.cpp
//
//  Created by Yuri Barmin on 01.09.2023.
//

#include "cpp_serialized.h"
#include "nlohmann_storage.h"
#include "tests_engine.h"
#include "enum_string.h"

struct autoJsonDeserContainerItem {
	DEFINE_DESER_CONTAINER_GETTER(Item, int)
	
	BEGIN_DESER_META_TABLE
	DEFINE_DESER_META_TABLE_CONTAINER_ITEM(Item)
	END_DESER_META_TABLE
	
};

struct autoJsonDeserContainerAdapterItem {
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
struct autoJsonDeserUnitTestStruct {
		//Getters
	DEFINE_DESER_GETTER(intProp,int)
	DEFINE_DESER_GETTER(boolPropTrue, bool)
	DEFINE_DESER_GETTER(boolPropFalse, bool)
	DEFINE_DESER_GETTER(floatProp, float)
	DEFINE_DESER_GETTER(stringProp, std::string)
	DEFINE_DESER_GETTER(vectorOfIntsProp, std::vector<int>)
	DEFINE_DESER_GETTER(vectorOfStringsProp, std::vector<std::string>)
	using mapOfInts = std::map<int, int>;
	DEFINE_DESER_GETTER(mapOfIntsProp, mapOfInts)
	using mapOfStrings = std::map<std::string, std::string>;
	DEFINE_DESER_GETTER(mapOfStringsProp, mapOfStrings)
	DEFINE_DESER_GETTER(adapterProp, int)
	
	DEFINE_DESER_GETTER(intPropEmpty, int)
	DEFINE_DESER_GETTER(intPropEmpty2, int)
	DEFINE_DESER_GETTER(intPropEmpty3, int)
	DEFINE_DESER_GETTER(intPropDefault, int)
	DEFINE_DESER_GETTER(adapterPropEmpty, int)
	DEFINE_DESER_GETTER(adapterPropDefault, int)
	
	DEFINE_DESER_GETTER(intPropNotEmpty, int)
	DEFINE_DESER_GETTER(intPropNotDefault, int)
	DEFINE_DESER_GETTER(adapterPropNotEmpty, int)
	DEFINE_DESER_GETTER(adapterPropNotDefault, int)
	
	DEFINE_DESER_GETTER(vectorOfIntsContItems, std::vector<autoJsonDeserContainerItem>)
	DEFINE_DESER_GETTER(vectorOfIntsContAdaptItems, std::vector<autoJsonDeserContainerAdapterItem>)
	
	
	//Meta table
	BEGIN_DESER_META_TABLE
	DEFINE_DESER_META_TABLE_ITEM(intProp)
	DEFINE_DESER_META_TABLE_ITEM(boolPropTrue)
	DEFINE_DESER_META_TABLE_ITEM(boolPropFalse)
	DEFINE_DESER_META_TABLE_ITEM(floatProp)
	DEFINE_DESER_META_TABLE_ITEM(stringProp)
	DEFINE_DESER_META_TABLE_ITEM(vectorOfIntsProp)
	DEFINE_DESER_META_TABLE_ITEM(vectorOfStringsProp)
	DEFINE_DESER_META_TABLE_ITEM(mapOfIntsProp)
	DEFINE_DESER_META_TABLE_ITEM(mapOfStringsProp)
	
	DEFINE_DESER_META_TABLE_ITEM_ADAPTER(adapterProp, std::string, [](const std::string& item){
		if(item.length() < 1) {
			return -1;
		}
		const auto s = item.substr(1);
		return yb::string_utils::string_to_val<int>(s);
	})
	
	DEFINE_DESER_META_TABLE_ITEM_EMPTY(intPropEmpty)
	DEFINE_DESER_META_TABLE_ITEM_EMPTY(intPropEmpty2)
	DEFINE_DESER_META_TABLE_ITEM(intPropEmpty3)
	DEFINE_DESER_META_TABLE_ITEM_DEFAULT(intPropDefault, 100)
	
	DEFINE_DESER_META_TABLE_ITEM_EMPTY_ADAPTER(adapterPropEmpty, std::string, [](const std::string& item){
		if(item.empty()) {
			return 1111;
		}
		return 2222;
	})
	DEFINE_DESER_META_TABLE_ITEM_DEFAULT_ADAPTER(adapterPropDefault, "5555", std::string, [](const std::string& item){
		return yb::string_utils::string_to_val<int>(item);
	})
	
	DEFINE_DESER_META_TABLE_ITEM_EMPTY(intPropNotEmpty)
	DEFINE_DESER_META_TABLE_ITEM_DEFAULT(intPropNotDefault, 987654)
	DEFINE_DESER_META_TABLE_ITEM_EMPTY_ADAPTER(adapterPropNotEmpty, std::string, [](const std::string& item){
		if(item.empty()) {
			return 1111;
		}
		return 2222;
	})
	DEFINE_DESER_META_TABLE_ITEM_DEFAULT_ADAPTER(adapterPropNotDefault, "344", std::string, [](const std::string& item){
		if(item.length() < 1) {
			return -1;
		}
		const auto s = item.substr(1);
		return yb::string_utils::string_to_val<int>(s);
	})
	
	
	DEFINE_DESER_META_TABLE_ITEM(vectorOfIntsContItems)
	DEFINE_DESER_META_TABLE_ITEM(vectorOfIntsContAdaptItems)
	
	END_DESER_META_TABLE
};

TEST(nlohmannToStl, read) {
	
	const auto srcJson =
	"{\n"
	"\t\"intProp\": 15,\n"
	"\t\"intPropEmpty3\": 28,\n"
	"\t\"boolPropTrue\": true,\n"
	"\t\"boolPropFalse\": false,\n"
	"\t\"floatProp\": 15.5,\n"
	"\t\"stringProp\": \"The Persuaders\",\n"
	"\t\"vectorOfIntsProp\": [1, 2, 10, 20],\n"
	"\t\"vectorOfStringsProp\": [\"s1\", \"s2\", \"s10\", \"s20\"],\n"
	"\t\"mapOfIntsProp\": {\n"
	"\t\t\"1\": 86400,  \n"
	"\t\t\"2\": 172800, \n"
	"\t\t\"3\": 259200 \n"
	"\t},\n"
	"\t\"mapOfStringsProp\": {\n"
	"\t\t\"1\": \"s86400\",\n"
	"\t\t\"2\": \"s172800\",\n"
	"\t\t\"3\": \"s259200\" \n"
	"\t},\n"
	"\t\"adapterProp\": \"s12345\",\n"
	"\t\"intPropNotEmpty\": 1000,\n"
	"\t\"intPropNotDefault\": 2000,\n"
	" \t\"adapterPropNotEmpty\": \"s6000\",\n"
	"\t\"adapterPropNotDefault\": \"s7777\",\n"
	"\t\"vectorOfIntsContItems\": [10, 20, 100, 200],\n"
	"\t\"vectorOfIntsContAdaptItems\": [\"s100\", \"s200\", \"s1000\", \"s2000\"]\n"
	"}";
	
	nlohmann::json autoJsonDeserUnitTestJson;
	try {
		autoJsonDeserUnitTestJson = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{autoJsonDeserUnitTestJson};

	autoJsonDeserUnitTestStruct autoJsonDeserUnitTestData;

	const auto getResult = yb::assist::deserialize_to(storage, autoJsonDeserUnitTestData);
	ASSERT_EQ(getResult, true);
	
	
	EXPECT_EQ(autoJsonDeserUnitTestData.get_intProp(), 15);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_boolPropTrue(), true);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_boolPropFalse(), false);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_floatProp(), 15.5);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_stringProp(), "The Persuaders");
	
	const auto testVectorOfInts = std::vector {1, 2, 10, 20};
	EXPECT_EQ(autoJsonDeserUnitTestData.get_vectorOfIntsProp(), testVectorOfInts);
	
	const auto testVectorOfStrings = std::vector<std::string> {"s1", "s2", "s10", "s20"};
	EXPECT_EQ(autoJsonDeserUnitTestData.get_vectorOfStringsProp(), testVectorOfStrings);
	
	const auto testMapOfIntsProp = std::map<int, int> { {1,86400}, {2, 172800}, {3, 259200}};
	EXPECT_EQ(autoJsonDeserUnitTestData.get_mapOfIntsProp(), testMapOfIntsProp);
	
	const auto testMapOfStringsProp = std::map<std::string, std::string> {{"1", "s86400"}, {"2", "s172800"}, {"3", "s259200"}};
	EXPECT_EQ(autoJsonDeserUnitTestData.get_mapOfStringsProp(), testMapOfStringsProp);
	
	EXPECT_EQ(autoJsonDeserUnitTestData.get_adapterProp(), 12345);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_intPropEmpty(), 0);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_intPropEmpty2(), 0);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_intPropEmpty3(), 28);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_intPropDefault(), 100);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_adapterPropEmpty(), 1111);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_adapterPropDefault(), 5555);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_intPropNotEmpty(), 1000);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_intPropNotDefault(), 2000);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_adapterPropNotEmpty(), 2222);
	EXPECT_EQ(autoJsonDeserUnitTestData.get_adapterPropNotDefault(), 7777);
	{
		const auto testVectorOfIntsContItems = std::vector {10, 20, 100, 200};
		EXPECT_EQ(testVectorOfIntsContItems.size(), autoJsonDeserUnitTestData.get_vectorOfIntsContItems().size());
		if(testVectorOfIntsContItems.size() == autoJsonDeserUnitTestData.get_vectorOfIntsContItems().size()) {
			for(size_t i = 0; i < autoJsonDeserUnitTestData.get_vectorOfIntsContItems().size();i++) {
				EXPECT_EQ(autoJsonDeserUnitTestData.get_vectorOfIntsContItems()[i], testVectorOfIntsContItems[i]);
				if(autoJsonDeserUnitTestData.get_vectorOfIntsContItems()[i] != testVectorOfIntsContItems[i]) {
					break;
				}
			}
		}
	}
	
	{
		const auto testVectorOfIntsContAdaptItems = std::vector {100, 200, 1000, 2000};
		EXPECT_EQ(testVectorOfIntsContAdaptItems.size(), autoJsonDeserUnitTestData.get_vectorOfIntsContAdaptItems().size());
		if(testVectorOfIntsContAdaptItems.size() == autoJsonDeserUnitTestData.get_vectorOfIntsContAdaptItems().size()) {
			for(size_t i = 0; i < autoJsonDeserUnitTestData.get_vectorOfIntsContAdaptItems().size();i++) {
				EXPECT_EQ(autoJsonDeserUnitTestData.get_vectorOfIntsContAdaptItems()[i], testVectorOfIntsContAdaptItems[i]);
				if(autoJsonDeserUnitTestData.get_vectorOfIntsContAdaptItems()[i] != testVectorOfIntsContAdaptItems[i]) {
					break;
				}
			}
		}
	}
}

DEFINE_ENUM_STRING(myReadEnum, ME_1, ME_2)

TEST(nlohmannToStl, readEnum) {
	const auto srcJson =
 R"({
 "1": "ME_2"
 })";
	
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}

	yb::nlohmann::Json_storage_adapter_to_cpp storage{json};

	myReadEnum enumVar;
	const auto getResult = yb::assist::deserialize_to(yb::assist::find_storage(storage, "1"), enumVar);
	ASSERT_EQ(getResult, true);
	EXPECT_EQ(enumVar, myReadEnum::ME_2);
}

TEST(nlohmannToStl, readEnumFail) {
	const auto srcJson =
 R"({
 "1": "ME_234647647675"
 })";
	
	
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{json};

	myReadEnum enumVar;
	const auto getResult = yb::assist::deserialize_to(yb::assist::find_storage(storage, "1"), enumVar);
	ASSERT_EQ(getResult, false);
}

TEST(nlohmannToStl, readIntToFloat) {
	const auto srcJson =
	"2";
	
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{json};
	
	float myVar;
	
	auto deser = yb::to_cpp::storage_to_cpp_instance(myVar, storage);
	const bool success = deser.read_from();
	
	ASSERT_EQ(success, true);
	EXPECT_EQ(myVar, 2.f);
}

TEST(nlohmannToStl, readFloatToInt) {
	const auto srcJson =
	"2.0";
	
	
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	int myVar;
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage_adapter{json};
	auto deser = yb::to_cpp::storage_to_cpp_instance(myVar, storage_adapter);
	const bool getResult = deser.read_from();
	
	ASSERT_EQ(getResult, false);
}

TEST(nlohmannToStlUtils, timePointFromJson) {
	const time_t data {1234567890L};
	
	std::chrono::time_point<std::chrono::system_clock> value;
	nlohmann::json curNode = std::to_string(data);
	
	const auto result = yb::assist::deserialize_to(yb::nlohmann::Json_storage_adapter_to_cpp(curNode), value);

	ASSERT_EQ(result, true);
	ASSERT_EQ(std::chrono::system_clock::to_time_t(value), data);
}

TEST(nlohmannToStlUtils, timePointFromJsonFail) {
	std::chrono::time_point<std::chrono::system_clock> value;
	nlohmann::json curNode = 98989333; //wrong data type
	
	const auto result = yb::assist::deserialize_to(yb::nlohmann::Json_storage_adapter_to_cpp(curNode), value);

	ASSERT_EQ(result, false);
}

TEST(nlohmannToStl, readPairKeyString) {
	const auto srcJson =
 R"([
 "key", 1234
 ])";
	
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{json};

	std::pair<std::string, int> pairVar;
	const auto getResult = yb::assist::deserialize_to(yb::nlohmann::Json_storage_adapter_to_cpp(json), pairVar);
	ASSERT_EQ(getResult, true);
	
	const std::pair<std::string, int> standardValue{"key", 1234};
	EXPECT_EQ(pairVar, standardValue);
}

TEST(nlohmannToStl, readPairKeyInt) {
	const auto srcJson =
 R"([
 98765, 1234
 ])";
	
	
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{json};

	std::pair<int, int> pairVar;
	const auto getResult = yb::assist::deserialize_to(yb::nlohmann::Json_storage_adapter_to_cpp(json), pairVar);
	ASSERT_EQ(getResult, true);
	
	const std::pair<int, int> standardValue{98765, 1234};
	EXPECT_EQ(pairVar, standardValue);
}

TEST(nlohmannToStl, readTupleStringInt) {
	const auto srcJson =
 R"([
 "98765", 1234
 ])";
	
	
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{json};

	std::tuple<std::string, int> tupleVar;
	const auto getResult = yb::assist::deserialize_to(yb::nlohmann::Json_storage_adapter_to_cpp(json), tupleVar);
	ASSERT_EQ(getResult, true);
	
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


TEST(nlohmannToStl, readTupleComplex) {
	const auto srcJson =
 R"([
 "98765", 1234, [1, 2, 3], [100, 200, 300], {"item1": 100, "item2": "200"}
 ])";
	
	
	using testType = std::tuple<std::string, int, std::vector<int>, std::tuple<int, int, int>, tupleDeserStructItem>;
	
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{json};

	testType tupleVar;
	const auto getResult = yb::assist::deserialize_to(yb::nlohmann::Json_storage_adapter_to_cpp(json), tupleVar);
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

struct testDeserializeStruct {
	DEFINE_DATA(item, int)
	DEFINE_DATA(item2, int)
	
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(item)
	DEFINE_META_TABLE_ITEM(item2)
	END_META_TABLE
};

TEST(nlohmannToStl, readStruct0) {
	const auto srcJson = "{"
	"\"item\": 12345, \"item2\": 56789}";
	
	nlohmann::json autoJsonDeserUnitTestJson;
	try {
		autoJsonDeserUnitTestJson = nlohmann::json::parse(srcJson);
	}
	catch(...) {
		ASSERT_EQ(false, true);
		return;
	}
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage{autoJsonDeserUnitTestJson};

	testDeserializeStruct value;
	
	yb::nlohmann::Json_storage_adapter_to_cpp storage_adapter{autoJsonDeserUnitTestJson};
	auto deser = yb::to_cpp::storage_to_cpp_instance(value, storage_adapter);
	const bool success = deser.read_from();

	ASSERT_EQ(success, true);
	
	EXPECT_EQ(value.item, 12345);
	EXPECT_EQ(value.item2, 56789);
}
