//
//  enum_string.h
//
//  Created by Yuri Barmin on 19.06.2021.
//

#pragma once

#include <optional>

#include "string_utils.h"

// Make a FOREACH macro
#define GET_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,NAME,...) NAME
#define FE_0(WHAT)
#define FE_1(WHAT, X) WHAT(X)
#define FE_2(WHAT, X, ...) WHAT(X)FE_1(WHAT, __VA_ARGS__)
#define FE_3(WHAT, X, ...) WHAT(X)FE_2(WHAT, __VA_ARGS__)
#define FE_4(WHAT, X, ...) WHAT(X)FE_3(WHAT, __VA_ARGS__)
#define FE_5(WHAT, X, ...) WHAT(X)FE_4(WHAT, __VA_ARGS__)
#define FE_6(WHAT, X, ...) WHAT(X)FE_5(WHAT, __VA_ARGS__)
#define FE_7(WHAT, X, ...) WHAT(X)FE_6(WHAT, __VA_ARGS__)
#define FE_8(WHAT, X, ...) WHAT(X)FE_7(WHAT, __VA_ARGS__)
#define FE_9(WHAT, X, ...) WHAT(X)FE_8(WHAT, __VA_ARGS__)
#define FE_10(WHAT, X, ...) WHAT(X)FE_9(WHAT, __VA_ARGS__)
#define FE_11(WHAT, X, ...) WHAT(X)FE_10(WHAT, __VA_ARGS__)
#define FE_12(WHAT, X, ...) WHAT(X)FE_11(WHAT, __VA_ARGS__)
#define FE_13(WHAT, X, ...) WHAT(X)FE_12(WHAT, __VA_ARGS__)
#define FE_14(WHAT, X, ...) WHAT(X)FE_13(WHAT, __VA_ARGS__)
#define FE_15(WHAT, X, ...) WHAT(X)FE_14(WHAT, __VA_ARGS__)
#define FE_16(WHAT, X, ...) WHAT(X)FE_15(WHAT, __VA_ARGS__)
#define FE_17(WHAT, X, ...) WHAT(X)FE_16(WHAT, __VA_ARGS__)
#define FE_18(WHAT, X, ...) WHAT(X)FE_17(WHAT, __VA_ARGS__)
#define FE_19(WHAT, X, ...) WHAT(X)FE_18(WHAT, __VA_ARGS__)
#define FE_20(WHAT, X, ...) WHAT(X)FE_19(WHAT, __VA_ARGS__)
#define FE_21(WHAT, X, ...) WHAT(X)FE_20(WHAT, __VA_ARGS__)
#define FE_22(WHAT, X, ...) WHAT(X)FE_21(WHAT, __VA_ARGS__)
#define FE_23(WHAT, X, ...) WHAT(X)FE_22(WHAT, __VA_ARGS__)
#define FE_24(WHAT, X, ...) WHAT(X)FE_23(WHAT, __VA_ARGS__)
#define FE_25(WHAT, X, ...) WHAT(X)FE_24(WHAT, __VA_ARGS__)
#define FE_26(WHAT, X, ...) WHAT(X)FE_25(WHAT, __VA_ARGS__)
#define FE_27(WHAT, X, ...) WHAT(X)FE_26(WHAT, __VA_ARGS__)
#define FE_28(WHAT, X, ...) WHAT(X)FE_27(WHAT, __VA_ARGS__)
#define FE_29(WHAT, X, ...) WHAT(X)FE_28(WHAT, __VA_ARGS__)
#define FE_30(WHAT, X, ...) WHAT(X)FE_29(WHAT, __VA_ARGS__)
#define FE_31(WHAT, X, ...) WHAT(X)FE_30(WHAT, __VA_ARGS__)
#define FE_32(WHAT, X, ...) WHAT(X)FE_31(WHAT, __VA_ARGS__)
#define FE_33(WHAT, X, ...) WHAT(X)FE_32(WHAT, __VA_ARGS__)
#define FE_34(WHAT, X, ...) WHAT(X)FE_33(WHAT, __VA_ARGS__)
#define FE_35(WHAT, X, ...) WHAT(X)FE_34(WHAT, __VA_ARGS__)
#define FE_36(WHAT, X, ...) WHAT(X)FE_35(WHAT, __VA_ARGS__)
#define FE_37(WHAT, X, ...) WHAT(X)FE_36(WHAT, __VA_ARGS__)
#define FE_38(WHAT, X, ...) WHAT(X)FE_37(WHAT, __VA_ARGS__)
#define FE_39(WHAT, X, ...) WHAT(X)FE_38(WHAT, __VA_ARGS__)
#define FE_40(WHAT, X, ...) WHAT(X)FE_39(WHAT, __VA_ARGS__)
#define FE_41(WHAT, X, ...) WHAT(X)FE_40(WHAT, __VA_ARGS__)
#define FE_42(WHAT, X, ...) WHAT(X)FE_41(WHAT, __VA_ARGS__)
#define FE_43(WHAT, X, ...) WHAT(X)FE_42(WHAT, __VA_ARGS__)
#define FE_44(WHAT, X, ...) WHAT(X)FE_43(WHAT, __VA_ARGS__)
#define FE_45(WHAT, X, ...) WHAT(X)FE_44(WHAT, __VA_ARGS__)
#define FE_46(WHAT, X, ...) WHAT(X)FE_45(WHAT, __VA_ARGS__)
#define FE_47(WHAT, X, ...) WHAT(X)FE_46(WHAT, __VA_ARGS__)
#define FE_48(WHAT, X, ...) WHAT(X)FE_47(WHAT, __VA_ARGS__)
#define FE_49(WHAT, X, ...) WHAT(X)FE_48(WHAT, __VA_ARGS__)
#define FE_50(WHAT, X, ...) WHAT(X)FE_49(WHAT, __VA_ARGS__)

