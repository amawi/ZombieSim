#include "Country.h"

long CCity::SetCountry(CCountry* pCountry)
	{
		if (pCountry == nullptr)
			return SET_COUNTRY_POINTER; //Error message
		m_pCountry = pCountry;
		return ERR_NOERROR;
	}

CCountry * CCity::GetCountry() { return m_pCountry; }
long CCity::GetCountry(CCountry** ppCountry)
{
	if (m_pCountry == nullptr)
		return GET_COUNTRY_POINTER; // Error Message

	*ppCountry = m_pCountry;
	return ERR_NOERROR;
}

long CCity::SetCityName(char * pCityName, size_t sizeToCopy)//init function
{
    if (m_pCityName)
        delete m_pCityName;

    if (sizeToCopy <= 0)
        return SET_CITY_NAME; // Error handling

    m_pCityName = new char[sizeToCopy + 10];
    memset(m_pCityName, '\0', (sizeToCopy + 10)); //zero terminate the string
    memcpy(m_pCityName, pCityName, sizeToCopy); //Copy the data into the buffer

    return ERR_NOERROR;

}

long CCity::SetCityPositions(float fCitySize, float px, float py) //Init function doesn't have to be thread safe
{
	m_mutex.lock();
    if (fCitySize <= 0.0)
        return SET_CITY_POSITION; // ErrorHandling

    if(px < 0.0)
        return SET_CITY_POSITION; // ErrorHandling

    if(py < 0.0)
        return SET_CITY_POSITION; // ErrorHandling

    m_fCitySize = fCitySize;
    m_px = px;
    m_py = py;
	m_mutex.unlock();

    return ERR_NOERROR;
}


float CCity::GetPosY() { boost::lock_guard<boost::mutex> lock{ m_mutex }; return m_py; }
float CCity::GetPosX() { boost::lock_guard<boost::mutex> lock{ m_mutex }; return m_px; }

char* CCity::GetCityName() { return m_pCityName; }

long CCity::SetCitySize(float fCitySize)
{
    if (fCitySize <= 0.0)
        return SET_CITY_SIZE; //Error Handling

    m_fCitySize = fCitySize;
    return ERR_NOERROR;
}

float CCity::GetCitySize() 
{ 
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_fCitySize; 
}

void CCity::SetCityX(float fPX)
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
    m_px = fPX;
}

float CCity::GetCityX()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
    return m_px;
}

float CCity::SetCityY(float fPY)
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
    m_py = fPY;
}

float CCity::GetCityY()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
    return m_py;
}

long CCity::GetHealthyPopulation() { boost::lock_guard<boost::mutex> lock{ m_mutex }; return m_lHealthy; }
long CCity::GetSickPopulation() { boost::lock_guard<boost::mutex> lock{ m_mutex }; return m_lSick; }
long CCity::GetDeathPopulation() { boost::lock_guard<boost::mutex> lock{ m_mutex }; return m_lDeath; }

void CCity::AddHealthy(bool bAddHealthy /*= true*/)
{
	m_mutex.lock();
    if (bAddHealthy == true)
        m_lHealthy++;
    else
        m_lHealthy--;
	m_mutex.unlock();

    m_pCountry->AddHealthy(bAddHealthy); //Has it's own mutex!
}

void CCity::AddDeath(bool bAddDeath /*= true*/)
{
	m_mutex.lock();
    if (bAddDeath == true)
        m_lDeath++;
    else
        m_lDeath--;
	m_mutex.unlock();

    m_pCountry->AddDeath(bAddDeath);
}

void CCity::AddSick(bool bAddSick/*= true*/)
{
	m_mutex.lock();
    if (bAddSick == true)
        m_lSick++;
    else
        m_lSick--;
	m_mutex.unlock();
    m_pCountry->AddSick(bAddSick);

}

void CCity::PushBack(CHuman* pHuman)
{
    m_vecHumanPointers.push_back(pHuman);
}

size_t CCity::GetHumanVectorSize() { 
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_vecHumanPointers.size(); }

CHuman * CCity::GetHuman(size_t idx)
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
    return m_vecHumanPointers.at(idx);
}

