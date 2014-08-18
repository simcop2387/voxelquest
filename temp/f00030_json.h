

enum JSONType { JSONType_Null, JSONType_String, JSONType_Bool, JSONType_Number, JSONType_Array, JSONType_Object };

class JSONValue;

typedef std::vector<JSONValue*> JSONArray;
typedef std::map<std::wstring, JSONValue*> JSONObject;

class JSONValue
{
	
	public:
		JSONValue(/*NULL*/);
		JSONValue(const wchar_t *m_char_value);
		JSONValue(const std::wstring &m_string_value);
		JSONValue(bool m_bool_value);
		JSONValue(double m_number_value);
		JSONValue(const JSONArray &m_array_value);
		JSONValue(const JSONObject &m_object_value);
		~JSONValue();

		bool IsNull() const;
		bool IsString() const;
		bool IsBool() const;
		bool IsNumber() const;
		bool IsArray() const;
		bool IsObject() const;
		
		const std::wstring &AsString() const;
		bool AsBool() const;
		double AsNumber() const;
		const JSONArray &AsArray() const;
		const JSONObject &AsObject() const;

		std::size_t CountChildren() const;
		bool HasChild(std::size_t index) const;
		JSONValue *Child(std::size_t index);
		bool HasChild(const wchar_t* name) const;
		JSONValue *Child(const wchar_t* name);

		std::wstring Stringify() const;

		static JSONValue *Parse(const wchar_t **data);

		static std::wstring StringifyString(const std::wstring &str);
	
		JSONType type;
		std::wstring string_value;
		bool bool_value;
		double number_value;
		JSONArray array_value;
		JSONObject object_value;
};



class JSON
{
	
	public:
		static JSONValue* Parse(const char *data);
		static JSONValue* Parse(const wchar_t *data);
		static std::wstring Stringify(const JSONValue *value);

		static bool SkipWhitespace(const wchar_t **data);
		static bool ExtractString(const wchar_t **data, std::wstring &str);
		static double ParseInt(const wchar_t **data);
		static double ParseDecimal(const wchar_t **data);

		JSON();
};




// Macros to free an array/object
#define FREE_ARRAY(x) { JSONArray::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete *iter; } }
#define FREE_OBJECT(x) { JSONObject::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete (*iter).second; } }

/**
 * Parses a JSON encoded value to a JSONValue object
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the data
 *
 * @return JSONValue* Returns a pointer to a JSONValue object on success, NULL on error
 */
