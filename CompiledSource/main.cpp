/*************************************************************************
* 
* 
* Copyright [2000] - [2012] Gavan Woolery 
* All Rights Reserved.
* 
* NOTICE:  All information contained herein is, and remains
* the sole property of Gavan Woolery. Dissemination of this information
* or reproduction of this material is strictly forbidden unless prior
* written permission is obtained from Gavan Woolery.
*
*
*************************************************************************/

//./mergefiles.sh;cmake -DCMAKE_BUILD_TYPE=Release CMakeLists.txt;make clean;make;./voxelquest

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <sstream>
#include <math.h>
#include <vector>
#include <assert.h>
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>

#if ( (defined(__MACH__)) && (defined(__APPLE__)) )   
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glext.h>
#endif

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Format.h"



using Poco::Net::ServerSocket;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


/*
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::tcp;
*/

using namespace std;


#define _USE_MATH_DEFINES

typedef unsigned int uint;


// Win32 incompatibilities
#if defined(WIN32) && !defined(__GNUC__)
	#define wcsncasecmp _wcsnicmp
	static inline bool isnan(double x) { return x != x; }
	static inline bool isinf(double x) { return !isnan(x) && isnan(x - x); }
#endif

// Linux compile fix - from quaker66
#ifdef __GNUC__
	// #include <cstring>
	// #include <cstdlib>
#endif

// Mac compile fixes - from quaker66, Lion fix by dabrahams
#if defined(__APPLE__) && __DARWIN_C_LEVEL < 200809L || (defined(WIN32) && defined(__GNUC__))
	#include <wctype.h>
	#include <wchar.h>
	
	static inline int wcsncasecmp(const wchar_t *s1, const wchar_t *s2, size_t n)
	{
		int lc1  = 0;
		int lc2  = 0;

		while (n--)
		{
			lc1 = towlower (*s1);
			lc2 = towlower (*s2);

			if (lc1 != lc2)
				return (lc1 - lc2);

			if (!lc1)
				return 0;

			++s1;
			++s2;
		}

		return 0;
	}
#endif

// Simple function to check a string 's' has at least 'n' characters
static inline bool simplejson_wcsnlen(const wchar_t *s, size_t n) {
	if (s == 0)
		return false;

	const wchar_t *save = s;
	while (n-- > 0)
	{
		if (*(save++) == 0) return false;
	}

	return true;
}





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
	size_t length = strlen(data) + 1;
	wchar_t *w_data = (wchar_t*)malloc(length * sizeof(wchar_t));
	
	#if defined(WIN32) && !defined(__GNUC__)
		size_t ret_value = 0;
		if (mbstowcs_s(&ret_value, w_data, length, data, length) != 0)
		{
			free(w_data);
			return NULL;
		}
	#else
		if (mbstowcs(w_data, data, length) == (size_t)-1)
		{
			free(w_data);
			return NULL;
		}
	#endif
	
	JSONValue *value = JSON::Parse(w_data);
	free(w_data);
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





struct iVector4 {
	int x;
	int y;
	int z;
	int w;
};
struct fVector4 {
	float x;
	float y;
	float z;
	float w;
};

struct iVector3 {
	int x;
	int y;
	int z;
};
struct fVector3 {
	float x;
	float y;
	float z;
};


struct iVector2 {
	int x;
	int y;
};
struct fVector2 {
	float x;
	float y;
};

struct iBoundingBox {
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

struct fBoundingBox {
	float xMin;
	float yMin;
	float xMax;
	float yMax;
};



class Vector3  {
public:
	
	float e[3];

    Vector3() { e[0] = 0; e[1] = 0; e[2] = 0;}
    Vector3(float e0, float e1, float e2) {e[0]=e0; e[1]=e1; e[2]=e2;}
    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }
    void setX(float a) { e[0] = a; }
    void setY(float a) { e[1] = a; }
    void setZ(float a) { e[2] = a; }
	void setXYZ(float a, float b, float c) {
		e[0] = a;
		e[1] = b;
		e[2] = c;
	}

    inline Vector3(const Vector3 &v) {
	 e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2];
    }

    const Vector3& operator+() const { return *this; }
    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    float& operator[](int i) {  return e[i]; }
    float operator[](int i) const { return e[i];}

    Vector3& operator+=(const Vector3 &v2);
    Vector3& operator-=(const Vector3 &v2);
    Vector3& operator*=(const float t);
    Vector3& operator/=(const float t);



    float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    float squaredLength() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }

    void makeUnitVector();




    float minComponent() const { return e[indexOfMinComponent()]; }
    float maxComponent() const { return e[indexOfMaxComponent()]; }
    float maxAbsComponent() const { return e[indexOfMaxAbsComponent()]; }
    float minAbsComponent() const { return e[indexOfMinAbsComponent()]; }
    int indexOfMinComponent() const {
	return (e[0]< e[1] && e[0]< e[2]) ? 0 : (e[1] < e[2] ? 1 : 2);
    }

    int indexOfMinAbsComponent() const {
	if (fabs(e[0]) < fabs(e[1]) && fabs(e[0]) < fabs(e[2]))
	    return 0;
	else if (fabs(e[1]) < fabs(e[2]))
	    return 1;
	else
	    return 2;
    }

    int indexOfMaxComponent() const {
	return (e[0]> e[1] && e[0]> e[2]) ? 0 : (e[1] > e[2] ? 1 : 2);
    }

    int indexOfMaxAbsComponent() const {
	if (fabs(e[0]) > fabs(e[1]) && fabs(e[0]) > fabs(e[2]))
	    return 0;
	else if (fabs(e[1]) > fabs(e[2]))
	    return 1;
	else
	    return 2;
    }

    
};



inline bool operator==(const Vector3 &t1, const Vector3 &t2) {
   return ((t1[0]==t2[0])&&(t1[1]==t2[1])&&(t1[2]==t2[2]));
}

inline bool operator!=(const Vector3 &t1, const Vector3 &t2) {
   return ((t1[0]!=t2[0])||(t1[1]!=t2[1])||(t1[2]!=t2[2]));
}

inline istream &operator>>(istream &is, Vector3 &t) {
   is >> t[0] >> t[1] >> t[2];
   return is;
}

inline ostream &operator<<(ostream &os, const Vector3 &t) {
   os << t[0] << " " << t[1] << " " << t[2];
   return os;
}

inline Vector3 unitVector(const Vector3& v) {
    float k = 1.0f / sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]);
    return Vector3(v.e[0]*k, v.e[1]*k, v.e[2]*k);
}

inline void Vector3::makeUnitVector() {
    float k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vector3 operator*(float t, const Vector3 &v) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, float t) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator/(const Vector3 &v, float t) {
    return Vector3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}


inline float dot(const Vector3 &v1, const Vector3 &v2) {
    return v1.e[0] *v2.e[0] + v1.e[1] *v2.e[1]  + v1.e[2] *v2.e[2];
}

inline Vector3 cross(const Vector3 &v1, const Vector3 &v2) {
    return Vector3( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
		      (v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2]),
		      (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}


inline Vector3& Vector3::operator+=(const Vector3 &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    e[2]  += v.e[2];
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    e[2]  -= v.e[2];
    return *this;
}

inline Vector3& Vector3::operator*=(const float t) {
    e[0]  *= t;
    e[1]  *= t;
    e[2]  *= t;
    return *this;
}

inline Vector3& Vector3::operator/=(const float t) {
    e[0]  /= t;
    e[1]  /= t;
    e[2]  /= t;
    return *this;
}

inline Vector3 reflect(const Vector3& in, const Vector3& normal)
{
  // assumes unit length normal
  return in - normal * (2 * dot(in, normal));
}


const float AIR_VAL = 0.0f;
const float WATER_VAL = 1.0f;
const float SOLID_THRESH = 2.0f;
const float GRASS_VAL = 2.0f;
const float ROCK_VAL = 3.0f;
const float DIRT_VAL = 4.0f;


enum eProgramState {
	E_PS_MENU,
	E_PS_IN_GAME,
	E_PS_SIZE
};

enum eProgramAction {
	E_PA_QUIT,
	E_PA_TOGGLE_FULLSCREEN,
	E_PA_REFRESH,
	E_PA_SIZE
};

enum eRenderStates {
	E_RENDER_SOLID,
	E_RENDER_WATER
};

enum eMouseStates {
	E_MOUSE_UP,
	E_MOUSE_DOWN
};

enum TEX_ENUMS {
	E_TEX_NOISE,
	E_TEX_SIMPLEX,
	E_TEX_VORONOI,
	E_TEX_TOTAL
};

enum PAGE_FUNCS {
	OP_NULL,
	S_INIT,
	E_INIT,
	S_OP_CLEAR,
	E_OP_CLEAR,
	S_OP_SIMPLEX_NOISE,
	E_OP_SIMPLEX_NOISE,
	S_OP_SEAMLESS_SIMPLEX_NOISE,
	E_OP_SEAMLESS_SIMPLEX_NOISE,
	S_OP_VORONOI,
	E_OP_VORONOI,
	S_OP_APPLY_TEXTURE,
	E_OP_APPLY_TEXTURE,
	S_RENDER_VOXELS,
	E_RENDER_VOXELS,
	S_CREATE_DL,
	E_CREATE_DL
};

string PAGE_FUNC_STRINGS[] = {
	"OP_NULL",
	"S_INIT",
	"E_INIT",
	"S_OP_SIMPLEX_NOISE",
	"E_OP_SIMPLEX_NOISE",
	"S_OP_SEAMLESS_SIMPLEX_NOISE",
	"E_OP_SEAMLESS_SIMPLEX_NOISE",
	"S_OP_VORONOI",
	"E_OP_VORONOI",
	"S_OP_APPLY_TEXTURE",
	"E_OP_APPLY_TEXTURE",
	"S_OP_CLEAR",
	"E_OP_CLEAR",
	"S_RENDER_VOXELS",
	"E_RENDER_VOXELS",
	"S_CREATE_DL",
	"E_CREATE_DL"
};

enum RULE_SETS {
	RS_TERRAIN
};

enum FILL_STATES {
	FILL_STATE_AIR,
	FILL_STATE_WATER,
	FILL_STATE_SOLID,
	FILL_STATE_MIXED
};

enum eSSProperties {
	
	E_SS_BGCOL0_R,
	E_SS_BGCOL0_G,
	E_SS_BGCOL0_B,
	E_SS_BGCOL0_A,

	E_SS_BGCOL1_R,
	E_SS_BGCOL1_G,
	E_SS_BGCOL1_B,
	E_SS_BGCOL1_A,

	E_SS_FGCOL0_R,
	E_SS_FGCOL0_G,
	E_SS_FGCOL0_B,
	E_SS_FGCOL0_A,

	E_SS_FGCOL1_R,
	E_SS_FGCOL1_G,
	E_SS_FGCOL1_B,
	E_SS_FGCOL1_A,

	E_SS_BGCOLTEXT0_R,
	E_SS_BGCOLTEXT0_G,
	E_SS_BGCOLTEXT0_B,
	E_SS_BGCOLTEXT0_A,

	E_SS_BGCOLTEXT1_R,
	E_SS_BGCOLTEXT1_G,
	E_SS_BGCOLTEXT1_B,
	E_SS_BGCOLTEXT1_A,

	E_SS_FGCOLTEXT0_R,
	E_SS_FGCOLTEXT0_G,
	E_SS_FGCOLTEXT0_B,
	E_SS_FGCOLTEXT0_A,

	E_SS_FGCOLTEXT1_R,
	E_SS_FGCOLTEXT1_G,
	E_SS_FGCOLTEXT1_B,
	E_SS_FGCOLTEXT1_A,

	E_SS_TGCOL0_R,
	E_SS_TGCOL0_G,
	E_SS_TGCOL0_B,
	E_SS_TGCOL0_A,

	E_SS_TGCOL1_R,
	E_SS_TGCOL1_G,
	E_SS_TGCOL1_B,
	E_SS_TGCOL1_A,

	E_SS_BDCOL_R,
	E_SS_BDCOL_G,
	E_SS_BDCOL_B,
	E_SS_BDCOL_A,

	E_SS_PADDING,
	E_SS_BORDER,
	E_SS_MARGIN,
	E_SS_CORNERRAD,
	E_SS_ROUNDNESS,

	E_SS_PROPS_SIZE
};

enum eCompStates {
	E_COMP_UP,
	E_COMP_OVER,
	E_COMP_DOWN,
	E_COMP_TOTAL
};
int traceLevel = 0;
int popCount = 0;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


vector<string> split(const string &s, char delim) {
	vector<string> elems;
	return split(s, delim, elems);
}


inline float clamp(float val) {
	float retval = val;
	if (retval < 0.0f) retval = 0.0f;
	if (retval > 1.0f) retval = 1.0f;
	return retval;
}

inline float fGenRand() {
	return ((float)(rand()%100000))/100000.0f;
}

inline int iGenRand(int val) {
	return rand()%(val+1) - val/2;
}

string intToString(int i) {
	string s;
	stringstream out;
	out << i;
	s = out.str();
	return s;
}
string floatToString(float f) {
	string s;
	stringstream out;
	out << f;
	s = out.str();
	return s;
}

void doTrace(string traceVal0 = "", string traceVal1 = "",string traceVal2 = "",string traceVal3 = "",string traceVal4 = "",string traceVal5 = "", string traceVal6 = "",string traceVal7 = "",string traceVal8 = "",string traceVal9 = "",string traceVal10 = "") {
	int i;
	
	for (i = 0; i < traceLevel; i++) {
		cout << "|  ";
	}
	
	cout << traceVal0 << " " << traceVal1 << " " << traceVal2 << " " << traceVal3 << " " << traceVal4 << " " << traceVal5 << " " << traceVal6 << " " << traceVal7 << " " << traceVal8 << " " << traceVal9 << " " << traceVal10 << "\n";
	
}

void pushTrace(string traceVal0 = "", string traceVal1 = "",string traceVal2 = "",string traceVal3 = "",string traceVal4 = "",string traceVal5 = "", string traceVal6 = "",string traceVal7 = "",string traceVal8 = "",string traceVal9 = "",string traceVal10 = "") {
	doTrace(traceVal0,traceVal1,traceVal2,traceVal3,traceVal4,traceVal5,traceVal6,traceVal7,traceVal8,traceVal9,traceVal10);
	traceLevel++;
	popCount=0;
}
void popTrace() {
	traceLevel--;
	popCount++;
	if (popCount >= 2) {
		doTrace("END");
	}
}

float distance(Vector3* v1, Vector3* v2) {
	float val1 = v1->e[0] - v2->e[0];
	float val2 = v1->e[1] - v2->e[1];
	float val3 = v1->e[2] - v2->e[2];
	
	return sqrt(
		val1*val1 + val2*val2 + val3*val3
	);
	
	//return length(v1-v2);
}

const static int MAX_OG_RAD = 20;
const static int MAX_OG_DIAM = MAX_OG_RAD*2+1;
const static int MAX_OG_SIZE = MAX_OG_DIAM * MAX_OG_DIAM * MAX_OG_DIAM;
const static int OG_NULL_VAL = 9999;

class OffsetGrid {
private:
	
public:
	iVector3 offsetGrid[MAX_OG_RAD+1][MAX_OG_SIZE];
	iVector3 offsetGridCum[(MAX_OG_RAD+1)*(MAX_OG_SIZE)];

	int offsetGridCounters[MAX_OG_RAD+1];
	int offsetGridCumCounters[MAX_OG_RAD+1];

	
	
	OffsetGrid() {
		int i;
		int j;
		int k;
		int curCount;
		
		for (j = 0; j < MAX_OG_RAD+1; j++) {
			offsetGridCounters[j] = 0;
			for (i = 0; i < MAX_OG_SIZE; i++) {
				offsetGrid[j][i].x = OG_NULL_VAL;
				offsetGrid[j][i].y = OG_NULL_VAL;
				offsetGrid[j][i].z = OG_NULL_VAL;
			}
		}
		
		
		int xdis;
		int ydis;
		int zdis;
		
		int max1;
		int max2;
		int ind;

		
		
		for (i = 0; i < MAX_OG_DIAM; i++) {
			for (j = 0; j < MAX_OG_DIAM; j++) {
				for (k = 0; k < MAX_OG_DIAM; k++) {
					xdis = i-MAX_OG_RAD;
					ydis = j-MAX_OG_RAD;
					zdis = k-MAX_OG_RAD;
					
					max1 = max(abs(xdis),abs(ydis));
					max2 = max(max1,abs(zdis));
					
					ind = offsetGridCounters[max2];
					
					offsetGrid[max2][ind].x = xdis;
					offsetGrid[max2][ind].y = ydis;
					offsetGrid[max2][ind].z = zdis;
					
					//cout << max2 << "," << ind << ": " << offsetGrid[max2][ind].x << " " << offsetGrid[max2][ind].y << " " << offsetGrid[max2][ind].z << "\n"; 
					

					offsetGridCounters[max2]++;
					
					
					
				}
			}
		}

		curCount = 0;
		

		for (j = 0; j <= MAX_OG_RAD; j++) {
			for (i = 0; i < offsetGridCounters[j]; i++) {
				offsetGridCum[curCount].x = offsetGrid[j][i].x;
				offsetGridCum[curCount].y = offsetGrid[j][i].y;
				offsetGridCum[curCount].z = offsetGrid[j][i].z;

				curCount++;
			}
			offsetGridCumCounters[j] = curCount;

		}

	}
};



class Shader {
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
public:
	
	Shader() {
		pushTrace("Shader()");
		popTrace();
	}

	Shader(const char *shaderFile) {
		pushTrace("Shader(", shaderFile, ")");
		init(shaderFile);
		popTrace();
	}


	static char* textFileRead(const char *fileName) {
		pushTrace("textFileRead(",  fileName, ")");
		
		char* text;
	    
		if (fileName != NULL) {
	        FILE *file = fopen(fileName, "rt");
	        
			if (file != NULL) {
	            fseek(file, 0, SEEK_END);
	            int count = ftell(file);
	            rewind(file);
	            
				if (count > 0) {
					text = (char*)malloc(sizeof(char) * (count + 1));
					count = fread(text, sizeof(char), count, file);
					text[count] = '\0';
				}
				fclose(file);
			}
		}
		
		popTrace();
		
		return text;
	}

	static void validateShader(GLuint shader, const char* file = 0) {
		pushTrace("validateShader(", file, ")");
		
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;
	    
		glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
		if (length > 0) {
			cout << "Shader " << shader << " (" << (file?file:"") << ") compile error: " << buffer << endl;
		}
		popTrace();
	}

	static int validateProgram(GLuint program) {
		pushTrace("validateProgram()");
		
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;
	    
		memset(buffer, 0, BUFFER_SIZE);
		glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
		if (length > 0) {
			cout << "Program " << program << " link error: " << buffer << endl;
			popTrace();
			return 0;
		}
	    
		glValidateProgram(program);
		GLint status;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		if (status == GL_FALSE) {
			cout << "Error validating shader " << program << endl;
			popTrace();
			return 0;
		}
		
		popTrace();
		return 1;
		
	}
	

	void init(const char *shaderFile) {
		pushTrace("init(", shaderFile, ")");
		shader_vp = glCreateShader(GL_VERTEX_SHADER);
		shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
	    
		const char* allText = textFileRead(shaderFile);

		if (allText == NULL) {
	        cerr << "Either vertex shader or fragment shader file not found." << endl;
	    }
	    else {
	    	string allTextString(allText);

			vector<string> allTextStringSplit = split(allTextString, '$');

			if (allTextStringSplit.size() == 3) {

				string vertStr = allTextStringSplit[0] + allTextStringSplit[1];
				string fragStr = allTextStringSplit[0] + allTextStringSplit[2];

				char* vertCS = new char[vertStr.length() + 1];
				char* fragCS = new char[fragStr.length() + 1];

				std::strcpy(vertCS,vertStr.c_str());
				std::strcpy(fragCS,fragStr.c_str());


		    	glShaderSource(shader_vp, 1, &(vertCS), 0);
				glShaderSource(shader_fp, 1, &(fragCS), 0);
			    
				glCompileShader(shader_vp);
				validateShader(shader_vp, shaderFile);
				glCompileShader(shader_fp);
				validateShader(shader_fp, shaderFile);
			    
				shader_id = glCreateProgram();
				glAttachShader(shader_id, shader_fp);
				glAttachShader(shader_id, shader_vp);
				glLinkProgram(shader_id);
				validateProgram(shader_id);

				delete [] vertCS;
				delete [] fragCS;

			}
			else {
				cerr << "Error: " << shaderFile << "does not contain proper amount of splits ($)\n";
			}
			
			
	    }
		
		
		popTrace();
		
	}

	~Shader() {
		pushTrace("~Shader()");
		glDetachShader(shader_id, shader_fp);
		glDetachShader(shader_id, shader_vp);
	    
		glDeleteShader(shader_fp);
		glDeleteShader(shader_vp);
		glDeleteProgram(shader_id);
		popTrace();
	}

	unsigned int id() {
		pushTrace("id()");
		return shader_id;
		popTrace();
	}

	void bind() {
		//pushTrace("bind()");
		glUseProgram(shader_id);
		//popTrace();
	}

	void unbind() {
		//pushTrace("unbind()");
		glUseProgram(0);
		//popTrace();
	}

	void setTexture(const GLchar* name, int texUnit, int texID) {
		GLint baseImageLoc = glGetUniformLocation(shader_id, name);

		glUniform1i(baseImageLoc, texUnit); //Texture unit 0 is for base images.
		
		//When rendering an objectwith this program.
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	
	void setVec(const GLchar* name, const GLfloat* vecData, int vecSize) {
		
		GLint loc = glGetUniformLocation(shader_id, name);
		
		switch (vecSize) {
			case 0:
				cout << "Error: vecSize of 0 in setVec";
			break;
			case 1:
				glUniform1fv(loc, 1, vecData);
			break;
			case 2:
				glUniform2fv(loc, 1, vecData);
			break;
			case 3:
				glUniform3fv(loc, 1, vecData);
			break;
			case 4:
				glUniform4fv(loc, 1, vecData);
			break;
		}
	}

	void setVecString(string name, const GLfloat* vecData, int vecSize) {
		
		GLint loc = glGetUniformLocation(shader_id, name.c_str());
		
		switch (vecSize) {
			case 0:
				cout << "Error: vecSize of 0 in setVec";
			break;
			case 1:
				glUniform1fv(loc, 1, vecData);
			break;
			case 2:
				glUniform2fv(loc, 1, vecData);
			break;
			case 3:
				glUniform3fv(loc, 1, vecData);
			break;
			case 4:
				glUniform4fv(loc, 1, vecData);
			break;
		}
	}

	void setShaderFloat(string paramName, float x) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform1f(loc,x);
	}
	void setShaderVec2(string paramName, float x, float y) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform2f(loc,x,y);
	}
	void setShaderVec3(string paramName, float x, float y, float z) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform3f(loc,x,y,z);
	}
	void setShaderVec4(string paramName, float x, float y, float z, float w) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform4f(loc,x,y,z,w);
	}

	void setShaderInt(string paramName, int multitexNumber) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform1iARB(loc,multitexNumber);
	}

	

};






