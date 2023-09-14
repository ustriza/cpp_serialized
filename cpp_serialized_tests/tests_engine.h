//
//  tests_engine.h
//
//  Created by Yuri Barmin on 14.08.2023.
//

#pragma once

#include <iostream>
#include <vector>

#if __cplusplus >= 202002L
#include <ranges>
#endif

#include <memory>
#include <algorithm>

#define EXPECT_EQ(A, B) if((A) != (B)) {Tests_engine::get_instance()->fire_error();}
#define ASSERT_EQ(A, B) if((A) != (B)) {Tests_engine::get_instance()->fire_error(); return;}

using Test_func_ptr = void (*)();
class Tests_engine {
	static inline std::unique_ptr<Tests_engine> m_instance;
public:
	static Tests_engine* get_instance() {
		if(!m_instance) {
			m_instance = std::make_unique<Tests_engine>();
		}
		return m_instance.get();
	}
	
	
	int add_test(const std::string& name, Test_func_ptr func) {
		m_test_funcs.emplace_back(name, func);
		return {};
	}
	
	auto get_test_funcs(const std::string& filter) {
#if __cplusplus >= 202002L
		auto funcs_out = m_test_funcs | std::ranges::views::filter([&filter](const auto& item) -> bool {
			return filter.empty() || item.first.find(filter) != std::string::npos;
		});
#else
		decltype(m_test_funcs) funcs_out{};
		std::copy_if(m_test_funcs.begin(), m_test_funcs.end(), std::back_inserter(funcs_out), [&filter](const auto& item) -> bool {
			return filter.empty() || item.first.find(filter) != std::string::npos;
		});
		
#endif
		return funcs_out;
	}
	
	void fire_error() {
		m_error = true;
	}
	bool get_error_and_clear() {
		const auto ret = m_error;
		m_error = false;
		return ret;
	}
private:
	std::vector<std::pair<std::string, Test_func_ptr>> m_test_funcs;
	bool m_error{};
};


#define TEST(TCLASS, TMETHOD) \
void TCLASS ##_## TMETHOD();   \
const int __ ## TCLASS ##_## TMETHOD = Tests_engine::get_instance()->add_test(#TCLASS "_" #TMETHOD, &TCLASS ##_## TMETHOD); \
void TCLASS ##_## TMETHOD()


class Tests_execute {
public:
	Tests_execute(std::string_view filter = {}): m_filter(std::string(filter)){}
	void run() {
		auto test_funcs = Tests_engine::get_instance()->get_test_funcs(m_filter);

		std::cout << "Run " << std::count_if(test_funcs.begin(), test_funcs.end(), [](auto){return true;}) <<" tests." ;
		if(!m_filter.empty()) {
			std::cout << " Filter set to '" << m_filter << "'.";
		}
		std::cout << std::endl;

		const auto get_class = [](std::string_view name){
			const auto pos = name.find("_");
			return name.substr(0, pos);
		};
		const auto get_name = [](std::string_view name){
			const auto pos = name.find("_");
			return name.substr(pos + 1);
		};

		int success_count{}, failed_count{};
		int total_success_count{}, total_failed_count{};
		std::vector<std::string> failed_tests;
		std::string_view last_name;
		bool was{};
		for(const auto& [full_name, func]: test_funcs) {
			const auto s_class = get_class(full_name);
			const auto s_name = get_name(full_name);
			const bool new_name{last_name != s_class};
			last_name = s_class;
			if(new_name) {
				if(was) {
					std::cout <<"Success tests: " << success_count << ", failed tests: " << failed_count << std::endl;

					total_success_count += success_count;
					total_failed_count += failed_count;
					success_count = failed_count = 0;
				}
				was = true;
				
				std::cout << std::endl;
				std::cout << "Test class: " << s_class << std::endl;

			}
			std::cout << "\t" << s_name;
			func();
			const bool error = Tests_engine::get_instance()->get_error_and_clear();
			if(!error) {
				success_count++;
				std::cout << "\tSUCCESS" << std::endl;
			}
			else {
				failed_count++;
				failed_tests.push_back(std::string{ s_class } + ": " + std::string{ s_name });
				std::cout << "\tFAIL" << std::endl;
			}
		}
		std::cout <<"Success tests: " << success_count << ", failed tests: " << failed_count << std::endl;
		total_success_count += success_count;
		total_failed_count += failed_count;

		std::cout << std::endl;
		std::cout <<"TOTAL: Success tests: " << total_success_count << ", failed tests: " << total_failed_count << std::endl;
		if (!failed_tests.empty()) {
			std::cout << "Failed tests:" << std::endl;
			for (const auto& item : failed_tests) {
				std::cout << "\t" << item << std::endl;
			}
		}
		std::cout << std::endl;
	}
private:
	const std::string m_filter;
};
