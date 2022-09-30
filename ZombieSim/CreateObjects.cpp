#include "Helper.h"
#include "Country.h"


long CreatePlayFieldObjects(){
    //It should be nested
    long lReturn = ERR_NOERROR;

    lReturn = country.SetCities(); ///Country is already set when we create the object
    if (lReturn != ERR_NOERROR)
        return lReturn;
    lReturn = country.SetPopulation(); //Set the population.  aka movestatus, livingstatus, current loc, work loc, home loc etc.
    if (lReturn != ERR_NOERROR)
        return lReturn;

    lReturn = country.SetLivingStatuses(); //For some we are going to make them purposely sick of course. By default they are healthy
    if (lReturn != ERR_NOERROR)
        return lReturn;




    return ERR_NOERROR;
}