class Timer
{
public:
    Timer(){
        #ifdef WIN32
            QueryPerformanceFrequency(&frequency);
            startCount.QuadPart = 0;
            endCount.QuadPart = 0;
        #else
            startCount.tv_sec = startCount.tv_usec = 0;
            endCount.tv_sec = endCount.tv_usec = 0;
        #endif

            stopped = 0;
            startTimeInMicroSec = 0;
            endTimeInMicroSec = 0;
        }
    ~Timer() {

    }

    void   start() {
        stopped = 0; // reset stop flag
        #ifdef WIN32
        QueryPerformanceCounter(&startCount);
        #else
        gettimeofday(&startCount, NULL);
        #endif
    }
    void   stop() {
        stopped = 1; // set timer stopped flag

        #ifdef WIN32
        QueryPerformanceCounter(&endCount);
        #else
        gettimeofday(&endCount, NULL);
        #endif
    }
    double getElapsedTime() {
        return this->getElapsedTimeInSec();
    }
    double getElapsedTimeInSec() {
        return this->getElapsedTimeInMicroSec() * 0.000001;
    }

    double getElapsedTimeInMilliSec() {
        return this->getElapsedTimeInMicroSec() * 0.001;
    }

    double getElapsedTimeInMicroSec() {
        #ifdef WIN32
        if(!stopped) {
            QueryPerformanceCounter(&endCount);
        }

        startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
        endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
        #else
        if(!stopped) {
            gettimeofday(&endCount, NULL);
        }

        startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
        endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
        #endif

        return endTimeInMicroSec - startTimeInMicroSec;
    }



protected:


private:
    double startTimeInMicroSec;                 // starting time in micro-second
    double endTimeInMicroSec;                   // ending time in micro-second
    int    stopped;                             // stop flag 
#ifdef WIN32
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
#else
    timeval startCount;                         //
    timeval endCount;                           //
#endif
};
char* g_RecBuffer;
static int g_MAX_FRAME_SIZE = 16777216; //16 MB

class WebSocketRequestHandler: public HTTPRequestHandler
	/// Handle a WebSocket connection.
{
public:

	

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{

		JSONValue *value;



		Application& app = Application::instance();
		try
		{
			WebSocket ws(request, response);
			app.logger().information("WebSocket connection established.");
			//char buffer[1024];
			int flags;
			int n;
			
			do
			{
				n = ws.receiveFrame(g_RecBuffer, g_MAX_FRAME_SIZE, flags);
				app.logger().information(Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)));
				ws.sendFrame(g_RecBuffer, n, flags);


				if (n > 0) {

					g_RecBuffer[n+1] = '\0';

					//cout << g_RecBuffer << '\n';



					value = JSON::Parse(g_RecBuffer);
					if (value == NULL)
					{
						app.logger().information("Invalid JSON\n\n");
					}
					else {
						app.logger().information("Valid JSON\n\n");
						cout << value->Child(L"x")->number_value << "\n\n";

						delete value;
						value = NULL;

					}

				}

			}
			while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
			app.logger().information("WebSocket connection closed.");



			


		}
		catch (WebSocketException& exc)
		{
			app.logger().log(exc);
			switch (exc.code())
			{
			case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
				response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
				// fallthrough
			case WebSocket::WS_ERR_NO_HANDSHAKE:
			case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
			case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
				response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
				response.setContentLength(0);
				response.send();
				break;
			}
		}


		

	}
};


class RequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		Application& app = Application::instance();
		app.logger().information("Request from " 
			+ request.clientAddress().toString()
			+ ": "
			+ request.getMethod()
			+ " "
			+ request.getURI()
			+ " "
			+ request.getVersion());
			
		for (HTTPServerRequest::ConstIterator it = request.begin(); it != request.end(); ++it)
		{
			app.logger().information(it->first + ": " + it->second);
		}

		return new WebSocketRequestHandler;
	}
};


class WebSocketServer: public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the WebSocketServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To use the sample configuration file (WebSocketServer.properties),
	/// copy the file to the directory where the WebSocketServer executable
	/// resides. If you start the debug version of the WebSocketServer
	/// (WebSocketServerd[.exe]), you must also create a copy of the configuration
	/// file named WebSocketServerd.properties. In the configuration file, you
	/// can specify the port on which the server is listening (default
	/// 9980) and the format of the date/time string sent back to the client.
	///
	/// To test the WebSocketServer you can use any web browser (http://localhost:9980/).
{
public:
	WebSocketServer(): _helpRequested(false)
	{
	}
	
	~WebSocketServer()
	{
	}

	int main(const std::vector<std::string>& args);

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}
		
	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);
		
		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A sample HTTP server supporting the WebSocket protocol.");
		helpFormatter.format(std::cout);
	}

	/*
	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested)
		{
			displayHelp();
		}
		else
		{
			// get parameters from configuration file
			unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 9980);
			
			// set-up a server socket
			ServerSocket svs(port);
			// set-up a HTTPServer instance
			HTTPServer srv(new RequestHandlerFactory, svs, new HTTPServerParams);
			// start the HTTPServer
			srv.start();
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the HTTPServer
			srv.stop();
		}
		return Application::EXIT_OK;
	}
	*/
	
private:
	bool _helpRequested;
};







// Multi-octave Simplex noise - multiple noise values are combined.
float simplexNoise( const float, const float, const float, const float, const float );
float simplexNoise( const float, const float, const float, const float, const float, const float );
float simplexNoise( const float, const float, const float, const float, const float, const float, const float );


// Multi-octave Scaled Simplex noise - the result will be between the two parameters passed.
float simplexScaledNoise( const float, const float, const float, const float, const float, const float, const float );
float simplexScaledNoise( const float, const float, const float, const float, const float, const float, const float, const float );
float simplexScaledNoise( const float, const float, const float, const float, const float, const float, const float, const float, const float );

// Scaled Raw Simplex noise - the result will be between the two parameters passed.
float simplexScaledRawNoise( const float, const float, const float, const float );
float simplexScaledRawNoise( const float, const float, const float, const float, const float );
float simplexScaledRawNoise( const float, const float, const float, const float, const float, const float );


// Raw Simplex noise - a single noise value.
float simplexRawNoise( const float, const float );
float simplexRawNoise( const float, const float, const float );
float simplexRawNoise( const float, const float, const float, const float );


int fastfloor( const float x ) { return x > 0 ? (int) x : (int) x - 1; }
float dot( const int* g, const float x, const float y ) { return g[0]*x + g[1]*y; }
float dot( const int* g, const float x, const float y, const float z ) { return g[0]*x + g[1]*y + g[2]*z; }
float dot( const int* g, const float x, const float y, const float z, const float w ) { return g[0]*x + g[1]*y + g[2]*z + g[3]*w; }




