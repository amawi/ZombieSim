#pragma once
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Country.h"
#include "Helper.h"

extern CGlobal glb;
class CCity;

struct CIRCLEFIGURE
{
	float px, py; //position
	float vx, vy; //velocity
	float ax, ay; //accelaration
	float fRadius; //Radius of the cirlce

	long ID_Circle; //ID of the circle because we need to find overlapping circles.
};



class CPhysicsEngine : public olc::PixelGameEngine
{
private:
	float m_fScreenSize = 0.0;
	std::vector<CHuman*>* m_pvecHumans;
	std::vector<CCity*>* m_pvecCities;
public:

	CPhysicsEngine() {};
	~CPhysicsEngine() {};

	long SetHumanVector(std::vector<CHuman*>* pvecHumans)
	{
		if (pvecHumans->size() <= 0)
			return SET_HUMAN_VECTOR; // Error Message

		m_pvecHumans = pvecHumans;
		return ERR_NOERROR;
	}

	long SetCityVector(std::vector<CCity*> * pvecCities)
	{
		if (pvecCities->size() <= 0)
			return SET_CITY_VECTOR; // Error Message

		m_pvecCities = pvecCities;
		return ERR_NOERROR;

	}

	bool OnUserCreate()
	{
		sAppName = "ZombieSim";

		float fScreenSize = 0.0;
		
		long lReturn = glb.propertyBag.GetCountrySize(&fScreenSize);
		if (lReturn != ERR_NOERROR)
			return false;

		m_fScreenSize = fScreenSize;
		Clear(olc::WHITE);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		glb.fElapsedTimeProperty = fElapsedTime;
		static long lDuration = glb.propertyBag.GetSimulationDuration(); //Set it only one time.
		static boost::posix_time::ptime starttime = glb.m_startTime;

		//Check First if we need to stop.
		boost::posix_time::ptime currenttime = boost::posix_time::second_clock::local_time();

		boost::posix_time::time_duration diff = currenttime - starttime;
		if (diff.total_seconds() > lDuration)
		{
			return false;
		}


		//Draw the country
		Clear(olc::BLACK);


		//Draw the Cities;
		size_t sizeCities = m_pvecCities->size();

		float fxCity = 0;
		float fyCity = 0;
		float fySize = 0;

		class CCity* pCity = nullptr;

		for (size_t idxcities = 0; idxcities < sizeCities; idxcities++)
		{
			pCity = m_pvecCities->at(idxcities);
			fxCity = pCity->GetCityX();
			fyCity = pCity->GetCityY();
			fySize = pCity->GetCitySize();

			//Draw the cities
			FillRect(fxCity, fyCity, fySize, fySize, olc::DARK_YELLOW);
			FillRect(fxCity, fyCity + (fySize / 2.0), fySize, (fySize / 2.0), olc::DARK_CYAN);
		}


		size_t sizeHumans = m_pvecHumans->size();
		
		CHuman* pHuman = nullptr;
		LIVINGSTATUS eLivingStatus;
		float CurrentXPos = 0.0;
		float CurrentYPos = 0.0;

		for (size_t idxHumans = 0; idxHumans < sizeHumans; idxHumans++)
		{
			pHuman = m_pvecHumans->at(idxHumans);
			
			eLivingStatus = pHuman->GetLivingStatus();
			pHuman->GetCurrentPos(&CurrentXPos, &CurrentYPos);

			//Draw Humans based on LivingStatus
			switch (eLivingStatus)
			{
			case HEALTHY:
			case GETSICK:
				FillCircle(CurrentXPos, CurrentYPos, 1, olc::GREEN);
				break;
			case SICK:
			case GETHEALTHY:
				FillCircle(CurrentXPos, CurrentYPos, 2, olc::RED);
				break;
			case DEAD:
				FillCircle(CurrentXPos, CurrentYPos, 1, olc::GREY);
				break;
			default:
				printf("Could not get the right living status to draw the human object %s", EOL);
				break;
			}

		}

		return true;
	}

};


extern CPhysicsEngine visualField;
