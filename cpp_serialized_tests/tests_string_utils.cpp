//
//  tests_string_utils.cpp
//  CrookedPines_tst
//
//  Created by Yuri Barmin on 19.05.2021.
//

#include "cpp_serialized.h"
#include "tests_engine.h"

#include "string_utils.h"
#include "enum_string.h"

template<typename T>
bool numbers_equal(T a, T b) {
	return std::abs(a - b) <= std::numeric_limits<T>::epsilon();
}

TEST(string_utils, string_to_valString) {
	EXPECT_EQ(yb::string_utils::string_to_val<std::string>("12345"), "12345");
	
	const std::string inData = "67890";
	const std::string& outData = yb::string_utils::string_to_val<std::string>(inData);
	EXPECT_EQ(inData, outData);
	EXPECT_EQ(&inData, &outData);
}

TEST(string_utils, string_to_valInt) {
	EXPECT_EQ(yb::string_utils::string_to_val<int>("12345"), 12345);
	EXPECT_EQ(yb::string_utils::string_to_val<int>("-12345"), -12345);
}

TEST(string_utils, string_to_valFloat) {
	EXPECT_EQ(yb::string_utils::string_to_val<float>("12345.6"), 12345.6f);
	EXPECT_EQ(yb::string_utils::string_to_val<float>("-12345.7"), -12345.7f);
}

TEST(string_utils, string_to_valDouble) {
	EXPECT_EQ(yb::string_utils::string_to_val<double>("12345.38"), 12345.38);
	EXPECT_EQ(yb::string_utils::string_to_val<double>("-12345.56"), -12345.56);
}

TEST(string_utils, string_to_valBool) {
	EXPECT_EQ(yb::string_utils::string_to_val<bool>("true"), 1);
	EXPECT_EQ(yb::string_utils::string_to_val<bool>("false"), 0);
	
	EXPECT_EQ(yb::string_utils::string_to_val<bool>("1"), 1);
	EXPECT_EQ(yb::string_utils::string_to_val<bool>("0"), 0);
	
	EXPECT_EQ(yb::string_utils::string_to_val<bool>("123"), 1);
	
	EXPECT_EQ(yb::string_utils::string_to_val<bool>("-123"), 1);
}

//TEST(string_utils, valToStringString) {
//	static_assert(std::is_same_v<std::result_of_t<decltype(&yb::string_utils::valToString<std::string>)(const std::string&)>, const std::string&>);
//	static_assert(std::is_same_v<std::result_of_t<decltype(&yb::string_utils::valToString<char*>)(char*)>,  std::string>);
//
//	EXPECT_EQ(yb::string_utils::valToString("12345"), "12345");
//	
//	const std::string inData = "67890";
//	const std::string& outData = yb::string_utils::valToString(inData);
//	
//	EXPECT_EQ(outData, inData);
//	ASSERT_EQ(&outData, &inData);
//}
//
//TEST(string_utils, valToStringInt) {
//	EXPECT_EQ(yb::string_utils::valToString(12345), "12345");
//	EXPECT_EQ(yb::string_utils::valToString(-12345), "-12345");
//}
//
//TEST(string_utils, valToStringChar) {
//	EXPECT_EQ(yb::string_utils::valToString(char(123)), "123");
//	EXPECT_EQ(yb::string_utils::valToString(char(-123)), "-123");
//	EXPECT_EQ(yb::string_utils::valToString((unsigned char)123), "123");
//}
//
//TEST(string_utils, valToStringBool) {
//	EXPECT_EQ(yb::string_utils::valToString(true), "true");
//	EXPECT_EQ(yb::string_utils::valToString(false), "false");
//}
//
//TEST(string_utils, valToStringFloat) {
//	EXPECT_EQ(yb::string_utils::valToString(12345.34f), "12345.34");
//	EXPECT_EQ(yb::string_utils::valToString(-12345.58f), "-12345.58");
//}
//
//TEST(string_utils, valToStringDouble) {
//	EXPECT_EQ(yb::string_utils::valToString(12345.24), "12345.24");
//	EXPECT_EQ(yb::string_utils::valToString(-12345.72), "-12345.72");
//}

TEST(string_utils, staticToLower) {
	constexpr char chA = yb::string_utils::staticToLower('A');
	constexpr char cha = yb::string_utils::staticToLower('a');
	constexpr char dig = yb::string_utils::staticToLower('1');
	constexpr char dot = yb::string_utils::staticToLower('.');
	
	static_assert(chA == 'a');
	static_assert(cha == 'a');
	static_assert(dig == '1');
	static_assert(dot == '.');

	EXPECT_EQ(chA, 'a');
	EXPECT_EQ(cha, 'a');
	EXPECT_EQ(dig, '1');
	EXPECT_EQ(dot, '.');
}