// The gradients are the midpoints of the vertices of a cube.
static const int grad3[12][3] = {
	{1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
	{1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
	{0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
};


// The gradients are the midpoints of the vertices of a hypercube.
static const int grad4[32][4]= {
	{0,1,1,1},  {0,1,1,-1},  {0,1,-1,1},  {0,1,-1,-1},
	{0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
	{1,0,1,1},  {1,0,1,-1},  {1,0,-1,1},  {1,0,-1,-1},
	{-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
	{1,1,0,1},  {1,1,0,-1},  {1,-1,0,1},  {1,-1,0,-1},
	{-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
	{1,1,1,0},  {1,1,-1,0},  {1,-1,1,0},  {1,-1,-1,0},
	{-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}
};


// Permutation table.  The same list is repeated twice.
static const int perm[512] = {
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
	172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
	228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
	107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
	172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
	228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
	107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};


// A lookup table to traverse the simplex around a given point in 4D.
static const int simplex[64][4] = {
	{0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
	{0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
	{1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
	{2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
};


// 2D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float simplexNoise( const float octaves, const float persistence, const float scale, const float x, const float y ) {
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += simplexRawNoise( x * frequency, y * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}


// 3D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float simplexNoise( const float octaves, const float persistence, const float scale, const float x, const float y, const float z ) {
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += simplexRawNoise( x * frequency, y * frequency, z * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}


// 4D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float simplexNoise( const float octaves, const float persistence, const float scale, const float x, const float y, const float z, const float w ) {
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += simplexRawNoise( x * frequency, y * frequency, z * frequency, w * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}



// 2D Scaled Multi-octave Simplex noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y ) {
	return simplexNoise(octaves, persistence, scale, x, y) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}


// 3D Scaled Multi-octave Simplex noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z ) {
	return simplexNoise(octaves, persistence, scale, x, y, z) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}

// 4D Scaled Multi-octave Simplex noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z, const float w ) {
	return simplexNoise(octaves, persistence, scale, x, y, z, w) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}



// 2D Scaled Simplex raw noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledRawNoise( const float loBound, const float hiBound, const float x, const float y ) {
	return simplexRawNoise(x, y) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}


// 3D Scaled Simplex raw noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledRawNoise( const float loBound, const float hiBound, const float x, const float y, const float z ) {
	return simplexRawNoise(x, y, z) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}

// 4D Scaled Simplex raw noise.
//
// Returned value will be between loBound and hiBound.
float simplexScaledRawNoise( const float loBound, const float hiBound, const float x, const float y, const float z, const float w ) {
	return simplexRawNoise(x, y, z, w) * (hiBound - loBound) / 2 + (hiBound + loBound) / 2;
}



// 2D raw Simplex noise
float simplexRawNoise( const float x, const float y ) {
	// Noise contributions from the three corners
	float n0, n1, n2;

	// Skew the input space to determine which simplex cell we're in
	float F2 = 0.5 * (sqrtf(3.0) - 1.0);
	// Hairy factor for 2D
	float s = (x + y) * F2;
	int i = fastfloor( x + s );
	int j = fastfloor( y + s );

	float G2 = (3.0 - sqrtf(3.0)) / 6.0;
	float t = (i + j) * G2;
	// Unskew the cell origin back to (x,y) space
	float X0 = i-t;
	float Y0 = j-t;
	// The x,y distances from the cell origin
	float x0 = x-X0;
	float y0 = y-Y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
	int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
	if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
	else {i1=0; j1=1;} // upper triangle, YX order: (0,0)->(0,1)->(1,1)

	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6
	float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
	float y1 = y0 - j1 + G2;
	float x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
	float y2 = y0 - 1.0 + 2.0 * G2;

	// Work out the hashed gradient indices of the three simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int gi0 = perm[ii+perm[jj]] % 12;
	int gi1 = perm[ii+i1+perm[jj+j1]] % 12;
	int gi2 = perm[ii+1+perm[jj+1]] % 12;

	// Calculate the contribution from the three corners
	float t0 = 0.5 - x0*x0-y0*y0;
	if(t0<0) n0 = 0.0;
	else {
	t0 *= t0;
	n0 = t0 * t0 * dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
	}

	float t1 = 0.5 - x1*x1-y1*y1;
	if(t1<0) n1 = 0.0;
	else {
	t1 *= t1;
	n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
	}

	float t2 = 0.5 - x2*x2-y2*y2;
	if(t2<0) n2 = 0.0;
	else {
	t2 *= t2;
	n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 70.0 * (n0 + n1 + n2);
}


// 3D raw Simplex noise
float simplexRawNoise( const float x, const float y, const float z ) {
	float n0, n1, n2, n3; // Noise contributions from the four corners

	// Skew the input space to determine which simplex cell we're in
	float F3 = 1.0/3.0;
	float s = (x+y+z)*F3; // Very nice and simple skew factor for 3D
	int i = fastfloor(x+s);
	int j = fastfloor(y+s);
	int k = fastfloor(z+s);

	float G3 = 1.0/6.0; // Very nice and simple unskew factor, too
	float t = (i+j+k)*G3;
	float X0 = i-t; // Unskew the cell origin back to (x,y,z) space
	float Y0 = j-t;
	float Z0 = k-t;
	float x0 = x-X0; // The x,y,z distances from the cell origin
	float y0 = y-Y0;
	float z0 = z-Z0;

	// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
	// Determine which simplex we are in.
	int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
	int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

	if(x0>=y0) {
	if(y0>=z0)
	{ i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; } // X Y Z order
	else if(x0>=z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; } // X Z Y order
	else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; } // Z X Y order
	}
	else { // x0<y0
	if(y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; } // Z Y X order
	else if(x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; } // Y Z X order
	else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; } // Y X Z order
	}

	// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
	// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
	// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
	// c = 1/6.
	float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;
	float x2 = x0 - i2 + 2.0*G3; // Offsets for third corner in (x,y,z) coords
	float y2 = y0 - j2 + 2.0*G3;
	float z2 = z0 - k2 + 2.0*G3;
	float x3 = x0 - 1.0 + 3.0*G3; // Offsets for last corner in (x,y,z) coords
	float y3 = y0 - 1.0 + 3.0*G3;
	float z3 = z0 - 1.0 + 3.0*G3;

	// Work out the hashed gradient indices of the four simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;
	int gi0 = perm[ii+perm[jj+perm[kk]]] % 12;
	int gi1 = perm[ii+i1+perm[jj+j1+perm[kk+k1]]] % 12;
	int gi2 = perm[ii+i2+perm[jj+j2+perm[kk+k2]]] % 12;
	int gi3 = perm[ii+1+perm[jj+1+perm[kk+1]]] % 12;

	// Calculate the contribution from the four corners
	float t0 = 0.6 - x0*x0 - y0*y0 - z0*z0;
	if(t0<0) n0 = 0.0;
	else {
	t0 *= t0;
	n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
	}

	float t1 = 0.6 - x1*x1 - y1*y1 - z1*z1;
	if(t1<0) n1 = 0.0;
	else {
	t1 *= t1;
	n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
	}

	float t2 = 0.6 - x2*x2 - y2*y2 - z2*z2;
	if(t2<0) n2 = 0.0;
	else {
	t2 *= t2;
	n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
	}

	float t3 = 0.6 - x3*x3 - y3*y3 - z3*z3;
	if(t3<0) n3 = 0.0;
	else {
	t3 *= t3;
	n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to stay just inside [-1,1]
	return 32.0*(n0 + n1 + n2 + n3);
}


// 4D raw Simplex noise
float simplexRawNoise( const float x, const float y, const float z, const float w ) {
	// The skewing and unskewing factors are hairy again for the 4D case
	float F4 = (sqrtf(5.0)-1.0)/4.0;
	float G4 = (5.0-sqrtf(5.0))/20.0;
	float n0, n1, n2, n3, n4; // Noise contributions from the five corners

	// Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
	float s = (x + y + z + w) * F4; // Factor for 4D skewing
	int i = fastfloor(x + s);
	int j = fastfloor(y + s);
	int k = fastfloor(z + s);
	int l = fastfloor(w + s);
	float t = (i + j + k + l) * G4; // Factor for 4D unskewing
	float X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
	float Y0 = j - t;
	float Z0 = k - t;
	float W0 = l - t;

	float x0 = x - X0; // The x,y,z,w distances from the cell origin
	float y0 = y - Y0;
	float z0 = z - Z0;
	float w0 = w - W0;

	// For the 4D case, the simplex is a 4D shape I won't even try to describe.
	// To find out which of the 24 possible simplices we're in, we need to
	// determine the magnitude ordering of x0, y0, z0 and w0.
	// The method below is a good way of finding the ordering of x,y,z,w and
	// then find the correct traversal order for the simplex we're in.
	// First, six pair-wise comparisons are performed between each possible pair
	// of the four coordinates, and the results are used to add up binary bits
	// for an integer index.
	int c1 = (x0 > y0) ? 32 : 0;
	int c2 = (x0 > z0) ? 16 : 0;
	int c3 = (y0 > z0) ? 8 : 0;
	int c4 = (x0 > w0) ? 4 : 0;
	int c5 = (y0 > w0) ? 2 : 0;
	int c6 = (z0 > w0) ? 1 : 0;
	int c = c1 + c2 + c3 + c4 + c5 + c6;

	int i1, j1, k1, l1; // The integer offsets for the second simplex corner
	int i2, j2, k2, l2; // The integer offsets for the third simplex corner
	int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

	// simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
	// Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
	// impossible. Only the 24 indices which have non-zero entries make any sense.
	// We use a thresholding to set the coordinates in turn from the largest magnitude.
	// The number 3 in the "simplex" array is at the position of the largest coordinate.
	i1 = simplex[c][0]>=3 ? 1 : 0;
	j1 = simplex[c][1]>=3 ? 1 : 0;
	k1 = simplex[c][2]>=3 ? 1 : 0;
	l1 = simplex[c][3]>=3 ? 1 : 0;
	// The number 2 in the "simplex" array is at the second largest coordinate.
	i2 = simplex[c][0]>=2 ? 1 : 0;
	j2 = simplex[c][1]>=2 ? 1 : 0;
	k2 = simplex[c][2]>=2 ? 1 : 0;
	l2 = simplex[c][3]>=2 ? 1 : 0;
	// The number 1 in the "simplex" array is at the second smallest coordinate.
	i3 = simplex[c][0]>=1 ? 1 : 0;
	j3 = simplex[c][1]>=1 ? 1 : 0;
	k3 = simplex[c][2]>=1 ? 1 : 0;
	l3 = simplex[c][3]>=1 ? 1 : 0;
	// The fifth corner has all coordinate offsets = 1, so no need to look that up.

	float x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
	float y1 = y0 - j1 + G4;
	float z1 = z0 - k1 + G4;
	float w1 = w0 - l1 + G4;
	float x2 = x0 - i2 + 2.0*G4; // Offsets for third corner in (x,y,z,w) coords
	float y2 = y0 - j2 + 2.0*G4;
	float z2 = z0 - k2 + 2.0*G4;
	float w2 = w0 - l2 + 2.0*G4;
	float x3 = x0 - i3 + 3.0*G4; // Offsets for fourth corner in (x,y,z,w) coords
	float y3 = y0 - j3 + 3.0*G4;
	float z3 = z0 - k3 + 3.0*G4;
	float w3 = w0 - l3 + 3.0*G4;
	float x4 = x0 - 1.0 + 4.0*G4; // Offsets for last corner in (x,y,z,w) coords
	float y4 = y0 - 1.0 + 4.0*G4;
	float z4 = z0 - 1.0 + 4.0*G4;
	float w4 = w0 - 1.0 + 4.0*G4;

	// Work out the hashed gradient indices of the five simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;
	int ll = l & 255;
	int gi0 = perm[ii+perm[jj+perm[kk+perm[ll]]]] % 32;
	int gi1 = perm[ii+i1+perm[jj+j1+perm[kk+k1+perm[ll+l1]]]] % 32;
	int gi2 = perm[ii+i2+perm[jj+j2+perm[kk+k2+perm[ll+l2]]]] % 32;
	int gi3 = perm[ii+i3+perm[jj+j3+perm[kk+k3+perm[ll+l3]]]] % 32;
	int gi4 = perm[ii+1+perm[jj+1+perm[kk+1+perm[ll+1]]]] % 32;

	// Calculate the contribution from the five corners
	float t0 = 0.6 - x0*x0 - y0*y0 - z0*z0 - w0*w0;
	if(t0<0) n0 = 0.0;
	else {
	t0 *= t0;
	n0 = t0 * t0 * dot(grad4[gi0], x0, y0, z0, w0);
	}

	float t1 = 0.6 - x1*x1 - y1*y1 - z1*z1 - w1*w1;
	if(t1<0) n1 = 0.0;
	else {
	t1 *= t1;
	n1 = t1 * t1 * dot(grad4[gi1], x1, y1, z1, w1);
	}

	float t2 = 0.6 - x2*x2 - y2*y2 - z2*z2 - w2*w2;
	if(t2<0) n2 = 0.0;
	else {
	t2 *= t2;
	n2 = t2 * t2 * dot(grad4[gi2], x2, y2, z2, w2);
	}

	float t3 = 0.6 - x3*x3 - y3*y3 - z3*z3 - w3*w3;
	if(t3<0) n3 = 0.0;
	else {
	t3 *= t3;
	n3 = t3 * t3 * dot(grad4[gi3], x3, y3, z3, w3);
	}

	float t4 = 0.6 - x4*x4 - y4*y4 - z4*z4 - w4*w4;
	if(t4<0) n4 = 0.0;
	else {
	t4 *= t4;
	n4 = t4 * t4 * dot(grad4[gi4], x4, y4, z4, w4);
	}

	// Sum up and scale the result to cover the range [-1,1]
	return 27.0 * (n0 + n1 + n2 + n3 + n4);
}



//Converts a four-character array to an integer, using little-endian form
int toInt(const char* bytes) {
	return (int)(((unsigned char)bytes[3] << 24) |
				 ((unsigned char)bytes[2] << 16) |
				 ((unsigned char)bytes[1] << 8) |
				 (unsigned char)bytes[0]);
}

//Converts a two-character array to a short, using little-endian form
short toShort(const char* bytes) {
	return (short)(((unsigned char)bytes[1] << 8) |
				   (unsigned char)bytes[0]);
}

//Reads the next four bytes as an integer, using little-endian form
int readInt(ifstream &input) {
	char buffer[4];
	input.read(buffer, 4);
	return toInt(buffer);
}

//Reads the next two bytes as a short, using little-endian form
short readShort(ifstream &input) {
	char buffer[2];
	input.read(buffer, 2);
	return toShort(buffer);
}



//Just like auto_ptr, but for arrays
template<class T>
class auto_array {
	private:
		T* array;
		mutable bool isReleased;
	public:
		explicit auto_array(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}
		
		auto_array(const auto_array<T> &aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}
		
		~auto_array() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}
		
		T* get() const {
			return array;
		}
		
		T &operator*() const {
			return *array;
		}
		
		void operator=(const auto_array<T> &aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}
		
		T* operator->() const {
			return array;
		}
		
		T* release() {
			isReleased = true;
			return array;
		}
		
		void reset(T* array_ = NULL) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = array_;
		}
		
		T* operator+(int i) {
			return array + i;
		}
		
		T &operator[](int i) {
			return array[i];
		}
};

//Represents an image
class Image {
public:
		
	/* An array of the form (R1, G1, B1, R2, G2, B2, ...) indicating the
	 * color of each pixel in image.  Color components range from 0 to 255.
	 * The array starts the bottom-left pixel, then moves right to the end
	 * of the row, then moves up to the next column, and so on.  This is the
	 * format in which OpenGL likes images.
	 */
	char* pixels;
	int width;
	int height;

	Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {
		
	}

	~Image() {
		delete[] pixels;
	}

};


Image* loadBMP(const char* filename) {
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
	input.ignore(8);
	int dataOffset = readInt(input);
	
	//Read the header
	int headerSize = readInt(input);
	int width;
	int height;
	switch(headerSize) {
		case 40:
			//V3
			width = readInt(input);
			height = readInt(input);
			input.ignore(2);
			assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
			assert(readShort(input) == 0 || !"Image is compressed");
			break;
		case 12:
			//OS/2 V1
			width = readShort(input);
			height = readShort(input);
			input.ignore(2);
			assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
			break;
		case 64:
			//OS/2 V2
			assert(!"Can't load OS/2 V2 bitmaps");
			break;
		case 108:
			//Windows V4
			assert(!"Can't load Windows V4 bitmaps");
			break;
		case 124:
			//Windows V5
			assert(!"Can't load Windows V5 bitmaps");
			break;
		default:
			assert(!"Unknown bitmap format");
	}
	
	//Read the data
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);
	
	//Get the data into the right format
	auto_array<char> pixels2(new char[width * height * 3]);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			for(int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] =
					pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}
	
	input.close();
	return new Image(pixels2.release(), width, height);
}

GLuint loadTexture(Image* image) {
	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return tid;
}

class FBOWrapper
{
public:

	uint color_tex;
	//uint color_buf;
	//uint depth_buf;
	uint slot;
	int width;
	int height;
	int bytesPerChannel;

    FBOWrapper() {}
    ~FBOWrapper() {}
    int init(int _width, int _height, int _bytesPerChannel, int _slot) {
		width = _width;
		height = _height;
		bytesPerChannel = _bytesPerChannel;

		int w = width;
		int h = height;


		slot = GL_COLOR_ATTACHMENT0_EXT;

		switch (_slot) {
			case 0:
				slot = GL_COLOR_ATTACHMENT0_EXT;
			break;
			case 1:
				slot = GL_COLOR_ATTACHMENT1_EXT;
			break;
			case 2:
				slot = GL_COLOR_ATTACHMENT2_EXT;
			break;
			case 3:
				slot = GL_COLOR_ATTACHMENT3_EXT;
			break;
			case 4:
				slot = GL_COLOR_ATTACHMENT4_EXT;
			break;
			case 5:
				slot = GL_COLOR_ATTACHMENT5_EXT;
			break;
			case 6:
				slot = GL_COLOR_ATTACHMENT6_EXT;
			break;
			case 7:
				slot = GL_COLOR_ATTACHMENT7_EXT;
			break;
		}

	    

		glGenTextures(1, &color_tex);
		glBindTexture(GL_TEXTURE_2D, color_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	    switch (bytesPerChannel) {
	    	case 1:
	    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	    	break;
	    	case 2:
	            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);
	    	break;
	    	case 4:
	            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
	    	break;
	    }
		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, slot, GL_TEXTURE_2D, color_tex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	    return 1;

	}

};






class FBOSet
{
public:
	int numBufs;
	int width;
	int height;
	int bytesPerChannel;

	GLuint mFBO;

	FBOWrapper* fbos;

    FBOSet() {}
    ~FBOSet() {}
    void init(int _numBufs, int _width, int _height, int _bytesPerChannel) {
		int i;

		numBufs = _numBufs;
		height = _height;
		width = _width;
		bytesPerChannel = _bytesPerChannel;

		fbos = new FBOWrapper[numBufs];


		glGenFramebuffersEXT(1, &mFBO);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

		for (i = 0; i < numBufs; i++) {
			fbos[i].init(width, height, bytesPerChannel, i);
		}

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
    void bind(int doClear) {

		//setWH(width, height);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_COLOR_ATTACHMENT4_EXT};
		glDrawBuffers(numBufs, buffers);

		if (doClear) {
	    	glClearColor(0.0,0.0,0.0,0.0);
	    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    }

	}
	/*
    void setWH(int w, int h) {
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
		glMatrixMode(GL_MODELVIEW);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
	}
	*/
    void copyFromMem(int ind, uint* dat) {

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

	    glBindTexture(GL_TEXTURE_2D,fbos[ind].color_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, fbos[ind].slot, GL_TEXTURE_2D, fbos[ind].color_tex, 0);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D,0);

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
};
class Singleton {
public:

	vector<string> shaderStrings;
	vector<string> fboStrings;
	vector<string> shaderTextureIDs;

	map<string, Shader*> shaderMap;
	map<string, FBOSet*> fboMap;

	string curShader;
	string allText;

	eProgramState programState;

	eProgramAction progActionsDown[E_PS_SIZE*256];
	eProgramAction progActionsUp[E_PS_SIZE*256];

	bool isFullScreen;

	int baseW;
	int baseH;

	int defaultWinW;
	int defaultWinH;

    float currentFBOResolutionX;
    float currentFBOResolutionY;

	float mouseX;
	float mouseY;
    float mouseXUp;
    float mouseYUp;

    bool shiftPressed;
	bool ctrlPressed;
	bool altPressed;

    fVector3 cameraPos;
    float cameraZoom;

    uint* lookup2to3;
    //uint* lookup3to2;
    GLuint lookup2to3ID;
    //GLuint lookup3to2ID;

	int shadersAreLoaded;
	int readyToRecompile;



	Singleton() {

	}
	~Singleton() {

	}

	inline void setProgAction(eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown) {

		if (isDown) {
			progActionsDown[ps*256 + kc] = pa;
		}
		else {
			progActionsUp[ps*256 + kc] = pa;
		}
	}

	inline void setProgActionAll(unsigned char kc, eProgramAction pa, bool isDown) {
		int i;

		for (i = 0; i < E_PS_SIZE; i++) {
			setProgAction((eProgramState)i, kc, pa, isDown);
		}

	}

	void initKeys() {

		setProgActionAll(27,E_PA_QUIT, false);
		setProgActionAll('p',E_PA_TOGGLE_FULLSCREEN, false);
		setProgActionAll('r',E_PA_REFRESH, false);

	}

    
    
	void init(int _defaultWinW, int _defaultWinH) {

		defaultWinW = _defaultWinW;
		defaultWinH = _defaultWinH;

		isFullScreen = false;
	    shadersAreLoaded = 0;
	    readyToRecompile = 0;

		programState = E_PS_IN_GAME;

	    cameraPos.x = 0.0f;
	    cameraPos.y = 0.0f;
	    cameraZoom = 1.0f;

	    shadersAreLoaded = 0;
	    readyToRecompile = 0;

	    mouseX = 0.0f;
	    mouseY = 0.0f;
	    mouseXUp = 0.0f;
	    mouseYUp = 0.0f;

	    shiftPressed = false;
		ctrlPressed = false;
		altPressed = false;


	    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 4-byte pixel alignment
	    glEnable(GL_TEXTURE_2D);
	    glDisable(GL_DEPTH_TEST);
	    glDisable(GL_CULL_FACE);
	    glDisable(GL_LIGHTING);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glDisable(GL_BLEND);




	    fboStrings.push_back("testFBO");
	    fboStrings.push_back("resultFBO");
	    fboStrings.push_back("volGenFBO");

	    shaderStrings.push_back("shaderWater");

	    /*
	    shaderStrings.push_back("ShaderTarg2");
	    shaderStrings.push_back("ShaderLighting");
	    shaderStrings.push_back("GenerateVolume");
	    shaderStrings.push_back("RenderVolume");
	    shaderStrings.push_back("Simplex2D");
	    shaderStrings.push_back("CalcFlow");
	    shaderStrings.push_back("Erode");
	    shaderStrings.push_back("DLA");
	    shaderStrings.push_back("MapShader");
	    */

	    shaderTextureIDs.push_back("Texture0");
	    shaderTextureIDs.push_back("Texture1");
	    shaderTextureIDs.push_back("Texture2");
	    shaderTextureIDs.push_back("Texture3");
	    shaderTextureIDs.push_back("Texture4");
	    shaderTextureIDs.push_back("Texture5");
	    shaderTextureIDs.push_back("Texture6");
	    shaderTextureIDs.push_back("Texture7");


	    int i;

	    //initLookups();

	    for (i = 0; i < shaderStrings.size(); i++) {
	        shaderMap.insert(  pair<string,Shader*>(shaderStrings[i], NULL)  );
	    }
	    doShaderRefresh();

	    //fboSize = 512;
	    //bufsPerFBO = 2;

	    for (i = 0; i < fboStrings.size(); i++) {
	        fboMap.insert(  pair<string, FBOSet*>(fboStrings[i], new FBOSet())  );
	    }

	    //init(int _numBufs, int _width, int _height, int _bytesPerChannel);
	    fboMap["testFBO"]->init(2, 1024, 1024, 1);
	    fboMap["resultFBO"]->init(1, 1024, 1024, 1);
	    fboMap["volGenFBO"]->init(1, 4096, 4096, 1);
	    



	}

	void doShaderRefresh() {
	    std::cout << "doShaderRefresh\n";

	    readyToRecompile = 0;

	    int i;
	    int j;


	    for (i = 0; i < shaderStrings.size(); i++) {
	        if (shaderMap[ shaderStrings[i] ]) {
	            delete shaderMap[ shaderStrings[i] ];
	        }
	        shaderMap[ shaderStrings[i] ] = new Shader( ("shaders/"+shaderStrings[i]+".c").c_str() );
	    }
	    
	}




	
	void setMatrices(int w, int h) {
		glViewport(0,0,w,h);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity ();
	    glMatrixMode (GL_PROJECTION);
	    glLoadIdentity ();
	}

	void setWH(int w, int h) {
	    baseW = w;
	    baseH = h;
	}

	void sampleFBODirect(FBOSet* fbos) {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = 0; i < fbos->numBufs; i++) {
	            setShaderTexture(shaderTextureIDs[i], fbos->fbos[i].color_tex, i);
	        }
	    }
	}

	void unsampleFBODirect(FBOSet* fbos) {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = fbos->numBufs - 1; i >= 0; i--) {
	            setShaderTexture(shaderTextureIDs[i], 0, i);
	        }
	    }
	}

	void bindFBODirect(FBOSet* fbos) {
	    setMatrices(fbos->width,fbos->height);
	    fbos->bind(1);
	    currentFBOResolutionX = fbos->width;
	    currentFBOResolutionY = fbos->height;
	}

	////

	void sampleFBO(string fboName) {
	    FBOSet* fbos = fboMap[fboName];
	    sampleFBODirect(fbos);
	}

	void unsampleFBO(string fboName) {
	    
	    FBOSet* fbos = fboMap[fboName];
	    unsampleFBODirect(fbos);
	}

	void bindFBO(string fboName) {
	    
	    FBOSet* fbos = fboMap[fboName];
	    bindFBODirect(fbos);
	}

	void unbindFBO() {
	    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	    setMatrices(baseW,baseH);
	    
	}



	void bindShader(string shaderName) {

	    if (shadersAreLoaded) {
	        curShader = shaderName;
	        shaderMap[curShader]->bind();
	    }
	    

	}

	void unbindShader() {
	    if (shadersAreLoaded) {
	        shaderMap[curShader]->unbind();
	    }
	    
	}

	void setShaderFloat(string paramName, float x) {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x);

	        shaderMap[curShader]->setShaderFloat(paramName, x);
	    }
	}
	void setShaderVec2(string paramName, float x, float y) {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderVec2(paramName, x, y);
	    }
	}
	void setShaderVec3(string paramName, float x, float y, float z) {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderVec3(paramName, x, y, z);
	    }
	}
	void setShaderVec4(string paramName, float x, float y, float z, float w) {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y,z,w);
	        shaderMap[curShader]->setShaderVec4(paramName, x, y, z, w);
	    }
	}

	void setShaderTexture(string shaderSamplerName, uint texID, int multitexNumber) {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_2D, texID);
	        //glUniform1iARB(shaderMap[curShader]->uniformLocation(shaderSamplerName),multitexNumber);
	        shaderMap[curShader]->setShaderInt(shaderSamplerName,multitexNumber);
	    }
	}

	void setShaderTexture3D(string shaderSamplerName, uint texID, int multitexNumber) {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_3D, texID);
	        //glUniform1iARB(shaderMap[curShader]->uniformLocation(shaderSamplerName),multitexNumber);
	        shaderMap[curShader]->setShaderInt(shaderSamplerName,multitexNumber);
	    }
	}



	void drawFSQuad(float zoom) {
	    drawFSQuadOffset(0.0f,0.0f,zoom);
	}

	void drawFSQuadOffset(float xOff, float yOff, float zoom) {
	    float fx1 = (xOff-1.0f)*zoom;
	    float fy1 = (yOff-1.0f)*zoom;
	    float fx2 = (xOff+1.0f)*zoom;
	    float fy2 = (yOff+1.0f)*zoom;

	    glColor4f(1, 1, 1, 1);
	    glBegin(GL_QUADS);
	    glNormal3f(0, 0, 1);
	    
	    glTexCoord2f(0.0f, 0.0f);
	    glVertex3f(fx1,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 0.0f);
	    glVertex3f(fx2,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 1.0f);
	    glVertex3f(fx2,fy2,0.0f);
	    
	    glTexCoord2f(0.0f, 1.0f);
	    glVertex3f(fx1,fy2,0.0f);
	    
	    glEnd();
	}

	void drawFBO(string fboName, int ind, float zoom) {
	    drawFBOOffset(fboName, ind, 0.0f, 0.0f, zoom);
	}

	void drawFBOOffsetDirect(FBOSet* fbos, int ind, float xOff, float yOff, float zoom) {

	    glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
	    //glClearColor(0.2,0.2,0.2,0.0);
	    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    drawFSQuadOffset(xOff,yOff,zoom);
	    glBindTexture(GL_TEXTURE_2D, 0);
	}

	void drawFBOOffset(string fboName, int ind, float xOff, float yOff, float zoom) {
	    FBOSet* fbos = fboMap[fboName];
	    drawFBOOffsetDirect(fbos, ind, xOff, yOff, zoom);
	}


	void moveCamera(float dx, float dy, float zoom) {
	    cameraPos.x += dx/(baseW*zoom/2.0);
	    cameraPos.y -= dy/(baseH*zoom/2.0);
	}


	void doAction(eProgramAction pa) {
		switch (pa) {
			case E_PA_QUIT:
				if (isFullScreen) {
					//glutLeaveGameMode();
				}

				exit(0);
			break;
			case E_PA_TOGGLE_FULLSCREEN:
				isFullScreen = !isFullScreen;

				if (isFullScreen) {
					//glutGameModeString( "1280x800:32@75" ); //the settings for fullscreen mode
					//glutEnterGameMode();
					glutFullScreen();
				}
				else {
					glutReshapeWindow(defaultWinW, defaultWinH);
					//glutLeaveGameMode();
				}

			break;
			case E_PA_REFRESH:
				cameraPos.x = 0.0f;
            	cameraPos.y = 0.0f;
            	cameraZoom = 1.0f;
            	doShaderRefresh();
			break;
			case E_PA_SIZE:
				//unused
			break;

		}
	}



	void processSpecialKeys(int key, int x, int y) {
		/*
		GLUT_KEY_F1		F1 function key
		GLUT_KEY_F2		F2 function key
		GLUT_KEY_F3		F3 function key
		GLUT_KEY_F4		F4 function key
		GLUT_KEY_F5		F5 function key
		GLUT_KEY_F6		F6 function key
		GLUT_KEY_F7		F7 function key
		GLUT_KEY_F8		F8 function key
		GLUT_KEY_F9		F9 function key
		GLUT_KEY_F10		F10 function key
		GLUT_KEY_F11		F11 function key
		GLUT_KEY_F12		F12 function key
		GLUT_KEY_LEFT		Left function key
		GLUT_KEY_RIGHT		Right function key
		GLUT_KEY_UP		Up function key
		GLUT_KEY_DOWN		Down function key
		GLUT_KEY_PAGE_UP	Page Up function key
		GLUT_KEY_PAGE_DOWN	Page Down function key
		GLUT_KEY_HOME		Home function key
		GLUT_KEY_END		End function key
		GLUT_KEY_INSERT		Insert function key
		*/

		switch(key) {
			case GLUT_KEY_F1 :

			break;
			case GLUT_KEY_F2 :

			break;
			case GLUT_KEY_F3 :

			break;
		}
	}
	void processKey(unsigned char key, int x, int y, bool isPressed) {
		
	}
	void keyboardUp(unsigned char key, int x, int y) {

		int keymod = glutGetModifiers();

		if (keymod&GLUT_ACTIVE_SHIFT) {} else {
			shiftPressed = false;
		}
		if (keymod&GLUT_ACTIVE_CTRL) {} else {
			ctrlPressed = false;
		}
		if (keymod&GLUT_ACTIVE_ALT) {} else {
			altPressed = false;
		}

		doAction(progActionsUp[((int)programState)*256 + key]);
	}
	void keyboardDown(unsigned char key, int x, int y) {

		int keymod = glutGetModifiers();

		if (keymod&GLUT_ACTIVE_SHIFT) {
			shiftPressed = true;
		}
		if (keymod&GLUT_ACTIVE_CTRL) {
			ctrlPressed = true;
		}
		if (keymod&GLUT_ACTIVE_ALT) {
			altPressed = true;
		}

		doAction(progActionsDown[((int)programState)*256 + key]);
	}
	void mouseMovementWithButton(int x, int y) {
		
	}
	void mouseMovementWithoutButton(int x, int y) {
		
	}
	void mouseClick(int button, int state, int x, int y) {
		if (state == GLUT_DOWN) {
			if (button == GLUT_LEFT_BUTTON) {

			}
		}
		if (state == GLUT_UP) {
			
		}
	}
	void display(void) {
		
	}
	void reshape(int w, int h) {
		setWH(w,h);
		setMatrices(baseW,baseH);
	}
	void idleFunc(void) {
		
	}

};