//Init functions. not thread related!!
long CCountry::SetPopulation()
	{
		long lReturn = ERR_NOERROR;
		
		//Set population size
		size_t sizeHumanPopulation = 0;;
		lReturn = glb.propertyBag.GetPopulationSize(&sizeHumanPopulation);
		m_sizePopulation = (unsigned)sizeHumanPopulation; //population can never be negative.
		if (lReturn != ERR_NOERROR)
			return GET_POPULATION_SIZE;

		long lCities;


		//Set healthy group
		m_sizeHealthy = m_sizePopulation;
		m_sizeSick = 0;
		m_sizeDeath = 0;

		//Now the population has to be divided over the amount of cities (for now we are just going to equally divide it.
		//In the future we reduce the city size amount to increase the population density.

		//Get the amount of cities based on the vector. If vector size is = 0 call Set Cities
		//Call it again af it is again 0, return error message.

		size_t sizeCities = m_vecCities.size();
		if (sizeCities <= 0)
			lReturn = SetCities();

		if (lReturn != ERR_NOERROR)
			return lReturn;

		if (sizeCities <= 0)
			return SET_CITY_SIZE; //Error handling


		/*
		Now we are going to fill the human object vectorand set the City* within them.
		We are going to get the amount of humans per city Humans/City.
		Where the last city gets the remaining humans when we have a fraction.

		*/

		bool bFraction = false;

		double dFraction = (double)(sizeHumanPopulation/ sizeCities);
		double dFractionSubstract = dFraction - static_cast<long long>(dFraction);
		if (dFraction > 0.0)//if it's a fraction or not!
			bFraction = true;

		long long llDivide = static_cast<long long>(dFraction);
		CHuman* pHuman = nullptr;
		CCity* pCity = nullptr;
		
		//Going to create Human Objects and set the City Pointers.
		for (size_t idx = 0; idx < sizeCities; idx++)
		{
			pCity = m_vecCities.at(idx);
			for (long long idxHuman = 0; idxHuman < llDivide; idxHuman++)
			{
				pHuman = new CHuman;
				lReturn = pHuman->SetCityPointer(pCity);
				if (lReturn != ERR_NOERROR)
					return lReturn;
				m_vecHumans.push_back(pHuman);
				pCity->PushBack(pHuman); //this is just for easy access/calculations.
			}
		}

		//if we have a fraction we have some remaining humans they are added to the last one
		if (bFraction == true)
		{
			size_t sizeHumanPushed = (size_t)llDivide * sizeCities;
			size_t sizeRemainingToBePushed = sizeHumanPopulation - sizeHumanPushed;

			pCity = m_vecCities.at(sizeCities - 1);

			for (size_t idxHumanToBePushed = 0; idxHumanToBePushed < sizeRemainingToBePushed; idxHumanToBePushed++)
			{
				pHuman = new CHuman;
				lReturn = pHuman->SetCityPointer(pCity);
				if (lReturn != ERR_NOERROR)
					return lReturn;
				m_vecHumans.push_back(pHuman);
				pCity->PushBack(pHuman); //this is just for easy access/calculations.
			}
		}

		//So at this point we have all the cities and all the humans.
		//But the humans do not have a home address or a work address time to set those.


		//Step 1, Set the Home Address and work address in the same City.
		//First we have to know how big a human family unit is. Aka how many humans live for sure on the same spot.
		long lHumanFamilyUnit = 0;
		lReturn = glb.propertyBag.GetFamilyUnit(&lHumanFamilyUnit);
		if (lReturn != ERR_NOERROR)
			return lReturn;

		if (lHumanFamilyUnit <= 0)
			lHumanFamilyUnit = 2; //Just as a default number in case someone added something weird.

		//Now we do the same thing as we did for the cities. We need to know if we have a fraction or not. The remaining family will be Smaller. (lucky them!
		//Get the Respective pCity, since we have to know how many family units we have, so we round it up
		//eg we have 9 humans, family unit = 4. Divide is 2.25 We will have 3 family units. 4,4,1

		size_t sizeHumanVectorInCity = 0;
		size_t sizeRoundUpFamilyUnit = 0;
		size_t idxHumanVector = 0;

		float fCityX = 0.0;
		float fHumanHomeX = 0.0;
		float fHumanWorkX = 0.0;
		float fCityY = 0.0;
		float fCityY2 = 0.0;
		float fHumanHomeY = 0.0;
		float fHumanWorkY = 0.0;
		float fCitySize = 0.0;

		std::random_device RNG;  // Will be used to obtain a seed for the random number engine
		std::mt19937 Seed(RNG()); // Standard mersenne_twister_engine seeded with RNG()

		
		for (size_t idxCity = 0; idxCity < sizeCities; idxCity++)
		{
			idxHumanVector = 0; //Reset it
			
			pCity = m_vecCities.at(idxCity);
			sizeHumanVectorInCity = pCity->GetHumanVectorSize();
			if (sizeHumanVectorInCity <= 0)
				return GET_HUMAN_VECTOR_SIZE; // Error Handling.

			fCitySize = pCity->GetCitySize();
			fCityX = pCity->GetCityX();
			fCityY = pCity->GetCityY();
			fCityY2 = fCityY + (fCitySize * 0.5);
								
			std::uniform_real_distribution<> XCoordinateHome(fCityX, (fCityX + fCitySize));
			std::uniform_real_distribution<> XCoordinateWork(fCityX, (fCityX + fCitySize));
			std::uniform_real_distribution<> YCoordinateHome(fCityY, fCityY2);
			std::uniform_real_distribution<> YCoordinateWork(fCityY2, (fCityY + fCitySize));

			sizeRoundUpFamilyUnit = (size_t)std::round(((long double)sizeHumanVectorInCity / (long double)lHumanFamilyUnit));
			for (size_t idxFamilyUnit = 0; idxFamilyUnit < sizeRoundUpFamilyUnit; idxFamilyUnit++)
			{

				fHumanHomeX = XCoordinateHome(Seed); //stay for every family unit the same;
				fHumanHomeY = YCoordinateHome(Seed);

				for (long lFamilyUnitMember = 0; lFamilyUnitMember < lHumanFamilyUnit; lFamilyUnitMember++)
				{
					if (idxHumanVector < sizeHumanVectorInCity)
					{ 
						pHuman = pCity->GetHuman(idxHumanVector);
						fHumanWorkX = XCoordinateWork(Seed);
						fHumanWorkY = YCoordinateWork(Seed);

						lReturn = pHuman->InitPositions(fHumanHomeX, fHumanHomeY, fHumanWorkX, fHumanWorkY, fHumanHomeX, fHumanHomeY);
						if (lReturn != ERR_NOERROR)
							return lReturn;

						idxHumanVector++;
					}
					else
						break;
					
				}
			}

			

		}

		//We have no set the positions of all our human objects.
		//Now we have some randomization that some humans go visit other cities.
		//Step 2 Set % of humans to work in other cities;
		long lRateOfDifferentCityWorkAddress = 0;
		lReturn = glb.propertyBag.GetRateOfDifferentWorkAddress(&lRateOfDifferentCityWorkAddress);
		if (lReturn != ERR_NOERROR)
			return lReturn;

		//So every city have x% members that work in a different city. We round it down
		//With 1 City -> no randomization
		//With 2 City -> City 1 and City 2 swap.
		//With 3 cities -> City1 -> City 2, City 2 -> City 3 and City 3 -> City 1. Etc.

		size_t sizeCityLoopRuns = 0;
		
		CCity* pMoveToCity = nullptr;
		size_t idxCityToMove = 0;
		size_t idxCityToMovePos = 0;
		size_t sizeRateOfDifferentCityWorkAddress = (size_t)lRateOfDifferentCityWorkAddress;

		if (sizeCities >= 2)
		{
			for (size_t idxCity2 = 0; idxCity2 < sizeCities; idxCity2++)
			{
				idxCityToMovePos = 0;
				pCity = m_vecCities.at(idxCity2);
				if (idxCity2 + 1 == sizeCities)
					idxCityToMove = 0;
				else
					idxCityToMove = idxCity2 + 1;

				pMoveToCity = m_vecCities.at(idxCityToMove);
				sizeCityLoopRuns = std::round((float) pCity->GetHumanVectorSize() / (float)lRateOfDifferentCityWorkAddress);
			
				fCitySize = pMoveToCity->GetCitySize();
				fCityX = pMoveToCity->GetCityX();
				fCityY = pMoveToCity->GetCityY();
				fCityY2 = fCityY + (fCitySize * 0.5);

				std::uniform_real_distribution<> XCoordinateWork2(fCityX, (fCityX + fCitySize));
				std::uniform_real_distribution<> YCoordinateWork2(fCityY2, (fCityY + fCitySize));

				for (size_t idxCityLoop = 0; idxCityLoop < sizeCityLoopRuns; idxCityLoop++)
				{
					pHuman = pCity->GetHuman(idxCityToMovePos);
					fHumanWorkX = XCoordinateWork2(Seed);
					fHumanHomeY = YCoordinateWork2(Seed);

					lReturn = pHuman->SetWork(fHumanWorkX, fHumanWorkY);

					idxCityToMovePos = idxCityToMovePos + sizeRateOfDifferentCityWorkAddress;
				}
			}
		}

		//Done
		return lReturn;
	}

