#pragma once
#include "Helper.h"
#include "Humans.h"

class CCountry;

class CCity
{
protected:
     std::vector<CHuman*> m_vecHumanPointers;
    float m_px = 0;
    float m_py = 0;
    unsigned long m_lPopulation = 0;
    unsigned long m_lHealthy = 0;
    unsigned long m_lDeath = 0;
    unsigned long m_lSick = 0;
    char* m_pCityName = nullptr;
    float m_fCitySize = 10.0; //default size
    size_t m_sizeCitizens = 0;
public:
	CCity()
    {

    };
	~CCity()
    {
        m_vecHumanPointers.clear(); //we don't have to delete the objects, these are just pointers. The objects are within CCountry!
    };

	///Getters and Setters

    long SetCityName(char * pCityName, size_t sizeToCopy)
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

    long SetCityPositions(float fCitySize, float px, float py)
    {
        if (fCitySize <= 0.0)
            return SET_CITY_SIZE; // ErrorHandling

        if(px < 0.0)
            return SET_CITY_SIZE; // ErrorHandling

        if(py < 0.0)
            return SET_CITY_SIZE; // ErrorHandling

        m_fCitySize = fCitySize;
        m_px = px;
        m_py = py;

        return ERR_NOERROR;
    }


    float GetPosY() { return m_py; }
    float GetPosX() { return m_px; }

    char* GetCityName() { return m_pCityName; }

    long SetCitySize(float fCitySize)
    {
        if (fCitySize <= 0.0)
            return SET_CITY_SIZE; // Error Handling

        m_fCitySize = fCitySize;
        return ERR_NOERROR;
    }

    float GetCitySize() { return m_fCitySize; }

    void SetCityX(float fPX)
    {
        m_px = fPX;
    }

    float GetCityX()
    {
        return m_px;
    }

    float SetCityY(float fPY)
    {
        m_py = fPY;
    }

    float GetCityY()
    {
        return m_py;
    }

    long GetHealthyPopulation() { return m_lHealthy; }
    long GetSickPopulation() { return m_lSick; }
    long GetDeathPopulation() { return m_lDeath; }

    void AddHealthy(bool bAddHealthy = true)
    {
        if (bAddHealthy == true)
            m_lHealthy++;
        else
            m_lHealthy--;

        AddHealthy(bAddHealthy);
    }

    void AddDeath(bool bAddDeath = true)
    {
        if (bAddDeath == true)
            m_lDeath++;
        else
            m_lDeath--;

        //m_pCountry->AddDeath(bAddDeath);
    }

    void AddSick(bool bAddSick = true)
    {
        if (bAddSick == true)
            m_lSick++;
        else
            m_lSick--;
        //m_pCountry->AddSick(bAddSick);
    }

    void PushBack(CHuman* pHuman)
    {
        m_vecHumanPointers.push_back(pHuman);
    }

    size_t GetHumanVectorSize() { return m_vecHumanPointers.size(); }

    CHuman * GetHuman(size_t idx)
    {
        return m_vecHumanPointers.at(idx);
    }

};