/*
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

    if (event->buttons() & Qt::LeftButton) {
        singleton->fMouseDown = 1.0;
    } else if (event->buttons() & Qt::RightButton) {

    }
}

void GLWidget::wheelEvent(QWheelEvent *event){
  myDelta += event->delta()/300.0f;
  singleton->cameraZoom = pow(2.0, myDelta);

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    singleton->fMouseDown = 0.0;

    singleton->mouseXUp = event->x();
    singleton->mouseYUp = event->y();

    if (event->buttons() & Qt::LeftButton) {
        singleton->fMouseDown = 1.0;

        singleton->moveCamera((float)dx, (float)dy, singleton->cameraZoom);
    } else if (event->buttons() & Qt::RightButton) {
        singleton->mouseX = event->x();
        singleton->mouseY = event->y();

        
    }
    lastPos = event->pos();
}
*/










//UNIT -> PAGE -> GROUP
float timeValSec;
float timeVal;
float curFPS;
float totalFPS = 0.0f;
int frameCount = 0;
int giGUI_IDS;
Timer timer;
Timer globalTimer;
Vector3 gvLightDir;

const static int fpsNumFrames = 				100;
const static int giSEA_LEVEL =	 				260;
const static float gfSEA_LEVEL =				(float)giSEA_LEVEL;
const static int giNUM_THREADS = 				8;
const static int giLOAD_RAD = 					16;
const static int giRENDER_RAD = 				16;
const static int giUNITS_PER_PAGE = 			128;
const static int giPAGES_PER_GROUP[3] = 		{64,4,64};
const static int giMIP_LEVELS =					1;
const static int giMIP_SCALE =					2;
const static int giMIN_MIP = 					0;
const static int giRAY_STEPS =					16;
const static int giTEXTURE_SIZE =				32;
const static int giNUM_TEXTURES =				4;
const static int giMATERIAL_SLOTS =				256;
const static int giNUM_MATERIALS = 				20;
const static int giNUM_THREAD_PARAMS =			8;
const static int giUNITS_PER_GROUP[3] = 		{giPAGES_PER_GROUP[0]*giUNITS_PER_PAGE, 	giPAGES_PER_GROUP[1]*giUNITS_PER_PAGE, 	giPAGES_PER_GROUP[2]*giUNITS_PER_PAGE};
const static float gfCLIP_DIST = 				2000.0;
const static float gfTEXTURE_SIZE =				(float)giTEXTURE_SIZE;
const static float gfMIP_LEVELS = 				(float)giMIP_LEVELS;
const static float gfUNITS_PER_PAGE = 			(float)giUNITS_PER_PAGE;
const static float gfPAGES_PER_GROUP[3] = 		{(float)giPAGES_PER_GROUP[0],  	(float)giPAGES_PER_GROUP[1],		(float)giPAGES_PER_GROUP[2]};
const static float gfUNITS_PER_GROUP[3] = 		{(float)giUNITS_PER_GROUP[0], 		(float)giUNITS_PER_GROUP[1], 		(float)giUNITS_PER_GROUP[2]};
const static float gfRAY_STEPS =				(float)giRAY_STEPS;
const static float gfMATERIAL_SLOTS =			(float)giMATERIAL_SLOTS;
const static int static_imods[6] = {-1,1,0,0,0,0};
const static int static_jmods[6] = {0,0,-1,1,0,0};
const static int static_kmods[6] = {0,0,0,0,-1,1};
const static int static_ADJ_NEIGHBOR_COUNT = 27;
const static int static_octantsX[8] = 	{0,1,0,1,0,1,0,1};
const static int static_octantsY[8] = 	{0,0,1,1,0,0,1,1};
const static int static_octantsZ[8] = 	{0,0,0,0,1,1,1,1};
const static int static_nxmods[static_ADJ_NEIGHBOR_COUNT] = 	{
									-1, 0, 1,
									-1, 0, 1,
									-1, 0, 1,
									
									-1, 0, 1,
									-1, 0, 1,
									-1, 0, 1,

									-1, 0, 1,
									-1, 0, 1,
									-1, 0, 1
								};
const static int static_nymods[static_ADJ_NEIGHBOR_COUNT] = 	{
									-1, -1, -1,
									0, 0, 0,
									1, 1, 1, 

									-1, -1, -1,
									0, 0, 0,
									1, 1, 1, 

									-1, -1, -1,
									0, 0, 0,
									1, 1, 1
								};
const static int static_nzmods[static_ADJ_NEIGHBOR_COUNT] = 	{
									-1, -1, -1,
									-1, -1, -1,
									-1, -1, -1,

									0, 0, 0,
									0, 0, 0,
									0, 0, 0,

									1, 1, 1,
									1, 1, 1,
									1, 1, 1

								};
