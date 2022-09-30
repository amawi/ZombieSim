#include "Management.h"

long CManagement::ErrorChecker(long lError) const
{
	if (lError != ERR_NOERROR)
	{

#ifdef _WINDOWS
		printf_s("Error %i detected: %s \n", lError, GetErrorDescription(lError));
#elif __linux__
		printf("Error %i detected: %s \r\n", lError, GetErrorDescription(lError));
#endif

		return lError;
	}
	else
		return ERR_NOERROR;

}

const char* CManagement::GetErrorDescription(long lError)const
{
	switch (lError)
	{
	case ERR_NOERROR:
		return "Error ERR_NOERROR, No Error Occured.";
	case PARSE_COMMAND_LINE:
		return "Error PARSE_COMMAND_LINE, could not parse the command line.";
	case DIRLIST_FAIL:
		return "Error DIRLIST_FAIL, failed to obtain files with the chosen directory.";
	case PARSE_COMMAND_LINE_MAN:
		return "Presented the different options (flags) to run this program.";
	case SET_FILE_BUFFER_FAIL:
		return "Error SET_FILE_BUFFER_FAIL, could not set the file buffer.";
	case INIT_FILE_SAFE_MODE:
		return "Error INIT_FILE_SAFE_MODE, could not init the file buffer containers, do not know how many file containers have to be made.";
	case SET_CITY_INTO_VECTOR:
		return "Error SET_CITY_INTO_VECTOR, could not insert the City object into the Country Object.";
	case GET_COUNTRY_SIZE_FROM_PROPERTYBAG:
		return "Error GET_COUNTRY_SIZE_FROM_PROPERTYBAG, could not obtain the country size from the propertybag.";
	case SET_POPULATION_SIZE:
		return "Error SET_POPULATION_SIZE, could not set the population size.";
	case GET_POPULATION_SIZE:
		return "Error GET_POPULATION_SIZE, could not get the population size.";
	case SET_CITY_POINTER:
		return "Error SET_CITY_POINTER, could not set the City pointer within the object.";
	case SET_HUMAN_VECTOR:
		return "Error SET_HUMAN_VECTOR, could not set the human vector.";
	case SET_CITY_VECTOR:
		return "Error SET_CITY_VECTOR, could not set the City Vector.";
	case SET_COUNTRY_POINTER:
		return "Error SET_COUNTRY_POINTER, could not set the Country pointer.";
	case GET_COUNTRY_POINTER:
		return "Error GET_COUNTRY_POINTER, could not get the country pointer.";
	case SET_CITY_NAME:
		return "Error SET_CITY_NAME, could not set the City name of the city.";
	case SET_CITY_POSITION:
		return "Error SET_CITY_POSITION, could not set the city position on the for on the playfield.";
	case SET_CITY_SIZE:
		return "Error SET_CITY_SIZE, could not set the city size.";
	case GET_CITY_AMOUNT:
		return "Error GET_CITY_AMOUNT, could not get the amount of cities made within the playfield.";
	case GET_COUNTRY_SIZE:
		return "Error GET_COUNTRY_SIZE, could not get the size of the country.";
	case GET_START_SICK:
		return "ERROR GET_START_SICK, could not get the amount of sick humans for starting the simulation.";
	case GET_HUMAN_VECTOR_SIZE:
		return "Error GET_HUMAN_VECTOR_SIZE, could not get the amount of humans needed for the human object vector.";
	case CREATE_VISUAL_CONSTRUCT_FAIL:
		return "Error CREATE_VISUAL_CONSTRUCT_FAIL, could not create the visual layer for the ZombieSim.";
	default:
		return "ERROR UNKOWN, no error number, unknown error.";
	}
	return "ERROR UNKNOWN ERROR occured";
}