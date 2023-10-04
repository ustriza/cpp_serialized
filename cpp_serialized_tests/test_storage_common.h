//
//  test_storage_common.h
//
//  Created by Yuri Barmin on 09.08.2023.
//

#pragma once
#include <variant>

namespace yb::tests {

class MapKey {
public:
	using Value_type = std::variant<size_t, int, std::string>;
	
	MapKey(std::string_view key): m_value(std::string(key)){}
//	MapKey(const std::string& key): m_value(key){}
	MapKey(size_t key): m_value(key){}
	MapKey(int key): m_value(key){}
	
	template<typename T1>
	auto& operator = (const T1& other) {
		m_value = other;
		return *this;
	}
	
	const Value_type& deserialize() const {
		return m_value;
	}
private:
	Value_type m_value;
	
};
} // end of namespace yb::to

namespace std {
template<> struct less<yb::tests::MapKey> {
	bool operator() (const yb::tests::MapKey& lhs, const yb::tests::MapKey& rhs) const
	{
		if(std::holds_alternative<size_t>(lhs.deserialize())) {
			return std::get<size_t>(lhs.deserialize()) < std::get<size_t>(rhs.deserialize());
		}
		else if(std::holds_alternative<int>(lhs.deserialize())) {
			return std::get<int>(lhs.deserialize()) < std::get<int>(rhs.deserialize());
		}
		else {
			return std::less<std::string>{}(std::get<std::string>(lhs.deserialize()), std::get<std::string>(rhs.deserialize()));
		}
	}
};
} // end of namespace std