JSONValue *JSONValue::Parse(const wchar_t **data)
{
	// Is it a string?
	if (**data == '"')
	{
		std::wstring str;
		if (!JSON::ExtractString(&(++(*data)), str))
			return NULL;
		else
			return new JSONValue(str);
	}
	
	// Is it a boolean?
	else if ((simplejson_wcsnlen(*data, 4) && wcsncasecmp(*data, L"true", 4) == 0) || (simplejson_wcsnlen(*data, 5) && wcsncasecmp(*data, L"false", 5) == 0))
	{
		bool value = wcsncasecmp(*data, L"true", 4) == 0;
		(*data) += value ? 4 : 5;
		return new JSONValue(value);
	}
	
	// Is it a null?
	else if (simplejson_wcsnlen(*data, 4) && wcsncasecmp(*data, L"null", 4) == 0)
	{
		(*data) += 4;
		return new JSONValue();
	}
	
	// Is it a number?
	else if (**data == L'-' || (**data >= L'0' && **data <= L'9'))
	{
		// Negative?
		bool neg = **data == L'-';
		if (neg) (*data)++;

		double number = 0.0;

		// Parse the whole part of the number - only if it wasn't 0
		if (**data == L'0')
			(*data)++;
		else if (**data >= L'1' && **data <= L'9')
			number = JSON::ParseInt(data);
		else
			return NULL;
		
		// Could be a decimal now...
		if (**data == '.')
		{
			(*data)++;

			// Not get any digits?
			if (!(**data >= L'0' && **data <= L'9'))
				return NULL;
			
			// Find the decimal and sort the decimal place out
			// Use ParseDecimal as ParseInt won't work with decimals less than 0.1
			// thanks to Javier Abadia for the report & fix
			double decimal = JSON::ParseDecimal(data);
			
			// Save the number
			number += decimal;
		}

		// Could be an exponent now...
		if (**data == L'E' || **data == L'e')
		{
			(*data)++;

			// Check signage of expo
			bool neg_expo = false;
			if (**data == L'-' || **data == L'+')
			{
				neg_expo = **data == L'-';
				(*data)++;
			}
			
			// Not get any digits?
			if (!(**data >= L'0' && **data <= L'9'))
				return NULL;

			// Sort the expo out
			double expo = JSON::ParseInt(data);
			for (double i = 0.0; i < expo; i++)
				number = neg_expo ? (number / 10.0) : (number * 10.0);
		}

		// Was it neg?
		if (neg) number *= -1;

		return new JSONValue(number);
	}

	// An object?
	else if (**data == L'{')
	{
		JSONObject object;
		
		(*data)++;
	
		while (**data != 0)
		{
			// Whitespace at the start?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// Special case - empty object
			if (object.size() == 0 && **data == L'}')
			{
				(*data)++;
				return new JSONValue(object);
			}
			
			// We want a string now...
			std::wstring name;
			if (!JSON::ExtractString(&(++(*data)), name))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// More whitespace?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// Need a : now
			if (*((*data)++) != L':')
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// More whitespace?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// The value is here			
			JSONValue *value = Parse(data);
			if (value == NULL)
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// Add the name:value
			if (object.find(name) != object.end())
				delete object[name];
			object[name] = value;
			
			// More whitespace?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			// End of object?
			if (**data == L'}')
			{
				(*data)++;
				return new JSONValue(object);
			}
			
			// Want a , now
			if (**data != L',')
			{
				FREE_OBJECT(object);
				return NULL;
			}
			
			(*data)++;
		}
		
		// Only here if we ran out of data
		FREE_OBJECT(object);
		return NULL;
	}
	
	// An array?
	else if (**data == L'[')
	{
		JSONArray array;
		
		(*data)++;
		
		while (**data != 0)
		{
			// Whitespace at the start?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_ARRAY(array);
				return NULL;
			}
			
			// Special case - empty array
			if (array.size() == 0 && **data == L']')
			{
				(*data)++;
				return new JSONValue(array);
			}
			
			// Get the value
			JSONValue *value = Parse(data);
			if (value == NULL)
			{
				FREE_ARRAY(array);
				return NULL;
			}
			
			// Add the value
			array.push_back(value);
			
			// More whitespace?
			if (!JSON::SkipWhitespace(data))
			{
				FREE_ARRAY(array);
				return NULL;
			}
			
			// End of array?
			if (**data == L']')
			{
				(*data)++;
				return new JSONValue(array);
			}
			
			// Want a , now
			if (**data != L',')
			{
				FREE_ARRAY(array);
				return NULL;
			}
			
			(*data)++;
		}
		
		// Only here if we ran out of data
		FREE_ARRAY(array);
		return NULL;
	}
	
	// Ran out of possibilites, it's bad!
	else
	{
		return NULL;
	}
}

/**
 * Basic constructor for creating a JSON Value of type NULL
 *
 * @access public
 */
JSONValue::JSONValue(/*NULL*/)
{
	type = JSONType_Null;
}

/**
 * Basic constructor for creating a JSON Value of type String
 *
 * @access public
 *
 * @param wchar_t* m_char_value The string to use as the value
 */
JSONValue::JSONValue(const wchar_t *m_char_value)
{
	type = JSONType_String;
	string_value = std::wstring(m_char_value);
}

/**
 * Basic constructor for creating a JSON Value of type String
 *
 * @access public
 *
 * @param std::wstring m_string_value The string to use as the value
 */
JSONValue::JSONValue(const std::wstring &m_string_value)
{
	type = JSONType_String;
	string_value = m_string_value;
}

/**
 * Basic constructor for creating a JSON Value of type Bool
 *
 * @access public
 *
 * @param bool m_bool_value The bool to use as the value
 */
