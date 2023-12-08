//
//  tests_date_formatter.cpp
//  cpp_serialized
//
//  Created by Yuri Barmin on 05.12.2023.
//

#include "tests_engine.h"
#include "date_formatter.h"

using namespace yb::date_formatter;

std::string test_compile_format(std::string_view format) {
	return Date_formatter<std::chrono::steady_clock>::compile_format(format);
}

TEST(DateFormatter, compile_format_failed) {
	EXPECT_EQ(test_compile_format(""), "");
	EXPECT_EQ(test_compile_format("y."), "");
	EXPECT_EQ(test_compile_format("Q."), "");
	EXPECT_EQ(test_compile_format("."), "");
}

TEST(DateFormatter, compile_format_y1) {
	EXPECT_EQ(test_compile_format("y"), "y1");
}

TEST(DateFormatter, compile_format_y2) {
	EXPECT_EQ(test_compile_format("yy"), "y2");
}

TEST(DateFormatter, compile_format_y4) {
	EXPECT_EQ(test_compile_format("yyyy"), "y4");
}

TEST(DateFormatter, compile_format_y_m_d) {
	EXPECT_EQ(test_compile_format("y.m.d"), "y1m1d1");
}

TEST(DateFormatter, compile_format_yyyy_mm_dd) {
	EXPECT_EQ(test_compile_format("yyyy.mm.dd"), "y4m2d2");
}

TEST(DateFormatter, compile_format_y1_h1) {
	EXPECT_EQ(test_compile_format("y h"), "y1h1");
}
TEST(DateFormatter, compile_format_y1__h1) {
	EXPECT_EQ(test_compile_format("y  h"), "y1h1");
}

//TEST(DateFormatter, compile_format_MM_dd_yyyy_HH_mm) {
//	EXPECT_EQ(test_compile_format("MM-dd-yyyy HH:mm"), "M2d2y4 H2:m2");
//}

