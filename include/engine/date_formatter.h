//
//  date_formatter.h
//  cpp_serialized
//
//  Created by Yuri Barmin on 04.12.2023.
//

#pragma once

#include <string>
#include <chrono>
#include <string>
#include <optional>
#include <cstdint>
#include <sstream>
#include <ctime>
#include <iomanip>

//std::string test_compile_format(std::string_view);


namespace yb::date_formatter {

std::time_t mktime_utc(const std::tm& tm) {
	std::tm tm_orig = tm;
	
	std::time_t local_time = std::mktime(&tm_orig);
	if (local_time == -1) return -1;
	
	std::tm tm_utc = *std::gmtime(&local_time);
	const std::time_t utc_time = std::mktime(&tm_utc);
	
	const std::time_t timezone_offset = local_time - utc_time;
	
	return local_time + timezone_offset;
}

using date_time_t = std::chrono::system_clock::time_point;

std::string get_string_from(const date_time_t& date_time, const std::string& format, const std::string& locale = {})
{
	const std::time_t time = std::chrono::system_clock::to_time_t(date_time);
	const std::tm utc_tm = *std::gmtime(&time);
	
	std::stringstream ss;
	
	if (!locale.empty()) {
		ss.imbue(std::locale(locale));
	}
	ss << std::put_time(&utc_tm, format.c_str());
	
	return ss.str();
}

date_time_t get_date_from(const std::string& str_date_time, const std::string& format, const std::string& locale = {}) {

	std::istringstream ss(str_date_time);
	
	if (!locale.empty()) {
		ss.imbue(std::locale(locale));
	}

	std::tm tm;
	ss >> std::get_time(&tm, format.c_str());
	
	const date_time_t ret_value = std::chrono::system_clock::from_time_t(mktime_utc(tm));

	return ret_value;
}

} //end of namespace yb::date_formatter