#define FOR_EACH(action,...) \
GET_MACRO(_0,__VA_ARGS__,FE_50,FE_49,FE_48,FE_47,FE_46,FE_45,FE_44,FE_43,FE_42,FE_41,FE_40,FE_39,FE_38,FE_37,FE_36,FE_35,FE_34,FE_33,FE_32,FE_31,FE_30,FE_29,FE_28,FE_27,FE_26,FE_25,FE_24,FE_23,FE_22,FE_21,FE_20,FE_19,FE_18,FE_17,FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10,FE_9,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1,FE_0)(action,__VA_ARGS__)


#define ARG_FE_0(ARG, WHAT)
#define ARG_FE_1(ARG, WHAT, X) WHAT(ARG, X)
#define ARG_FE_2(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_1(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_3(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_2(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_4(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_3(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_5(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_4(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_6(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_5(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_7(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_6(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_8(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_7(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_9(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_8(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_10(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_9(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_11(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_10(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_12(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_11(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_13(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_12(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_14(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_13(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_15(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_14(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_16(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_15(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_17(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_16(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_18(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_17(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_19(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_18(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_20(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_19(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_21(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_20(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_22(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_21(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_23(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_22(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_24(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_23(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_25(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_24(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_26(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_25(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_27(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_26(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_28(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_27(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_29(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_28(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_30(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_29(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_31(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_30(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_32(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_31(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_33(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_32(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_34(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_33(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_35(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_34(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_36(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_35(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_37(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_36(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_38(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_37(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_39(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_38(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_40(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_39(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_41(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_40(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_42(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_41(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_43(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_42(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_44(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_43(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_45(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_44(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_46(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_45(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_47(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_46(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_48(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_47(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_49(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_48(ARG, WHAT, __VA_ARGS__)
#define ARG_FE_50(ARG, WHAT, X, ...) WHAT(ARG, X)ARG_FE_49(ARG, WHAT, __VA_ARGS__)


#define ARG_FOR_EACH(ARG, action,...) \
GET_MACRO(_0,__VA_ARGS__,ARG_FE_50,ARG_FE_49,ARG_FE_48,ARG_FE_47,ARG_FE_46,ARG_FE_45,ARG_FE_44,ARG_FE_43,ARG_FE_42,ARG_FE_41,ARG_FE_40,ARG_FE_39,ARG_FE_38,ARG_FE_37,ARG_FE_36,ARG_FE_35,ARG_FE_34,ARG_FE_33,ARG_FE_32,ARG_FE_31,ARG_FE_30,ARG_FE_29,ARG_FE_28,ARG_FE_27,ARG_FE_26,ARG_FE_25,ARG_FE_24,ARG_FE_23,ARG_FE_22,ARG_FE_21,ARG_FE_20,ARG_FE_19,ARG_FE_18,ARG_FE_17,ARG_FE_16,ARG_FE_15,ARG_FE_14,ARG_FE_13,ARG_FE_12,ARG_FE_11,ARG_FE_10,ARG_FE_9,ARG_FE_8,ARG_FE_7,ARG_FE_6,ARG_FE_5,ARG_FE_4,ARG_FE_3,ARG_FE_2,ARG_FE_1,ARG_FE_0)(ARG, action,__VA_ARGS__)

