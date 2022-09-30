#pragma once
#include <cstdio>

#ifndef ERR_NOERROR
#define ERR_NOERROR 0
#endif // !ERR_NOERROR

enum MANAGEMENT_ERRORS
{
	ERR_ERROR = 1,
	DIRLIST_FAIL,
	PARSE_COMMAND_LINE,
	PARSE_COMMAND_LINE_MAN,
	SET_FILE_BUFFER_FAIL,
	INIT_FILE_SAFE_MODE,

	SET_CITY_INTO_VECTOR = 100,
	SET_POPULATION_SIZE,
	GET_POPULATION_SIZE,
	SET_HUMAN_INTO_CITYVECTOR = 200,
	SET_POPULATION_SIZE_CITY,
	GET_POPULATION_SIZE_CITY,
	SET_SICK_IN_CITY,
	GET_COUNTRY_SIZE_FROM_PROPERTYBAG,
	SET_CITY_POINTER,
	SET_HUMAN_VECTOR,
	SET_CITY_VECTOR,
	SET_COUNTRY_POINTER,
	GET_COUNTRY_POINTER,
	SET_CITY_NAME,
	SET_CITY_POSITION,
	SET_CITY_SIZE,
	GET_CITY_AMOUNT,
	GET_COUNTRY_SIZE,
	GET_START_SICK,
	GET_HUMAN_VECTOR_SIZE,
	CREATE_VISUAL_CONSTRUCT_FAIL,
};

class CManagement
{
public:
	CManagement() {};
	~CManagement() {};
	virtual const char* GetErrorDescription(long lError) const;
	long ErrorChecker(long lError) const;
};