long CCountry::SetCities()
{
	long lReturn = ERR_NOERROR;

	long lCitiesAmount = 0;
	lReturn = glb.propertyBag.GetCitiesAmount(&lCitiesAmount);
	if (lReturn != ERR_NOERROR)
		return GET_CITY_AMOUNT; //Error Message

	CCity* pCity = nullptr;
	for (size_t idx = 0; idx < lCitiesAmount; idx++) //create city objects
	{
		pCity = new CCity;
		m_vecCities.push_back(pCity);
	}

	std::string strCityNames;
	lReturn = glb.propertyBag.GetCitiesName(&strCityNames);
	size_t sizeString = strCityNames.length();

	long lCount = 0; //Keep track of how many names there are;
	char* pPos;

	char* szString = &strCityNames.at(0);
	char* pEnd = szString + sizeString -1 ;
	pPos = szString;
	while (pPos != pEnd && pPos) //While pPos is not equal to PEnd and pPos exist, should never reach pass the pEnd, but just to be sure!
	{
		if (*pPos == ',')
		{
			if (pPos == pEnd)//in case the user used a comma at the end of the line
				break;

			pCity = m_vecCities.at(lCount);
			lReturn = pCity->SetCityName(szString, pPos - szString);
			if (lReturn != ERR_NOERROR)
				return SET_CITY_NAME; // Error Message;

			lCount++;
			if (lCount == lCitiesAmount)
				break;


			pPos++; //Set it 1 past the ,
			szString = pPos; //Set it to the 
		}
		pPos++;
	}

	if (pPos == pEnd) //get the last item
	{
		pCity = m_vecCities.at(lCount);
		if(*pPos == ',')
			lReturn = pCity->SetCityName(szString, pPos - szString);
		else
			lReturn = pCity->SetCityName(szString, pPos - szString+1);
		if (lReturn != ERR_NOERROR)
			return SET_CITY_NAME; //Too Error Message;

		lCount++;
	}

	//Check if all cities are filled if not add default name
	std::string strCityNameDefault;
	if (lCount < lCitiesAmount)
	{
		while (lCount < lCitiesAmount)
		{
			pCity = m_vecCities.at(lCount);
			strCityNameDefault = "CityName" + std::to_string(lCount+1);
			lReturn = pCity->SetCityName(&strCityNameDefault[0], strCityNameDefault.length());
			lCount++;
		}
	}

	//Now lets determine the sizes of the cities and the positions!
	//check how we have to divide the space of the screen based on our number of cities.
	//Since the screen is a perfect square we can determine how much cities have to fit verticall and horizontally
	float fSpaceDivide = ceil(sqrt((float)lCitiesAmount)); //we have to round it up not down. 8 cities will result in a square of 3 x 3 cities, with one space being unoccupied but that's fine

	float fScreenSize = 0.0;

	lReturn = glb.propertyBag.GetCountrySize(&fScreenSize);
	if (lReturn != ERR_NOERROR)
		return GET_COUNTRY_SIZE; //ErrorHandling

	float fCitySize = round(fScreenSize / fSpaceDivide) - 10; //We round it down for quick calculation AND give a gap of 10 so that the cities are not touch one another.
		
	float fStartPosX = 0.0;
	float fStartPosY = 0.0;

	//Fill the positions and sizes in the cities;
	lCount = 0;
	for (float idxPosY = 0; idxPosY < fSpaceDivide; idxPosY++)
	{
		for (float idxPosX = 0; idxPosX < fSpaceDivide; idxPosX++)
		{

			pCity = m_vecCities.at(lCount);
			lReturn = pCity->SetCityPositions(fCitySize, ((fCitySize * idxPosX) + (10 * idxPosX)), ((fCitySize * idxPosY) + (10 * idxPosY)));
			if (lReturn != ERR_NOERROR)
				return lReturn;

			lCount++;
			if (lCount == lCitiesAmount)
				break;
		}
	}


	return lReturn;

}

