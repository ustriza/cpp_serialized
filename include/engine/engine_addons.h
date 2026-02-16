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

}

namespace yb::from_cpp {
template<Storage_concept_from_cpp Storage>
void meta_table_from_cpp(const std::chrono::system_clock::time_point &value, Storage& cur_node) {
	const std::string& date_format = cur_node.interface_get_date_format();
	if (date_format.empty()) {
		const auto tvalue = std::chrono::system_clock::to_time_t(value);
		const auto svalue = yb::string_utils::val_to_string(tvalue);
		
		cur_node.interface_assign_from(svalue);
	}
	else {
		const std::string svalue = yb::date_formatter::get_string_from(value, date_format);
		cur_node.interface_assign_from(svalue);
	}
}
	
}
