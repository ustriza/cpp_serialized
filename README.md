# Author **Yuri Barmin**, October 3, 2023

## Automatic serialization and deserialization of arbitrary C++ types.

## Version 2.1.


**cpp_serialized** can automatically serialize and deserialize C++ types to/from arbitrary storage. You can use existing repositories or create support for your own repositories. **cpp_serialized** comes with the following storage support:

1. TextStorage - serialization into text.
2. Json_storage_adapter_from_cpp - serialization to Json (via a third-party jsoncpp parser).
3. Json_storage_adapter_to_cpp - deserialization from Json (via a third-party jsoncpp parser).

Types of supported data.
1. int, unsigned int, long, unsigned long, float, double, bool
2. std::string
3. std::pair, std::tuple
4. Own struct/class. To support serialization and deserialization, it is necessary to declare the fields of a structure/class [in a special way](#struct).
5. enum class - when using [enum string](#enum_string).
6. Third-party structures and classes. To support serialization and deserialization, it is necessary to define [read and write functions](#third_party_struct) for these structures and classes.
7. std::vector, std::list, std::deque, std::set, std::unordered_set.
8. std::map, std::unordered_map.

**cpp_serialized** will allow you to serialize and deserialize complex data structures and containers in one line. For example:

```cpp
//declare data
const std::vector<std::map<int, std::string>> value{{{12345, "string_test0"}}, {{54321, "string_test1"}}};
Json::Value json;

//cpp_serialized
yb::assist::set_value(yb::jsoncpp::Json_storage_adapter_from_cpp{json}, value);
```

The binary value of the following Json text will be written to the json variable:

```
[
    {
       "12345" : "string_test0",
       "98765" : "string_test1"
    },
    {
       "1234321" : "string_test12",
       "54321" : "string_test11"
    }
]
```

**All examples from this article are located in the file cpp_serialized_tests/examples_README.cpp**

## How to use
1. cpp_serialized is a header library. To add cpp_serialized to your project, you need to copy the include folder.

2. Add the following line to the source.

```cpp
#include "cpp_serialized.h"
```

5. Prepare a storage object into which serialization will take place or from which deserialization will be carried out. In the examples we will use two test stores:

* yb::from_cpp::TestStorage - for serialization.
* yb::to_cpp::TestStorage - for deserialization.

You can also use storage adapters for Json (third party jsoncpp parser, see classes 'Json_storage_adapter_to_cpp' and 'Json_storage_adapter_from_cpp' in include/useful_storages/jsoncpp_storage.h) and export to text (supplied with cpp_serialized, TextStorage class in include/useful_storages/text_storage .h)

4. Call cpp_serialized one of the functions:

* yb::assist::serialize - serialization
* yb::assist::deserialize_to and yb::assist::deserialize (deserialization).

The examples are located in the file 'cpp_serialized_tests/examples_README.cpp'.

### yb::assist::serialize - serialization
Declaration:
```cpp
template <Storage_concept_from_cpp Storage, typename T>
void serialize(Storage&& storage, const T& from_value);
```

This function has two arguments.
1. storage - storage object into which serialization will be performed.
2. from_value - C++ source object.

**Example 1:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third-party jsoncpp parser
#include "jsoncpp_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>

bool set_value_example() {
//Declare data for serialization
const std::vector<std::map<int, std::string>> value{{{12345, "string_test0"}, {98765, "string_test1"}}, {{54321, "string_test11"}, {1234321 , "string_test12"}}};

//Declare json object - third-party jsoncpp parser.
Json::Value json;

//Serialize using cpp_serialized from 'value' to 'json'
yb::assist::serialize(yb::jsoncpp::Json_storage_adapter_from_cpp{json}, value);

//Check serialized data
if(!json.isArray()) {
return false;
}
if(json.size() != 2) {
return false;
}

if(json[0u]["12345"].asString() != "string_test0") {
return false;
}
if(json[0u]["98765"].asString() != "string_test1") {
return false;
}

if(json[1u]["54321"].asString() != "string_test11") {
return false;
}
if(json[1u]["1234321"].asString() != "string_test12") {
return false;
}

return true;
}
```

**Example 2:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third party jsoncpp parser
#include "jsoncpp_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>

//You can serialize/deserialize your own structs and classes.
struct Struct_example {
DEFINE_DATA(int_prop, int)
DEFINE_DATA(string_prop, std::string)
DEFINE_DATA(vector_int_prop, std::vector<int>)

BEGIN_META_TABLE
DEFINE_META_TABLE_ITEM(int_prop)
DEFINE_META_TABLE_ITEM(string_prop)
DEFINE_META_TABLE_ITEM(vector_int_prop)
END_META_TABLE
};

bool serialize_example_2() {
//Declare data for serialization
Struct_example value;
value.int_prop = 12345;
value.string_prop = "string_12345";
value.vector_int_prop = std::vector{12345, 56789};

//Declare json object - third-party jsoncpp parser.
Json::Value json;

//Serialize using cpp_serialized from 'value' to 'json'
yb::assist::serialize(yb::jsoncpp::Json_storage_adapter_from_cpp{json}, value);

//Check serialized data
if(!json.isObject()) {
return false;
}
if(json["int_prop"].asInt() != 12345) {
return false;
}
if(json["string_prop"].asString() != "string_12345") {
return false;
}

const auto& vector_object = json["vector_int_prop"];
if(!vector_object.isArray()) {
return false;
}
if(vector_object[0u] != 12345) {
return false;
}
if(vector_object[1u] != 56789) {
return false;
}

return true;

}
```

### yb::assist::deserialize_to - deserialization.
Declaration:
```cpp
template <typename T, Storage_concept_to_cpp Storage>
bool deserialize_to(const Storage& storage, T& to_value)
```

This function has two arguments.
1. storage - storage object from which deserialization will be performed.
2. to_value - C++ receiver object.

Return value:
* true if the data was read correctly;
* false if an error occurred during deserialization. In this case, the state of the to_value object is undefined.

**Example:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third-party jsoncpp parser
#include "jsoncpp_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>

bool deserialize_to() {
//Source Json text
const auto srcJson =
  R"([
  "key", 1234
  ])";

//Declare json object - third-party jsoncpp parser.
Json::Reader reader;
Json::Value json;
[[maybe_unused]] const auto parse_json_result = reader.parse(srcJson, json);
if(!parse_json_result) {
return false;
}

//Declare data for deserialization. cpp_serialized suupports std::pair, std::tuple class templates.
std::pair<std::string, int> value;

//Deserialize from 'json' to 'value' using cpp_serialized
const auto deserialize_status = yb::assist::deserialize_to(yb::jsoncpp::Json_storage_adapter_to_cpp(json), value);

//Check deserialize status
if(!deserialize_status) {
return false;
}

//Check deserialized data
const std::pair<std::string, int> testValue{"key", 1234};
if(value != testValue) {
return false;
}

return true;
}
```

### yb::assist::deserialize (1) - deserialization.
Declaration:
```cpp
template <typename T, Storage_concept_to_cpp Storage>
std::optional<T> get_value(const Storage& storage);
```

This function has one argument.
1. storage - storage object from which deserialization will be performed.

The return value is std::optional<T>, where T is the type of the variable to be deserialized into.

**Example:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third-party jsoncpp parser
#include "jsoncpp_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>

bool deserialize() {
//Source Json text
const auto srcJson =
  R"([
  "key", 1234, true
  ])";

//Declare json object - third-party jsoncpp parser.
Json::Reader reader;
Json::Value json;
[[maybe_unused]] const auto parse_json_result = reader.parse(srcJson, json);
if(!parse_json_result) {
return false;
}

//Deserialize from 'json' to 'value' using cpp_serialized. cpp_serialized suupports std::pair, std::tuple class templates.
const auto value = yb::assist::deserialize<std::tuple<std::string, int, bool>>(yb::jsoncpp::Json_storage_adapter_to_cpp(json));

//Check deserialize status
if(!value.has_value()) {
return false;
}

//Check deserialized data
const std::tuple<std::string, int, bool> testValue{"key", 1234, true};
if(value.value() != testValue) {
return false;
}

return true;
}
```

### yb::assist::deserialize (2) - deserialization.
Declaration:
```cpp
template <typename T, Storage_concept_to_cpp Storage>
T deserialize(const Storage& storage, const T& def);

template <typename T, Storage_concept_to_cpp Storage>
T deserialize(const Storage& storage, T&& def);
```
This function has two arguments.
1. storage - storage object from which deserialization will be performed.
2. def - the default value that will be returned if deserialization fails.

The return value is the object into which the deserialization was performed. If deserialization is unsuccessful, the value of the def argument will be returned.

**Example:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third-party jsoncpp parser
#include "jsoncpp_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>

bool deserialize_def() {
using Value_type = std::tuple<std::string, int, std::vector<int>>;

//Source Json text. The second element("1234") has wrong type string instead of integer. So this json will no be deserialized to the Value_type
const auto srcJson =
  R"([
  "key", "1234", [56789, 98765]
  ])";

