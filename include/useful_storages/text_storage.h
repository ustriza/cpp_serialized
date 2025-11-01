//
//  text_storage.h
//  to
//
//  Created by Yuri Barmin on 09.08.2023.
//

#pragma once

#include <vector>

#include "engine_from_cpp.h"
#include "storage_common.h"

namespace yb::text_from_cpp {

class TextStorage {
public:
	//Interface implementation
	template<OptionsForEngine option>
	static constexpr auto get_options_for_engine(){}

	void interface_init_container(yb::Type type) {
		set_type(type);
		
		if(get_parent_type() != Type::object_container && get_count() > 1) {
			add_comma();
			add_new_line();
		}
		switch (type) {
			case Type::array_container:
				if(get_parent_type() != Type::object_container) {
					add_pad();
				}
				open_square_bracket();
				add_new_line();
				break;
			case Type::object_container:
				if(get_parent_type() != Type::object_container) {
					add_pad();
				}
				open_brace();
				add_new_line();
				break;
			default:
				add_pad();
				break;
		}
		inc_level();
	}
	
	void interface_deinit_container(yb::Type type) {
		if(get_count() > 0) {
			add_new_line();
		}
		dec_level();
		add_pad();
		close_bracket(type);
	}

	template<typename T1>
	void interface_assign_from(const T1& value) {
		switch (get_parent_type()) {
			case Type::array_container:
				if(get_count() > 1) {
					add_comma();
					add_new_line();
				}
				add_pad();

				add_value(value);
				break;
			case Type::object_container:
				if(get_parent_type() != Type::object_container && get_count() > 1) {
					add_comma();
					add_new_line();
				}

				add_value(value);
				break;
			default:
				add_pad();
				add_value(value);
				break;
		}
	}

	
	TextStorage& interface_append_array_item() {
		inc_count();
		return *this;
	}
	
	template<typename TKey>
	TextStorage& interface_append_map_item(const TKey& key) {
		std::string s_key;
		add_value(key, s_key);

		inc_count();
		
		if(get_count() > 1) {
			add_comma();
			add_new_line();
		}
		add_pad();
		m_buffer += s_key;
		m_buffer += ":";
		m_buffer += std::string(m_spaces_after_key, ' ');

		return *this;
	}
	//End if Interface implementation

	//--------------------------------------

	TextStorage() {
		m_states.emplace_back(yb::Type::null_value, 0);
	}

	std::string& get_text() {
		return m_buffer;
	}
private:
	static constexpr int m_spaces_in_level{2};
	static constexpr int m_spaces_after_key{1};

	int m_level{};
	std::string m_buffer;
	std::vector<std::tuple<yb::Type, int>> m_states; //type, count
	
	yb::Type get_parent_type() const {
		return m_states.size() <= 1 ? yb::Type::null_value : std::get<0>(*(m_states.end() - 2));
	}
	void set_type( yb::Type value) {
		std::get<0>(m_states.back()) = value;
	}
	
	static bool get_brace(yb::Type type) {
		return type == Type::object_container;
	}
	
	int get_count() const {
		return std::get<1>(m_states.back());
	}
	void inc_count() {
		std::get<1>(m_states.back())++;
	}

	void inc_level() {
		m_level++;
		m_states.emplace_back();
	}
	void dec_level() {
		m_level--;
		m_states.pop_back();
	}
	void add_pad() {
		m_buffer += std::string(m_level * m_spaces_in_level, ' ');
	}
	void open_square_bracket() { // '['
		m_buffer += "[";
	}
	void open_brace() { // '{'
		m_buffer += "{";
	}
	void close_bracket(yb::Type type) {
		m_buffer += get_brace(type) ? "}" : "]";
	}

	template<typename T1>
	void add_value(const T1& value) {
		add_value(value, m_buffer);
	}
	
	template<typename T1>
	static void add_value(const T1& value, std::string& out_buffer) {
		constexpr bool is_string{yb::string_utils::is_string<T1>()};
		if constexpr(is_string) {
			out_buffer += "\"";
		}

		out_buffer += yb::string_utils::val_to_string(value);

		if constexpr(is_string) {
			out_buffer += "\"";
		}
	}
	void add_comma() {
		m_buffer += ",";
	}
	void add_new_line() {
		m_buffer += "\n";
	}
};
#if __cplusplus >= 202002L
static_assert(Storage_concept_from_cpp<TextStorage>);
#endif
} //end of namespace yb::text_from_cpp

namespace yb::assist {

#if __cplusplus >= 202002L //C++ 20
template<typename T>
std::string to_string_impl(t1_arg_t<T>& from_value) {
    yb::text_from_cpp::TextStorage storage;
    auto inst = yb::from_cpp::cpp_to_storage_instance<T>(from_value, storage);
    inst.write_to();
    return std::move(storage.get_text());
}

template<typename T>
requires has_non_const_begin_v<T>
std::string to_string(T&& from_value) {
    static_assert(!std::is_const_v<std::remove_reference_t<decltype(from_value)>>, "Do not use 'const' for std::views::filter");
    return to_string_impl<T>(from_value);
}

template<typename T>
requires (!has_begin<T> || has_const_begin_v<T>)
std::string to_string(const T& from_value) {
	return to_string_impl<T>(from_value);
}

#else
template<typename T>
std::string to_string(const T& from_value) {
    yb::text_from_cpp::TextStorage storage;
    auto inst = yb::from_cpp::cpp_to_storage_instance<T>(from_value, storage);
    inst.write_to();
    return std::move(storage.get_text());
}
#endif
} // end of yb::assist
