#pragma once

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <charconv>
#include <array>
#include <set>
#include <unordered_set>

#include "engine_types.h"

namespace yb::string_utils {

template<typename T>
using is_char_array = std::conjunction<
	std::is_array<T>,
	std::is_same<std::remove_cv_t<std::remove_extent_t<T>>, char>
>;

template <typename T, typename std::enable_if_t<yb::types::is_supported_int_type<T>(), int>* = nullptr>
inline T string_to_val(const char* s) {
	T value;
	auto [ptr, ec] = std::from_chars(s, s + strlen(s), value);
	if (ec == std::errc()) {
		return value;
	}
	return 0;
}

template <typename T, typename std::enable_if_t<std::is_enum_v<T>, int>* = nullptr>
inline T string_to_val(const char* s) {
	const auto result = yb_enum_from_string(s, T());
	if(result.has_value()) {
		return result.value();
	}
	return T(-1);
}

template <typename T, typename std::enable_if_t<yb::types::is_supported_float_type<T>(), int>* = nullptr>
inline T string_to_val(const char* s) {
	if (*s == '\0') {
		return 0.0;
	}

	char* end{};
	T result;
	if constexpr(std::is_same_v<T, yb::types::float_t>) {
		result = std::strtof(s, &end);
	}
	else {
		result = std::strtod(s, &end);
	}
	if (errno == ERANGE) {
		return 0.0;
	}
	
	return result;
}

template <typename T, typename std::enable_if_t<std::is_same_v<T, bool>, int>* = nullptr>
inline bool string_to_val(const char* s) {
	return std::string_view{s} == "true" || string_to_val<yb::types::int32_t>(s) != 0;
}

template <typename T, typename std::enable_if_t<std::is_same_v<T, std::string>, int>* = nullptr>
inline std::string string_to_val(const char* s) {
	return std::string{s};
}

template <typename T, typename std::enable_if_t<yb::types::is_supported_type<T>(), int>* = nullptr>
inline auto string_to_val(const std::string& s) -> std::conditional_t<std::is_same_v<T, std::string>, const std::string&, T> {
	if constexpr(std::is_same_v<T, std::string>) {
		return s;
	}
	else {
		return string_to_val<T>(s.c_str());
	}
}

//------------------------------------
template <typename T>
inline auto val_to_string(const T& value) -> std::conditional_t<std::is_same_v<T, std::string>, const std::string&, std::string> {
	if constexpr(std::is_same_v<T, std::string>) {
		return value;
	}
	else if constexpr(std::is_same_v<T, std::string_view>) {
		return std::string(value);
	}
	else if constexpr(std::is_same_v<char, std::remove_cv_t<std::remove_pointer_t<T>>> || std::is_same_v<unsigned char, std::remove_cv_t<std::remove_pointer_t<T>>>) {
		return std::string(value);
	}
	else if constexpr(yb::string_utils::is_char_array<T>::value) {
		return std::string(value);
	}
	else if constexpr(std::is_same_v<T, bool>) {
		return value ? "true" : "false";
	}
	else if constexpr(std::is_enum_v<T>) {
		const auto result = yb_enum_to_string(value);
		return std::string(result);
	}
	else if constexpr(std::is_integral_v<T>) {
//		return fmt::format_int(value).str();
		return std::to_string(value);
	}
	else {
		return std::to_string(value);
	}
}

/// split source string and put result to a container
template<typename T, typename TDelim>
void split(std::string_view source, const TDelim& delim, T& result) {
	if(source.empty()) {
		return;
	}
	
	constexpr bool isDelimChar = std::is_same_v<TDelim, char> || std::is_same_v<TDelim, unsigned char>;
	size_t delimLen{ 1u };
	if constexpr(!isDelimChar) {
		delimLen = std::string_view(delim).length();
	}
	
	if constexpr (std::is_same_v<T, std::vector<typename T::value_type>> && isDelimChar) {
		const auto itemsCount = std::count(source.begin(), source.end(), delim) + 1;
		result.reserve(itemsCount);
	}

	bool lastElem{};
	while(!lastElem) {
		const auto foundDelimPos = source.find(delim);
		lastElem = (foundDelimPos == decltype(source)::npos);
		std::string_view str;
		if(lastElem) {
			if(source.empty()) {
				break;
			}
			str = source;
		}
		else {
			str = source.substr(0, foundDelimPos);
			source = source.substr(foundDelimPos + delimLen);
		}
		
		auto val = string_to_val<typename T::value_type>(std::string(str));
		if constexpr (std::is_same_v<T, std::set<typename T::value_type>> || std::is_same_v<T, std::unordered_set<typename T::value_type>>) {
			result.emplace(std::move(val));
		}
		else {
			result.emplace_back(std::move(val));
		}
	}
}

template<typename T, typename TDelim>
T split(const std::string& source, const TDelim& delim) {
	T result;
	split(source, delim, result);
	return result;
}

template<typename T1, typename T2>
std::string joinPair(
					 const T1& first,
					 const T2& second,
					 std::string_view delim) {
	std::string result = yb::string_utils::val_to_string(first);
	result += std::string(delim);
	result += yb::string_utils::val_to_string(second);
	return result;
}

template<typename T>
std::string join(const T& cont, std::string_view delim1, std::string_view delim2) {
	std::string result;
	if(cont.empty()) {
		return result;
	}
	
	for(const auto& item: cont) {
		const auto& first = std::get<0>(item);
		const auto& second = std::get<1>(item);
		
		const auto strValue = joinPair(first, second, delim1);
		if(!result.empty()) {
			result += delim2;
		}
		result += strValue;;
	}
	return result;
}

inline std::string floatToString(float val, int precision = 2) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << val;
	return stream.str();
}

inline std::string wrapWithQuotes(const std::string& str) {
	return "\"" + str + "\"";
}

inline constexpr char staticToLower(const char c) {
	return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

static constexpr bool static_compare_strings_ic(const std::string_view& str1, const std::string_view& str2) {
	if(str1.size() != str2.size()) {
		return false;
	}
	for(auto it1 = str1.begin(), it2 = str2.begin(); it1 != str1.end(); ++it1, ++it2) {
		const char ch1 = *it1;
		const char ch2 = *it2;
		if(ch1 == ch2) {
			continue;
		}
		if(staticToLower(ch1) != staticToLower(ch2)) {
			return false;
		}
	}
	return true;
}

template<size_t Size>
using Static_string = std::array<const char, Size>;

template<size_t Size, size_t ... Indexes>
static constexpr Static_string<sizeof ... (Indexes) + 1> staticLowerString(const char (& str)[Size],
																				 std::index_sequence<Indexes ...>) {
	return {staticToLower(str[Indexes]) ..., '\0'};
}

template<size_t Size>
static constexpr Static_string<Size> staticLowerString(const char (& str)[Size]) {
	return staticLowerString(str, std::make_index_sequence<Size - 1>{});
}

template<typename T>
constexpr bool is_string() {
	return
	   std::is_same_v<T, std::string>
	|| std::is_same_v<T, std::string_view>
	|| std::is_same_v<char, std::remove_cv_t<std::remove_pointer_t<T>>>
	|| std::is_same_v<unsigned char, std::remove_cv_t<std::remove_pointer_t<T>>>
	|| (std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, char>);
	
}

} // end of namespace yb::string_utils
