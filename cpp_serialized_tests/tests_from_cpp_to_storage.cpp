//
//  tests_from_cpp_to_storage.cpp
//
//  Created by Yuri Barmin on 08.08.2023.
//

#if __cplusplus >= 202002L //C++ 20
#include <ranges>
#endif

#include "test_storage_from_cpp.h"
#include "engine_from_cpp.h"
#include "access_assist.h"
#include "tests_engine.h"
#include "enum_string.h"


template<typename TValue, typename TMap>
TValue get_array_value(size_t index, const TMap& map) {
	const auto find_it = map.find(yb::tests::MapKey{index});
	if(find_it == map.end()) {
		return TValue{};
	}
	return find_it->second.template asValue<TValue>();
}

template<typename TValue, typename TKey, typename TMap>
TValue get_map_value(const TKey& key, const TMap& map) {
	const auto find_it = map.find(yb::tests::MapKey{key});
	if(find_it == map.end()) {
		return TValue{};
	}
	return find_it->second.template asValue<TValue>();
}

template<typename TMap>
const yb::from_cpp::TestStorage& get_storage_value(size_t index, const TMap& map) {
	static const yb::from_cpp::TestStorage empty_storage;
	const auto find_it = map.find(yb::tests::MapKey{index});
	if(find_it == map.end()) {
		return empty_storage;
	}
	return find_it->second;
}

template<typename TValue, typename TMap>
TValue get_set_value(const TValue& value, const TMap& map) {
	const auto find_it = std::find_if(map.begin(), map.end(), [&value](const auto& item) {
		return item.second.template asValue<TValue>() == value;
	});
	if(find_it == map.end()) {
		return TValue{};
	}
	return find_it->second.template asValue<TValue>();
}

TEST(TestsFromStl, fromInt_Test) {
	const int value{12345};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	ASSERT_EQ(storage.isInt(), true);
	EXPECT_EQ(storage.asValue<int>(), 12345);
}

TEST(TestsFromStl, fromFloatMax_Test) {
	const float value{std::numeric_limits<float>::max()};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	ASSERT_EQ(storage.isFloat(), true);
	EXPECT_EQ(yb::numbers_equal(storage.asValue<float>(), std::numeric_limits<float>::max()), true);
}

TEST(TestsFromStl, fromDoubleMax_Test) {
	const double value{std::numeric_limits<double>::max()};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();
	
	ASSERT_EQ(storage.isFloat(), true);
	EXPECT_EQ(yb::numbers_equal(storage.asValue<double>(), std::numeric_limits<double>::max()), true);
}