long CCountry::SetLivingStatuses()
{
	long lReturn = ERR_NOERROR;

	long lDefaultSick = 0;
	lReturn = glb.propertyBag.GetStartSick(&lDefaultSick);
	if (lReturn != ERR_NOERROR)
		return GET_START_SICK; // Error Handling

	if (lDefaultSick == 0)
		lDefaultSick = 1; //As a default

	//Have to be spread equally;
	size_t SizeSpreadRate = (size_t) std::round(m_vecHumans.size() / (size_t)lDefaultSick); //we round it down to make sure we don't try to access non existing vector objects.

	size_t sizeSpreadPos = 0;
	CHuman* pHuman = nullptr;
	for (size_t idx = 0; idx < (size_t)lDefaultSick; idx++)
	{
		pHuman = m_vecHumans.at(sizeSpreadPos);
		pHuman->SetLivingStatus(LIVINGSTATUS::SICK);
		sizeSpreadPos = sizeSpreadPos + SizeSpreadRate;

	}

	return lReturn;
}


//Thread start

long CCountry::GetHealthyPopulation() { boost::lock_guard<boost::mutex> lock{ m_mutex }; return m_sizeHealthy; }

long CCountry::GetSickPopulation() { boost::lock_guard<boost::mutex> lock{ m_mutex }; return m_sizeSick; }
long CCountry::GetDeathPopulation() { boost::lock_guard<boost::mutex> lock{ m_mutex }; return m_sizeDeath; }