JSONValue::JSONValue(bool m_bool_value)
{
	type = JSONType_Bool;
	bool_value = m_bool_value;
}

/**
 * Basic constructor for creating a JSON Value of type Number
 *
 * @access public
 *
 * @param double m_number_value The number to use as the value
 */
JSONValue::JSONValue(double m_number_value)
{
	type = JSONType_Number;
	number_value = m_number_value;
}

/**
 * Basic constructor for creating a JSON Value of type Array
 *
 * @access public
 *
 * @param JSONArray m_array_value The JSONArray to use as the value
 */
JSONValue::JSONValue(const JSONArray &m_array_value)
{
	type = JSONType_Array;
	array_value = m_array_value;
}

/**
 * Basic constructor for creating a JSON Value of type Object
 *
 * @access public
 *
 * @param JSONObject m_object_value The JSONObject to use as the value
 */
JSONValue::JSONValue(const JSONObject &m_object_value)
{
	type = JSONType_Object;
	object_value = m_object_value;
}

/**
 * The destructor for the JSON Value object
 * Handles deleting the objects in the array or the object value
 *
 * @access public
 */
JSONValue::~JSONValue()
{
	if (type == JSONType_Array)
	{
		JSONArray::iterator iter;
		for (iter = array_value.begin(); iter != array_value.end(); iter++)
			delete *iter;
	}
	else if (type == JSONType_Object)
	{
		JSONObject::iterator iter;
		for (iter = object_value.begin(); iter != object_value.end(); iter++)
		{
			delete (*iter).second;
		}
	}
}

/**
 * Checks if the value is a NULL
 *
 * @access public
 *
 * @return bool Returns true if it is a NULL value, false otherwise
 */
bool JSONValue::IsNull() const
{
	return type == JSONType_Null;
}

/**
 * Checks if the value is a String
 *
 * @access public
 *
 * @return bool Returns true if it is a String value, false otherwise
 */
bool JSONValue::IsString() const
{
	return type == JSONType_String;
}

/**
 * Checks if the value is a Bool
 *
 * @access public
 *
 * @return bool Returns true if it is a Bool value, false otherwise
 */
bool JSONValue::IsBool() const
{
	return type == JSONType_Bool;
}

/**
 * Checks if the value is a Number
 *
 * @access public
 *
 * @return bool Returns true if it is a Number value, false otherwise
 */
bool JSONValue::IsNumber() const
{
	return type == JSONType_Number;
}

/**
 * Checks if the value is an Array
 *
 * @access public
 *
 * @return bool Returns true if it is an Array value, false otherwise
 */
bool JSONValue::IsArray() const
{
	return type == JSONType_Array;
}

/**
 * Checks if the value is an Object
 *
 * @access public
 *
 * @return bool Returns true if it is an Object value, false otherwise
 */
bool JSONValue::IsObject() const
{
	return type == JSONType_Object;
}

/**
 * Retrieves the String value of this JSONValue
 * Use IsString() before using this method.
 *
 * @access public
 *
 * @return std::wstring Returns the string value
 */
const std::wstring &JSONValue::AsString() const
{
	return string_value;
}

/**
 * Retrieves the Bool value of this JSONValue
 * Use IsBool() before using this method.
 *
 * @access public
 *
 * @return bool Returns the bool value
 */
bool JSONValue::AsBool() const
{
	return bool_value;
}

/**
 * Retrieves the Number value of this JSONValue
 * Use IsNumber() before using this method.
 *
 * @access public
 *
 * @return double Returns the number value
 */
double JSONValue::AsNumber() const
{
	return number_value;
}

/**
 * Retrieves the Array value of this JSONValue
 * Use IsArray() before using this method.
 *
 * @access public
 *
 * @return JSONArray Returns the array value
 */
const JSONArray &JSONValue::AsArray() const
{
	return array_value;
}

/**
 * Retrieves the Object value of this JSONValue
 * Use IsObject() before using this method.
 *
 * @access public
 *
 * @return JSONObject Returns the object value
 */