const static int static_faceOffsets[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

struct GameMaterial {
	float r[giMATERIAL_SLOTS];
	float g[giMATERIAL_SLOTS];
	float b[giMATERIAL_SLOTS];
};

GameMaterial mainMaterials[giNUM_MATERIALS];

bool ptBusy[giNUM_THREADS];
float windowSize[2] = {1024.0f, 1024.0f};
pthread_t threadStore[giNUM_THREADS];

OffsetGrid* oGrid = new OffsetGrid;

class fPage3D {
private:

public:
	


	int imods[6];
	int jmods[6];
	int kmods[6];

	int octantsX[8];
	int octantsY[8];
	int octantsZ[8];

	int nxmods[27];
	int nymods[27];
	int nzmods[27];

	int faceOffsets[72];



	int iPageSize;
	int iTotalUnits;

	iVector3 iOffsets;

	float fPageSize;
	float fTotalUnits;

	fVector3 fOffsets;

	float* fDataArray;

	int currentState;
	int fillState;

	fPage3D* neighbors[27];
	fPage3D** textures;

	
	GLuint unitDL;
	GLuint waterDL;

	float* mipMapsR[giMIP_LEVELS];
	float* mipMapsG[giMIP_LEVELS];
	float* mipMapsB[giMIP_LEVELS];
	float* mipMapsC[giMIP_LEVELS];
	unsigned int* mipMapsF[giMIP_LEVELS];

	bool noSolidFaces;
	bool noWaterFaces;


	

	fPage3D() {
		
	}

	inline bool testStates(int testState) {
		int i;
		bool res = true;

		for (i = 0; i < 27; i++) {
			if (neighbors[i] == NULL) {
				res = false;
			}
			else {
				if (neighbors[i]->currentState < testState) {
					if (neighbors[i]->currentState != E_OP_CLEAR) {
						res = false;
					}
				}
			}
		}
		return res;
	}

	void setAllNeighbors(fPage3D* fp) {
		int i;

		for (i = 0; i < 27; i++) {
			neighbors[i] = fp;
		}
	}

	inline float getIndex(int xval, int yval, int zval) {

		
		int xtest = xval + iPageSize;
		int ytest = yval + iPageSize;
		int ztest = zval + iPageSize;

		int nx = xtest/iPageSize;
		int ny = ytest/iPageSize;
		int nz = ztest/iPageSize;

		int lx = xtest%iPageSize;
		int ly = ytest%iPageSize;
		int lz = ztest%iPageSize;

		int newIndex = lx + ly*iPageSize + lz*iPageSize*iPageSize;

		//pushTrace(intToString(nx),intToString(ny),intToString(nz));
		//popTrace();

		
		fPage3D* curPage = neighbors[nx + ny*3 + nz*9];

		if (curPage == NULL) {
			return AIR_VAL;
		}
		else {
			switch (curPage->fillState) {
				case FILL_STATE_MIXED:
					return (curPage->fDataArray[newIndex]);
				break;
				case FILL_STATE_AIR:
					return AIR_VAL;
				break;
				case FILL_STATE_WATER:
					return WATER_VAL;
				break;
				case FILL_STATE_SOLID:
					return SOLID_THRESH;
				break;
				default:
					return AIR_VAL;
				break;

			}
		}

		
	}





	void init(int _iPageSize, int _iOffsetX, int _iOffsetY, int _iOffsetZ, fPage3D** _textures=NULL) {
		int i;
		int j;

		////


		noSolidFaces = true;
		noWaterFaces = true;


		for (i = 0; i < 6; i++) {
			imods[i] = static_imods[i];
			jmods[i] = static_jmods[i];
			kmods[i] = static_kmods[i];
		}
		for (i = 0; i < 8; i++) {
			octantsX[i] = static_octantsX[i];
			octantsY[i] = static_octantsY[i];
			octantsZ[i] = static_octantsZ[i];
		}

		for (i = 0; i < 27; i++) {
			nxmods[i] = static_nxmods[i];
			nymods[i] = static_nymods[i];
			nzmods[i] = static_nzmods[i];
		}

		for (i = 0; i < 72; i++) {
			faceOffsets[i] = static_faceOffsets[i];
		}


		////



		textures = _textures;

		currentState = S_INIT;
		fillState = FILL_STATE_MIXED;

		iPageSize = _iPageSize;
		fPageSize = (float)iPageSize;

		iOffsets.x = _iOffsetX*iPageSize;
		iOffsets.y = _iOffsetY*iPageSize;
		iOffsets.z = _iOffsetZ*iPageSize;

		fOffsets.x = (float)iOffsets.x;
		fOffsets.y = (float)iOffsets.y;
		fOffsets.z = (float)iOffsets.z;

		iTotalUnits = iPageSize*iPageSize*iPageSize;
		fTotalUnits = (float)iTotalUnits;

		fDataArray = new float[iTotalUnits];


		j = iTotalUnits;
		for (i = 0; i < giMIP_LEVELS; i++) {
			mipMapsR[i] = new float[j];
			mipMapsG[i] = new float[j];
			mipMapsB[i] = new float[j];
			mipMapsC[i] = new float[j];
			mipMapsF[i] = new unsigned int[j];
			
			j = j/8;
		}

		for (i = 0; i < 27; i++) {
			neighbors[i] = NULL;
		}

		

		currentState = E_INIT;
	}

	inline float sampleTexture(int texNum, int x, int y, int z) {
		int ind;

		ind = (x%giTEXTURE_SIZE) + (y%giTEXTURE_SIZE)*giTEXTURE_SIZE + (z%giTEXTURE_SIZE)*giTEXTURE_SIZE*giTEXTURE_SIZE;

		return textures[texNum]->fDataArray[ind];
	}

	void opApplyTexture() {
		currentState = S_OP_APPLY_TEXTURE;

		int i;

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		float testVal1;
		float testVal2;
		float testVal3;
		float testVal4;
		float testVal5;

		bool empty;

		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize) + iOffsets.x;
			iy = (i%(iPageSize*iPageSize))/iPageSize + iOffsets.y;
			iz = i/(iPageSize*iPageSize) + iOffsets.z;

			fx = ((float)ix);
			fy = ((float)iy);
			fz = ((float)iz);

			
			
			testVal4 = simplexScaledNoise(
											8.0f, //octaves
											0.5f, //persistence (amount added in each successive generation)
											1.f/512.0f, //scale (frequency)
											0.0f,
											1.0f,
											fx,
											fy,
											fz
										);


			//sampleTexture(E_TEX_SIMPLEX, ix, 0, iz)*sampleTexture(E_TEX_SIMPLEX, (ix+7)/3, 0, (iz+5)/5);

			/*
			if (testVal1*testVal2 > 0.5f) {
				
				if (testVal3 > 3.0f) {
					fDataArray[i] = 1.0f;
				}
				else {
					fDataArray[i] = 0.0f;
				}
				
			}*/
			
			//else

			testVal5 = fy/gfUNITS_PER_GROUP[1];

			empty = false;

			if (testVal5 < testVal4) {
				
				/*
				//testVal1 = sampleTexture(E_TEX_SIMPLEX, ix, iy, iz);
				//testVal2 = sampleTexture(E_TEX_SIMPLEX, (ix+7)/3, (iy+3)/7, (iz+5)/5);
				testVal3 = sampleTexture(E_TEX_VORONOI, ix, iy, iz);

				if (testVal3 > testVal4*3.0) {
					fDataArray[i] = ROCK_VAL;
				}
				else {
					empty = true;
				}*/

				fDataArray[i] = ROCK_VAL;
			}
			else {
				empty = true;
			}

			if (empty) {
				if (fy < gfSEA_LEVEL) {  //(fy + testVal4*32.0)
					fDataArray[i] = WATER_VAL;
				}
				else {
					fDataArray[i] = AIR_VAL;
				}
			}
			
		}

		currentState = E_OP_APPLY_TEXTURE;

	}

	void opSeamlessSimplexNoise(float oct, float pers, float freq) {
		
		currentState = S_OP_SEAMLESS_SIMPLEX_NOISE;

		int i;

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		float testVal;

		//float oct = 4.0;
		//float pers = 0.5;
		//float freq =- 1.0/gfTEXTURE_SIZE;

		float ts = gfTEXTURE_SIZE;


		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize);
			iy = (i%(iPageSize*iPageSize))/iPageSize;
			iz = i/(iPageSize*iPageSize);

			fx = ((float)ix) + fOffsets.x;
			fy = ((float)iy) + fOffsets.y;
			fz = ((float)iz) + fOffsets.z;

			//float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z ) {
			testVal = 
				(
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy, 	fz) * 		(ts-fx) *		(ts-fy) *	(ts-fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy, 	fz) * 		(fx) *			(ts-fy) *	(ts-fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy-ts, 	fz) * 		(ts-fx) *		(fy) *		(ts-fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy-ts, 	fz) * 		(fx) *			(fy) *		(ts-fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy, 	fz-ts) * 	(ts-fx) *		(ts-fy) *	(fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy, 	fz-ts) * 	(fx) *			(ts-fy) *	(fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy-ts, 	fz-ts) * 	(ts-fx) *		(fy) *		(fz) +
				simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy-ts, 	fz-ts) * 	(fx) *			(fy) *		(fz)
				)/(ts*ts*ts);

			fDataArray[i] = testVal;

			
		}

		currentState = E_OP_SEAMLESS_SIMPLEX_NOISE;
	}

	void opVoronoi(float _fPointScale, float _fPointDensity) {
		
		currentState = S_OP_VORONOI;

		int iVoroTexSize;
		float fVoroTexSize;

		int iPointScale;
		int iPointGridSize;
		float fPointScale;
		float fPointGridSize;
		float fPointDensity;


		

		fPointDensity = _fPointDensity;
		fPointScale = _fPointScale;
		iPointScale = (int)fPointScale;
		
		
		iVoroTexSize = iPageSize;
		iPointGridSize = iVoroTexSize/iPointScale;
		
		fVoroTexSize = (float)iVoroTexSize;
		fPointGridSize = (float)iPointGridSize;
		


		//Dynamic Allocs
		int* idArray = new int[iVoroTexSize*iVoroTexSize*iVoroTexSize];
		float* pointGridBlanks = new float[iPointGridSize*iPointGridSize*iPointGridSize];
		Vector3* pointGrid = new Vector3[iPointGridSize*iPointGridSize*iPointGridSize];
		
		
		float pointBuf = 0.1f;
		float pointBufInv = 1.0-(pointBuf*2.0);
		
		
		int i;
		int j;
		int k;
		
		int i2;
		int j2;
		int k2;
		
		int m;
		int n;
		
		int voroRad = 2;
		
		float disMax = 1000000.0f;
		
		float testDis;
		float dis1;
		float dis2;
		
		
		
		int pointCount;
		
		int curInd;
		int curInd2;
		int idVal;
		
		float x;
		float y;
		float z;
		
		float xo;
		float yo;
		float zo;
		
		
		float imod;
		float jmod;
		float kmod;
		
		
		int ioffset;
		int joffset;
		int koffset;
		
		bool stillGoing;
		
		Vector3 startPoint;
		Vector3 endPoint;
		Vector3 bestPoint;
		
		////////////////////////
		//Generate Random Points
		////////////////////////
		
		for (i = 0; i < iPointGridSize; i++) {
			x = ((float)i);
			
			for (j = 0; j < iPointGridSize; j++) {
				y = ((float)j);
				
				for (k = 0; k < iPointGridSize; k++) {
					z = (float(k));
					
					
					if (fGenRand() >= fPointDensity) {
						xo = -1.0f;
						yo = -1.0f;
						zo = -1.0f;
					}
					else {
						xo = (fGenRand()*pointBufInv + pointBuf + x)*fPointScale;
						yo = (fGenRand()*pointBufInv + pointBuf + y)*fPointScale;
						zo = (fGenRand()*pointBufInv + pointBuf + z)*fPointScale;
					}
					
					
					
					curInd = i + j*iPointGridSize + k*iPointGridSize*iPointGridSize;
					
					pointGrid[curInd].setXYZ(xo, yo, zo);
					pointGridBlanks[curInd] = fGenRand();
					
				}
			}
		}
		
		
		
		////////////////////////////////////////////////////////////////////
		//Make sure every space has at least 2 points within 1 block radius
		////////////////////////////////////////////////////////////////////
		
		
		
		for (i = 0; i < iPointGridSize; i++) {
			x = ((float)i);
			
			for (j = 0; j < iPointGridSize; j++) {
				y = ((float)j);
				
				for (k = 0; k < iPointGridSize; k++) {
					z = (float(k));
					
					
					
					
					pointCount = 0;
					stillGoing = true;
					
					for (m = 0; (m <= voroRad)&&(stillGoing); m++) {
						for (n = 0; (n < oGrid->offsetGridCounters[m])&&(stillGoing); n++) {
							
							
							ioffset = i + oGrid->offsetGrid[m][n].x;
							joffset = j + oGrid->offsetGrid[m][n].y;
							koffset = k + oGrid->offsetGrid[m][n].z;

							//wrap offsets

							if (ioffset < 0) {
								ioffset = iPointGridSize + ioffset;
							}
							if (ioffset >= iPointGridSize) {
								ioffset = ioffset-iPointGridSize;
							}

							if (joffset < 0) {
								joffset = iPointGridSize + joffset;
							}
							if (joffset >= iPointGridSize) {
								joffset = joffset-iPointGridSize;
							}

							if (koffset < 0) {
								koffset = iPointGridSize + koffset;
							}
							if (koffset >= iPointGridSize) {
								koffset = koffset-iPointGridSize;
							}
							
							
							curInd = ioffset + joffset*iPointGridSize + koffset*iPointGridSize*iPointGridSize;
							
							if (pointGrid[curInd].x() >= 0.0f) {
								pointCount++;
							}
							
							if (pointCount >= 2) {
								stillGoing = false;
							}
							
						}
					}
					
					
					if (stillGoing) {
						//less than 2 points found, generate 1
						
						xo = (fGenRand()*pointBufInv + pointBuf + x)*fPointScale;
						yo = (fGenRand()*pointBufInv + pointBuf + y)*fPointScale;
						zo = (fGenRand()*pointBufInv + pointBuf + z)*fPointScale;
						curInd = i + j*iPointGridSize + k*iPointGridSize*iPointGridSize;
						pointGrid[curInd].setXYZ(xo, yo, zo);
						
					}
					else {
						//2 or more points found
					}	
					
				}
			}
		}
		
		
		
		///////////////////////////////
		//Generate the Voronoi Diagram
		///////////////////////////////
		
		
		for (i2 = 0; i2 < iVoroTexSize; i2++) {
			x = ((float)i2);
			i = i2/iPointScale;

			for (j2 = 0; j2 < iVoroTexSize; j2++) {
				y = ((float)j2);
				j = j2/iPointScale;

				for (k2 = 0; k2 < iVoroTexSize; k2++) {
					z = (float(k2));
					k = k2/iPointScale;
					
					
					dis1 = disMax;
					dis2 = disMax;
					
					
					startPoint.setXYZ(x,y,z);
					
					for (m = 0; m <= voroRad; m++) {
						for (n = 0; n < oGrid->offsetGridCounters[m]; n++) {

							
							
							
							ioffset = i + oGrid->offsetGrid[m][n].x;
							joffset = j + oGrid->offsetGrid[m][n].y;
							koffset = k + oGrid->offsetGrid[m][n].z;

							//wrap offsets
							
							imod = 0.0f;
							jmod = 0.0f;
							kmod = 0.0f;
							
							if (ioffset < 0) {
								ioffset = iPointGridSize + ioffset;
								imod = -fVoroTexSize;
							}
							if (ioffset >= iPointGridSize) {
								ioffset = ioffset-iPointGridSize;
								imod = fVoroTexSize;
							}

							if (joffset < 0) {
								joffset = iPointGridSize + joffset;
								jmod = -fVoroTexSize;
							}
							if (joffset >= iPointGridSize) {
								joffset = joffset-iPointGridSize;
								jmod = fVoroTexSize;
							}

							if (koffset < 0) {
								koffset = iPointGridSize + koffset;
								kmod = -fVoroTexSize;
							}
							if (koffset >= iPointGridSize) {
								koffset = koffset-iPointGridSize;
								kmod = fVoroTexSize;
							}
							
							
							
							
							curInd = ioffset + joffset*iPointGridSize + koffset*iPointGridSize*iPointGridSize;
							
							endPoint.setXYZ(pointGrid[curInd].e[0] + imod, pointGrid[curInd].e[1] + jmod, pointGrid[curInd].e[2] + kmod);
							
							if (pointGrid[curInd].x() >= 0.0f) {
								testDis = distance(&startPoint, &endPoint);
								
								if (testDis < dis1) {
									
									idVal = curInd;
									
									bestPoint.setXYZ(endPoint.e[0],endPoint.e[1],endPoint.e[2]);
									
									dis2 = dis1;
									dis1 = testDis;
									
								}
								else {
									if (testDis < dis2) {
										dis2 = testDis;
									}
								}
								
							}
							
						}
					}
					
					curInd2 = i2 + j2*iVoroTexSize + k2*iVoroTexSize*iVoroTexSize;
					
					
					
					fDataArray[curInd2] = fabs(dis2-dis1);///max(dis1,dis2);
					idArray[curInd2] = idVal;
					//vecArray[curInd2].setXYZ(bestPoint.e[0],bestPoint.e[1],bestPoint.e[2]);
					
				}
			}
		}

		currentState = E_OP_VORONOI;
		
	}

	/*
	void opVoronoi(float fBlockSize, float fDensity) {
		

		int i;
		int j;
		int k;

		float buf = 0.1f;
		float buf2 = buf*2.0f;
		float ibuf = 1.0f-buf2;

		int iBlockSize = (int)fBlockSize;
		int iBlockSize2 = iBlockSize*2;
		int distBlockSize = iBlockSize2*iBlockSize2*iBlockSize2;
		int iCellsSize = iPageSize/iBlockSize;
		int iTotalCells = iCellsSize*iCellsSize*iCellsSize;


		float* distBlock = new float[distBlockSize];

		for (i = 0; i < distBlockSize; i++) {
			
		}

		for (i = 0; i < iTotalCells; i++) {
			ix = i%(iCellsSize);
			iy = (i%(iCellsSize*iCellsSize))/iCellsSize;
			iz = i/(iCellsSize*iCellsSize);

			fx = ((float)ix)+ (genRand()*ibuf)+buf;
			fy = ((float)iy)+ (genRand()*ibuf)+buf;
			fz = ((float)iz)+ (genRand()*ibuf)+buf;

			if (fGenRand() < fDensity) {
				
			}

		}



		int* closestIndex1 = new int[iTotalUnits];
		int* closestIndex2 = new int[iTotalUnits];
		float* distFromCenter1 = new float[iTotalUnits];
		float* distFromCenter2 = new float[iTotalUnits];




		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize);
			iy = (i%(iPageSize*iPageSize))/iPageSize;
			iz = i/(iPageSize*iPageSize);

			fx = ((float)ix) + fOffsets.x;
			fy = ((float)iy) + fOffsets.y;
			fz = ((float)iz) + fOffsets.z;

		}
	}
	*/

	void opSimplexNoise(float oct, float pers, float freq) {
		
		currentState = S_OP_SIMPLEX_NOISE;

		int i;

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		float testVal;


		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize);
			iy = (i%(iPageSize*iPageSize))/iPageSize;
			iz = i/(iPageSize*iPageSize);

			fx = ((float)ix) + fOffsets.x;
			fy = ((float)iy) + fOffsets.y;
			fz = ((float)iz) + fOffsets.z;

			//float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z ) {
			testVal = simplexScaledNoise(
											oct, //octaves
											pers, //persistence (amount added in each successive generation)
											freq, //scale (frequency)
											0.0f,
											1.0f,
											fx,
											fy,
											fz
										);

			fDataArray[i] = testVal;

			
		}

		currentState = E_OP_SIMPLEX_NOISE;
	}

	void opClear() {
		int i;

		currentState = S_OP_CLEAR;
		for (i = 0; i < iTotalUnits; i++) {
			fDataArray[i] = 0.0f;
		}
		currentState = E_OP_CLEAR;

		fillState = FILL_STATE_AIR;
	}

	

	void renderVoxels() {
		
		if (fDataArray == NULL) {
			cout << "WARNING RENDERVOXELS called with fDataArray == NULL\n";
			return;
		}

		currentState = S_RENDER_VOXELS;

		int i;
		int j;
		int k;
		int m;
		int n;
		
		Vector3 normVec;
		
		float curDis;
		float totalDis;
		
		int curInd;
		
		//bool touchingWater;
		//bool touchingAir;

		float shadowVal;
		float waterVal;
		float iWaterVal;
		float aoVal;
		
		float rval;
		float gval;
		float bval;
		
		float vecx;
		float vecy;
		float vecz;
		
		
		float fLightVal;
		int iLightVal;
		
		
		int aoRad = 2;
		int normRad = 3;
		
		float val1;
		float val2;
		
		
		

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		int ishad;
		int jshad;
		int kshad;
		
		int mat;
		
		float fishad;
		float fjshad;
		float fkshad;

		float lv1;

		noSolidFaces = true;
		noWaterFaces = true;

		unsigned int faceBits;
		unsigned int bitMask[6] = {1,2,4,8,16,32};
		unsigned int solidBit = 64;
		unsigned int waterBit = 128;

		float shadFactor = gfUNITS_PER_PAGE/(gfRAY_STEPS*1.2);
		float shadFactor2 = gfUNITS_PER_PAGE/(gfRAY_STEPS*3.0);
		
		
		j = iTotalUnits;
		for (i = 0; i < giMIP_LEVELS; i++) {
			for (k = 0; k < j; k++) {
				mipMapsR[i][k] = 0.0f;
				mipMapsG[i][k] = 0.0f;
				mipMapsB[i][k] = 0.0f;
				mipMapsC[i][k] = 0.0f;
				mipMapsF[i][k] = 0;
			}
			j = j/8;
		}
		
		
		

		for (i = 0; i < iTotalUnits; i++) {
			ix = i%(iPageSize);
			iy = (i%(iPageSize*iPageSize))/iPageSize;
			iz = i/(iPageSize*iPageSize);

			fx = ((float)ix) + fOffsets.x;
			fy = ((float)iy) + fOffsets.y;
			fz = ((float)iz) + fOffsets.z;
			
			faceBits = 0;
			
			val1 = fDataArray[i];

			//touchingWater = false;
			//touchingAir = false;
			
			if (val1 >= SOLID_THRESH) {
				
				for (m = 0; m < 6; m++) {
					val2 = getIndex(ix+imods[m], iy+jmods[m], iz+kmods[m]);
					
					if (val2 < SOLID_THRESH) {
						faceBits = faceBits|bitMask[m];

						/*
						if (val2 == WATER_VAL) {
							touchingWater = true;
						}
						else {
							//touchingAir = true;
						}
						*/
					}

				}
				
				if (faceBits > 0) {
					noSolidFaces = false;


					//################


					//find shadow value
					
					shadowVal = 0.0f;
					aoVal = 0.0f;
					totalDis = 0.0f;
					vecx = 0.0f;
					vecy = 0.0f;
					vecz = 0.0f;
					
					for (m = 1; m <= giRAY_STEPS; m++) {
						
						rval = ((float)m)*shadFactor+1.0f;//+fGenRand()*3.0f;
						
						ishad = (int)(-gvLightDir.e[0]*rval);
						jshad = (int)(-gvLightDir.e[1]*rval);
						kshad = (int)(-gvLightDir.e[2]*rval);
						
						val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);
						
						if (val2 >= SOLID_THRESH) {
							shadowVal += 1.0f;
						}
					}
					
					for (n = 1; n < normRad; n++) {
						
						
						
						for (m = 0; m < oGrid->offsetGridCounters[n]; m++) {

						
							ishad = oGrid->offsetGrid[n][m].x;
							jshad = oGrid->offsetGrid[n][m].y;
							kshad = oGrid->offsetGrid[n][m].z;

							fishad = (float)ishad;
							fjshad = (float)jshad;
							fkshad = (float)kshad;

							curDis = sqrt(fishad*fishad + fjshad*fjshad + fkshad*fkshad);

							val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);

							if (val2 < SOLID_THRESH) {
								vecx += fishad/curDis;
								vecy += fjshad/curDis;
								vecz += fkshad/curDis;
							}
						}
					}
					
					
					for (m = 0; m < oGrid->offsetGridCounters[aoRad]; m++) {

						ishad = oGrid->offsetGrid[aoRad][m].x*4.0f;
						jshad = oGrid->offsetGrid[aoRad][m].y*4.0f;
						kshad = oGrid->offsetGrid[aoRad][m].z*4.0f;

						fishad = (float)ishad;
						fjshad = (float)jshad;
						fkshad = (float)kshad;

						curDis = sqrt(fishad*fishad + fjshad*fjshad + fkshad*fkshad);

						val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);

						if (val2 < SOLID_THRESH) {
							aoVal += 1.0f/curDis;
						}
						totalDis += 1.0f/curDis;
					}
					
					
					
					normVec.setXYZ(vecx, vecy, vecz);
					normVec.makeUnitVector();
					
					aoVal = clamp(aoVal/totalDis);
					
					
					//waterVal = clamp(waterVal*2.0f/gfRAY_STEPS);
					//iWaterVal = 1.0f-waterVal;
					shadowVal = 1.0f-shadowVal/gfRAY_STEPS;
					shadowVal = shadowVal*shadowVal;

					lv1 = clamp(dot(-gvLightDir, normVec));
					fLightVal = clamp( (shadowVal*lv1 + (1.0-shadowVal)*aoVal*0.2)*0.8 + aoVal*0.2 );
					
					
					
					mat = val1;
					
					/*
					if (mat == 1) {
						if (normVec.e[1]+(genRand()-0.5)/3.0f < 0.1f) {
							mat = 3;
						}
					}
					
					if (mat == 1) {
						fLightVal = clamp(fLightVal + ((((float)((int)(x*z) % 237))/237.0f)-0.5f)/4.0f );
					}
					*/
					
					iLightVal = (int) (fLightVal*(gfMATERIAL_SLOTS-1.0f));
					
					rval = mainMaterials[mat].r[iLightVal];
					gval = mainMaterials[mat].g[iLightVal];
					bval = mainMaterials[mat].b[iLightVal];

					/*
					rval += (fLightVal*fLightVal*fLightVal)/3.0f;
					gval += (fLightVal*fLightVal*fLightVal)/3.0f;
					bval += (fLightVal*fLightVal*fLightVal)/3.0f;
					*/

					/*
					if (touchingWater && !touchingAir) {
						rval = rval*0.5f + 0.0f*0.5f;
						gval = gval*0.5f + 0.5*0.5f;
						bval = bval*0.5f + 1.0*0.5f;
					}
					*/
					

					//rval = ((float)ix)/fPageSize;
					//gval = ((float)iy)/fPageSize;
					//bval = ((float)iz)/fPageSize;
					
					//fLightVal = val1 - 1.0f;

					//fLightVal = floor(fLightVal*12.0f)/12.0f;

					//rval = shadowVal;
					//gval = shadowVal;
					//bval = shadowVal;

					//rval = fLightVal;
					//gval = fLightVal;
					//bval = fLightVal;

					faceBits |= solidBit;

					
					mipMapsR[0][i] = rval;
					mipMapsG[0][i] = gval;
					mipMapsB[0][i] = bval;
					mipMapsC[0][i] = 0.0f;
					mipMapsF[0][i] = faceBits;


					//################
					

					//Add a face if its on the edge to smooth between mipmaps
					if (ix == 0) {
						faceBits |= bitMask[0];
					}
					if (ix == iPageSize-1) {
						faceBits |= bitMask[1];
					}
					if (iy == 0) {
						faceBits |= bitMask[2];
					}
					if (iy == iPageSize-1) {
						faceBits |= bitMask[3];
					}
					if (iz == 0) {
						faceBits |= bitMask[4];
					}
					if (iz == iPageSize-1) {
						faceBits |= bitMask[5];
					}



					n = 2;
					for (m = 1; m < giMIP_LEVELS; m++) {
						
						curInd = ( ix/n) + (iy/n)*(iPageSize/n) + (iz/n)*(iPageSize/n)*(iPageSize/n);
						
						mipMapsR[m][ curInd ] += rval;
						mipMapsG[m][ curInd ] += gval;
						mipMapsB[m][ curInd ] += bval;
						mipMapsC[m][ curInd ] += 1.0f;
						mipMapsF[m][ curInd ] |= faceBits;
						//mipMapsF[m][ curInd ] &= (~waterBit);
						
						n = n*2;
					}
				}
				
				
			}
			else if (val1 == WATER_VAL) {





				for (m = 0; m < 6; m++) {
					val2 = getIndex(ix+imods[m], iy+jmods[m], iz+kmods[m]);
					
					if (val2 == AIR_VAL) {
						faceBits = faceBits|bitMask[m];
					}

				}
				
				if (faceBits > 0) {
					noWaterFaces = false;


					//################


					//find shadow value
					
					shadowVal = 0.0f;
					aoVal = 0.0f;
					totalDis = 0.0f;
					vecx = 0.0f;
					vecy = 0.0f;
					vecz = 0.0f;


					for (m = 1; m <= giRAY_STEPS; m++) {
						
						rval = ((float)m)*shadFactor2+1.0f;//+fGenRand()*3.0f;
						
						ishad = (int)(-gvLightDir.e[0]*rval);
						jshad = (int)(-gvLightDir.e[1]*rval);
						kshad = (int)(-gvLightDir.e[2]*rval);
						
						val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);
						
						if (val2 >= SOLID_THRESH) {
							shadowVal += 1.0f;
						}
					}
					
					for (n = 1; n < normRad; n++) {
						
						for (m = 0; m < oGrid->offsetGridCounters[n]; m++) {

						
							ishad = oGrid->offsetGrid[n][m].x;
							jshad = oGrid->offsetGrid[n][m].y;
							kshad = oGrid->offsetGrid[n][m].z;

							fishad = (float)ishad;
							fjshad = (float)jshad;
							fkshad = (float)kshad;

							curDis = sqrt(fishad*fishad + fjshad*fjshad + fkshad*fkshad);

							val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);

							if (val2 == AIR_VAL) {
								vecx += fishad/curDis;
								vecy += fjshad/curDis;
								vecz += fkshad/curDis;
							}
						}
					}



					for (m = 0; m < oGrid->offsetGridCounters[aoRad]; m++) {

						ishad = oGrid->offsetGrid[aoRad][m].x*4.0f;
						jshad = oGrid->offsetGrid[aoRad][m].y*4.0f;
						kshad = oGrid->offsetGrid[aoRad][m].z*4.0f;

						fishad = (float)ishad;
						fjshad = (float)jshad;
						fkshad = (float)kshad;

						curDis = sqrt(fishad*fishad + fjshad*fjshad + fkshad*fkshad);

						val2 = getIndex( ix+ishad, iy+jshad, iz+kshad);

						if (val2 >= SOLID_THRESH) {
							aoVal += 1.0f/curDis;
						}
						totalDis += 1.0f/curDis;
					}
					
					shadowVal = 1.0f-shadowVal/gfRAY_STEPS;
					shadowVal = shadowVal*shadowVal;
					aoVal = clamp(aoVal/totalDis);
					normVec.setXYZ(vecx, vecy, vecz);
					normVec.makeUnitVector();

					lv1 = clamp(dot(-gvLightDir, normVec));
					fLightVal = clamp((lv1+aoVal)*shadowVal);//clamp(lv1+aoVal);//clamp( (shadowVal*lv1 + (1.0-shadowVal)*aoVal*0.2)*0.8 + aoVal*0.2 );
					
					
					
					mat = val1;
					
					
					iLightVal = (int) (fLightVal*(gfMATERIAL_SLOTS-1.0f));
					
					rval = mainMaterials[mat].r[iLightVal];
					gval = mainMaterials[mat].g[iLightVal];
					bval = mainMaterials[mat].b[iLightVal];

					
					rval = rval + aoVal;
					gval = gval + aoVal;
					bval = bval + aoVal;
					

					//rval = ((float)ix)/fPageSize;
					//gval = ((float)iy)/fPageSize;
					//bval = ((float)iz)/fPageSize;
					
					//fLightVal = val1 - 1.0f;

					//fLightVal = floor(fLightVal*12.0f)/12.0f;

					//rval = shadowVal;
					//gval = shadowVal;
					//bval = shadowVal;

					//rval = fLightVal;
					//gval = fLightVal;
					//bval = fLightVal;

					faceBits |= waterBit;

					
					mipMapsR[0][i] = rval;
					mipMapsG[0][i] = gval;
					mipMapsB[0][i] = bval;
					mipMapsC[0][i] = 0.0f;
					mipMapsF[0][i] = faceBits;


					//################
					
					/*

					//Add a face if its on the edge to smooth between mipmaps
					if (ix == 0) {
						faceBits |= bitMask[0];
					}
					if (ix == iPageSize-1) {
						faceBits |= bitMask[1];
					}
					if (iy == 0) {
						faceBits |= bitMask[2];
					}
					if (iy == iPageSize-1) {
						faceBits |= bitMask[3];
					}
					if (iz == 0) {
						faceBits |= bitMask[4];
					}
					if (iz == iPageSize-1) {
						faceBits |= bitMask[5];
					}
					*/



					n = 2;
					for (m = 1; m < giMIP_LEVELS; m++) {
						
						curInd = ( ix/n) + (iy/n)*(iPageSize/n) + (iz/n)*(iPageSize/n)*(iPageSize/n);
						
						mipMapsR[m][ curInd ] += rval;
						mipMapsG[m][ curInd ] += gval;
						mipMapsB[m][ curInd ] += bval;
						mipMapsC[m][ curInd ] += 1.0f;
						mipMapsF[m][ curInd ] |= faceBits;
						/*
						if ( (mipMapsF[m][ curInd ] & solidBit) == 0) {
							mipMapsF[m][ curInd ] |= waterBit;
						}
						*/
						
						
						n = n*2;
					}
				}

			}
		}
				
		
		
		
		j = (iTotalUnits)/8;
		for (i = 1; i < giMIP_LEVELS; i++) {
			for (k = 0; k < j; k++) {
				mipMapsR[i][k] = mipMapsR[i][k]/mipMapsC[i][k];
				mipMapsG[i][k] = mipMapsG[i][k]/mipMapsC[i][k];
				mipMapsB[i][k] = mipMapsB[i][k]/mipMapsC[i][k];
			}
			j = j/8;
		}

		//cout << noSolidFaces << " " << noWaterFaces << "\n" ;

		if (noSolidFaces && noWaterFaces) {
			if (fDataArray) {
				if (fDataArray[0] == AIR_VAL) {
					fillState = FILL_STATE_AIR;
				}
				else if (fDataArray[0] == WATER_VAL) {
					fillState = FILL_STATE_WATER;
				}
				else {
					fillState = FILL_STATE_SOLID;
				}
			}
			else {
				fillState = FILL_STATE_AIR;
			}
			
		}
		else {
			
			fillState = FILL_STATE_MIXED;
		}
		
		currentState = E_RENDER_VOXELS;
	}


	void createDL() {

		

		if (fDataArray == NULL) {
			cout << "WARNING CREATEDL called with fDataArray == NULL\n";
			return;
		}

		currentState = S_CREATE_DL;

		int i;
		int j;
		int k;
		int m;
		int n;

		int ix;
		int iy;
		int iz;

		float fx;
		float fy;
		float fz;

		
		int mult;
		float fmult;

		float val2;
		
		int curSize;
		int curTot;

		int colCounter = 0;
		int curMipLevel = 0;
		
		int curInd;
		int curInd2;
		int cvSize;
		int mipCounter;

		unsigned int waterInd;

		unsigned int faceBits;
		unsigned int bitVal;
		unsigned int bitMask[6] = {1,2,4,8,16,32};
		unsigned int solidBit = 64;
		unsigned int waterBit = 128;

		int minInd;
		int maxInd;

		bool procFlag;

		
		if (fillState != FILL_STATE_MIXED) {

			if (fDataArray != NULL) {
				delete[] fDataArray;
				fDataArray = NULL;
			}
		}
		else {

			if (noSolidFaces) {
				minInd = 1;
			}
			else {
				unitDL = glGenLists(giMIP_LEVELS);
				minInd = 0;
			}

			if (noWaterFaces) {
				maxInd = 1;
			}
			else {
				waterDL = glGenLists(giMIP_LEVELS);
				maxInd = 2;
				
			}

			for (waterInd = minInd; waterInd < maxInd; waterInd++) {
				

				for (curMipLevel = 0; curMipLevel < giMIP_LEVELS; curMipLevel++) {
					
					if (waterInd == 0) {
						glNewList(unitDL+curMipLevel,GL_COMPILE);
					}
					else {
						glNewList(waterDL+curMipLevel,GL_COMPILE);
					}



					
					glBegin(GL_QUADS);


					curSize = iPageSize;
					curTot = iTotalUnits;
					mult = 1;
					for (i = 0; i < curMipLevel; i++) {
						curSize = curSize / 2;
						curTot = curTot / 8;
						mult = mult*2;
					}
					fmult = (float)mult;


					for (i = 0; i < curTot; i++) {
						ix = i%(curSize);
						iy = (i%(curSize*curSize))/curSize;
						iz = i/(curSize*curSize);

						fx = ((float)ix*mult) + fOffsets.x;
						fy = ((float)iy*mult) + fOffsets.y;
						fz = ((float)iz*mult) + fOffsets.z;

						faceBits = mipMapsF[curMipLevel][i];

						
						if (waterInd == 0) {
							if (faceBits >= waterBit) {
								procFlag = false;
							}
							else {
								procFlag = true;
							}
						}
						else {
							if (faceBits >= waterBit) {
								procFlag = true;
							}
							else {
								procFlag = false;
							}
						}

						
						
						
						if ( procFlag && (faceBits > 0) ) { //
							

							glColor3f(mipMapsR[curMipLevel][i], mipMapsG[curMipLevel][i], mipMapsB[curMipLevel][i]);

							
							for (m = 0; m < 6; m++) {
								bitVal = faceBits & (bitMask[m]);
								if (bitVal > 0) {
									glVertex3f(faceOffsets[m*12+0]*fmult+fx, faceOffsets[m*12+1]*fmult+fy, faceOffsets[m*12+2]*fmult+fz);
									glVertex3f(faceOffsets[m*12+3]*fmult+fx, faceOffsets[m*12+4]*fmult+fy, faceOffsets[m*12+5]*fmult+fz);
									glVertex3f(faceOffsets[m*12+6]*fmult+fx, faceOffsets[m*12+7]*fmult+fy, faceOffsets[m*12+8]*fmult+fz);
									glVertex3f(faceOffsets[m*12+9]*fmult+fx, faceOffsets[m*12+10]*fmult+fy, faceOffsets[m*12+11]*fmult+fz);		
									
								}
							}
						}
					}
					

					glEnd();
					glEndList();
					
				}
			}
			
			
			
			for (curMipLevel = 0; curMipLevel < giMIP_LEVELS; curMipLevel++) {
				delete[] mipMapsR[curMipLevel];
				delete[] mipMapsG[curMipLevel];
				delete[] mipMapsB[curMipLevel];
				delete[] mipMapsC[curMipLevel];
				delete[] mipMapsF[curMipLevel];

				mipMapsR[curMipLevel] = NULL;
				mipMapsG[curMipLevel] = NULL;
				mipMapsB[curMipLevel] = NULL;
				mipMapsC[curMipLevel] = NULL;
				mipMapsF[curMipLevel] = NULL;
			}
			

			
			
		}
		
		currentState = E_CREATE_DL;
	}

	void drawCube(float cr, float cg, float cb) {
		glBegin(GL_QUADS);		// Draw The Cube Using quads
	    glColor3f(cr,cg,cb);	// Color Blue

	    float sx = fOffsets.x;
	    float sy = fOffsets.y;
	    float sz = fOffsets.z;
	    float ex = fOffsets.x + fPageSize;
	    float ey = fOffsets.y + fPageSize;
	    float ez = fOffsets.z + fPageSize;
	    
	    glVertex3f(ex,ey,sz);	// Top Right Of The Quad (Top)
	    glVertex3f(sx,ey,sz);	// Top Left Of The Quad (Top)
	    glVertex3f(sx,ey,ez);	// Bottom Left Of The Quad (Top)
	    glVertex3f(ex,ey,ez);	// Bottom Right Of The Quad (Top)

	    glVertex3f(ex,sy,ez);	// Top Right Of The Quad (Bottom)
	    glVertex3f(sx,sy,ez);	// Top Left Of The Quad (Bottom)
	    glVertex3f(sx,sy,sz);	// Bottom Left Of The Quad (Bottom)
	    glVertex3f(ex,sy,sz);	// Bottom Right Of The Quad (Bottom)

	    glVertex3f(ex,ey,ez);	// Top Right Of The Quad (Front)
	    glVertex3f(sx,ey,ez);	// Top Left Of The Quad (Front)
	    glVertex3f(sx,sy,ez);	// Bottom Left Of The Quad (Front)
	    glVertex3f(ex,sy,ez);	// Bottom Right Of The Quad (Front)

	    glVertex3f(ex,sy,sz);	// Top Right Of The Quad (Back)
	    glVertex3f(sx,sy,sz);	// Top Left Of The Quad (Back)
	    glVertex3f(sx,ey,sz);	// Bottom Left Of The Quad (Back)
	    glVertex3f(ex,ey,sz);	// Bottom Right Of The Quad (Back)

	    glVertex3f(sx,ey,ez);	// Top Right Of The Quad (Left)
	    glVertex3f(sx,ey,sz);	// Top Left Of The Quad (Left)
	    glVertex3f(sx,sy,sz);	// Bottom Left Of The Quad (Left)
	    glVertex3f(sx,sy,ez);	// Bottom Right Of The Quad (Left)

	    glVertex3f(ex,ey,sz);	// Top Right Of The Quad (Right)
	    glVertex3f(ex,ey,ez);	// Top Left Of The Quad (Right)
	    glVertex3f(ex,sy,ez);	// Bottom Left Of The Quad (Right)
	    glVertex3f(ex,sy,sz);	// Bottom Right Of The Quad (Right)
	  	glEnd();
	}

	inline void renderDL(int mipLev, int renderState) {
		if (fDataArray == NULL) {
			//cout << "WARNING RENDERDL called with fDataArray == NULL\n";
			return;
		}

		if (currentState == E_CREATE_DL) {
			if (fillState == FILL_STATE_MIXED) {

				switch (renderState) {
					case E_RENDER_SOLID:
						if (noSolidFaces) {

						}
						else {
							glCallList(unitDL + max(
											min(
												mipLev,
												(giMIP_LEVELS-1)
											),
											giMIN_MIP
										)
							);
						}
					break;
					case E_RENDER_WATER:
						if (noWaterFaces) {

						}
						else {
							glCallList(waterDL + max(
											min(
												mipLev,
												(giMIP_LEVELS-1)
											),
											giMIN_MIP
										)
							);
						}
						
					break;
					default:
						cout << "ERROR INVALID RENDER STATE\n";
						return;
				}

				
				//drawCube(fOffsets.x/100.0f,fOffsets.y/100.0f,fOffsets.z/100.0f);
			}
		}
	}
	
	
};