TEST(TestsFromStl, fromVectorInt_Test) {
	const std::vector<int> vec{1, 2, 3};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(vec, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isArray(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_array_value<int>(0, storage.get_map()), 1);
	EXPECT_EQ(get_array_value<int>(1, storage.get_map()), 2);
	EXPECT_EQ(get_array_value<int>(2, storage.get_map()), 3);
	EXPECT_EQ(get_array_value<int>(3, storage.get_map()), 0); //value does not exists
}

TEST(TestsFromStl, fromVectorUInt_Test) {
	const std::vector<unsigned int> vec{1, 2, 3};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(vec, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isArray(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_array_value<unsigned int>(0, storage.get_map()), 1u);
	EXPECT_EQ(get_array_value<unsigned int>(1, storage.get_map()), 2u);
	EXPECT_EQ(get_array_value<unsigned int>(2, storage.get_map()), 3u);
	EXPECT_EQ(get_array_value<unsigned int>(3, storage.get_map()), 0u); //value does not exists
}

TEST(TestsFromStl, fromVectorFloat_Test) {
	const std::vector<float> vec{1.f, 2.f, 3.f};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(vec, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isArray(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_array_value<float>(0, storage.get_map()), 1.f);
	EXPECT_EQ(get_array_value<float>(1, storage.get_map()), 2.f);
	EXPECT_EQ(get_array_value<float>(2, storage.get_map()), 3.f);
	EXPECT_EQ(get_array_value<float>(3, storage.get_map()), 0.f); //value does not exists
}

TEST(TestsFromStl, fromVectorString_Test) {
	const std::vector<std::string> vec{"s1", "s2", "s3"};

	yb::from_cpp::TestStorage storage;

	auto inst = yb::from_cpp::cpp_to_storage_instance(vec, storage);
	inst.write_to();

	EXPECT_EQ(storage.isArray(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_array_value<std::string>(0, storage.get_map()), "s1");
	EXPECT_EQ(get_array_value<std::string>(1, storage.get_map()), "s2");
	EXPECT_EQ(get_array_value<std::string>(2, storage.get_map()), "s3");
	EXPECT_EQ(get_array_value<std::string>(3, storage.get_map()), std::string()); //value does not exists
}

TEST(TestsFromStl, fromDequeBool_Test) {
	const std::deque<bool> vec{true, false, true};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(vec, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isArray(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_array_value<bool>(0, storage.get_map()), true);
	EXPECT_EQ(get_array_value<bool>(1, storage.get_map()), false);
	EXPECT_EQ(get_array_value<bool>(2, storage.get_map()), true);
	EXPECT_EQ(get_array_value<bool>(3, storage.get_map()), bool()); //value does not exists
}

TEST(TestsFromStl, fromMapIntInt_Test) {
	const std::map<int, int> map{{1, 10}, {2, 20}, {3, 30}};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(map, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isObject(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_map_value<int>(1, storage.get_map()), 10);
	EXPECT_EQ(get_map_value<int>(2, storage.get_map()), 20);
	EXPECT_EQ(get_map_value<int>(3, storage.get_map()), 30);
	EXPECT_EQ(get_map_value<int>(4, storage.get_map()), 0); //value does not exists
}

TEST(TestsFromStl, fromMapStringString_Test) {
	const std::map<std::string, std::string> map{{"s1", "s10"}, {"s2", "s20"}, {"s3", "s30"}};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(map, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isObject(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_map_value<std::string>("s1", storage.get_map()), "s10");
	EXPECT_EQ(get_map_value<std::string>("s2", storage.get_map()), "s20");
	EXPECT_EQ(get_map_value<std::string>("s3", storage.get_map()), "s30");
	EXPECT_EQ(get_map_value<std::string>("s4", storage.get_map()), ""); //value does not exists
}

TEST(TestsFromStl, fromSetInt_Test) {
	const std::set<int> vec{1, 2, 3};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(vec, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isArray(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_array_value<int>(0, storage.get_map()), 1);
	EXPECT_EQ(get_array_value<int>(1, storage.get_map()), 2);
	EXPECT_EQ(get_array_value<int>(2, storage.get_map()), 3);
	EXPECT_EQ(get_array_value<int>(3, storage.get_map()), 0); //value does not exists
}

TEST(TestsFromStl, fromListInt_Test) {
	const std::set<int> vec{1, 2, 3};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(vec, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isArray(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_array_value<int>(0, storage.get_map()), 1);
	EXPECT_EQ(get_array_value<int>(1, storage.get_map()), 2);
	EXPECT_EQ(get_array_value<int>(2, storage.get_map()), 3);
	EXPECT_EQ(get_array_value<int>(3, storage.get_map()), 0); //value does not exists
}

TEST(TestsFromStl, fromUnorderedSetInt_Test) {
	const std::unordered_set<int> vec{1, 2, 3};
	
	yb::from_cpp::TestStorage storage;
	
	auto inst = yb::from_cpp::cpp_to_storage_instance(vec, storage);
	inst.write_to();
	
	EXPECT_EQ(storage.isArray(), true);
	EXPECT_EQ(storage.get_map().size(), 3);
	EXPECT_EQ(get_set_value<int>(1, storage.get_map()), 1);
	EXPECT_EQ(get_set_value<int>(2, storage.get_map()), 2);
	EXPECT_EQ(get_set_value<int>(3, storage.get_map()), 3);
	EXPECT_EQ(get_set_value<int>(4, storage.get_map()), 0); //value does not exists
}

TEST(TestsFromStl, writeNumbersInt) {
	constexpr std::array values {0, -1, -100, 1, 100, 123456789, -123456789};
	for(const int value: values) {
		yb::from_cpp::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isInt(), true);
		EXPECT_EQ(storage.asInt(), value);
	}
}

TEST(TestsFromStl, writeNumbersFloat) {
	constexpr std::array values {0.f, -1.f, -100.f, 1.f, 100.f, 123456789.f, -123456789.f};
	for(const float value: values) {
		yb::from_cpp::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isFloat(), true);
		EXPECT_EQ(storage.asFloat(), value);
	}
}

TEST(TestsFromStl, writeNumbersBool) {
	constexpr std::array values {true, false};
	for(const bool value: values) {
		yb::from_cpp::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isBool(), true);
		EXPECT_EQ(storage.asBool(), value);
	}
}

TEST(TestsFromStl, writeStrings) {
	constexpr std::array values {"", "str123456", "87879879879osklsnlsdnksjdksdbsdmn,smdns,dbsmndsmndbsmndbsm"};
	for(const std::string value: values) {
		yb::from_cpp::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isString(), true);
		EXPECT_EQ(storage.asString(), value);
	}
}

template<class T>
void toStorageTestArrays(const T& values) {
	for(const auto& value: values) {
		yb::from_cpp::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();
		
		EXPECT_EQ(storage.isArray(), true);
		EXPECT_EQ(storage.size(), value.size());
		for(size_t i{}; i< storage.size(); i++) {
			auto it = value.begin();
			std::advance(it, i);
			const auto& v = (*it);
			const auto resultStorage = storage[i];
			EXPECT_EQ(resultStorage.isNull(), false);
			EXPECT_EQ(resultStorage.isObject(), false);
			EXPECT_EQ(resultStorage.isArray(), false);
			EXPECT_EQ(resultStorage.template asValue<typename T::value_type::value_type>(), v);
		}
	}
}

TEST(TestsFromStl, writeVectorsInt) {
	using usedContainer = std::vector<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeDequesInt) {
	using usedContainer = std::deque<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeListsInt) {
	using usedContainer = std::list<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeSetsInt) {
	using usedContainer = std::set<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeUnorderedSetsInt) {
	using usedContainer = std::unordered_set<int>;
	const std::array values {usedContainer{}, usedContainer{0, 123045, -34567}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeVectorsString) {
	using usedContainer = std::vector<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeDequesString) {
	using usedContainer = std::deque<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeListsString) {
	using usedContainer = std::list<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeSetsString) {
	using usedContainer = std::set<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toStorageTestArrays(values);
}

TEST(TestsFromStl, writeUnorderedSetString) {
	using usedContainer = std::unordered_set<std::string>;
	const std::array values {usedContainer{}, usedContainer{"0", "0123045sd", "3kjkjkjhHH"}};
	toStorageTestArrays(values);
}

template<class T>
void toStorageTestMaps(const T& values) {
	for(const auto& value: values) {
		yb::from_cpp::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		EXPECT_EQ(storage.isObject(), true);
		EXPECT_EQ(storage.size(), value.size());
		for(auto it = begin(value); it != end(value); ++it) {
			const auto& resultStorage = storage[yb::tests::MapKey{it->first}];
			EXPECT_EQ(resultStorage.asValue<typename T::value_type::mapped_type>(), it->second);
		}
	}
}

TEST(TestsFromStl, writeMapsStrInt) {
	using usedContainer = std::map<std::string, int>;
	const std::array values {usedContainer{}, usedContainer{{"01234", 1234}, {"056789",
		56789}}};
	
	toStorageTestMaps(values);
}

TEST(TestsFromStl, writeMapsStrStr) {
	using usedContainer = std::map<std::string, std::string>;
	const std::array values {usedContainer{}, usedContainer{{"01234", "1234sdsds"}, {"056789",
		"asdfdKKl56789"}}};
	
	toStorageTestMaps(values);
}

TEST(TestsFromStl, writeMapsIntInt) {
	using usedContainer = std::map<int, int>;
	const std::array values {usedContainer{}, usedContainer{{1234, 123478}, {56789,
		56789343}}};
	
	toStorageTestMaps(values);
}

TEST(TestsFromStl, writeMapsIntStr) {
	using usedContainer = std::map<int, std::string>;
	const std::array values {usedContainer{}, usedContainer{{1234, "01234"}, {56789, "056789"}}};
	
	toStorageTestMaps(values);
}


TEST(TestsFromStl, writeUnorderedMapsStrInt) {
	using usedContainer = std::unordered_map<std::string, int>;
	const std::array values {usedContainer{}, usedContainer{{"01234", 1234}, {"056789",
		56789}}};
	
	toStorageTestMaps(values);
}

TEST(TestsFromStl, writeUnorderedMapsStrStr) {
	using usedContainer = std::unordered_map<std::string, std::string>;
	const std::array values {usedContainer{}, usedContainer{{"01234", "1234sdsds"}, {"056789",
		"asdfdKKl56789"}}};
	
	toStorageTestMaps(values);
}

TEST(TestsFromStl, writeUnorderedMapsIntInt) {
	using usedContainer = std::unordered_map<int, int>;
	const std::array values {usedContainer{}, usedContainer{{1234, 123478}, {56789,
		56789343}}};
	
	toStorageTestMaps(values);
}

TEST(TestsFromStl, writeUnorderedMapsIntStr) {
	using usedContainer = std::unordered_map<int, std::string>;
	const std::array values {usedContainer{}, usedContainer{{1234, "01234"}, {56789, "056789"}}};
	
	toStorageTestMaps(values);
}

TEST(TestsFromStl, writePairKeyString) {
	std::pair<std::string, int> value {"key", 12345};
	
	yb::from_cpp::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.isArray(), true);
	ASSERT_EQ(storage.size(), 2);
	EXPECT_EQ(storage[0u].asString(), "key");
	EXPECT_EQ(storage[1u].asInt(), 12345);
}

TEST(TestsFromStl, writePairKeyInt) {
	std::pair<int, int> value {98765, 12345};
	
	yb::from_cpp::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.isArray(), true);
	ASSERT_EQ(storage.size(), 2);
	EXPECT_EQ(storage[0u].asInt(), 98765);
	EXPECT_EQ(storage[1u].asInt(), 12345);
}

TEST(TestsFromStl, writeTupleStringInt) {
	std::tuple<std::string, int> value {"key", 12345};
	
	yb::from_cpp::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.isArray(), true);
	ASSERT_EQ(storage.size(), 2);
	EXPECT_EQ(storage[0u].asString(), "key");
	EXPECT_EQ(storage[1u].asInt(), 12345);
}

TEST(TestsFromStlUtils, writeTimePoint) {
	const time_t data {1234567890L};
	std::chrono::time_point<std::chrono::system_clock> value = std::chrono::system_clock::from_time_t(data);

	yb::from_cpp::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	ASSERT_EQ(storage.isString(), true);
	ASSERT_EQ(storage.asString(), std::to_string(data));
}

DEFINE_ENUM_STRING(myWriteEnum, ME_1, ME_2)

TEST(TestsFromStl, writeEnum) {
	const myWriteEnum value = myWriteEnum::ME_2;
	
	yb::from_cpp::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.isString(), true);
	EXPECT_EQ(storage.asString(), "ME_2");
}

TEST(TestsFromStl, writeEnumFail) {
	const myWriteEnum value = myWriteEnum(123456);
	
	yb::from_cpp::TestStorage storage;
	auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
	inst.write_to();

	EXPECT_EQ(storage.isString(), true);
	EXPECT_EQ(storage.asString(), "");
}

#if __cplusplus >= 202002L //C++ 20
TEST(TestsFromStl, to_string_RangeViewNotContainer) {
	
	{
		const auto value = std::ranges::single_view{1};
		
		yb::from_cpp::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value, storage);
		inst.write_to();

		EXPECT_EQ(storage.isArray(), true);
		EXPECT_EQ(storage.get_map().size(), 1);
		EXPECT_EQ(get_array_value<int>(0, storage.get_map()), 1);
	}
}

TEST(TestsFromStl, to_string_RangeViewMapIntString) {
	const std::map<int, std::string> value{{1, "value1"}, {2, "value2"}};
	
	{
		
		yb::from_cpp::TestStorage storage;
		auto inst = yb::from_cpp::cpp_to_storage_instance(value | std::ranges::views::all, storage);
		inst.write_to();

		EXPECT_EQ(storage.isObject(), true);
		EXPECT_EQ(storage.get_map().size(), 2);
		EXPECT_EQ(get_map_value<std::string>(1, storage.get_map()), "value1");
		EXPECT_EQ(get_map_value<std::string>(2, storage.get_map()), "value2");
	}

	{
		
		yb::from_cpp::TestStorage storage;

		yb::assist::serialize(storage, value | std::ranges::views::filter([](const auto& item){return item.first > 1;}));
		
		EXPECT_EQ(storage.isObject(), true);
		EXPECT_EQ(storage.get_map().size(), 1);
		EXPECT_EQ(get_map_value<std::string>(2, storage.get_map()), "value2");
	}

	{
		
		yb::from_cpp::TestStorage storage;
		
		yb::assist::serialize(storage, value
		   | std::ranges::views::filter([](const auto& item){return item.first > 1;})
		   | std::ranges::views::transform([](const auto& item){return std::pair{"k: " + std::to_string(item.first), item.second};}));
		
		EXPECT_EQ(storage.isObject(), true);
		EXPECT_EQ(storage.get_map().size(), 1);
		EXPECT_EQ(get_map_value<std::string>("k: 2", storage.get_map()), "value2");
	}

	{
		yb::from_cpp::TestStorage storage;
		
		yb::assist::serialize(storage, value
		   | std::ranges::views::filter([](const auto& item){return item.first > 1;})
		   | std::ranges::views::transform([](const auto& item){return item.second;})
		);
		
		ASSERT_EQ(storage.isArray(), true);
		ASSERT_EQ(storage.get_map().size(), 1);
		EXPECT_EQ(get_array_value<std::string>(0, storage.get_map()), "value2");
	}
	
	{
		yb::from_cpp::TestStorage storage;
		
		yb::assist::serialize(storage, value
							  | std::ranges::views::filter([](const auto& item){return item.first > 1;})
							  | std::ranges::views::transform([](const auto& item){return std::tuple{"k: " + std::to_string(item.first), item.second};})
							  );
		
		ASSERT_EQ(storage.isArray(), true);
		ASSERT_EQ(storage.get_map().size(), 1);

		const auto& tuple_storage = get_storage_value(0, storage.get_map());
		
		const auto result_key = get_array_value<std::string>(0, tuple_storage.get_map());
		const auto result_value = get_array_value<std::string>(1, tuple_storage.get_map());

		EXPECT_EQ(std::make_tuple(result_key, result_value), std::make_tuple("k: 2", "value2"));
	}

}

#endif
