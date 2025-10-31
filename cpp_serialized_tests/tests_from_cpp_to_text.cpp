//
//  tests_from_cpp_to_text
//
//  Created by Yuri Barmin on 08.08.2023.
//

#if __cplusplus >= 202002L //C++ 20
#include <ranges>
#endif

#include "cpp_serialized.h"
#include "text_storage.h"
#include "tests_engine.h"

TEST(TestsToText, writeInt) {
	const int value = 123456;
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.get_text(), "123456");
}

TEST(TestsToText, writeEmptyVectorInt) {
	const std::vector<int> value;
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeVectorEmptyVectorInt) {
	const std::vector<std::vector<int>> value{{}, {}};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  [\n"
	"  ],\n"
	"  [\n"
	"  ]\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeEmptyMapIntInt) {
	const std::map<int, int> value;
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "{\n"
	"}";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeVectorEmptyMapIntInt) {
	const std::vector<std::map<int, int>> value{{}, {}};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  {\n"
	"  },\n"
	"  {\n"
	"  }\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}


TEST(TestsToText, writeVectorInt) {
	const std::vector<int> value{12345, 67890};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  12345,\n"
	"  67890"
	"\n]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeString) {
	const std::string value = "123456";
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.get_text(), R"("123456")");
}

TEST(TestsToText, writeVectorString) {
	const std::vector<std::string> value{"12345", "67890"};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  \"12345\",\n"
	"  \"67890\""
	"\n]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeMapIntInt) {
	const std::map<int, int> value{{1, 12345}, {2, 67890}};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "{\n"
	"  1: 12345,\n"
	"  2: 67890"
	"\n}";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeMapStringString) {
	const std::map<std::string, std::string> value{{"1", "12345"}, {"2", "67890"}};

	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	const std::string test_data = "{\n"
	"  \"1\": \"12345\",\n"
	"  \"2\": \"67890\""
	"\n}";

	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeVectorVectorInt) {
	const std::vector<std::vector<int>> value{{12345, 67890}, {54321, 9876}};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  [\n"
	"    12345,\n"
	"    67890\n"
	"  ],\n"
	"  [\n"
	"    54321,\n"
	"    9876\n"
	"  ]\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeVectorVectorString) {
	const std::vector<std::vector<std::string>> value{{"12345", "67890"}, {"54321", "9876"}};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  [\n"
	"    \"12345\",\n"
	"    \"67890\"\n"
	"  ],\n"
	"  [\n"
	"    \"54321\",\n"
	"    \"9876\"\n"
	"  ]\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeVectorMapIntInt) {
	const std::vector<std::map<int, int>> value{{{12345, 67890}}, {{54321, 9876}}};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  {\n"
	"    12345: 67890\n"
	"  },\n"
	"  {\n"
	"    54321: 9876\n"
	"  }\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeVectorMapStringString) {
	const std::vector<std::map<std::string, std::string>> value{{{"12345", "67890"}}, {{"54321", "9876"}}};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  {\n"
	"    \"12345\": \"67890\"\n"
	"  },\n"
	"  {\n"
	"    \"54321\": \"9876\"\n"
	"  }\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, writeVectorMapStringVectorInt) {
	const std::vector<std::map<std::string, std::vector<int>>> value{
		{{"key0", {67890, 11111}}}
		, {{"key1", {9876, 222222}}}
		, {{"key2", {2605, 333333}}}
	};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  {\n"
	"    \"key0\": [\n"
	"      67890,\n"
	"      11111\n"
	"    ]\n"
	"  },\n"
	"  {\n"
	"    \"key1\": [\n"
	"      9876,\n"
	"      222222\n"
	"    ]\n"
	"  },\n"
	"  {\n"
	"    \"key2\": [\n"
	"      2605,\n"
	"      333333\n"
	"    ]\n"
	"  }\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}


TEST(TestsToText, writeVectorMapStringVectorMapIntFloat) {
	const std::vector<std::map<std::string, std::map<int, float>>> value{
		{{"key0", {{67890, 11111.23f}, {678654, 55555.56f}}}}
		, {{"key1", {{9876, 222222.45f}, {98725, 666666.32f}}}}
		, {{"key2", {{2605, 333333.63f}, {198342, 77777.78f}}}}
	};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  {\n"
	"    \"key0\": {\n"
	"      67890: 11111.230469,\n"
	"      678654: 55555.558594\n"
	"    }\n"
	"  },\n"
	"  {\n"
	"    \"key1\": {\n"
	"      9876: 222222.453125,\n"
	"      98725: 666666.312500\n"
	"    }\n"
	"  },\n"
	"  {\n"
	"    \"key2\": {\n"
	"      2605: 333333.625000,\n"
	"      198342: 77777.781250\n"
	"    }\n"
	"  }\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

struct Nested_struct_to_string {
	DEFINE_DATA(intProp, int)
	DEFINE_DATA(stringProp, std::string)

	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(intProp)
	DEFINE_META_TABLE_ITEM(stringProp)
	END_META_TABLE
};


struct Struct_to_string {
	DEFINE_DATA(intProp, int)
	DEFINE_DATA(boolTrueProp, bool)
	DEFINE_DATA(boolFalseProp, bool)
	DEFINE_DATA(floatProp, float)
	DEFINE_DATA(stringProp, std::string)
	DEFINE_DATA(vectorIntProp, std::vector<int>)
	DEFINE_DATA(vectorStringProp, std::vector<std::string>)

	using Vector_map_int_string = std::vector<std::map<int, std::string>>;
	DEFINE_DATA(vectorMapIntStringProp, Vector_map_int_string)

	using Vector_map_int_vector_string = std::vector<std::map<int, std::vector<std::string>>>;
	DEFINE_DATA(vectorMapIntVectorStringProp, Vector_map_int_vector_string)

	DEFINE_DATA(nestedStructToString, Nested_struct_to_string)

	BEGIN_META_TABLE
	DEFINE_META_TABLE_ITEM(intProp)
	DEFINE_META_TABLE_ITEM(boolTrueProp)
	DEFINE_META_TABLE_ITEM(boolFalseProp)
	DEFINE_META_TABLE_ITEM(floatProp)
	DEFINE_META_TABLE_ITEM(stringProp)
	DEFINE_META_TABLE_ITEM(vectorIntProp)
	DEFINE_META_TABLE_ITEM(vectorStringProp)
	DEFINE_META_TABLE_ITEM(vectorMapIntStringProp)
	DEFINE_META_TABLE_ITEM(vectorMapIntVectorStringProp)
	DEFINE_META_TABLE_ITEM(nestedStructToString)
	END_META_TABLE
};


TEST(TestsStructToString, writeStruct) {
	Struct_to_string value;
	value.intProp = 123456;
	value.boolTrueProp = true;
	value.boolFalseProp = false;
	value.floatProp = 30000.f;
	value.stringProp = "s30000";
	value.vectorIntProp = {100, 200, 300};
	value.vectorStringProp = {"s500", "s600", "s700"};
	value.vectorMapIntStringProp = {
		  {{100, "s100"}, {200, "s200"}, {300, "s300"}}
		, {{1000, "s1000"}, {2000, "s2000"}, {3000, "s3000"}}
	};
	value.vectorMapIntVectorStringProp = {
		  {{100, {"s100", "s101", "s102"}}, {200, {"s200"}}, {300, {"s300"}}, {400, {}}}
		, {{1000, {"s1000", "s1001", "s1002"}}, {2000, {"s2000"}}, {3000, {"s3000"}}, {4000, {}}}
	};
	
	value.nestedStructToString.intProp = 9000;
	value.nestedStructToString.stringProp = "s9000";

	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	const std::string test_data = "{\n"
	"  \"intProp\": 123456,\n"
	"  \"boolTrueProp\": true,\n"
	"  \"boolFalseProp\": false,\n"
	"  \"floatProp\": 30000.000000,\n"
	"  \"stringProp\": \"s30000\",\n"
	"  \"vectorIntProp\": [\n"
	"    100,\n"
	"    200,\n"
	"    300\n"
	"  ],\n"
	"  \"vectorStringProp\": [\n"
	"    \"s500\",\n"
	"    \"s600\",\n"
	"    \"s700\"\n"
	"  ],\n"
	"  \"vectorMapIntStringProp\": [\n"
	"    {\n"
	"      100: \"s100\",\n"
	"      200: \"s200\",\n"
	"      300: \"s300\"\n"
	"    },\n"
	"    {\n"
	"      1000: \"s1000\",\n"
	"      2000: \"s2000\",\n"
	"      3000: \"s3000\"\n"
	"    }\n"
	"  ],\n"
	"  \"vectorMapIntVectorStringProp\": [\n"
	"    {\n"
	"      100: [\n"
	"        \"s100\",\n"
	"        \"s101\",\n"
	"        \"s102\"\n"
	"      ],\n"
	"      200: [\n"
	"        \"s200\"\n"
	"      ],\n"
	"      300: [\n"
	"        \"s300\"\n"
	"      ],\n"
	"      400: [\n"
	"      ]\n"
	"    },\n"
	"    {\n"
	"      1000: [\n"
	"        \"s1000\",\n"
	"        \"s1001\",\n"
	"        \"s1002\"\n"
	"      ],\n"
	"      2000: [\n"
	"        \"s2000\"\n"
	"      ],\n"
	"      3000: [\n"
	"        \"s3000\"\n"
	"      ],\n"
	"      4000: [\n"
	"      ]\n"
	"    }\n"
	"  ],\n"
	"  \"nestedStructToString\": {\n"
	"    \"intProp\": 9000,\n"
	"    \"stringProp\": \"s9000\"\n"
	"  }\n"
	"}";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

struct Deser_struct_to_string {
	DEFINE_DESER_GETTER_UNITTEST(intProp, int)
	DEFINE_DESER_GETTER_UNITTEST(vectorIntProp, std::vector<int>)
	
	BEGIN_META_TABLE
	DEFINE_DESER_META_TABLE_ITEM(intProp)
	DEFINE_DESER_META_TABLE_ITEM(vectorIntProp)
	END_META_TABLE
};

TEST(TestsStructToString, writeDeserStruct) {
	Deser_struct_to_string value;
	value.intProp = 123456;
	value.vectorIntProp = {100, 200, 300};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	const std::string test_data = "{\n"
	"  \"intProp\": 123456,\n"
	"  \"vectorIntProp\": [\n"
	"    100,\n"
	"    200,\n"
	"    300\n"
	"  ]\n"
	"}";
	
	EXPECT_EQ(storage.get_text(), test_data);
}


TEST(TestsStructToString, writeTupleStringInt) {
	std::tuple<std::string, int> value {"key", 12345};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  \"key\",\n"
	"  12345\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsStructToString, writeTimePoint) {
	const time_t data {1234567890L};
	std::chrono::time_point<std::chrono::system_clock> value = std::chrono::system_clock::from_time_t(data);

	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "\"1234567890\"";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsStructToString, writePairStringInt) {
	std::pair<std::string, int> value {"key", 12345};
	
	yb::text_from_cpp::TextStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	const std::string test_data = "[\n"
	"  \"key\",\n"
	"  12345\n"
	"]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, set_value_VectorInt) {
	const std::vector<int> value{12345, 67890};
	
	yb::text_from_cpp::TextStorage storage;
	
	yb::assist::serialize(storage, value);
	
	const std::string test_data = "[\n"
	"  12345,\n"
	"  67890"
	"\n]";
	
	EXPECT_EQ(storage.get_text(), test_data);
}

TEST(TestsToText, to_string_VectorInt) {
	const std::vector<int> value{12345, 67890};
	
	const auto result_text = yb::assist::to_string(value);
	
	const std::string test_data = "[\n"
	"  12345,\n"
	"  67890"
	"\n]";
	
	EXPECT_EQ(result_text, test_data);
}

#if __cplusplus >= 202002L //C++ 20
TEST(TestsToText, to_string_RangeViewAllInt) {
	const std::vector<int> value{12345, 67890};
	
	{
		const auto result_text = yb::assist::to_string(value | std::ranges::views::all);
		
		const std::string test_data = "[\n"
		"  12345,\n"
		"  67890"
		"\n]";
		
		EXPECT_EQ(result_text, test_data);
	}
	
	{
		const auto view = std::ranges::views::all;
		const auto result_text = yb::assist::to_string(value | view);
		
		const std::string test_data = "[\n"
		"  12345,\n"
		"  67890"
		"\n]";
		
		EXPECT_EQ(result_text, test_data);
	}

	{
		auto view = std::ranges::views::all;
		const auto result_text = yb::assist::to_string(value | view);
		
		const std::string test_data = "[\n"
		"  12345,\n"
		"  67890"
		"\n]";
		
		EXPECT_EQ(result_text, test_data);
	}

}

TEST(TestsToText, to_string_RangeViewFilterInt) {
	const std::vector<int> value{12345, 67890};
	
	{
		const auto result_text = yb::assist::to_string(value | std::ranges::views::filter(
			[](const auto& item){return item > 12345;}));
					
		const std::string test_data = "[\n"
		"  67890"
		"\n]";
		
		EXPECT_EQ(result_text, test_data);
	}
	
	{
		auto value_view = value | std::ranges::views::filter([](const auto& item){return item > 12345;});
		
		const auto result_text = yb::assist::to_string(value_view);
		
		const std::string test_data = "[\n"
		"  67890"
		"\n]";
		
		EXPECT_EQ(result_text, test_data);
	}

}

TEST(TestsToText, to_string_RangeViewNotContainer) {
	{
		const auto result_text = yb::assist::to_string(std::ranges::iota_view{1, 10});
		
		const std::string test_data = "[\n  1,\n  2,\n  3,\n  4,\n  5,\n  6,\n  7,\n  8,\n  9\n]";
		EXPECT_EQ(result_text, test_data);
	}

	{
		const auto result_text = yb::assist::to_string(std::ranges::single_view{1});
		
		const std::string test_data = "[\n  1\n]";
		EXPECT_EQ(result_text, test_data);
	}

	{
		const auto result_text = yb::assist::to_string(std::ranges::empty_view<int>{});
		
		const std::string test_data = "[\n]";
		EXPECT_EQ(result_text, test_data);
	}

	{
		const auto result_text = yb::assist::to_string(std::ranges::views::all(std::ranges::iota_view{1, 10}));
		
		const std::string test_data = "[\n  1,\n  2,\n  3,\n  4,\n  5,\n  6,\n  7,\n  8,\n  9\n]";
		EXPECT_EQ(result_text, test_data);
	}

//	{
//		auto value_view = std::ranges::iota_view{1, 10} | std::ranges::views::filter([](const int item){return item > 5;});
//		
//		const auto result_text = yb::assist::to_string(value_view);
//		
//		const std::string test_data = "[\n  6,\n  7,\n  8,\n  9\n]";
//		EXPECT_EQ(result_text, test_data);
//	}

}

#endif
