//
//  tests_date_formatter.cpp
//  cpp_serialized
//
//  Created by Yuri Barmin on 05.12.2023.
//

#include "tests_engine.h"
#include "date_formatter.h"

using namespace yb::date_formatter;

TEST(DateFormatter, dateTimeToStringUTC) {
	const std::tm tm {
		.tm_sec = 7,
		.tm_min = 17,
		.tm_hour = 14,
		.tm_mday = 7,
		.tm_mon = 10,
		.tm_year = 2025 - 1900,
		.tm_wday = 5,
		.tm_yday = 310,
		.tm_isdst = 0,
	};
	
	const auto time_point = std::chrono::system_clock::from_time_t(mktime_utc(tm));
	
	EXPECT_EQ(get_string_from(time_point, "%Y-%m-%d %H:%M:%S"), "2025-11-07 14:17:07");
}

TEST(DateFormatter, dateTimeToStringUTCWithLocale) {
	const std::tm tm {
		.tm_sec = 7,
		.tm_min = 17,
		.tm_hour = 14,
		.tm_mday = 7,
		.tm_mon = 10,
		.tm_year = 2025 - 1900,
		.tm_wday = 5,
		.tm_yday = 310,
		.tm_isdst = 0,
	};
	
	const auto time_point = std::chrono::system_clock::from_time_t(mktime_utc(tm));
	
	EXPECT_EQ(get_string_from(time_point, "%c", "ru_RU"), "пятница,  7 ноября 2025 г. 14:17:07");
}

TEST(DateFormatter, stringToDateTimeUTC) {
	EXPECT_EQ(get_string_from(get_date_from("2025-11-07 14:17:07", "%Y-%m-%d %H:%M:%S"),"%Y-%m-%d %H:%M:%S"), "2025-11-07 14:17:07");
}