struct PageThreadParam {
	fPage3D* threadPage;
	int opNum;
	int threadNum;
	float fParams[giNUM_THREAD_PARAMS];
};

PageThreadParam ptParams[giNUM_THREADS];


void *pageThreadStart( void *ptr )
{
	
	PageThreadParam *ptParam;
	ptParam = (PageThreadParam *) ptr;
	
	switch (ptParam->opNum) {
		case S_OP_SEAMLESS_SIMPLEX_NOISE:
			ptParam->threadPage->opSeamlessSimplexNoise(ptParam->fParams[0],ptParam->fParams[1],ptParam->fParams[2]);
		break;
		case S_OP_SIMPLEX_NOISE:
			ptParam->threadPage->opSimplexNoise(ptParam->fParams[0],ptParam->fParams[1],ptParam->fParams[2]);
		break;
		case S_OP_VORONOI:
			ptParam->threadPage->opVoronoi(ptParam->fParams[0],ptParam->fParams[1]);
		break;
		case S_OP_APPLY_TEXTURE:
			ptParam->threadPage->opApplyTexture();
		break;
		case S_RENDER_VOXELS:
			ptParam->threadPage->renderVoxels();
		break;
		default:
			cout << "WARNING: undefined operation in pageThreadStart: " << ptParam->opNum << "\n";
		break;
	}
	
	
	ptBusy[ptParam->threadNum] = false;
	//pthread_exit(0);

	return NULL;
}