//Declare json object - third-party jsoncpp parser.
Json::Reader reader;
Json::Value json;
[[maybe_unused]] const auto parse_json_result = reader.parse(srcJson, json);
if(!parse_json_result) {
return false;
}

const Value_type default_value{"key1", 1, {1234, 4321}};

//Deserialize from 'json' to 'value' using cpp_serialized. cpp_serialized suupports std::pair, std::tuple class templates.
const auto value = yb::assist::deserialize<Value_type>(yb::jsoncpp::Json_storage_adapter_to_cpp(json), default_value);

//Check deserialized data
if(value != default_value) {
return false;
}

return true;
}
```

## How to use serialization to text.

Heading:

```cpp
#include "text_storage.h"
```

Storage:
```cpp
yb::text_from_cpp::TextStorage
```

The yb::text_from_cpp::TextStorage store has one method to obtain the serialization result:
Declaration:
```cpp
std::string& get_text();
```
Return value: text, the result of data serialization.


**Example:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage for serialization to text.
#include "text_storage.h"

struct Struct_example {
DEFINE_DATA(int_prop, int)
DEFINE_DATA(string_prop, std::string)
DEFINE_DATA(vector_int_prop, std::vector<int>)

BEGIN_META_TABLE
DEFINE_META_TABLE_ITEM(int_prop)
DEFINE_META_TABLE_ITEM(string_prop)
DEFINE_META_TABLE_ITEM(vector_int_prop)
END_META_TABLE
};

struct Struct_serialize_to_text {
DEFINE_DATA(int_prop, int)
using Tuple_type = std::tuple<int, std::string>;
DEFINE_DATA(tuple_prop, Tuple_type)
DEFINE_DATA(struct_example_prop, Struct_example)

BEGIN_META_TABLE
DEFINE_META_TABLE_ITEM(int_prop)
DEFINE_META_TABLE_ITEM(tuple_prop)
DEFINE_META_TABLE_ITEM(struct_example_prop)
END_META_TABLE
};

void serialize_to_text() {
//Declare data for serilization
const Struct_serialize_to_text value{
987654,
std::tuple{56789, "string_tuple_elm"},
Struct_example{12345, "string_12345", std::vector{12345, 56789}}};

//Declare storage
yb::text_from_cpp::TextStorage storage;


//Serialize from 'value' to text using cpp_serialized
yb::assist::serialize(storage, value);

//Output serialized value
std::cout << std::endl;
std::cout << storage.get_text();
std::cout << std::endl;
}
```

