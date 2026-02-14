//
//  engine_addons.h
//
//  Created by Yuri Barmin on 01.08.2021.
//

#pragma once

#include <chrono>
#include <time.h>

#include "engine_common.h"
#include "string_utils.h"
#include "date_formatter.h"

namespace yb::to_cpp {

template<Storage_concept_to_cpp Storage>
bool meta_table_to_cpp(std::chrono::system_clock::time_point &value, const Storage& cur_node) {
	if(cur_node.interface_get_type() != Type::string_value) {
		return false;
	}

	const auto sval = cur_node.template interface_get_value<std::string>();

	const std::string& date_format = cur_node.interface_get_date_format();
	if (date_format.empty()) {
		const auto ttval = yb::string_utils::string_to_val<time_t>(sval);
		value = std::chrono::system_clock::from_time_t(ttval);
	}
	else {
		const std::chrono::system_clock::time_point time_point = yb::date_formatter::get_date_from(sval, date_format);
		
		static_assert(sizeof(value) >= sizeof(time_point));
		value = time_point;
	}
	
	return true;

}
}

namespace yb::from_cpp {
template<Storage_concept_from_cpp Storage>
void meta_table_from_cpp(const std::chrono::system_clock::time_point &value, Storage& cur_node) {
	const auto tvalue = std::chrono::system_clock::to_time_t(value);
	const auto svalue = yb::string_utils::val_to_string(tvalue);
	cur_node.interface_assign_from(svalue);
}
	
}
