//
//  tests_enum_str.cpp
//
//  Created by Yuri Barmin on 14.05.2021.
//


#include "tests_engine.h"
#include "enum_string.h"

namespace testNS {
DEFINE_ENUM_STRING(myTestEnum, ME_1, ME_2)
DEFINE_ENUM_STRING_LC(myTestEnumIC, ME_1, ME_2)
}

TEST(EnumStr, yb_enum_to_string) {
	constexpr auto resStr1 = yb_enum_to_string(testNS::myTestEnum::ME_1);
	static_assert(!resStr1.empty(), "");

	constexpr auto resStr2 = yb_enum_to_string(testNS::myTestEnum::ME_2);
	static_assert(!resStr2.empty(), "");

	constexpr auto resStr3 = yb_enum_to_string(testNS::myTestEnum(1234567));
	static_assert(resStr3.empty(), "");

	EXPECT_EQ(resStr1, "ME_1");
	EXPECT_EQ(resStr2, "ME_2");
	EXPECT_EQ(resStr3, "");
}

TEST(EnumStr, yb_enum_from_string) {
	constexpr auto resEnum1 = yb_enum_from_string("ME_1", testNS::myTestEnum());
	static_assert(resEnum1 && resEnum1.value() == testNS::myTestEnum::ME_1);
	
	constexpr auto resEnum2 = yb_enum_from_string("ME_2", testNS::myTestEnum());
	static_assert(resEnum2.has_value() && resEnum2.value() == testNS::myTestEnum::ME_2);

	constexpr auto resEnum3 = yb_enum_from_string("ME_76565656", testNS::myTestEnum());
	static_assert(!resEnum3);

	ASSERT_EQ(resEnum1.has_value(), true);
	ASSERT_EQ(resEnum1.value(), testNS::myTestEnum::ME_1);
	
	ASSERT_EQ(resEnum2.has_value(), true);
	ASSERT_EQ(resEnum2.value(), testNS::myTestEnum::ME_2);
	
	ASSERT_EQ(resEnum3.has_value(), false);
}

TEST(EnumStr, yb_enum_from_stringLC) {
	constexpr auto resEnum1 = yb_enum_from_string("Me_1", testNS::myTestEnumIC());
	static_assert(resEnum1 && resEnum1.value() == testNS::myTestEnumIC::ME_1);
	
	constexpr auto resEnum2 = yb_enum_from_string("mE_2", testNS::myTestEnumIC());
	static_assert(resEnum2 && resEnum2.value() == testNS::myTestEnumIC::ME_2);
	
	constexpr auto resEnum3 = yb_enum_from_string("me_2", testNS::myTestEnumIC::ME_2);
	static_assert(resEnum3);
	
	ASSERT_EQ(resEnum1.has_value(), true);
	ASSERT_EQ(resEnum1.value(), testNS::myTestEnumIC::ME_1);
	
	ASSERT_EQ(resEnum2.has_value(), true);
	ASSERT_EQ(resEnum2.value(), testNS::myTestEnumIC::ME_2);
	
	ASSERT_EQ(resEnum3.has_value(), true);
	ASSERT_EQ(resEnum3.value(), testNS::myTestEnumIC::ME_2);
}

TEST(EnumStr, valToString) {
	const auto resStr1 = yb::string_utils::val_to_string(testNS::myTestEnum::ME_1);
	
	const auto resStr2 = yb::string_utils::val_to_string(testNS::myTestEnum::ME_2);
	
	const auto resStr3 = yb::string_utils::val_to_string(testNS::myTestEnum(1234567));
	
	EXPECT_EQ(resStr1, "ME_1");
	EXPECT_EQ(resStr2, "ME_2");
	EXPECT_EQ(resStr3, "");
}

TEST(EnumStr, stringToVal) {
	const auto resEnum1 = yb::string_utils::string_to_val<testNS::myTestEnum>("ME_1");
	const auto resEnum2 = yb::string_utils::string_to_val<testNS::myTestEnum>("ME_2");
	const auto resEnum3 = yb::string_utils::string_to_val<testNS::myTestEnum>("ME_76565656");
	
	EXPECT_EQ(resEnum1, testNS::myTestEnum::ME_1);
	EXPECT_EQ(resEnum2, testNS::myTestEnum::ME_2);
	EXPECT_EQ(resEnum3, testNS::myTestEnum(-1));
}

TEST(EnumStr, stringToValLC) {
	const auto resEnum1 = yb::string_utils::string_to_val<testNS::myTestEnum>("Me_1");
	const auto resEnum2 = yb::string_utils::string_to_val<testNS::myTestEnum>("mE_2");
	const auto resEnum3 = yb::string_utils::string_to_val<testNS::myTestEnum>("me_2");
	
	EXPECT_EQ(resEnum1, testNS::myTestEnum::ME_1);
	EXPECT_EQ(resEnum2, testNS::myTestEnum::ME_2);
	EXPECT_EQ(resEnum3, testNS::myTestEnum::ME_2);
}