TEST(string_utils, staticCompareStringsIC) {
	constexpr bool chAa = yb::string_utils::static_compare_strings_ic("QWERTYUIOPASDFGHJKLZXCVBNM_!.123456789", "qwertyuiopasdfghjklzxcvbnm_!.123456789");

	constexpr bool chAA = yb::string_utils::static_compare_strings_ic("QWERTYUIOPASDFGHJKLZXCVBNM_!.123456789", "QWERTYUIOPASDFGHJKLZXCVBNM_!.123456789");

	constexpr bool chaa = yb::string_utils::static_compare_strings_ic("qwertyuiopasdfghjklzxcvbnm_!.123456789", "qwertyuiopasdfghjklzxcvbnm_!.123456789");

	constexpr bool chAaDiff = !yb::string_utils::static_compare_strings_ic("QWERTYUIOPASDFGHJKLZXCVBNM_!.123456789", "qwertyuiopasdfghjklzxcvbnm_!.987654321");

	constexpr bool chSize = !yb::string_utils::static_compare_strings_ic("qwertyuiopas", "qw");

	static_assert(chAa);
	static_assert(chAA);
	static_assert(chaa);
	static_assert(chAaDiff);
	static_assert(chSize);

	EXPECT_EQ(chAa, true);
	EXPECT_EQ(chAA, true);
	EXPECT_EQ(chaa, true);
	EXPECT_EQ(chAaDiff, true);
	EXPECT_EQ(chSize, true);
}

TEST(string_utils, splitVector) {
	std::vector<std::string> result;
	yb::string_utils::split("123\n4567\n\n890", '\n', result);
	
	ASSERT_EQ(result.size(), 4);
	auto it = result.begin();
	EXPECT_EQ(*it++, "123");
	EXPECT_EQ(*it++, "4567");
	EXPECT_EQ(*it++, "");
	EXPECT_EQ(*it++, "890");
}

TEST(string_utils, splitVectorLastEmpty) {
	std::vector<std::string> result;
	yb::string_utils::split("123\n4567\n\n890\n", '\n', result);
	
	ASSERT_EQ(result.size(), 4);
	auto it = result.begin();
	EXPECT_EQ(*it++, "123");
	EXPECT_EQ(*it++, "4567");
	EXPECT_EQ(*it++, "");
	EXPECT_EQ(*it++, "890");
}

TEST(string_utils, splitSet) {
	std::set<std::string> result;
	yb::string_utils::split("123\n456\n789\n", '\n', result);
	
	ASSERT_EQ(result.size(), 3);
	auto it = result.begin();
	EXPECT_EQ(*it++, "123");
	EXPECT_EQ(*it++, "456");
	EXPECT_EQ(*it++, "789");
}

TEST(string_utils, splitIntVector) {
	std::vector<int> result;
	yb::string_utils::split("123\n4567\n890", '\n', result);
	
	ASSERT_EQ(result.size(), 3);
	auto it = result.begin();
	EXPECT_EQ(*it++, 123);
	EXPECT_EQ(*it++, 4567);
	EXPECT_EQ(*it++, 890);
}

TEST(string_utils, splitEmptyVector) {
	std::vector<std::string> result;
	yb::string_utils::split("", '\n', result);
	
	ASSERT_EQ(result.size(), 0);
}

TEST(string_utils, splitOneDelimVector) {
	std::vector<std::string> result;
	yb::string_utils::split("\n", '\n', result);
	
	ASSERT_EQ(result.size(), 1);
}

TEST(string_utils, splitVectorByStr) {
	std::vector<std::string> result;
	yb::string_utils::split("123%p4567%p%p890", "%p", result);
	
	ASSERT_EQ(result.size(), 4);
	auto it = result.begin();
	EXPECT_EQ(*it++, "123");
	EXPECT_EQ(*it++, "4567");
	EXPECT_EQ(*it++, "");
	EXPECT_EQ(*it++, "890");
}

TEST(string_utils, splitIntVector2) {
	const auto result = yb::string_utils::split<std::vector<int>>("123\n4567\n890", '\n');
	
	ASSERT_EQ(result.size(), 3);
	auto it = result.begin();
	EXPECT_EQ(*it++, 123);
	EXPECT_EQ(*it++, 4567);
	EXPECT_EQ(*it++, 890);
}

DEFINE_ENUM_STRING(JoinPairTest, ITEM1, ITEM2);
TEST(string_utils, joinPair) {
	ASSERT_EQ(yb::string_utils::joinPair(1, 100, ":"), "1:100");
	ASSERT_EQ(yb::string_utils::joinPair(1.f, 100.f, ":"), "1.000000:100.000000");
	ASSERT_EQ(yb::string_utils::joinPair("1", "100", ":"), "1:100");
	ASSERT_EQ(yb::string_utils::joinPair(std::string("1"), std::string("100"), ":"), "1:100");
	ASSERT_EQ(yb::string_utils::joinPair(JoinPairTest::ITEM1, JoinPairTest::ITEM2, ":"), "ITEM1:ITEM2");
}

TEST(string_utils, joinIntIntVector) {
	std::vector<std::pair<int, int>> inData;
	inData.emplace_back(1, 100);
	inData.emplace_back(2, 200);
	inData.emplace_back(3, 300);

	const auto result = yb::string_utils::join(inData, ":", ";");

	ASSERT_EQ(result, "1:100;2:200;3:300");
}

TEST(string_utils, joinStringIntVector) {
	std::vector<std::pair<std::string, int>> inData;
	inData.emplace_back("1", 100);
	inData.emplace_back("2", 200);
	inData.emplace_back("3", 300);
	
	const auto result = yb::string_utils::join(inData, ":", ";");
	
	ASSERT_EQ(result, "1:100;2:200;3:300");
}

//TEST(string_utils, valToStringItemIdType) {
//	const cp::itemsModule::itemIdType idType{12356756};
//	const std::string result = common::utilsModule::valToString(idType);
//
//	ASSERT_EQ(result, "12356756");
//}
