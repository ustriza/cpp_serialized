
# Автор **Юрий Бармин**, 3 октября 2023 г. 

## Автоматическая сериализация и десериализация произвольных типов C++. 

## Version 2.1. 


**cpp_serialized** может автоматически сериализовать и десериализовать C++ типы в/из произвольное хранилище. Вы моежте использовать уже существующие хранилища или создать поддержку собственных хранилищ. **cpp_serialized** поставляется со следующей поддержкой хранилищ:

1. TextStorage - сериализация в текст.
2. Json_storage_adapter_from_cpp - сериализация в Json (через сторонний jsoncpp парсер).
3. Json_storage_adapter_to_cpp - десериализация из Json (через сторонний jsoncpp парсер).

Типы поддерживаемых данных.
1. int, unsigned int, long, unsigned long, float, double, bool
2. std::string
3. std::pair, std::tuple
4. Собственные struct / class. Для поддержки сериализации и десериализации необхдимо объявить поля структуры/класса [специальном образом](#struct).
5. enum class - при использовании [enum string](#enum_string).
6. Сторонние структуры и классы. Для поддержки сериализации и десериализации необходимо определить [функции чтения и записи](#third_party_struct) этих структур и классов.
7. std::vector, std::list, std::deque, std::set, std::unordered_set.
8. std::map, std::unordered_map.

**cpp_serialized** позволит вам сериализовать и десериализовать сложные стректуры данных, контейнеры одной строкой. например:

```cpp
//declare data
const std::vector<std::map<int, std::string>> value{{{12345, "string_test0"}}, {{54321, "string_test1"}}};
Json::Value json;

//cpp_serialized
yb::assist::set_value(yb::jsoncpp::Json_storage_adapter_from_cpp{json}, value);
```

В переменную json будет записано двоичное значение следующего Json текста:

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

**Все примеры из этой статьи располагаются в файле cpp_serialized_tests/examples_README.cpp**

## Как использовать
1. cpp_serialized это заголовочная библиотека. Чтобы добавить cpp_serialized в свой проект, необходимо скопировать папку include.

2. Добавить в исходник следующую строку.

```cpp
#include "cpp_serialized.h"
```

5. Подготовить объект хранилища в который будет происводиться сериализация или из которого будет производиться десериализация. В примерах мы будем использовать два тестовых хранилища:

* yb::from_cpp::TestStorage - для сериализации.
* yb::to_cpp::TestStorage - для десериализации.

Также Вы можете использовать адаптеры хранилища для Json (сторонний jsoncpp парсер, см. классы 'Json_storage_adapter_to_cpp' и 'Json_storage_adapter_from_cpp' в include/useful_storages/jsoncpp_storage.h) и экспорта в текст (поставляются вместе с cpp_serialized, класс TextStorage в include/useful_storages/text_storage.h)

4. Вызвать сpp_serialized одной из функций: 

* yb::assist::serialize - сериализация
* yb::assist::deserialize_to и yb::assist::deserialize (десериализация).

Примеры расположены в файле 'cpp_serialized_tests/examples_README.cpp'.

### yb::assist::serialize - сериализация
Объявление:
```cpp
template <Storage_concept_from_cpp Storage, typename T>
void serialize(Storage&& storage, const T& from_value);
```

Эта функция имеет два аргумента.
1. storage - объект хранилища, в которое будет производиться сериализация.
2. from_value - c++ объект-источник.

**Пример 1:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third-party jsoncpp parser
#include "jsoncpp_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>

bool set_value_example() {
	//Declare data for serialization
	const std::vector<std::map<int, std::string>> value{{{12345, "string_test0"}, {98765, "string_test1"}}, {{54321, "string_test11"}, {1234321, "string_test12"}}};
	
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

**Пример 2:**
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

### yb::assist::deserialize_to - десериализация.
Объявление:
```cpp
template <typename T, Storage_concept_to_cpp Storage>
bool deserialize_to(const Storage& storage, T& to_value)
```

Эта функция имеет два аргумента.
1. storage - объект хранилища, из которого будет производиться десериализация.
2. to_value - c++ объект-приёмник.

Возвращаемое значение:
* true, если данные прочитаны верно; 
* false если при десериализации произошла ошибка. В этом случае состояние объекта to_value неопределено.

**Пример:**
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
	
	//Check deseserialize status
	if(!deserialize_status) {
		return false;
	}
	
	//Check deseserialized data
	const std::pair<std::string, int> testValue{"key", 1234};
	if(value != testValue) {
		return false;
	}
	
	return true;
}
```

### yb::assist::deserialize (1) - десериализация.
Объявление:
```cpp
template <typename T, Storage_concept_to_cpp Storage>
std::optional<T> get_value(const Storage& storage);
```

Эта функция имеет один аргумент.
1. storage - объект хранилища, из которого будет производиться десериализация.

Возвращаемое значение - std::optional<T>, где T тип переменной, в которую производится десериализация.

**Пример:**
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
	
	//Check deseserialize status
	if(!value.has_value()) {
		return false;
	}
	
	//Check deseserialized data
	const std::tuple<std::string, int, bool> testValue{"key", 1234, true};
	if(value.value() != testValue) {
		return false;
	}
	
	return true;
}
```

### yb::assist::deserialize (2) - десериализация.
Объявление:
```cpp
template <typename T, Storage_concept_to_cpp Storage>
T deserialize(const Storage& storage, const T& def);

template <typename T, Storage_concept_to_cpp Storage>
T deserialize(const Storage& storage, T&& def);
```
Эта функция имеет два аргумента.
1. storage - объект хранилища, из которого будет производиться десериализация.
2. def - значение по-умолчанию, которое будет возвращено, если десериализация окончится неудачно.

Возвращаемое значение - объект, в который была произведена десериализация. Если десериализация закончилась неуспешно, то будет возвращено значение аргумента def.

**Пример:**
```cpp
//cpp_serialized library main header.
#include "cpp_serialized.h"

//cpp_serialized storage adapter for the third-party jsoncpp parser
#include "jsoncpp_storage.h"

//Third-party jsoncpp parser header
#include <json/reader.h>

bool deserialize_def() {
	using Value_type = std::tuple<std::string, int, std::vector<int>>;

	//Source Json text. The second element("1234")  has wrong type string instead of integer. So this json will no be deserialized to the Value_type
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
	
	//Check deseserialized data
	if(value != default_value) {
		return false;
	}
	
	return true;
}
```

## Как использовать сериализацию в текст.

Заголовок:

```cpp
#include "text_storage.h"
```

Хранилище:
```cpp
yb::text_from_cpp::TextStorage
```

У хранилища yb::text_from_cpp::TextStorage имеется один метод для получения результата сериализации:
Объявление:
```cpp
	std::string& get_text();
```
Возвращаемое значение: текст, результат сериализации данных.


**Пример:**
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

Другая возможность сериализовать данные в текст это использовать функцию yb::assist::to_string, которая сама вызовет функцию сериализации yb::assist::serialize.

Объявление:
```cpp
template<typename T>
std::string to_string(const T& from_value);
```
Эта функция имеет один аргумент.
1. from_value - c++ объект-источник.

Возвращаемое значение - текст, результат сериализации объекта from_value.

**Пример:**
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

## Как создавать структуры для сериализации и десериализации <a name="struct"></a>

Чтобы структуры и классы поддерживали автоматическую сериализацию и десериализацию, необходимо описать поля специальным образом, а затем описать мета-таблицу, которая используется для доступа к этим полям. Размер такой структуры будет равен размеру структуры с полями, описанными обычным образом.

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

### Макро DEFINE_DATA определает поле структуры:
```cpp
DEFINE_DATA(name, type)
```


Макро DEFINE_DATA имеет два аргумента.
1. name - имя поля структуры / красса.
2. type - тип поля структуры / класса.

**Пример:**

```cpp
DEFINE_DATA(int_prop, int)
```

После обработки препроцессором будет создан следующий код:
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
Т.е. использование cpp_serialized не делает структуры более тяжелыми, чем обычно.

Если тип содержит запятую ',', как, например, std::map<int, int>, то необходимо сперва создать алиас для такого типа, а затем этот алиас использовать в DEFINE_DATA. В противном случае будет синтаксическая ошибка.

DEFINE_DATA добавляет в структуру три элемента:
1. Поле данных name (int_prop в примере).
2. Геттер get_##name (get_int_prop в примере).
3. Сеттер set_##name (set_int_prop в примере).

Таким образом, поле, определенное макро DEFINE_DATA, доступно для чнения и записи. Если необходимо определить поле, доступное только для чтения, используйте [DEFINE_DESER_GETTER](#DEFINE_DESER_GETTER)

**Пример:**
```cpp
using Map = std::map<std::string, int>;
DEFINE_DATA(map_prop, Map)
```

### Макро BEGIN_META_TABLE
Начинает определения таблицы для доступа к полям структуры / класса. Таблица определена как static constexpr std::tpule.

**Пример:**
```cpp
BEGIN_META_TABLE
```

###Макро DEFINE_META_TABLE_ITEM добавляет элемент в мета таблицу

```cpp
DEFINE_META_TABLE_ITEM(name)
```

Макро DEFINE_META_TABLE_ITEM имеет один аргумент.

1. name - имя поля структуры / красса. Каждому макро DEFINE_META_TABLE_ITEM с определенным name обязан существовать макро DEFINE_DATA с тем же name.

Запись с ключом name ОБЯЗАНА присутствовать в хранилище. Если запись с таким ключом не будет найдена, то сериализация закончится неуспешно.

**Пример:**
```cpp
DEFINE_META_TABLE_ITEM(int_prop)
```

### Макро DEFINE_META_TABLE_ITEM_EMPTY добавляет элемент в мета таблицу

```cpp
DEFINE_META_TABLE_ITEM_EMPTY(name)
```

Макро DEFINE_META_TABLE_ITEM_EMPTY имеет один аргумент.

1. name - имя поля структуры / красса. Каждому макро DEFINE_META_TABLE_ITEM_EMPTY с определенным name обязан существовать макро DEFINE_DATA с тем же name.

Запись с ключом name может отсутствовать в хранилище. Если запись с таким ключом не будет найдена, то будет использовано пустое значение.

**Пример:**
```cpp
DEFINE_META_TABLE_ITEM_EMPTY(string_prop)
```

### Макро DEFINE_META_TABLE_ITEM_DEFAULT добавляет элемент в мета таблицу

```cpp
DEFINE_META_TABLE_ITEM_DEFAULT(name, def)
```

Макро DEFINE_META_TABLE_ITEM_DEFAULT имеет два аргумента.

1. name - имя поля структуры / красса. Каждому макро DEFINE_META_TABLE_ITEM_DEFAULT с определенным name обязан существовать макро DEFINE_DATA с тем же name.
2. def - значение по-умолчанию.

Запись с ключом name может отсутствовать в хранилище. Если запись с таким ключом не будет найдена, то будет использовано значение def.

**Пример:**
```cpp
DEFINE_META_TABLE_ITEM_DEFAULT(vector_prop, (std::vector{1000, 2000}))
```

### DEFINE_DESER_GETTER<a name="DEFINE_DESER_GETTER"></a>

DEFINE_DESER_GETTER добавляет в структуру три элемента:
1. Поле данных name (int_prop в примере).
2. Геттер get_##name (get_int_prop в примере).

Поле структуры / класса определено как private, поэтому нет возможности изменить его. Этим гарантируется, что прочитанные из хранилища данные не могут быть изменены программистом. Доступно только чтение через геттер.

**Пример:**
```cpp
DEFINE_DESER_GETTER(int_prop, int)
```

После обработки препроцессором будет создан следующий код:
```cpp
private:
	int int_prop{};
public:
	const auto& get_int_prop() const noexcept {
		//Here a code for some compile-time checks

		return int_prop;}
public:
```

**Полный пример десериализации из Json**
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
	
	//Check deseserialize status
	if(!deserialize_status) {
		return false;
	}
	
	//Check deseserialized data
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
## Преобразование enum class в текст и обратно.<a name="enum_string"></a>
Заголовочный файл: **enum_string.h**. Используйте макро **DEFINE_ENUM_STRING** или **DEFINE_ENUM_STRING_LC** для определения enum class.
1. **DEFINE_ENUM_STRING** - после преобразования в текст символы будут на том же регистре, что и в оригинальных элементах enum class.
2. **DEFINE_ENUM_STRING_LC** - после преобразования в текст символы будут преобразованы в нижний регистр.

Больше примеров см. в файле **cpp_serialized_tests/tests_enum_str.cpp**.

**Пример 1**
Преобразоване enum class в строку.
```cpp
//Enum string
#include "enum_string.h"

//String utils
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

**Пример 1**
Преобразоване строки в enum class.
```cpp
//Enum string
#include "enum_string.h"

//String utils
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

## Как сериализовать и десериализовать сторонние структуры и классы.<a name="third_party_struct"></a>
Допустим нас необходимо сериалзовать или десериализовать сторонний объект, например **std::chrono::time_point<std::chrono::system_clock>**. Для этого необходимо создать пару шаблонных функций - для сериализации и десериализации. Заголовочный файл с этими функциями необходимо включать перед "cpp_serialized.h":

```cpp
#include "engine_addons.h"
#include "cpp_serialized.h"
```

Примеры расположены в файле **include/engine/engine_addons.h**.

1. yb::from_cpp::meta_table_from_cpp - сериализация.
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

2. yb::to_cpp::meta_table_to_cpp - десериализация.
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


## Поддерживаемые типы данных
```cpp
enum class Type {null_value, int_value, uint_value, float_value, string_value, boolean_value, array_container, object_container};
```

**cpp_serialized** работает со следующими типами данных.
--  null_value - значение не найдено методом interface_get_storage_by_key объекта хранилища (см. ниже).
--  int_value - целое (int).
--  uint_value - беззнаковое целое (unsigned int).
--  float_value - плавающая точка (float).
--  string_value - строка (std::string).
--  boolean_value - булевый (bool).
--  array_container - массив (std::vector, std::list, std::deque, std::set, std::unordered_set). Элементы массива могут быть любым типом, кроме null_value.
--  object_container - ассоциативный контейнер (std::map, std::unordered_map).


## Как создать своё хранилище для десериализации
--  int_value - целое (int).
Для создания ханилища для десериализации необходимо реализовать два класса. Класс итератора и класс хранилища. Если Вы используете C++20, то Ваши классы итератора и хранилища обязаны соответствовать концептам:

**Концепт итератора**
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

Вы можете использовать класс - шаблон для написания собственного класса-итератора. Итератор используется для чтения данных из C++ объекта контейнера, таких как std::vector, std::map и т.п.

**Шаблон итератора**
```cpp
class Const_value_iterator {
public:
	void interface_increment();
	bool interface_not_equal_to(const Const_value_iterator& other) const;
	template<typename T1>
	const T1& interface_get_key() const;
};
```

Класс итератора обязын иметь следующие методы.

1. **interface_increment()**. Переходит к следующему элементу. Возвращаемое значение не требуется. Соответствует operator++() у обычного итератора.
2. **interface_not_equal_to**. Сравнивает с другим итератором. На практике сравнение происходит со значением, возвращаемым методом interface_end() класса хранилища. Соответствует operator!= у обычного итератора.
3. **interface_get_key()**. Шаблонная функция возвращает значение ключа указанного типа, если итератор используется с C++ типами std::map, std::unordered_map. Соответствует first у обычного итератора.

**Шаблон хранилища**
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

Класс хранилища обязан иметь следующие методы.

1. **interface_get_type()**. Возвращает тип значения в хранилище.

Для хранилищ с типом int_value, uint_value, float_value, string_value, boolean_value.
2. **interface_get_value()**. Возвращает значение, ханящееся в данном хранилище.

Для хранилищ с типом array_container.
3. **interface_size()**. Возвращает размер масива. Соответствует методу size() обычного контейнера.

Для хранилищ с типом array_container, object_container.
4. **interface_begin()**. возвращает итератор на первый элемент в хранилище. Соответствует методу begin() обычного контейнера.
5. **interface_end()**. возвращает итератор на последний + 1 элемент в хранилище. Соответствует методу end() обычного контейнера.

Для хранилищ с типом object_container.
6. **interface_get_storage_by_key(const T1& key)**. Ищет значение 'key' в ассоциативном массиве хранилища. Если значение не найдено, то возвращается хранилище с типом ('interface_get_type()') 'Type::null_value'. Если значение найдено, то возвращает найденное хранилище. Возвращаемое значение может быть константной ссылкой на хранилище (const Storage&') или объектом хранилища ('Storage').
7. **interface_get_storage_from_iterator(const Const_value_iterator& iter)**. статический метод. Для переданного итератора возвращает хранилище, на который указывает этот итератор.  Соответствует 'second' у обычного итератора.

См. примеры хранилищ для десериализации:
--  Адаптер для стороннего Json парсера jsoncpp.
    Файл **include/useful_storages/jsoncpp_storage.h**.
	Класс **yb::jsoncpp::Json_storage_adapter_to_cpp** - хранилище для сериализации.
	Класс **yb::jsoncpp::Const_value_iterator** - класс итератора по объекту хранилища для десериализации.
	Класс **yb::jsoncpp::Json_storage_adapter_from_cpp** - класс адаптера для сериализации в сторонний парсер jsoncpp.
	Класс **yb::jsoncpp::Json_storage_adapter_to_cpp** - класс адаптера для десериализации из стороннего парсера jsoncpp.
	
--  Сериализация в форматированный текст.
    Файл **include/useful_storages/text_storage.h**.
    Класс **yb::text_from_cpp::TextStorage** - класс хранилища для сериализации в форматированный текст.

--  Тестовое единое хранилище для сериализации десериализации.
    Файл **cpp_serialized_tests/test_storage.h**.
    Класс **yb::both::TestStorage::Const_value_iterator** - класс итератора по объекту хранилища для десериализации.
    Класс **yb::both::TestStorage** - класс хранилища для сериализации и десериализации.

--  Тестовое хранилище для десериализации.
    Файл **cpp_serialized_tests/test_storage_to_cpp.h**.
    Класс **yb::to_cpp::TestStorage::Const_value_iterator** - класс итератора по объекту тестового хранилища для десериализации.
    Класс **yb::to_cpp::TestStorage** - класс тестового хранилища для десериализации из тестового хранилища.
    
--  Тестовое хранилище для сериализации.
    Файл **cpp_serialized_tests/test_storage_from_cpp.h**.
    Класс **yb::from_cpp::TestStorage** - класс тестового хранилища для сериализации в тестовое хранилище.

