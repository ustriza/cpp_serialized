//
//  examples_README.cpp
//  cpp_serialized
//
//  Created by Yuri Barmin on 19.09.2023.
//

#include <iostream>

//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third party jsoncpp parser
#include "jsoncpp_storage.h"

//cpp_serialized storage for serialization to text.
#include "text_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>

//Tests engine
#include "tests_engine.h"

//Enum string
#include "enum_string.h"

//String utils
#include "string_utils.h"


bool serialize_example() {
	//Declare data for serialization
	const std::vector<std::map<int, std::string>> value{{{12345, "string_test0"}, {98765, "string_test1"}}, {{54321, "string_test11"}, {1234321, "string_test12"}}};
	
	//Declare json object - third-party jsoncpp parser.
	Json::Value json;
	
	//Serialize from 'value' to 'json' using cpp_serialized
	yb::assist::serialize(yb::jsoncpp::Json_storage_adapter_from_cpp{json}, value);

	//Check serialized data
	if(!json.isArray()) {
		return false;
	}
	if(json.size() != 2) {
		return false;
	}
	
	if(json[0u]["12345"].asString() != "string_test0") {
		return false;
	}
	if(json[0u]["98765"].asString() != "string_test1") {
		return false;
	}
	
	if(json[1u]["54321"].asString() != "string_test11") {
		return false;
	}
	if(json[1u]["1234321"].asString() != "string_test12") {
		return false;
	}
	
	return true;
}
TEST(example_README, serialize) {
	ASSERT_EQ(serialize_example(), true);
}

//You can serialize/deserialize your own structs and classes.
struct Struct_example {
	DEFINE_DATA(int_prop, int)
	DEFINE_DATA(string_prop, std::string)
	DEFINE_DATA(vector_int_prop, std::vector<int>)

	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(int_prop)
	DEFINE_META_TABLE_ITEM(string_prop)
	DEFINE_META_TABLE_ITEM(vector_int_prop)
	END_META_TABLE
};

bool serialize_example_2() {
	//Declare data for serialization
	Struct_example value;
	value.int_prop = 12345;
	value.string_prop = "string_12345";
	value.vector_int_prop = std::vector{12345, 56789};
	
	//Declare json object - third-party jsoncpp parser.
	Json::Value json;

	//Serialize from 'value' to 'json' using cpp_serialized
	yb::assist::serialize(yb::jsoncpp::Json_storage_adapter_from_cpp{json}, value);

	//Check serialized data
	if(!json.isObject()) {
		return false;
	}
	if(json["int_prop"].asInt() != 12345) {
		return false;
	}
	if(json["string_prop"].asString() != "string_12345") {
		return false;
	}
	
	const auto& vector_object = json["vector_int_prop"];
	if(!vector_object.isArray()) {
		return false;
	}
	if(vector_object[0u] != 12345) {
		return false;
	}
	if(vector_object[1u] != 56789) {
		return false;
	}

	return true;

}
TEST(example_README, serialize_2) {
	ASSERT_EQ(serialize_example_2(), true);
}

bool deserialize_to() {
	//Source Json text
	const auto srcJson =
 R"([
 "key", 1234
 ])";
	
	//Declare json object - third-party jsoncpp parser.
	Json::Reader reader;
	Json::Value json;
	[[maybe_unused]] const auto parse_json_result = reader.parse(srcJson, json);
	if(!parse_json_result) {
		return false;
	}
	
	//Declare data for deserialization. cpp_serialized suupports std::pair, std::tuple class templates.
	std::pair<std::string, int> value;
	
	//Deserialize from 'json' to 'value' using cpp_serialized
	const auto deserialize_status = yb::assist::deserialize_to(yb::jsoncpp::Json_storage_adapter_to_cpp(json), value);
	
	//Check deseserialize status
	if(!deserialize_status) {
		return false;
	}
	
	//Check deseserialized data
	const std::pair<std::string, int> testValue{"key", 1234};
	if(value != testValue) {
		return false;
	}
	
	return true;
}
TEST(example_README, deserialize_to) {
	ASSERT_EQ(deserialize_to(), true);
}

bool deserialize() {
	//Source Json text
	const auto srcJson =
 R"([
 "key", 1234, true
 ])";
	
	//Declare json object - third-party jsoncpp parser.
	Json::Reader reader;
	Json::Value json;
	[[maybe_unused]] const auto parse_json_result = reader.parse(srcJson, json);
	if(!parse_json_result) {
		return false;
	}
	
	//Deserialize from 'json' to 'value' using cpp_serialized. cpp_serialized suupports std::pair, std::tuple class templates.
	const auto value = yb::assist::deserialize<std::tuple<std::string, int, bool>>(yb::jsoncpp::Json_storage_adapter_to_cpp(json));
	
	//Check deseserialize status
	if(!value.has_value()) {
		return false;
	}
	
	//Check deseserialized data
	const std::tuple<std::string, int, bool> testValue{"key", 1234, true};
	if(value.value() != testValue) {
		return false;
	}
	
	return true;
}
TEST(example_README, deserialize) {
	ASSERT_EQ(deserialize(), true);
}