**Output:**
```
{
   "int_prop": 987654,
   "tuple_prop": [
     56789,
     "string_tuple_elm"
   ],
   "struct_example_prop": {
     "int_prop": 12345,
     "string_prop": "string_12345",
     "vector_int_prop": [
       12345,
       56789
     ]
   }
}```

Another possibility to serialize data to text is to use the yb::assist::to_string function, which itself will call the yb::assist::serialize serialization function.

Declaration:
```cpp
template<typename T>
std::string to_string(const T& from_value);
```
This function has one argument.
1. from_value - C++ source object.

The return value is text, the result of serializing the from_value object.

**Example:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage for serialization to text.
#include "text_storage.h"

void serialize_to_string() {
//Declare data for serilization
const auto value = std::tuple{12345, std::string{"string_element"}, 123.5f, true};

//Serialize from 'value' to text using cpp_serialized
const auto serialized_text = yb::assist::to_string(value);

//Output serialized value
std::cout << std::endl;
std::cout << serialized_text;
std::cout << std::endl;
}
```

**Output:**
```
[
   12345,
   "string_element",
   123.500000,
   true
]
```

## How to create structures for serialization and deserialization <a name="struct"></a>

For structs and classes to support automatic serialization and deserialization, you must describe the fields in a special way, and then describe the meta table that is used to access those fields. The size of such a structure will be equal to the size of the structure with the fields described in the usual way.

```cpp
struct Struct_example_2 {
DEFINE_DATA(int_prop, int)
DEFINE_DATA(string_prop, std::string)
DEFINE_DATA(vector_int_prop, std::vector<int>)

BEGIN_META_TABLE
DEFINE_META_TABLE_ITEM(int_prop)
DEFINE_META_TABLE_ITEM_EMPTY(string_prop)
DEFINE_META_TABLE_ITEM_DEFAULT(vector_int_prop, (std::vector{1000, 2000}))
END_META_TABLE
};
```

### The DEFINE_DATA macro defines a structure field:
```cpp
DEFINE_DATA(name, type)
```


The DEFINE_DATA macro has two arguments.
1. name - name of the structure/crass field.
2. type - type of structure/class field.

**Example:**

```cpp
DEFINE_DATA(int_prop, int)
```

After processing by the preprocessor, the following code will be generated:
```cpp
  public:
  int int_prop{};
  void set_int_prop(const int& __value__) {
  int_prop = __value__;
}
constexpr const auto& get_int_prop() const noexcept {
//Here a code for some compile-time checks

return int_prop;
}
```
Those. using cpp_serialized does not make the structures any heavier than usual.

If a type contains a comma ',', such as std::map<int, int>, then you must first create an alias for this type, and then use this alias in DEFINE_DATA. Otherwise there will be a syntax error.

DEFINE_DATA adds three elements to the structure:
1. Name data field (int_prop in the example).
2. get_##name getter (get_int_prop in the example).
3. Setter set_##name (set_int_prop in the example).

Thus, the field defined by the DEFINE_DATA macro is readable and writable. If you need to define a read-only field, use [DEFINE_DESER_GETTER](#DEFINE_DESER_GETTER)

**Example:**
```cpp
using Map = std::map<std::string, int>;
DEFINE_DATA(map_prop, Map)
```

### Macro BEGIN_META_TABLE
Begins table definitions for accessing structure/class fields. The table is defined as static constexpr std::tpule.

**Example:**
```cpp
BEGIN_META_TABLE
```

###Macro DEFINE_META_TABLE_ITEM adds an element to the meta table

```cpp
DEFINE_META_TABLE_ITEM(name)
```

The DEFINE_META_TABLE_ITEM macro has one argument.

1. name - name of the structure/crass field. For every DEFINE_META_TABLE_ITEM macro with a given name, there must be a DEFINE_DATA macro with the same name.

An entry with the key name MUST be present in the repository. If a record with such a key is not found, serialization will fail.

**Example:**
```cpp
DEFINE_META_TABLE_ITEM(int_prop)
```

### Macro DEFINE_META_TABLE_ITEM_EMPTY adds an element to the meta table

```cpp
DEFINE_META_TABLE_ITEM_EMPTY(name)
```

The DEFINE_META_TABLE_ITEM_EMPTY macro has one argument.

1. name - name of the structure/crass field. For every DEFINE_META_TABLE_ITEM_EMPTY macro with a given name, there must be a DEFINE_DATA macro with the same name.

An entry with the key name may not be in the repository. If a record with such a key is not found, an empty value will be used.

**Example:**
```cpp
DEFINE_META_TABLE_ITEM_EMPTY(string_prop)
```

### Macro DEFINE_META_TABLE_ITEM_DEFAULT adds an element to the meta table

```cpp
DEFINE_META_TABLE_ITEM_DEFAULT(name, def)
```

The DEFINE_META_TABLE_ITEM_DEFAULT macro has two arguments.

1. name - name of the structure/crass field. For every DEFINE_META_TABLE_ITEM_DEFAULT macro with a specific name, there must be a DEFINE_DATA macro with the same name.
2. def - default value.

An entry with the key name may not be in the repository. If an entry with such a key is not found, the def value will be used.

**Example:**
```cpp
DEFINE_META_TABLE_ITEM_DEFAULT(vector_prop, (std::vector{1000, 2000}))
```

### DEFINE_DESER_GETTER<a name="DEFINE_DESER_GETTER"></a>

DEFINE_DESER_GETTER adds three elements to the structure:
1. Name data field (int_prop in the example).
2. get_##name getter (get_int_prop in the example).

The structure/class field is defined as private, so there is no way to change it. This ensures that the data read from the storage cannot be changed by the programmer. Only read via getter is available.

**Example:**
```cpp
DEFINE_DESER_GETTER(int_prop, int)
```

After processing by the preprocessor, the following code will be generated:
```cpp
private:
int int_prop{};
public:
const auto& get_int_prop() const noexcept {
//Here a code for some compile-time checks

return int_prop;}
public:
```

**Full example of deserialization from Json**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third-party jsoncpp parser
#include "jsoncpp_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>


struct Struct_deser_example {
DEFINE_DESER_GETTER(int_prop, int)
DEFINE_DESER_GETTER(string_prop, std::string)
DEFINE_DESER_GETTER(vector_int_prop, std::vector<int>)

BEGIN_META_TABLE
DEFINE_META_TABLE_ITEM(int_prop)
DEFINE_META_TABLE_ITEM_EMPTY(string_prop)
DEFINE_META_TABLE_ITEM_DEFAULT(vector_int_prop, (std::vector{1000, 2000}))
END_META_TABLE
};

bool deserialize_deser_to() {
//Source Json text
const auto srcJson =
  R"({
   "int_prop": 12345,
   "string_prop": "string_12345",
   "vector_int_prop": [
12345,
56789
   ]
})";

//Declare json object - third-party jsoncpp parser.
Json::Reader reader;
Json::Value json;
[[maybe_unused]] const auto parse_json_result = reader.parse(srcJson, json);
if(!parse_json_result) {
return false;
}

//Declare data for deserialization. cpp_serialized suupports std::pair, std::tuple class templates.
Struct_deser_example value;

//Deserialize from 'json' to 'value' using cpp_serialized
const auto deserialize_status = yb::assist::deserialize_to(yb::jsoncpp::Json_storage_adapter_to_cpp(json), value);

//Check deserialize status
if(!deserialize_status) {
return false;
}

//Check deserialized data
if(value.get_int_prop() != 12345) {
return false;
}
if(value.get_string_prop() != "string_12345") {
return false;
}
if(value.get_vector_int_prop() != std::vector{12345, 56789}) {
return false;
}

return true;
}
```
## Convert enum class to text and back.<a name="enum_string"></a>
Header file: **enum_string.h**. Use the **DEFINE_ENUM_STRING** or **DEFINE_ENUM_STRING_LC** macro to define an enum class.
1. **DEFINE_ENUM_STRING** - after conversion to text, the characters will be in the same case as in the original enum class elements.
2. **DEFINE_ENUM_STRING_LC** - after conversion to text, characters will be converted to lowercase.