const JSONObject &JSONValue::AsObject() const
{
	return object_value;
}

/**
 * Retrieves the number of children of this JSONValue.
 * This number will be 0 or the actual number of children
 * if IsArray() or IsObject().
 *
 * @access public
 *
 * @return The number of children.
 */
std::size_t JSONValue::CountChildren() const
{
	switch (type)
	{
		case JSONType_Array:
			return array_value.size();
		case JSONType_Object:
			return object_value.size();
		default:
			return 0;
	}
}

/**
 * Checks if this JSONValue has a child at the given index.
 * Use IsArray() before using this method.
 *
 * @access public
 *
 * @return bool Returns true if the array has a value at the given index.
 */
bool JSONValue::HasChild(std::size_t index) const
{
	if (type == JSONType_Array)
	{
		return index < array_value.size();
	}
	else
	{
		return false;
	}
}

/**
 * Retrieves the child of this JSONValue at the given index.
 * Use IsArray() before using this method.
 *
 * @access public
 *
 * @return JSONValue* Returns JSONValue at the given index or NULL
 *                    if it doesn't exist.
 */
JSONValue *JSONValue::Child(std::size_t index)
{
	if (index < array_value.size())
	{
		return array_value[index];
	}
	else
	{
		return NULL;
	}
}

/**
 * Checks if this JSONValue has a child at the given key.
 * Use IsObject() before using this method.
 *
 * @access public
 *
 * @return bool Returns true if the object has a value at the given key.
 */
bool JSONValue::HasChild(const wchar_t* name) const
{
	if (type == JSONType_Object)
	{
		return object_value.find(name) != object_value.end();
	}
	else
	{
		return false;
	}
}

/**
 * Retrieves the child of this JSONValue at the given key.
 * Use IsObject() before using this method.
 *
 * @access public
 *
 * @return JSONValue* Returns JSONValue for the given key in the object
 *                    or NULL if it doesn't exist.
 */