class fGroup3D {
private:

public:
	iVector3 iPagesPerGroup;
	iVector3 iUnitsPerGroup;
	int iTotalPages;
	int iPageSize;

	fVector3 fPagesPerGroup;
	fVector3 fUnitsPerGroup;
	float fTotalPages;
	float fPageSize;

	fPage3D** pageArray;
	fPage3D* blankPage;
	fPage3D** textures;


	fGroup3D() {
		
	}

	void init(int _iPagesPerGroupX, int _iPagesPerGroupY, int _iPagesPerGroupZ, int _iPageSize, fPage3D** _textures=NULL) {
		int i;

		textures = _textures;

		iPagesPerGroup.x = _iPagesPerGroupX;
		iPagesPerGroup.y = _iPagesPerGroupY;
		iPagesPerGroup.z = _iPagesPerGroupZ;
		iPageSize = _iPageSize;


		iUnitsPerGroup.x = iPagesPerGroup.x*iPageSize;
		iUnitsPerGroup.y = iPagesPerGroup.y*iPageSize;
		iUnitsPerGroup.z = iPagesPerGroup.z*iPageSize;

		iTotalPages = iPagesPerGroup.x*iPagesPerGroup.y*iPagesPerGroup.z;

		
		fPageSize = (float)iPageSize;

		fPagesPerGroup.x = (float)iPagesPerGroup.x;
		fPagesPerGroup.y = (float)iPagesPerGroup.y;
		fPagesPerGroup.z = (float)iPagesPerGroup.z;

		fUnitsPerGroup.x = (float)iUnitsPerGroup.x;
		fUnitsPerGroup.y = (float)iUnitsPerGroup.y;
		fUnitsPerGroup.z = (float)iUnitsPerGroup.z;

		fTotalPages = (float)iTotalPages;

		pageArray = new fPage3D*[iTotalPages];
		blankPage = new fPage3D();
		blankPage->init(_iPageSize, 0, 0, 0, textures);
		blankPage->opClear();


		for (i = 0; i < iTotalPages; i++) {
			pageArray[i] = NULL;
		}

	}



	void opPagesAt(iVector3 curPos, int loadRad, int maxLoads, int ruleSet) {
		int i;
		int j;


		int tx;
		int ty;
		int tz;

		int tx2;
		int ty2;
		int tz2;

		int ind;
		int ind2;
		int ind3;
		int curOp = OP_NULL;
		int testOp;

		int threadOp;
		int threadCount = 0;

		int createdCount = 0;

		int loadCount = 0;

		int testID;

		bool stillGoing;

		for (i = 0; (i < oGrid->offsetGridCumCounters[loadRad]) && (loadCount < maxLoads) && (createdCount < 200) && (threadCount < giNUM_THREADS); i++) {
			tx = oGrid->offsetGridCum[i].x + (curPos.x/iPageSize);
			ty = oGrid->offsetGridCum[i].y + (curPos.y/iPageSize);
			tz = oGrid->offsetGridCum[i].z + (curPos.z/iPageSize);

			curOp = OP_NULL;
			testOp = OP_NULL;

			if (tx < 0 || tx >= iPagesPerGroup.x || ty < 0 || ty >= iPagesPerGroup.y || tz < 0 || tz >= iPagesPerGroup.z) {
				//out of bounds
			}
			else {


				ind = tx + ty*(iPagesPerGroup.x) + tz*(iPagesPerGroup.x*iPagesPerGroup.y);
				
				createdCount = 0;

				if (pageArray[ind] == NULL) {
					pageArray[ind] = new fPage3D();
					pageArray[ind]->init(iPageSize, tx, ty, tz, textures);
					createdCount++;
				}

				

				for (j = 0; j < 27; j++) {
					tx2 = tx + static_nxmods[j];
					ty2 = ty + static_nymods[j];
					tz2 = tz + static_nzmods[j];

					ind2 = (static_nxmods[j]+1) + (static_nymods[j]+1)*3 + (static_nzmods[j]+1)*9;


					if (tx2 < 0 || tx2 >= iPagesPerGroup.x || ty2 < 0 || ty2 >= iPagesPerGroup.y || tz2 < 0 || tz2 >= iPagesPerGroup.z) {
						pageArray[ind]->neighbors[ind2] = blankPage;
					}
					else {

						ind3 = tx2 + ty2*(iPagesPerGroup.x) + tz2*(iPagesPerGroup.x*iPagesPerGroup.y);

						if (pageArray[ind]->neighbors[ind2] == NULL) {
							if (pageArray[ind3] == NULL) {
								pageArray[ind3] = new fPage3D();
								pageArray[ind3]->init(iPageSize, tx2, ty2, tz2, textures);
								createdCount++;
							}
							pageArray[ind]->neighbors[ind2] = pageArray[ind3];
						}
					}
				}

				testOp = pageArray[ind]->currentState;

				


				switch (ruleSet) {
					case RS_TERRAIN:
						
						switch (testOp) {
							case E_INIT:
								curOp = S_OP_APPLY_TEXTURE;
							break;
							case E_OP_APPLY_TEXTURE:
								if (pageArray[ind]->testStates(E_OP_APPLY_TEXTURE)) {
									curOp = S_RENDER_VOXELS;
								}
							break;
							case E_RENDER_VOXELS:
								curOp = S_CREATE_DL;
							break;
							default:
								curOp = OP_NULL;
							break;
						}

					break;
				}


				threadOp = OP_NULL;


				switch (curOp) {
					
					case S_OP_CLEAR:
						pageArray[ind]->opClear();
						loadCount += 1;
					break;
					case S_OP_SIMPLEX_NOISE:
						threadOp = S_OP_SIMPLEX_NOISE;
						loadCount += 1;
					break;
					case S_OP_APPLY_TEXTURE:
						threadOp = S_OP_APPLY_TEXTURE;
						loadCount += 1;
					break;
					case S_RENDER_VOXELS:
						threadOp = S_RENDER_VOXELS;
						loadCount += 1;
					break;
					case S_CREATE_DL:
						pageArray[ind]->createDL();
						loadCount += giNUM_THREADS/2;
					break;
					default:
						loadCount += 0;
					break;
				}

				stillGoing = true;

				if (threadOp != OP_NULL) {
					while (stillGoing && (threadCount < giNUM_THREADS) ) {
						if (ptBusy[threadCount]) {
							threadCount++;
						}
						else {
							stillGoing = false;
						}
					}
					if (threadCount < giNUM_THREADS) {
						ptBusy[threadCount] = true;
						ptParams[threadCount].threadPage = pageArray[ind];
						ptParams[threadCount].opNum = threadOp;
						ptParams[threadCount].threadNum = threadCount;
						testID = pthread_create( &threadStore[threadCount], NULL, pageThreadStart, (void*) &(ptParams[threadCount]));
						threadCount++;
					}
					
				}

				/*
				
				*/

				loadCount += createdCount;
				
				
			}
		}
	}

	void doRender(iVector3 curPos, int loadRad, int renderState) {
		int i;

		int tx;
		int ty;
		int tz;

		int ox;
		int oy;
		int oz;

		int ind;

		for (i = 0; i < oGrid->offsetGridCumCounters[loadRad]; i++) {
			ox = oGrid->offsetGridCum[i].x;
			oy = oGrid->offsetGridCum[i].y;
			oz = oGrid->offsetGridCum[i].z;

			tx = ox + (curPos.x/iPageSize);
			ty = oy + (curPos.y/iPageSize);
			tz = oz + (curPos.z/iPageSize);

			if (tx < 0 || tx >= iPagesPerGroup.x || ty < 0 || ty >= iPagesPerGroup.y || tz < 0 || tz >= iPagesPerGroup.z) {
				//out of bounds
			}
			else {


				ind = tx + ty*(iPagesPerGroup.x) + tz*(iPagesPerGroup.x*iPagesPerGroup.y);
				

				if (pageArray[ind] == NULL) {
					
				}
				else {
					pageArray[ind]->renderDL( ((int)sqrt(ox*ox+oy*oy+oz*oz))/giMIP_SCALE, renderState );
				}

				
				
			}
		}
	}

};






class PageManager {
private:
	
public:
	
	fGroup3D* fgTerrain;

	fPage3D* fpTextures[E_TEX_TOTAL];

	iVector3 curPos;

	PageManager() {
		
	}

	void init() {

		int i;
		int testID;
		bool doRun;

		for (i = 0; i < E_TEX_TOTAL; i++) {
			fpTextures[i] = new fPage3D();
			fpTextures[i]->init(giTEXTURE_SIZE, 0, 0, 0);
			fpTextures[i]->setAllNeighbors(fpTextures[i]);
		}

		for (i = 0; i < E_TEX_TOTAL; i++) {

			doRun = true;
			ptParams[i].threadPage = fpTextures[i];
			ptParams[i].threadNum = i;

			switch (i) {
				case E_TEX_NOISE:
					ptParams[i].opNum = S_OP_SIMPLEX_NOISE;
					ptParams[i].fParams[0] = 1.0f;
					ptParams[i].fParams[1] = 1.0f;
					ptParams[i].fParams[2] = 0.5f;
					//fpTextures[i]->opSimplexNoise(1.0f,1.0f,0.5f);
				break;
				case E_TEX_SIMPLEX:
					ptParams[i].opNum = S_OP_SEAMLESS_SIMPLEX_NOISE;
					ptParams[i].fParams[0] = 3.0f;
					ptParams[i].fParams[1] = 0.5f;
					ptParams[i].fParams[2] = 1.0f/64.0f;
					//fpTextures[i]->opSimplexNoise(4.0f,0.5,1.0f/32.0f);
				break;
				case E_TEX_VORONOI:
					ptParams[i].opNum = S_OP_VORONOI;
					ptParams[i].fParams[0] = 16.0f;
					ptParams[i].fParams[1] = 0.5f;
					//fpTextures[i]->opVoronoi(16.0f,0.5f);
				break;
				default:
					doRun = false;
					cout << "INVALID OP IN PAGEMANAGER INIT: " << i << "\n";
				break;
			}

			testID = pthread_create( &threadStore[i], NULL, pageThreadStart, (void*) &(ptParams[i]));

		}


		
		
		
		
				
		for (i = 0; i < E_TEX_TOTAL; i++) {
			pthread_join( threadStore[i], NULL);
		}
		

		fgTerrain = new fGroup3D();
		fgTerrain->init(giPAGES_PER_GROUP[0], giPAGES_PER_GROUP[1], giPAGES_PER_GROUP[2], giUNITS_PER_PAGE, fpTextures);

		curPos.x = 0;
		curPos.y = 0;
		curPos.z = 0;

	}

	void setPosition(float x, float y, float z) {
		curPos.x = (int)x;
		curPos.y = (int)y;
		curPos.z = (int)z;
	}

	void update() {
		fgTerrain->opPagesAt(curPos, giLOAD_RAD, giNUM_THREADS, RS_TERRAIN);
	}

	void doRender(int renderState) {
		fgTerrain->doRender(curPos, giRENDER_RAD, renderState);
	}

};

class StyleSheetState {
	private:
		
	public:

		float props[E_SS_PROPS_SIZE];

		StyleSheetState() {
			
		}

		void setVal(int offsetVal, float x, float y, float z, float w) {
			props[offsetVal] = x;
			props[offsetVal+1] = y;
			props[offsetVal+2] = z;
			props[offsetVal+3] = w;

		}

		void init() {

			int i;

			for (i = 0; i < E_SS_PROPS_SIZE; i++) {
				props[i] = 0.0f;
			}
		}

		void copyFrom(StyleSheetState* cf) {
			int i;

			for (i = 0; i < E_SS_PROPS_SIZE; i++) {
				props[i] = cf->props[i];
			}
		}


};

class StyleSheet {
private:
	
public:
	StyleSheetState compStates[E_COMP_TOTAL];

	StyleSheet() {
		
	}

	void init() {
		int i;

		for (i = 0; i < E_COMP_TOTAL; i++) {
			compStates[i].init();
		}

	}

	


			
};

class StyleSheetResult: public StyleSheetState {
private:

public:
	StyleSheet* baseSS;
	float incVal;
	float stateTotal;
	float stateAmounts[E_COMP_TOTAL];

	typedef StyleSheetState super;

	StyleSheetResult() {
		
	}

	void init(StyleSheet* _baseSS) {
		baseSS = _baseSS;
		super::init();

		int i;

		incVal = 0.1f;

		for (i = 0; i < E_COMP_TOTAL; i++) {
			stateAmounts[i] = 0.0f;
		}

		stateAmounts[E_COMP_UP] = 1.0f;
	}


	void incrementVal(int enumVal) {
		int i;
		for (i = 0; i < E_COMP_TOTAL; i++) {
			stateAmounts[i] -= incVal;
		}

		stateAmounts[enumVal] += 2.0*incVal;

		for (i = 0; i < E_COMP_TOTAL; i++) {
			if (stateAmounts[i] < 0.0f) {
				stateAmounts[i] = 0.0f;
			}
			if (stateAmounts[i] > 1.0f) {
				stateAmounts[i] = 1.0f;
			}
		}

		float tot = 0.0f;

		for (i = 0; i < E_COMP_TOTAL; i++) {
			tot += stateAmounts[i];
		}
		stateTotal = tot;

	}

	void update(bool mouseOver, bool mouseDown) {
		

		if (mouseOver) {
			if (mouseDown) {
				incrementVal(E_COMP_DOWN);
			}
			else {
				incrementVal(E_COMP_OVER);
			}
		}
		else {
			incrementVal(E_COMP_UP);
		}


		int i;
		int j;

		float bval;

		for (i = 0; i < E_SS_PROPS_SIZE; i++) {

			props[i] = 0.0f;
			for (j = 0; j < E_COMP_TOTAL; j++) {
				bval = baseSS->compStates[j].props[i];
				props[i] += bval*stateAmounts[j];
			}
			props[i] /= stateTotal;
		}

	}
};

class UIComponent {
private:
	
public:


	int uid;
	bool mouseOver;
	bool mouseDown;
	bool wasHit;

	float value;
	float divisions;

	iVector2 align;
	int fillDir;

	iVector2 originPos;
	iVector2 resultDim;

	string text;
	int charScale;
	int charsPerLine;
	int maxLines;
	iVector2 charDim;
	iVector2 spacing;
	vector<string> lineVec;
	vector< vector<string> > wordVec;

	iBoundingBox hitBounds;

	StyleSheet* mainSS;
	StyleSheetResult resSS;