For more examples, see the file **cpp_serialized_tests/tests_enum_str.cpp**.

**Example 1**
Converted enum class to string.
```cpp
//Enum string
#include "enum_string.h"

//String utilities
#include "string_utils.h"

DEFINE_ENUM_STRING(Example_enum, ME_1, ME_2)

bool enum_to_string() {
const auto res_str1 = yb::string_utils::val_to_string(Example_enum::ME_1);

const auto res_str2 = yb::string_utils::val_to_string(Example_enum::ME_2);

const auto res_str3 = yb::string_utils::val_to_string(Example_enum(1234567));

if(res_str1 != "ME_1") {
return false;
}
if(res_str2 != "ME_2") {
return false;
}
if(res_str3 != "") {
return false;
}
return true;
}

```

**Example 1**
Converted string to enum class.
```cpp
//Enum string
#include "enum_string.h"

//String utilities
#include "string_utils.h"

DEFINE_ENUM_STRING(Example_enum, ME_1, ME_2)

bool enum_from_string() {
const auto res_enum1 = yb::string_utils::string_to_val<Example_enum>("ME_1");
const auto res_enum2 = yb::string_utils::string_to_val<Example_enum>("ME_2");
const auto res_enum3 = yb::string_utils::string_to_val<Example_enum>("ME_76565656");

if(res_enum1 != Example_enum::ME_1) {
return false;
}
if(res_enum2 != Example_enum::ME_2) {
return false;
}
if(res_enum3 != Example_enum(-1)) {
return false;
}

return true;
}

```