JSONValue* JSONValue::Child(const wchar_t* name)
{
	JSONObject::const_iterator it = object_value.find(name);
	if (it != object_value.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

/**
 * Creates a JSON encoded string for the value with all necessary characters escaped
 *
 * @access public
 *
 * @return std::wstring Returns the JSON string
 */
std::wstring JSONValue::Stringify() const
{
	std::wstring ret_string;
	
	switch (type)
	{
		case JSONType_Null:
			ret_string = L"null";
			break;
		
		case JSONType_String:
			ret_string = StringifyString(string_value);
			break;
		
		case JSONType_Bool:
			ret_string = bool_value ? L"true" : L"false";
			break;
		
		case JSONType_Number:
		{
			if (isinf(number_value) || isnan(number_value))
				ret_string = L"null";
			else
			{
				std::wstringstream ss;
				ss.precision(15);
				ss << number_value;
				ret_string = ss.str();
			}
			break;
		}
		
		case JSONType_Array:
		{
			ret_string = L"[";
			JSONArray::const_iterator iter = array_value.begin();
			while (iter != array_value.end())
			{
				ret_string += (*iter)->Stringify();
				
				// Not at the end - add a separator
				if (++iter != array_value.end())
					ret_string += L",";
			}
			ret_string += L"]";
			break;
		}
		
		case JSONType_Object:
		{
			ret_string = L"{";
			JSONObject::const_iterator iter = object_value.begin();
			while (iter != object_value.end())
			{
				ret_string += StringifyString((*iter).first);
				ret_string += L":";
				ret_string += (*iter).second->Stringify();
				
				// Not at the end - add a separator
				if (++iter != object_value.end())
					ret_string += L",";
			}
			ret_string += L"}";
			break;
		}
	}

	return ret_string;
}

/**
 * Creates a JSON encoded string with all required fields escaped
 * Works from http://www.ecma-internationl.org/publications/files/ECMA-ST/ECMA-262.pdf
 * Section 15.12.3.
 *
 * @access private
 *
 * @param std::wstring str The string that needs to have the characters escaped
 *
 * @return std::wstring Returns the JSON string
 */
std::wstring JSONValue::StringifyString(const std::wstring &str)
{
	std::wstring str_out = L"\"";
	
	std::wstring::const_iterator iter = str.begin();
	while (iter != str.end())
	{
		wchar_t chr = *iter;

		if (chr == L'"' || chr == L'\\' || chr == L'/')
		{
			str_out += L'\\';
			str_out += chr;
		}
		else if (chr == L'\b')
		{
			str_out += L"\\b";
		}
		else if (chr == L'\f')
		{
			str_out += L"\\f";
		}
		else if (chr == L'\n')
		{
			str_out += L"\\n";
		}
		else if (chr == L'\r')
		{
			str_out += L"\\r";
		}
		else if (chr == L'\t')
		{
			str_out += L"\\t";
		}
		else if (chr < L' ')
		{
			str_out += L"\\u";
			for (int i = 0; i < 4; i++)
			{
				int value = (chr >> 12) & 0xf;
				if (value >= 0 && value <= 9)
					str_out += (wchar_t)('0' + value);
				else if (value >= 10 && value <= 15)
					str_out += (wchar_t)('A' + (value - 10));
				chr <<= 4;
			}
		}
		else
		{
			str_out += chr;
		}
		
		iter++;
	}
	
	str_out += L"\"";
	return str_out;
}



JSON::JSON()
{
}

/**
 * Parses a complete JSON encoded string
 * This is just a wrapper around the UNICODE Parse().
 *
 * @access public
 *
 * @param char* data The JSON text
 *
 * @return JSONValue* Returns a JSON Value representing the root, or NULL on error
 */

JSONValue *JSON::Parse(const char *data)
{

	cout << "JSON::Parse a\n";

	size_t length = strlen(data) + 4096;
	cout << "length: " << length << "\n";

	if (length == 0) {
		cout << "JSON::Parse Length 0\n";
		return NULL;
	}

	wchar_t *w_data = new wchar_t[length];
	size_t ret_value = 0;

	errno_t myError;

	cout << "JSON::Parse b\n";

	#if defined(WIN32) && !defined(__GNUC__)
		cout << "JSON::Parse c\n";
		

		// if (data) {

		// }
		// else {
		// 	cout << "JSON::Parse Data Null\n";
		// 	return NULL;
		// }

		// if (w_data) {

		// }
		// else {
		// 	cout << "JSON::Parse w_data Null\n";
		// 	return NULL;
		// }
		
		myError = mbstowcs_s(&ret_value, w_data, length, data, length);

		cout << "JSON::Parse c0\n";

		if (myError != 0)
		{
			cout << "JSON::Parse c2\n";

			delete[] w_data;

			cout << "JSON::Parse c3\n";

			return NULL;
		}
		cout << "JSON::Parse d\n";
	#else
		cout << "JSON::Parse e\n";
		if (mbstowcs(w_data, data, length) == (size_t)-1)
		{
			delete[] w_data;
			return NULL;
		}
		cout << "JSON::Parse f\n";
	#endif
	
	JSONValue *value = JSON::Parse(w_data);
	delete[] w_data;

	cout << "JSON::Parse g\n";

	return value;
}

/**
 * Parses a complete JSON encoded string (UNICODE input version)
 *
 * @access public
 *
 * @param wchar_t* data The JSON text
 *
 * @return JSONValue* Returns a JSON Value representing the root, or NULL on error
 */
JSONValue *JSON::Parse(const wchar_t *data)
{
	// Skip any preceding whitespace, end of data = no JSON = fail
	if (!SkipWhitespace(&data))
		return NULL;

	// We need the start of a value here now...
	JSONValue *value = JSONValue::Parse(&data);
	if (value == NULL)
		return NULL;
	
	// Can be white space now and should be at the end of the string then...
	if (SkipWhitespace(&data))
	{
		delete value;
		return NULL;
	}
	
	// We're now at the end of the string
	return value;
}

/**
 * Turns the passed in JSONValue into a JSON encode string
 *
 * @access public
 *
 * @param JSONValue* value The root value
 *
 * @return std::wstring Returns a JSON encoded string representation of the given value
 */
std::wstring JSON::Stringify(const JSONValue *value)
{
	if (value != NULL)
		return value->Stringify();
	else
		return L"";
}

/**
 * Skips over any whitespace characters (space, tab, \r or \n) defined by the JSON spec
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the JSON text
 *
 * @return bool Returns true if there is more data, or false if the end of the text was reached
 */
bool JSON::SkipWhitespace(const wchar_t **data)
{
	while (**data != 0 && (**data == L' ' || **data == L'\t' || **data == L'\r' || **data == L'\n'))
		(*data)++;
	
	return **data != 0;
}

/**
 * Extracts a JSON String as defined by the spec - "<some chars>"
 * Any escaped characters are swapped out for their unescaped values
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the JSON text
 * @param std::wstring& str Reference to a std::wstring to receive the extracted string
 *
 * @return bool Returns true on success, false on failure
 */
bool JSON::ExtractString(const wchar_t **data, std::wstring &str)
{
	str = L"";
	
	while (**data != 0)
	{
		// Save the char so we can change it if need be
		wchar_t next_char = **data;
		
		// Escaping something?
		if (next_char == L'\\')
		{
			// Move over the escape char
			(*data)++;
			
			// Deal with the escaped char
			switch (**data)
			{
				case L'"': next_char = L'"'; break;
				case L'\\': next_char = L'\\'; break;
				case L'/': next_char = L'/'; break;
				case L'b': next_char = L'\b'; break;
				case L'f': next_char = L'\f'; break;
				case L'n': next_char = L'\n'; break;
				case L'r': next_char = L'\r'; break;
				case L't': next_char = L'\t'; break;
				case L'u':
				{
					// We need 5 chars (4 hex + the 'u') or its not valid
					if (!simplejson_wcsnlen(*data, 5))
						return false;
					
					// Deal with the chars
					next_char = 0;
					for (int i = 0; i < 4; i++)
					{
						// Do it first to move off the 'u' and leave us on the
						// final hex digit as we move on by one later on
						(*data)++;
						
						next_char <<= 4;
						
						// Parse the hex digit
						if (**data >= '0' && **data <= '9')
							next_char |= (**data - '0');
						else if (**data >= 'A' && **data <= 'F')
							next_char |= (10 + (**data - 'A'));
						else if (**data >= 'a' && **data <= 'f')
							next_char |= (10 + (**data - 'a'));
						else
						{
							// Invalid hex digit = invalid JSON
							return false;
						}
					}
					break;
				}
				
				// By the spec, only the above cases are allowed
				default:
					return false;
			}
		}
		
		// End of the string?
		else if (next_char == L'"')
		{
			(*data)++;
			str.reserve(); // Remove unused capacity
			return true;
		}
		
		// Disallowed char?
		else if (next_char < L' ' && next_char != L'\t')
		{
			// SPEC Violation: Allow tabs due to real world cases
			return false;
		}
		
		// Add the next char
		str += next_char;
		
		// Move on
		(*data)++;
	}
	
	// If we're here, the string ended incorrectly
	return false;
}

/**
 * Parses some text as though it is an integer
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the JSON text
 *
 * @return double Returns the double value of the number found
 */
double JSON::ParseInt(const wchar_t **data)
{
	double integer = 0;
	while (**data != 0 && **data >= '0' && **data <= '9')
		integer = integer * 10 + (*(*data)++ - '0');
	
	return integer;
}

/**
 * Parses some text as though it is a decimal
 *
 * @access protected
 *
 * @param wchar_t** data Pointer to a wchar_t* that contains the JSON text
 *
 * @return double Returns the double value of the decimal found
 */
double JSON::ParseDecimal(const wchar_t **data)
{
	double decimal = 0.0;
  double factor = 0.1;
	while (**data != 0 && **data >= '0' && **data <= '9')
  {
    int digit = (*(*data)++ - '0');
		decimal = decimal + digit * factor;
    factor *= 0.1;
  }
	return decimal;
}




 
