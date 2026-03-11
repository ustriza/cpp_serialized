//
//  engine_types.h
//  cpp_serialized
//
//  Created by Yuri Barmin on 09.03.2026.
//

#pragma once

#include <cstdint>

namespace yb::types {

using int32_t = ::int32_t;
using int64_t = ::int64_t;

using uint32_t = ::uint32_t;
using uint64_t = ::uint64_t;

using float_t = float;
using double_t = double;

template<typename T>
constexpr inline bool is_supported_int_type() {
	return std::is_same_v<T, int32_t> || std::is_same_v<T, int64_t>
		|| std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>;
}

template<typename T>
constexpr inline bool is_supported_float_type() {
	return std::is_same_v<T, float_t> || std::is_same_v<T, double_t>;
}

template<typename T>
constexpr inline bool is_supported_type() {
	return is_supported_int_type<T>() || is_supported_float_type<T>()
		|| std::is_enum_v<T>|| std::is_same_v<T, std::string>;
}



}