## How to serialize and deserialize third party structures and classes.<a name="third_party_struct"></a>
Let's say we need to serialize or deserialize a third-party object, for example **std::chrono::time_point<std::chrono::system_clock>**. To do this, you need to create a couple of template functions - for serialization and deserialization. The header file with these functions must be included before "cpp_serialized.h":

```cpp
#include "engine_addons.h"
#include "cpp_serialized.h"
```

Examples are located in the file **include/engine/engine_addons.h**.

1. yb::from_cpp::meta_table_from_cpp - serialization.
```cpp
namespace yb::from_cpp {
template<Storage_concept_from_cpp Storage>
void meta_table_from_cpp(const std::chrono::time_point<std::chrono::system_clock> &value, Storage& cur_node) {
const auto tvalue = std::chrono::system_clock::to_time_t(value);
const auto svalue = yb::string_utils::val_to_string(tvalue);
cur_node.interface_assign_from(svalue);
}
}
```

2. yb::to_cpp::meta_table_to_cpp - deserialization.
```cpp
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
```


## Supported data types
```cpp
enum class Type {null_value, int_value, uint_value, float_value, string_value, boolean_value, array_container, object_container};
```

**cpp_serialized** works with the following data types.
* null_value - the value was not found by the interface_get_storage_by_key method of the storage object (see below).
* int_value - integer (int).
* uint_value - unsigned int.
* float_value - floating point (float).
* string_value - string (std::string).
* boolean_value - boolean (bool).
* array_container - array (std::vector, std::list, std::deque, std::set, std::unordered_set). Array elements can be any type except null_value.
* object_container - associative container (std::map, std::unordered_map).