bool deserialize_def() {
	using Value_type = std::tuple<std::string, int, std::vector<int>>;

	//Source Json text. The second element("1234")  has wrong type string instead of integer. So this json will no be deserialized to the Value_type
	const auto srcJson =
 R"([
 "key", "1234", [56789, 98765]
 ])";
	
	//Declare json object - third-party jsoncpp parser.
	Json::Reader reader;
	Json::Value json;
	[[maybe_unused]] const auto parse_json_result = reader.parse(srcJson, json);
	if(!parse_json_result) {
		return false;
	}
	
	const Value_type default_value{"key1", 1, {1234, 4321}};

	//Deserialize from 'json' to 'value' using cpp_serialized. cpp_serialized suupports std::pair, std::tuple class templates.
	const auto value = yb::assist::deserialize<Value_type>(yb::jsoncpp::Json_storage_adapter_to_cpp(json), default_value);
	
	//Check deseserialized data
	if(value != default_value) {
		return false;
	}
	
	return true;
}
TEST(example_README, deserialize_def) {
	ASSERT_EQ(deserialize_def(), true);
}

struct Struct_serialize_to_text {
	DEFINE_DATA(int_prop, int)
	using Tuple_type = std::tuple<int, std::string>;
	DEFINE_DATA(tuple_prop, Tuple_type)
	DEFINE_DATA(struct_example_prop, Struct_example)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(int_prop)
	DEFINE_META_TABLE_ITEM(tuple_prop)
	DEFINE_META_TABLE_ITEM(struct_example_prop)
	END_META_TABLE
};

void serialize_to_text() {
	//Declare data for serilization
	const Struct_serialize_to_text value{
		987654,
		std::tuple{56789, "string_tuple_elm"},
		Struct_example{12345, "string_12345", std::vector{12345, 56789}}};

	//Declare storage
	yb::text_from_cpp::TextStorage storage;
	
	
	//Serialize from 'value' to text using cpp_serialized
	yb::assist::serialize(storage, value);
	
	//Output serialized value
	std::cout << std::endl;
	std::cout << storage.get_text();
	std::cout << std::endl;
}
TEST(example_README, serialize_to_text) {
	serialize_to_text();
}

void serialize_to_string() {
	//Declare data for serilization
	const auto value = std::tuple{12345, std::string{"string_element"}, 123.5f, true};
	
	//Serialize from 'value' to text using cpp_serialized
	const auto serialized_text = yb::assist::to_string(value);
	
	//Output serialized value
	std::cout << std::endl;
	std::cout << serialized_text;
	std::cout << std::endl;
}
TEST(example_README, serialize_to_string) {
	serialize_to_string();
}


struct Struct_deser_example {
	DEFINE_DESER_GETTER(int_prop, int)
	DEFINE_DESER_GETTER(string_prop, std::string)
	DEFINE_DESER_GETTER(vector_int_prop, std::vector<int>)
	
	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(int_prop)
	DEFINE_META_TABLE_ITEM_EMPTY(string_prop)
	DEFINE_META_TABLE_ITEM_DEFAULT(vector_int_prop, (std::vector{1000, 2000}))
	END_META_TABLE
};

bool deserialize_deser_to() {
	//Source Json text
	const auto srcJson =
 R"({
  "int_prop": 12345,
  "string_prop": "string_12345",
  "vector_int_prop": [
	12345,
	56789
  ]
})";
	
	//Declare json object - third-party jsoncpp parser.
	Json::Reader reader;
	Json::Value json;
	[[maybe_unused]] const auto parse_json_result = reader.parse(srcJson, json);
	if(!parse_json_result) {
		return false;
	}
	
	//Declare data for deserialization. cpp_serialized suupports std::pair, std::tuple class templates.
	Struct_deser_example value;
	
	//Deserialize from 'json' to 'value' using cpp_serialized
	const auto deserialize_status = yb::assist::deserialize_to(yb::jsoncpp::Json_storage_adapter_to_cpp(json), value);
	
	//Check deseserialize status
	if(!deserialize_status) {
		return false;
	}
	
	//Check deseserialized data
	if(value.get_int_prop() != 12345) {
		return false;
	}
	if(value.get_string_prop() != "string_12345") {
		return false;
	}
	if(value.get_vector_int_prop() != std::vector{12345, 56789}) {
		return false;
	}

	return true;
}
TEST(example_README, deserialize_deser_to) {
	ASSERT_EQ(deserialize_deser_to(), true);
}

DEFINE_ENUM_STRING(Example_enum, ME_1, ME_2)

bool enum_to_string() {
	const auto res_str1 = yb::string_utils::val_to_string(Example_enum::ME_1);
	
	const auto res_str2 = yb::string_utils::val_to_string(Example_enum::ME_2);
	
	const auto res_str3 = yb::string_utils::val_to_string(Example_enum(1234567));
	
	if(res_str1 != "ME_1") {
		return false;
	}
	if(res_str2 != "ME_2") {
		return false;
	}
	if(res_str3 != "") {
		return false;
	}
	return true;
}

TEST(example_README, enum_to_string) {
	EXPECT_EQ(enum_to_string(), true);
}

bool enum_from_string() {
	const auto res_enum1 = yb::string_utils::string_to_val<Example_enum>("ME_1");
	const auto res_enum2 = yb::string_utils::string_to_val<Example_enum>("ME_2");
	const auto res_enum3 = yb::string_utils::string_to_val<Example_enum>("ME_76565656");
	
	if(res_enum1 != Example_enum::ME_1) {
		return false;
	}
	if(res_enum2 != Example_enum::ME_2) {
		return false;
	}
	if(res_enum3 != Example_enum(-1)) {
		return false;
	}
	
	return true;
}


TEST(example_README, enum_string) {
	EXPECT_EQ(enum_from_string(), true);
}
