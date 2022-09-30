#pragma once

#include "stdafx.h"
#include "Management.h"
#include "BB_PropertyBag.h"


//class CPropertyBag;
class CZombieSimPropertyBag : public CPropertyBag
{
protected:
	char m_szZombieSimPropertyBagErrMessage[255];
	boost::mutex m_mutex;
	//long m_lLastError = 0;
public:
	CZombieSimPropertyBag() {};
	~CZombieSimPropertyBag() {};

	const char* GetLastZombieSimPropertyBagErrorMessage() { return m_szZombieSimPropertyBagErrMessage; }

	long SetCountrySize(float fCountrySize) //Considered as meters //keep in mind. One human is 1 meter circle
	{
		
		return SetValueFloat("Country", "Size", fCountrySize);
		
	}

	long GetCountrySize(float * pfCountrySize)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueFloat("Country", "Size", pfCountrySize);
	}

	long SetPopulationSize(long lPopulation)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return SetValueLong("Country", "Population", lPopulation);
	}



	long GetPopulationSize(size_t * psizePopulation)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueSizeT("Country", "Population", psizePopulation);
	}

	long SetStartSick(long lStartSick)
	{
		
		return SetValueLong("Country", "StartSick", lStartSick);
	}

	long GetStartSick(long* plStartSick)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Country", "StartSick", plStartSick);
	}

	long SetCycleTime(long lCycleTime)
	{
		return SetValueLong("Country", "CycleDuration", lCycleTime);
	}

	long GetCycleTime(long* plCycleTime)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Country", "CycleDuration", plCycleTime);
	}

	long GetCycleTime() 
	{
		long lReturn = ERR_NOERROR;
		long lCycleDuration = 0;
		lReturn = GetValueLong("Country", "CycleDuration", &lCycleDuration);
		if (lReturn != ERR_NOERROR)
			lCycleDuration = 2;

		if (lCycleDuration <= 0)
			lCycleDuration = 2;

		return lCycleDuration;
	}

	long SetRateOfDifferentWorkAddress(long lRateOfDifferentCity)
	{
		return SetValueLong("Country", "RateOtherCities", lRateOfDifferentCity);
	}

	long GetRateOfDifferentWorkAddress(long* plRateOfDifferentCity)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Country", "RateOtherCities", plRateOfDifferentCity);
	}

	long SetFamilyUnit(long lFamilyUnit)
	{
		return SetValueLong("Country", "FamilyUnit", lFamilyUnit);
	}

	long GetFamilyUnit(long* plFamilyUnit)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Country", "FamilyUnit", plFamilyUnit);
	}

	long SetMovingSpeed(float fMovingSpeed)
	{
		return SetValueFloat("Country", "MovingSpeed", fMovingSpeed);
	}

	long GetMovingSpeed(float* pfMovingSpeed)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueFloat("Country", "MovingSpeed", pfMovingSpeed);
	}

	float GetMovingSpeed()
	{
		long lReturn = ERR_NOERROR;
		float fMovingSpeed = 0.0;

		boost::lock_guard<boost::mutex> lock{ m_mutex };
		lReturn = GetValueFloat("Country", "MovingSpeed", &fMovingSpeed);
		if (lReturn != ERR_NOERROR)
			return 5.0;
		else
			return fMovingSpeed;


	}

	long SetCitiesAmount(long lCitiesAmount)
	{
		return SetValueLong("City", "CitiesAmount", lCitiesAmount);
	}

	long GetCitiesAmount(long* plCitiesAmount)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("City", "CitiesAmount", plCitiesAmount);
	}

	long SetCitiesName(const char* szCitiesName)
	{
		return SetValueString("City", "Names", szCitiesName);
	
	}

	long GetCitiesName(std::string * pstrCitiesName)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueString("City", "Names", pstrCitiesName);
	}

	long SetCitiesSizes(const char* szCitiesSizes)
	{
		return SetValueString("City", "Sizes", szCitiesSizes);

	}

	long GetCitiesSizes(std::string* pstrCitiesSizes)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueString("City", "Sizes", pstrCitiesSizes);
	}


	long SetDeathRate(long lDeathRate)
	{
		return SetValueLong("Human", "DeathRate", lDeathRate);
	}

	long GetDeathRate(long* plDeathRate)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Human", "DeathRate", plDeathRate);
	}
	long GetDeathRate()
	{
		long lReturn = ERR_NOERROR;
		long lDeathRate = 0;
		m_mutex.lock();
		lReturn = GetValueLong("Human", "DeathRate", &lDeathRate);
		m_mutex.unlock();
		if (lReturn != ERR_NOERROR)
			lDeathRate = 1; //1% default
		if (lDeathRate <= 0)
			lDeathRate = 1; //1% default

		return lDeathRate;

	}

	long SetInfectionRate(long lInfectionRate)
	{
		return SetValueLong("Human", "InfectionRate", lInfectionRate);
	}

	long GetInfectionRate(long* plInfectionRate)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Human", "InfectionRate", plInfectionRate);
	}
	long GetInfectionRate() 
	{
		long lReturn = ERR_NOERROR;
		long lInfectionRate = 0;
		m_mutex.lock();
		lReturn = GetValueLong("Human", "InfectionRate", &lInfectionRate);
		m_mutex.unlock();
		if (lReturn != ERR_NOERROR)
			lInfectionRate = 5; //5% default
		if (lInfectionRate <= 0)
			lInfectionRate = 5; //5% default

		return lInfectionRate;

	}

	long SetSickDuration(long lSickDuration)
	{
		return SetValueLong("Human", "SickDurationTimesCycle", lSickDuration);
	}

	long GetSickDuration(long* plSickDuration)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Human", "SickDurationTimesCycle", plSickDuration);
	}

	long SetTreadBase(long lThreadBase)
	{
		return SetValueLong("Parallel", "ThreadBase", lThreadBase);
	}

	long GetThreadBase(long* plThreadBase)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Parallel", "ThreadBase", plThreadBase);
	}

	long GetThreadBase(size_t * sizelThreadBase)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueSizeT("Parallel", "ThreadBase", sizelThreadBase);
	}

	long SetThreadOverlapCheck(long lThreadOverlapCheck)
	{
		return SetValueLong("Parallel", "ThreadOverlapCheck", lThreadOverlapCheck);
	}

	long GetThreadOverlapCheck(long* plThreadOverlapCheck)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Parallel", "ThreadOverlapCheck", plThreadOverlapCheck);
	}
	long GetThreadOverlapCheck()
	{
		m_mutex.lock();
		long lThreadOverlapCheck = 0;
		long lReturn =  GetValueLong("Parallel", "ThreadOverlapCheck", &lThreadOverlapCheck);
		m_mutex.unlock();
		if (lReturn != ERR_NOERROR)
			return 1; //Only one thread for checking
		else if (lThreadOverlapCheck <= 0)
			return 1;
		else
			return lThreadOverlapCheck;
	}

	long SetSimulationDuration(long lSimulationDuration)
	{
		return SetValueLong("Parallel", "SimulationDuration", lSimulationDuration);
	}

	long GetSimulationDuration(long* plSimulationDuration)
	{
		boost::lock_guard<boost::mutex> lock{ m_mutex };
		return GetValueLong("Parallel", "SimulationDuration", plSimulationDuration);
	}

	long GetSimulationDuration()
	{
		long lSimulationDuration = 0;
		long lReturn = ERR_NOERROR;
		m_mutex.lock();
		lReturn = GetValueLong("Parallel", "SimulationDuration", &lSimulationDuration);
		m_mutex.unlock();

		if (lReturn != ERR_NOERROR)
			return 60; //1 minute simulation
		else if (lSimulationDuration <= 0)
			return 60;
		
		return lSimulationDuration;
		
	}
};