## How to create your own storage for deserialization
To create a storage facility for deserialization, you need to implement two classes. Iterator class and storage class. If you are using C++20, then your iterator and store classes must comply with the concepts:

**Iterator Concept**
```cpp
struct Value_model {};
struct Key_model {};

template<class T>
concept Const_iterator_concept =
requires(T a, const T ca) {
++a;
{ca != ca} -> std::convertible_to<bool>;
{ca.template interface_get_key<Key_model>()} -> std::convertible_to<Key_model>;
};
```

You can use the template class to write your own iterator class. An iterator is used to read data from a C++ container object such as std::vector, std::map, etc.

**Iterator pattern**
```cpp
class Const_value_iterator {
public:
void interface_increment();
bool interface_not_equal_to(const Const_value_iterator& other) const;
template<typename T1>
const T1& interface_get_key() const;
};
```

The iterator class must have the following methods.

1. **interface_increment()**. Moves to the next element. No return value is required. Corresponds to operator++() for a regular iterator.
2. **interface_not_equal_to**. Compares with another iterator. In practice, the comparison occurs with the value returned by the interface_end() method of the storage class. Corresponds to operator!= for a regular iterator.
3. **interface_get_key()**. The template function returns the value of the key of the specified type if the iterator is used with the C++ types std::map, std::unordered_map. Corresponds to first for a regular iterator.

