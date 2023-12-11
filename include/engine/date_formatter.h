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
	static std::string compile_format(std::string_view format) {
		std::string ret_value;

		for(auto it = format.begin(); it != format.end(); ++it) {
			const auto ch{*it};
			const char smb_count = [&]() mutable -> char {
				const auto find_end_smb_seq_it = std::find_if(it + 1, format.end(), [ch](auto arg){
					return ch != arg;
				});
				const auto new_it = find_end_smb_seq_it - 1;
				const char distance{static_cast<char>(new_it - it + 1 + '0')};
				it = new_it;
				
				return distance;
			}();

			ret_value.append({ch, smb_count});
		}
		
		return ret_value;
	}
	
	std::string m_compiled_format;
	std::optional<long> m_timezone;
};


} //end of namespace yb::date_formatter
