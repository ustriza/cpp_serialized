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

namespace yb::to_cpp {
template<Storage_concept_to_cpp Storage>
bool meta_table_to_cpp(std::chrono::time_point<std::chrono::system_clock> &value, const Storage& cur_node) {
	if(cur_node.interface_get_type() != Type::string_value) {
		return false;
	}

	const auto sval = cur_node.template interface_get_value<std::string>();
	const auto ttval = yb::string_utils::string_to_val<time_t>(sval);
	value = std::chrono::system_clock::from_time_t(ttval);
	
	return true;

}
}

namespace yb::from_cpp {
template<Storage_concept_from_cpp Storage>
void meta_table_from_cpp(const std::chrono::time_point<std::chrono::system_clock> &value, Storage& cur_node) {
	const auto tvalue = std::chrono::system_clock::to_time_t(value);
	const auto svalue = yb::string_utils::val_to_string(tvalue);
	cur_node.interface_assign_from(svalue);
}
	
}