TEST(EnumStr, toStringLC) {
	constexpr auto resStr1 = yb_enum_to_string(testNS::myTestEnumIC::ME_1);
	static_assert(!resStr1.empty(), "");
	
	constexpr auto resStr2 = yb_enum_to_string(testNS::myTestEnumIC::ME_2);
	static_assert(!resStr2.empty(), "");
	
	constexpr auto resStr3 = yb_enum_to_string(testNS::myTestEnumIC(1234567));
	static_assert(resStr3.empty(), "");

	EXPECT_EQ(resStr1, "me_1");
	EXPECT_EQ(resStr2, "me_2");
	EXPECT_EQ(resStr3, "");
}

namespace testNS {
	enum class MyStrConvEnumLC {ME_1, ME_2};
	DEFINE_STRING_CONV_FOR_ENUM_LC(MyStrConvEnumLC, ME_1, ME_2)

	enum class MyStrConvEnum {ME_1, ME_2};
	DEFINE_STRING_CONV_FOR_ENUM(MyStrConvEnum, ME_1, ME_2)
}

TEST(EnumStr, toStringConv) {
	constexpr auto resStr1 = yb_enum_to_string(testNS::MyStrConvEnum::ME_1);
	static_assert(!resStr1.empty(), "");
	
	constexpr auto resStr2 = yb_enum_to_string(testNS::MyStrConvEnum::ME_2);
	static_assert(!resStr2.empty(), "");
	
	constexpr auto resStr3 = yb_enum_to_string(testNS::MyStrConvEnum(1234567));
	static_assert(resStr3.empty(), "");
	
	EXPECT_EQ(resStr1, "ME_1");
	EXPECT_EQ(resStr2, "ME_2");
	EXPECT_EQ(resStr3, "");
}

TEST(EnumStr, yb_enum_from_stringConv) {
	constexpr auto resEnum1 = yb_enum_from_string("Me_1", testNS::MyStrConvEnum());
	static_assert(resEnum1 && resEnum1.value() == testNS::MyStrConvEnum::ME_1);
	
	constexpr auto resEnum2 = yb_enum_from_string("mE_2", testNS::MyStrConvEnum());
	static_assert(resEnum2 && resEnum2.value() == testNS::MyStrConvEnum::ME_2);
	
	constexpr auto resEnum3 = yb_enum_from_string("me_2", testNS::MyStrConvEnum::ME_2);
	static_assert(resEnum3);
	
	ASSERT_EQ(resEnum1.has_value(), true);
	ASSERT_EQ(resEnum1.value(), testNS::MyStrConvEnum::ME_1);
	
	ASSERT_EQ(resEnum2.has_value(), true);
	ASSERT_EQ(resEnum2.value(), testNS::MyStrConvEnum::ME_2);
	
	ASSERT_EQ(resEnum3.has_value(), true);
	ASSERT_EQ(resEnum3.value(), testNS::MyStrConvEnum::ME_2);
}

TEST(EnumStr, toStringConvLC) {
	constexpr auto resStr1 = yb_enum_to_string(testNS::MyStrConvEnumLC::ME_1);
	static_assert(!resStr1.empty(), "");
	
	constexpr auto resStr2 = yb_enum_to_string(testNS::MyStrConvEnumLC::ME_2);
	static_assert(!resStr2.empty(), "");
	
	constexpr auto resStr3 = yb_enum_to_string(testNS::MyStrConvEnumLC(1234567));
	static_assert(resStr3.empty(), "");
	
	EXPECT_EQ(resStr1, "me_1");
	EXPECT_EQ(resStr2, "me_2");
	EXPECT_EQ(resStr3, "");
}

TEST(EnumStr, yb_enum_from_stringConvLC) {
	constexpr auto resEnum1 = yb_enum_from_string("Me_1", testNS::MyStrConvEnumLC());
	static_assert(resEnum1 && resEnum1.value() == testNS::MyStrConvEnumLC::ME_1);

	constexpr auto resEnum2 = yb_enum_from_string("mE_2", testNS::MyStrConvEnumLC());
	static_assert(resEnum2 && resEnum2.value() == testNS::MyStrConvEnumLC::ME_2);
	
	constexpr auto resEnum3 = yb_enum_from_string("me_2", testNS::MyStrConvEnumLC::ME_2);
	static_assert(resEnum3);
	
	ASSERT_EQ(resEnum1.has_value(), true);
	ASSERT_EQ(resEnum1.value(), testNS::MyStrConvEnumLC::ME_1);
	
	ASSERT_EQ(resEnum2.has_value(), true);
	ASSERT_EQ(resEnum2.value(), testNS::MyStrConvEnumLC::ME_2);
	
	ASSERT_EQ(resEnum3.has_value(), true);
	ASSERT_EQ(resEnum3.value(), testNS::MyStrConvEnumLC::ME_2);
}
