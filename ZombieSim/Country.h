#pragma once
#include "Helper.h"
#include "Humans.h"

class CCountry;


class CCity
{
protected:
    std::vector<CHuman*> m_vecHumanPointers;
	CCountry* m_pCountry = nullptr;
    float m_px = 0;
    float m_py = 0;
    unsigned long m_lPopulation = 0;
    unsigned long m_lHealthy = 0;
    unsigned long m_lDeath = 0;
    unsigned long m_lSick = 0;
    char* m_pCityName = nullptr;
    float m_fCitySize = 10.0; //default size
    size_t m_sizeCitizens = 0;
    boost::mutex m_mutex;
public:
	CCity()
    {

    };
	~CCity()
    {
        m_vecHumanPointers.clear(); //we don't have to delete the objects, these are just pointers. The objects are within CCountry!
    };

	///Getters and Setters

    long SetCountry(CCountry* pCountry);

    CCountry* GetCountry();
    long GetCountry(CCountry** ppCountry);

    long SetCityName(char* pCityName, size_t sizeToCopy);
    long SetCityPositions(float fCitySize, float px, float py);
    float GetPosY();
    float GetPosX();

    char* GetCityName();

    long SetCitySize(float fCitySize);

    float GetCitySize();

    void SetCityX(float fPX);
    float GetCityX();
    float SetCityY(float fPY);
    float GetCityY();

    long GetHealthyPopulation();
    long GetSickPopulation();
    long GetDeathPopulation();

    void AddHealthy(bool bAddHealthy = true);
    void AddDeath(bool bAddDeath = true);
    void AddSick(bool bAddSick = true);

    void PushBack(CHuman* pHuman);

    size_t GetHumanVectorSize();

    CHuman* GetHuman(size_t idx);

};

class CCountry //ToDo Maaike
{
protected:
	std::vector<CCity*> m_vecCities;
	std::vector<CHuman*> m_vecHumans; //this is the actual container, the cities only have pointers towards the objects.
	size_t m_sizePopulation = 0;
	size_t m_sizeHealthy = 0;
	size_t m_sizeDeath = 0;
	size_t m_sizeSick = 0;
    boost::mutex m_mutex;
public:
	CCountry() 
	{

	};
	~CCountry() 
	{
		ClearVector<CCity>(&m_vecCities);
		ClearVector<CHuman>(&m_vecHumans);

		m_vecCities.clear();
		m_vecHumans.clear();

	};

	long SetPopulation();

	long SetCities();
    long SetLivingStatuses();

    long GetHealthyPopulation();
    long GetSickPopulation();
    long GetDeathPopulation();

    void AddHealthy(bool bAddHealthy = true);

    void AddDeath(bool bAddDeath = true);

    void AddSick(bool bAddSick = true);

	///Getters and setters
    long PushBack(CCity* pCity);

    size_t GetCityVectorSize();

    CCity* GetCityObject(size_t idx);

    size_t GetHumanVectorSize();

    CHuman* GetHumanObject(size_t idx);

    std::vector<CHuman*>* GetHumanVectorPointer();
    std::vector<CCity*>* GetCityVectorPointer();

};


extern CCountry country;