#define COMMA_X_FE(X)  X,
#define PREF_COMMA_X_FE(ENUM_TO_STRING_PREFIX, X)  ENUM_TO_STRING_PREFIX::X,
#define CASE_X_FE(ENUM_TO_STRING_PREFIX, X)  case ENUM_TO_STRING_PREFIX::X: return #X;

#define STATIC_X_IC_FE(ENUM_TO_STRING_PREFIX, X)  static constexpr auto __ ## ENUM_TO_STRING_PREFIX ## _ ## X = yb::string_utils::staticLowerString(#X);

#define CASE_X_IC_FE(ENUM_TO_STRING_PREFIX, X)  case ENUM_TO_STRING_PREFIX::X: return __ ## ENUM_TO_STRING_PREFIX ## _ ## X.data();


#define IF_X_FE(ENUM_TO_STRING_PREFIX, X)  if(yb::string_utils::static_compare_strings_ic(s, #X)) {return ENUM_TO_STRING_PREFIX::X;}

#define DEFINE_ENUM_STRING(name, ...)                                                 \
enum class name {                                                                     \
	FOR_EACH(COMMA_X_FE,__VA_ARGS__)                                                  \
};                                                                                    \
constexpr std::string_view yb_enum_to_string(name n) {                                         \
	switch(n) {                                                                       \
		ARG_FOR_EACH(name, CASE_X_FE,__VA_ARGS__)                                     \
	}                                                                                 \
	return {};                                                                        \
}                                                                                     \
constexpr std::optional<name> yb_enum_from_string(std::string_view s, name) { \
	ARG_FOR_EACH(name, IF_X_FE,__VA_ARGS__)                                           \
	return {};                                                                        \
}


#define DEFINE_ENUM_STRING_LC(name, ...)                                              \
enum class name {                                                                     \
	FOR_EACH(COMMA_X_FE,__VA_ARGS__)                                                  \
};                                                                                    \
ARG_FOR_EACH(name, STATIC_X_IC_FE,__VA_ARGS__)                                        \
constexpr std::string_view yb_enum_to_string(name n) {                                                 \
	switch(n) {                                                                       \
		ARG_FOR_EACH(name, CASE_X_IC_FE,__VA_ARGS__)                                  \
	}                                                                                 \
	return {};                                                                        \
}                                                                                     \
constexpr std::optional<name> yb_enum_from_string(std::string_view s, name) { \
	ARG_FOR_EACH(name, IF_X_FE,__VA_ARGS__)                                           \
	return {};                                                                        \
}

#define DEFINE_ENUM_STRING_OF(name, type, ...)                                        \
enum class name : type {                                                              \
	FOR_EACH(COMMA_X_FE,__VA_ARGS__)                                                  \
};                                                                                    \
constexpr std::string_view yb_enum_to_string(name n) {                                         \
	switch(n) {                                                                       \
		ARG_FOR_EACH(name, CASE_X_FE,__VA_ARGS__)                                     \
	}                                                                                 \
	return {};                                                                        \
}                                                                                     \
constexpr std::optional<name> yb_enum_from_string(std::string_view s, name) { \
	ARG_FOR_EACH(name, IF_X_FE,__VA_ARGS__)                                           \
	return {};                                                                        \
}                                                                                     \
const std::vector<name> name##List {                                                  \
    ARG_FOR_EACH(name, PREF_COMMA_X_FE, __VA_ARGS__)                                  \
};

//-------------------------------------------------------------------------------------

#define DEFINE_STRING_CONV_FOR_ENUM_LC(name, ...)                                     \
ARG_FOR_EACH(name, STATIC_X_IC_FE,__VA_ARGS__)                                        \
constexpr std::string_view yb_enum_to_string(name n) {                                         \
	switch(n) {                                                                       \
		ARG_FOR_EACH(name, CASE_X_IC_FE,__VA_ARGS__)                                  \
	}                                                                                 \
	return {};                                                                        \
}                                                                                     \
constexpr std::optional<name> yb_enum_from_string(std::string_view s, name) { \
	ARG_FOR_EACH(name, IF_X_FE,__VA_ARGS__)                                           \
	return {};                                                                        \
}


#define DEFINE_STRING_CONV_FOR_ENUM(name, ...)                                        \
constexpr std::string_view yb_enum_to_string(name n) {                                         \
	switch(n) {                                                                       \
		ARG_FOR_EACH(name, CASE_X_FE,__VA_ARGS__)                                     \
	}                                                                                 \
	return {};                                                                        \
}                                                                                     \
constexpr std::optional<name> yb_enum_from_string(std::string_view s, name) { \
	ARG_FOR_EACH(name, IF_X_FE,__VA_ARGS__)                                           \
	return {};                                                                        \
}
