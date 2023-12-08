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

std::string test_compile_format(std::string_view);

namespace yb::date_formatter {

template<typename TClock>
class Date_formatter final {
	friend std::string (::test_compile_format(std::string_view));
public:
	using Time_point = std::chrono::time_point<TClock>;
	
	Date_formatter() = default;
	Date_formatter(std::string_view format): m_compiled_format{compile_format(format)}{}
	
	void set_timezone(long value) {
		m_timezone = value;
	}
	long get_timezone() const {
		return m_timezone;
	}
	
	void set_date_format(std::string_view format) {
		m_compiled_format = compile_format(format);
	}
//	const std::string& get_date_format() {
//		return m_format;
//	}

	Time_point from(std::string_view value) {
		
	}
private:
	static bool is_allowed_format_symbol(char ch) {
		return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
	}
	static bool is_allowed_delim_symbol(char ch) {
		return ch == '.' || ch == '/' || ch == ' ';
	}
	
	static std::string compile_format(std::string_view format) {
		std::string ret_value;

		enum class Find_symbol{FORMAT, BOTH};
		Find_symbol find_format{Find_symbol::FORMAT};

		enum class Found_symbol {NONE, FORMAT, DELIM, WRONG};
		Found_symbol found_symbol{Found_symbol::NONE};
		
		std::string::value_type prev_char{};
		
		for(auto it = format.begin(); it != format.end(); ++it) {
			const bool is_last_ch{it + 1 == format.end()};
			const auto ch{*it};
			if(ch == ' ') { //skip the sequence of the space smb
				const auto find_non_blank_it = std::find_if(it + 1, format.end(), [ch](auto arg){
					return ch != arg;
				});
				it = find_non_blank_it - 1;
			}
			
			if(is_allowed_format_symbol(ch)) {
				found_symbol = Found_symbol::FORMAT;
			}
			else if(is_allowed_delim_symbol(ch)) {
				found_symbol = Found_symbol::DELIM;
			}
			else {
				found_symbol = Found_symbol::WRONG;
			}
			
			if(found_symbol == Found_symbol::WRONG
			   || (found_symbol == Found_symbol::DELIM && find_format != Find_symbol::BOTH)
			   || (found_symbol == Found_symbol::DELIM && is_last_ch)) {
				ret_value.clear();
				return ret_value;
			}
			
			if(found_symbol == Found_symbol::DELIM) {
				find_format = Find_symbol::FORMAT;
				prev_char = {};
				continue;
			}
			find_format = Find_symbol::BOTH;
			
			if(prev_char != ch) {
				ret_value.append({ch, '0'});
			}
			prev_char = ch;
			
			const auto it_count = ret_value.end() - 1;
			(*it_count)++;
		}
		
		return ret_value;
	}
	
	std::string m_compiled_format;
	std::optional<long> m_timezone;
};


} //end of namespace yb::date_formatter
