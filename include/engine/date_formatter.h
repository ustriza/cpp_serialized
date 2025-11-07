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


//static std::tm timePointToTmUTC(const std::chrono::system_clock::time_point& tp) {
//	std::time_t time = std::chrono::system_clock::to_time_t(tp);
//	std::tm utc_tm = *std::gmtime(&time);
//	return utc_tm;
//}
//
//static std::tm timePointToTmLocal(const std::chrono::system_clock::time_point& tp) {
//	std::time_t time = std::chrono::system_clock::to_time_t(tp);
//	std::tm utc_tm = *std::localtime(&time);
//	return utc_tm;
//}
//
//std::string to_utc_string(const std::chrono::system_clock::time_point& tp) {
//	const auto utc_tm = timePointToTmLocal(tp);
//	
//	std::stringstream ss;
//	
//	<< std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
//
//}


} //end of namespace yb::date_formatter