**Vault Template**
```cpp
class Deserialize_storage {
public:
yb::Type interface_get_type() const;

template<typename T1>
T1 interface_get_value() const;

//container support
size_t interface_size() const;
Const_value_iterator interface_begin() const;
Const_value_iterator interface_end() const;

template<typename T1>
const Deserialize_storage& interface_get_storage_by_key(const T1& key) const;

static const Deserialize_storage& interface_get_storage_from_iterator(const Const_value_iterator& iter);
};
```

The storage class must have the following methods.
1. **interface_get_type()**. Returns the type of the value in the store.

For storages with types int_value, uint_value, float_value, string_value, boolean_value.
2. **interface_get_value()**. Returns the value stored in this store.

For storages with the array_container type.
3. **interface_size()**. Returns the size of the array. Corresponds to the size() method of a regular container.

For storages with type array_container, object_container.
4. **interface_begin()**. returns an iterator to the first element in the store. Corresponds to the begin() method of a regular container.
5. **interface_end()**. returns an iterator to the last + 1 element in the store. Corresponds to the end() method of a regular container.

For storages with type object_container.
6. **interface_get_storage_by_key(const T1& key)**. Looks up the 'key' value in the associative storage array. If the value is not found, then a store with type ('interface_get_type()') 'Type::null_value' is returned. If the value is found, it returns the found store. The return value can be a constant storage reference (const Storage&') or a storage object ('Storage').
7. **interface_get_storage_from_iterator(const Const_value_iterator& iter)**. static method. For a passed iterator, returns the store that this iterator points to. Corresponds to 'second' for a regular iterator.

See examples of deserialization stores:
* Adapter for third-party Json parser jsoncpp.
     File **include/useful_storages/jsoncpp_storage.h**.
     Class **yb::jsoncpp::Json_storage_adapter_to_cpp** - storage for serialization.
     Class **yb::jsoncpp::Const_value_iterator** - iterator class over a storage object for deserialization.
     Class **yb::jsoncpp::Json_storage_adapter_from_cpp** - adapter class for serialization into a third-party jsoncpp parser.
     Class **yb::jsoncpp::Json_storage_adapter_to_cpp** - adapter class for deserialization from a third-party jsoncpp parser.

* Serialization to rich text.
     File **include/useful_storages/text_storage.h**.
     Class **yb::text_from_cpp::TextStorage** - storage class for serialization to formatted text.

* Test single store for deserialization serialization.
     File **cpp_serialized_tests/test_storage.h**.
     Class **yb::both::TestStorage::Const_value_iterator** - iterator class over a storage object for deserialization.
     Class **yb::both::TestStorage** - storage class for serialization and deserialization.

* Test storage for deserialization.
     File **cpp_serialized_tests/test_storage_to_cpp.h**.
     Class **yb::to_cpp::TestStorage::Const_value_iterator** - iterator class over the test storage object for deserialization.
     Class **yb::to_cpp::TestStorage** - test storage class for deserialization from test storage.
    
* Test storage for serialization.
     File **cpp_serialized_tests/test_storage_from_cpp.h**.
     Class **yb::from_cpp::TestStorage** - test storage class for serialization into test storage.