//Thread problem!
void CCountry::AddHealthy(bool bAddHealthy /*= true*/)
{
	m_mutex.lock();
	if (bAddHealthy == true)
		m_sizeHealthy++;
	else
		m_sizeHealthy--;
	m_mutex.unlock();
}

void CCountry::AddDeath(bool bAddDeath /*= true*/)
{
	m_mutex.lock();
	if (bAddDeath == true)
		m_sizeDeath++;
	else
		m_sizeDeath--;
	m_mutex.unlock();
}

void CCountry::AddSick(bool bAddSick /*= true*/)
{
	m_mutex.lock();
	if (bAddSick == true)
		m_sizeSick++;
	else
		m_sizeSick--;
	m_mutex.unlock();
}

///Getters and setters
long CCountry::PushBack(CCity * pCity)
{
	if (pCity == nullptr)
		return SET_CITY_INTO_VECTOR;

	m_vecCities.push_back(pCity);

	return ERR_NOERROR;

}

size_t CCountry::GetCityVectorSize()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_vecCities.size();
}

CCity * CCountry::GetCityObject(size_t idx)
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_vecCities.at(idx);
}

size_t CCountry::GetHumanVectorSize()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_vecHumans.size();
}

CHuman* CCountry::GetHumanObject(size_t idx)
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_vecHumans.at(idx);
}

std::vector<CHuman*>* CCountry::GetHumanVectorPointer()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return &m_vecHumans;
}

std::vector<CCity*>* CCountry::GetCityVectorPointer()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return &m_vecCities;
}