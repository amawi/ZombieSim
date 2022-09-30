#pragma once
#include "Helper.h"


enum LIVINGSTATUS
{
	HEALTHY = 1,
	GETHEALTHY,
	GETSICK,
	SICK,
	DEAD,
};

enum MOVESTATUS
{
	STATIC = 1,
	WORK,
	HOME,
};


class CCity; //We use a pointer to have a parent child (City vs Humans) relationship. Pointers make it possible to have a semi recursive declaration, but you can not use class instances.

class CHuman
{
protected: //You can either set the member variables within the constructor or within the declaration of the of the object.
	//I prefer the latter since you "can" forget to set it in the constructor especially when you have overloaded constructor functions.
	//Now you 100% have a default value set for all your member variables.
	LIVINGSTATUS m_eLivingStatus = HEALTHY; //To know if the human is sick, death or healthy. Or gets sick, or gets healthy.
	MOVESTATUS m_eMoveStatus = STATIC; //Wherever the object is moving towards to. Home, Work or Static (fix on it's spot)
	CCity* m_pCity = nullptr; //The parent relationship. We use it to say add sickness, death, healthy towards the number.
	boost::posix_time::ptime m_SickStarttime;
	boost::posix_time::ptime m_MoveStarttime;
	float m_fxHome = 0; //Work address X position
	float m_fyHome = 0; //Work address Y position
	float m_fxWork = 0; //Home address X Position
	float m_fyWork = 0; //Home Address Y Position
	float m_fxCurrentPos = 0; //Current X Position of Human Object;
	float m_fyCurrentPos = 0; //Current Y Position of Human Object
	float m_fxDistanceWork = 0; //Velocity of the object Direction X (if it moves);
	float m_fxDistanceHome = 0; //Velocity of the object Direction Y (if it moves)
	float m_fyDistanceWork = 0; //Acceleration of the object in the Direction of X. If it speeds up or not
	float m_fyDistanceHome = 0.0; //Acceleration of the object in the Direction of Y. If it speeds up or not
	boost::mutex m_mutex;
	//
public:
	CHuman()
	{

	};
	~CHuman()
	{
		//We don't have to delete anything since the object of the City pointer (aka the object where the pointer is pointing to) get's cleaned up somewhere else

	};

	MOVESTATUS GetMoveStatus() {return m_eMoveStatus;}
	float GetCurrentX();

	float GetCurrentY();

	LIVINGSTATUS GetLivingStatus();

	void SetStartTimeSickness();

	long InitPositions(float fxHome, float fyHome, float fxWork, float fyWork, float fxCurrent, float fyCurrent);

	void MoveToWork();

	void MoveToHome();

	void OnThreadCall(std::vector<CHuman*>* pvecHumans);

	void SetLivingStatus(LIVINGSTATUS eLivingStatus);

	void ProcessSickness(std::vector<CHuman*>* pvecHumans);

	long GetWork(float* pfWorkX, float* pfWorkY);

	long SetWork(float fWorkX, float fWorkY);

	long GetHome(float* fpHomeX, float* fpHomeY);

	long SetHome(float fHomeX, float fHomeY);

	long SetCurrentPos(float fCurrentX, float fCurrentY);

	long GetCurrentPos(float* fpCurrentX, float* fpCurrentY);

	long SetCityPointer(CCity* pCity);

	void SetMovingStatus(MOVESTATUS eMoveStatus);

	void GetDistanceHome(float* pfxDistanceHome, float* pfyDistanceHome);
	float GetDistanceHomeX() { return m_fxDistanceHome; }
	float GetDistanceHomeY() { return m_fyDistanceHome; }
	void GetDistanceWork(float* pfxDistanceWork, float* pfyDistanceWork);
	float GetDistanceWorkX() { return m_fxDistanceWork; }
	float GetDistanceWorkY() { return m_fyDistanceWork; }

	CCity* GetCity();
	boost::posix_time::ptime GetSickStartTime();
	void SetMoveStartTime();
	boost::posix_time::ptime GetMoveStartTime();

	void Kill();

};