	enum eAlignH {
		E_ALIGN_LEFT,
		E_ALIGN_RIGHT,
		E_ALIGN_CENTER	
	};

	enum eAlignV {
		E_ALIGN_TOP,
		E_ALIGN_MIDDLE,
		E_ALIGN_BOTTOM	
	};

	enum eFillDir {
		E_FILL_HORIZONTAL,
		E_FILL_VERTICAL
	}; 


	UIComponent() {
		
	}

	void init(
				StyleSheet* _mainSS,
				string _text,
				
				int _originPosX,
				int _originPosY,
				int _charsPerLine=40,
				int _maxLines=5,
				
				int _alignX=E_ALIGN_LEFT,
				int _alignY=E_ALIGN_TOP,
				int _fillDir=E_FILL_HORIZONTAL,
				
				int _charScale=2,
				int _spacingX=0,
				int _spacingY=0
				


	) {

		wasHit = false;
		value = 0.0f;
		divisions = 0.0f;

		uid = giGUI_IDS;
		giGUI_IDS++;

		mouseDown = false;
		mouseOver = false;

		mainSS = _mainSS;
		resSS.init(mainSS);
		originPos.x = _originPosX;
		originPos.y = _originPosY;

		hitBounds.xMin = 0;
		hitBounds.xMax = 0;
		hitBounds.yMin = 0;
		hitBounds.yMax = 0;


		align.x = _alignX;
		align.y = _alignY;
		fillDir = _fillDir;
		charScale = _charScale;

		spacing.x = _spacingX;
		spacing.y = _spacingY;

		text = _text;
		
		

		charsPerLine = _charsPerLine;
		maxLines = _maxLines;
		
		charDim.x = 7;
		charDim.y = 12;

		refresh();
	}

	void updateValue(int x, int y) {
		if (wasHit) {
			if (divisions == 0.0f) {
				value = ((float)(x-hitBounds.xMin))/((float)(hitBounds.xMax-hitBounds.xMin));
			}
			else {
				if (divisions == 1.0f) {
					
				}
				else {
					value = floor(((float)(x-hitBounds.xMin))/((float)(hitBounds.xMax-hitBounds.xMin))*divisions)/divisions;
				}
			}
		}
	}

	void testOver(int x, int y) {
		mouseOver = false;

		if (x < hitBounds.xMax && x > hitBounds.xMin && y < hitBounds.yMax && y > hitBounds.yMin) {
			mouseOver = true;
		}

		updateValue(x, y);

	}

	bool testHit(int button, int state, int x, int y) {
		wasHit = false;

		testOver(x,y);

		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) {
				if (mouseOver) {
					wasHit = true;
				}

				mouseDown = true;
			}
			else {
				if (mouseOver) {
					if (divisions == 1.0f) {
						value = 1.0f-value;
					}
				}


				mouseDown = false;
			}
		}
		else {
				
		}

		updateValue(x, y);

		return wasHit;
	}
	

	void setText(string _text) {
		text = _text;

		refresh();
	}

	void refresh() {

		lineVec.clear();
		wordVec.clear();

		lineVec = split(text, '\n');

		int i;
		int j;

		for (i = 0; i < lineVec.size(); i++) {
			wordVec.push_back(vector<string>());
			wordVec[i] = split(lineVec[i], ' ');
		}

		render(false);
	}

	inline void renderCharAt(int c, int px, int py) {
		
		iBoundingBox ibbPos;
		fBoundingBox fbbPos;

		fBoundingBox fbbSamplePos;

		ibbPos.xMin = px;
		ibbPos.yMin = py;
		ibbPos.xMax = px+charDim.x*charScale;
		ibbPos.yMax = py+charDim.y*charScale;

		if (c > 32 && c < 126) {
			fbbSamplePos.xMin = ((float)((c-32)*charDim.x - 1))/656.0;
		}
		else {
			fbbSamplePos.xMin = 0.0f;
		}

		fbbSamplePos.xMax = ((float)((c+1-32)*charDim.x - 1))/656.0;
		fbbSamplePos.yMin = 0.0f;
		fbbSamplePos.yMax = 1.0f;

		float x0 = ((float)ibbPos.xMin)/windowSize[0];
		float x1 = ((float)ibbPos.xMax)/windowSize[0];
		float y0 = ((float)ibbPos.yMin)/windowSize[1];
		float y1 = ((float)ibbPos.yMax)/windowSize[1];

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		
		//dimensions
		glMultiTexCoord4f(1, charDim.x*charScale, charDim.y*charScale, 0.0f, 0.0f);

		glMultiTexCoord4f(4, 1.0f, 1.0f, 1.0f, 1.0f);
		glMultiTexCoord4f(5, 1.0f, 1.0f, 1.0f, 1.0f);
		//border color
		glMultiTexCoord4f(6, 1.0f, 1.0f, 1.0f, 1.0f);
		//misc
		glMultiTexCoord4f(7, 0.0f, 0.0f, 0.0f, 0.0f);
		

		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOLTEXT1_R], resSS.props[E_SS_BGCOLTEXT1_G], resSS.props[E_SS_BGCOLTEXT1_B], resSS.props[E_SS_BGCOLTEXT1_A]);
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOLTEXT1_R], resSS.props[E_SS_FGCOLTEXT1_G], resSS.props[E_SS_FGCOLTEXT1_B], resSS.props[E_SS_FGCOLTEXT1_A]);
		
		
		glMultiTexCoord4f(0, fbbSamplePos.xMin, fbbSamplePos.yMin, 0.0f, 1.0f);
		glVertex3f (  x0, y1, -1.0f );
		glMultiTexCoord4f(0, fbbSamplePos.xMax, fbbSamplePos.yMin, 1.0f, 1.0f);
		glVertex3f (  x1, y1, -1.0f );

		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOLTEXT0_R], resSS.props[E_SS_BGCOLTEXT0_G], resSS.props[E_SS_BGCOLTEXT0_B], resSS.props[E_SS_BGCOLTEXT0_A]);
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOLTEXT0_R], resSS.props[E_SS_FGCOLTEXT0_G], resSS.props[E_SS_FGCOLTEXT0_B], resSS.props[E_SS_FGCOLTEXT0_A]);		

		glMultiTexCoord4f(0, fbbSamplePos.xMax, fbbSamplePos.yMax, 1.0f, 0.0f);
		glVertex3f (  x1, y0, -1.0f );
		glMultiTexCoord4f(0, fbbSamplePos.xMin, fbbSamplePos.yMax, 0.0f, 0.0f);
		glVertex3f (  x0, y0, -1.0f );

	}

	inline void renderQuad(int ix0, int iy0, int ix1, int iy1) {


		float x0 = ((float)ix0)/windowSize[0];
		float x1 = ((float)ix1)/windowSize[0];
		float y0 = ((float)iy0)/windowSize[1];
		float y1 = ((float)iy1)/windowSize[1];

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		
		//dimensions
		glMultiTexCoord4f(1, ix1-ix0, iy1-iy0, resSS.props[E_SS_BORDER], resSS.props[E_SS_CORNERRAD]);

		//glMultiTexCoord4f(4, 1.0f, 1.0f, 1.0f, 1.0f);
		glMultiTexCoord4f(5, 1.0f, 1.0f, 1.0f, 1.0f);
		
		//border color
		glMultiTexCoord4f(6, resSS.props[E_SS_BDCOL_R], resSS.props[E_SS_BDCOL_G], resSS.props[E_SS_BDCOL_B], resSS.props[E_SS_BDCOL_A]);
		//misc
		glMultiTexCoord4f(7, resSS.props[E_SS_ROUNDNESS], value, 1.0f, 1.0f);


		//bg
		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOL1_R], resSS.props[E_SS_BGCOL1_G], resSS.props[E_SS_BGCOL1_B], resSS.props[E_SS_BGCOL1_A]);
		//fg
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOL1_R], resSS.props[E_SS_FGCOL1_G], resSS.props[E_SS_FGCOL1_B], resSS.props[E_SS_FGCOL1_A]);
		//tg
		glMultiTexCoord4f(4, resSS.props[E_SS_TGCOL1_R], resSS.props[E_SS_TGCOL1_G], resSS.props[E_SS_TGCOL1_B], resSS.props[E_SS_TGCOL1_A]);
		
		
		glMultiTexCoord4f(0, 0.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f (  x0, y1, -1.0f );
		glMultiTexCoord4f(0, 0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f (  x1, y1, -1.0f );

		//bg
		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOL0_R], resSS.props[E_SS_BGCOL0_G], resSS.props[E_SS_BGCOL0_B], resSS.props[E_SS_BGCOL0_A]);
		//fg
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOL0_R], resSS.props[E_SS_FGCOL0_G], resSS.props[E_SS_FGCOL0_B], resSS.props[E_SS_FGCOL0_A]);
		//tg
		glMultiTexCoord4f(4, resSS.props[E_SS_TGCOL0_R], resSS.props[E_SS_TGCOL0_G], resSS.props[E_SS_TGCOL0_B], resSS.props[E_SS_TGCOL0_A]);

		glMultiTexCoord4f(0, 0.0f, 0.0f, 1.0f, 0.0f);
		glVertex3f (  x1, y0, -1.0f );
		glMultiTexCoord4f(0, 0.0f, 0.0f, 0.0f, 0.0f);
		glVertex3f (  x0, y0, -1.0f );


	}

	void render(bool visible) {
		
		

		int i;
		int j;
		int k;
		int curChar;

		iVector3 caretPos;
		
		if (visible) {
			resSS.update(mouseOver, mouseDown);
		}
		
		int carriage = originPos.x + resSS.props[E_SS_MARGIN] + resSS.props[E_SS_BORDER] + resSS.props[E_SS_PADDING];

		caretPos.x = carriage;
		caretPos.y = originPos.y + resSS.props[E_SS_MARGIN] + resSS.props[E_SS_BORDER] + resSS.props[E_SS_PADDING];

		int charCount = 0;

		int maxSize;

		int bufVal = resSS.props[E_SS_BORDER] + resSS.props[E_SS_PADDING];

		if (visible) {
			hitBounds.xMin = originPos.x - bufVal;
			hitBounds.xMax = resultDim.x + bufVal*2;
			hitBounds.yMin = originPos.y - bufVal;
			hitBounds.yMax = resultDim.y + bufVal*2;
			renderQuad(hitBounds.xMin, hitBounds.yMin, hitBounds.xMax, hitBounds.yMax);
		}

		//lines
		for (i = 0; i < wordVec.size(); i++) {

			//words in line
			for (j = 0; j < wordVec[i].size(); j++) {

				//if word won't fit, increment line
				if ( (charCount + wordVec[i][j].size()) > charsPerLine ) {
					charCount = 0;
					caretPos.x = carriage;
					caretPos.y += (charDim.y + spacing.y)*charScale;
				}
				//if word won't fit on line, limit chars
				maxSize = min ( (int)wordVec[i][j].size() , charsPerLine);
				
				//chars in word
				for (k = 0; k < maxSize; k++) {
					curChar = wordVec[i][j][k];
					if (visible) {
						renderCharAt(curChar, caretPos.x, caretPos.y);
					}
					caretPos.x += (charDim.x + spacing.x)*charScale;
					charCount++;
				}

				
				curChar = ' ';
				if (visible) {
					if (charCount+1 < charsPerLine) {
						renderCharAt(curChar, caretPos.x, caretPos.y);
					} 
					
				}
				caretPos.x += (charDim.x + spacing.x)*charScale;
				charCount++;

			}
			charCount = 0;
			caretPos.x = carriage;
			caretPos.y += (charDim.y + spacing.y)*charScale;

		}

		resultDim.x = carriage + charsPerLine*(charDim.x + spacing.x)*charScale;
		resultDim.y = caretPos.y-( (spacing.y)*charScale );

	}

};

class GameGUI {
private:
	
public:
	Image* textImage;
	GLuint guiTextureID;

	Shader shaderGUI;

	UIComponent frameRateTB;
	UIComponent testTB;

	StyleSheet mainSS;

	GameGUI() {
		
	}

	void init(float windowX, float windowY) {

		textImage = loadBMP("res/basicFont.bmp");
		guiTextureID = loadTexture(textImage);
	    shaderGUI.init( "shaders/shaderGUI.c");
	    mainSS.init();




		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL0_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL0_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL0_B] = 0.7f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL0_A] = 1.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL1_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL1_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL1_B] = 0.7f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL1_A] = 0.5f;

		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL0_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL0_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL0_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL0_A] = 0.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL1_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL1_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL1_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL1_A] = 0.0f;



		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL0_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL0_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL0_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL0_A] = 1.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL1_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL1_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL1_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL1_A] = 0.5f;

		

		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT0_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT0_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT0_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT0_A] = 0.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT1_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT1_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT1_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT1_A] = 0.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT0_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT0_G] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT0_B] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT0_A] = 1.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT1_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT1_G] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT1_B] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT1_A] = 1.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_BDCOL_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BDCOL_G] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BDCOL_B] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BDCOL_A] = 1.0f;



	    mainSS.compStates[E_COMP_UP].props[E_SS_PADDING] = 2.0f;
	    mainSS.compStates[E_COMP_UP].props[E_SS_BORDER] = 2.0f;
	    mainSS.compStates[E_COMP_UP].props[E_SS_MARGIN] = 2.0f;
	    mainSS.compStates[E_COMP_UP].props[E_SS_CORNERRAD] = 8.0f;
	    mainSS.compStates[E_COMP_UP].props[E_SS_ROUNDNESS] = 1.0f;

	    mainSS.compStates[E_COMP_OVER].copyFrom(& (mainSS.compStates[E_COMP_UP]) );
	    mainSS.compStates[E_COMP_DOWN].copyFrom(& (mainSS.compStates[E_COMP_UP]) );

	    
	    mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL0_R] = 0.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL0_G] = 0.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL0_B] = 1.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL0_A] = 1.0f;

		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL1_R] = 0.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL1_G] = 0.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL1_B] = 1.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL1_A] = 0.7f;


		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL0_R] = 0.2f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL0_G] = 0.2f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL0_B] = 1.0f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL0_A] = 1.0f;

		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL1_R] = 0.2f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL1_G] = 0.2f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL1_B] = 1.0f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL1_A] = 0.5f;




	    frameRateTB.init(&mainSS,"0.0f",20,20);
		testTB.init(
						&mainSS,
						"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi nec ligula ac massa consectetur consectetur. Sed vel malesuada arcu. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque laoreet lorem sed nisi blandit malesuada. Donec consequat urna et metus feugiat faucibus vulputate tortor blandit.\n\nCras dignissim, sapien ut tristique faucibus, enim augue venenatis neque, vel condimentum lacus est vitae urna...",
						20,
						80
					);
	}

	void testOver(int x, int y) {
		testTB.testOver(x, y);
	}

	bool testHit(int button, int state, int x, int y) {
		return testTB.testHit(button, state, x, y);
	}

	void renderGUI(bool showGUI) {

		glDisable(GL_DEPTH_TEST);
		glEnable (GL_BLEND);

		glMatrixMode (GL_MODELVIEW);
		glPushMatrix ();
		glLoadIdentity ();
		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();
		
		glEnable(GL_TEXTURE_2D);

		

		if (frameCount % 60 == 0) {
			string s1 = floatToString(curFPS);
			string s2 = " FPS";
			frameRateTB.setText(s1+s2);
		}
		

		shaderGUI.bind();

		shaderGUI.setTexture("Texture0", 0, guiTextureID);

		//shaderGUI.setVec("fgColor",fgCol,4);
		//shaderGUI.setVec("bgColor",bgCol,4);
		
		glBegin (GL_QUADS);
			if (showGUI) {
				testTB.render(true);
			}
			frameRateTB.render(true);
		glEnd ();

		shaderGUI.unbind();



		



		glPopMatrix ();
		glMatrixMode (GL_MODELVIEW);
		glPopMatrix ();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

};
Singleton* singleton;


void processSpecialKeys(int key, int x, int y) {
	singleton->processSpecialKeys(key,x,y);
}
void processKey(unsigned char key, int x, int y, bool isPressed) {
	singleton->processKey(key,x,y,isPressed);
}
void keyboardUp(unsigned char key, int x, int y) {
	singleton->keyboardUp(key,x,y);
}
void keyboardDown(unsigned char key, int x, int y) {
	singleton->keyboardDown(key,x,y);
}
void mouseMovementWithButton(int x, int y) {
	singleton->mouseMovementWithButton(x,y);
}
void mouseMovementWithoutButton(int x, int y) {
	singleton->mouseMovementWithoutButton(x,y);
}
void mouseClick(int button, int state, int x, int y) {
	singleton->mouseClick(button,state,x,y);
}
void display(void) {
	singleton->display();
}
void reshape (int w, int h) {
	singleton->reshape(w,h);
}
void idleFunc(void) {
	singleton->idleFunc();
}


int WebSocketServer::main(const std::vector<std::string>& args)
{

	srand ( time(NULL) );
	int winW = 1024;
	int winH = 1024;

	int argCount = 0;
	char** argStrs = NULL;

	g_RecBuffer = new char[g_MAX_FRAME_SIZE];


	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

	if (_helpRequested)
	{
		displayHelp();
	}
	else
	{
		unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 9980);
		ServerSocket svs(port);
		HTTPServer srv(new RequestHandlerFactory, svs, new HTTPServerParams);
		srv.start();
		//waitForTerminationRequest();



		glutInit(&argCount, argStrs); //&argc, argv

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	
		glutInitWindowSize(winW, winH);
		glutInitWindowPosition(100, 100);
		glutCreateWindow("Voxel Quest");
		glutDisplayFunc(display);
		glutIdleFunc(display);
		glutReshapeFunc(reshape);
		glutPassiveMotionFunc(mouseMovementWithoutButton);
		glutMotionFunc(mouseMovementWithButton);
		glutMouseFunc(mouseClick);
		glutKeyboardFunc(keyboardDown);
		glutKeyboardUpFunc(keyboardUp);
		glutSpecialFunc(processSpecialKeys);
		singleton = new Singleton();
		singleton->init(winW,winH);
		glutMainLoop();



		srv.stop();
	}

	delete[] g_RecBuffer;

	return Application::EXIT_OK;
}


POCO_SERVER_MAIN(WebSocketServer)

