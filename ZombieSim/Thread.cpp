#include "Threads.h"

inline bool DoHumansOverlap(float x1, float y1, float r1, float x2, float y2, float r2)
{
	
	return fabs((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2)) < ((r1 + r2)* (r1 + r2));
	
}

void ThreadWorkDivideIsRunning(CZombieSimThread* pThreadInfo) //Multithreaded item
{
	size_t sizeStart = pThreadInfo->GetStartPos();
	size_t sizeEndPos = pThreadInfo->GetEndPos();

	boost::mutex m_mutex;

	std::vector<CHuman*>* pvecHumans = country.GetHumanVectorPointer();
	
	CHuman* pHuman = nullptr;

	

	for (size_t idxStartPosHealth = sizeStart; idxStartPosHealth < sizeEndPos+1; idxStartPosHealth++)
		{
			m_mutex.lock();
			pHuman = pvecHumans->at(idxStartPosHealth);
			m_mutex.unlock();
			pHuman->OnThreadCall(pvecHumans);
			m_mutex.unlock();
		}

		//If running time is longer than the simulation simulation stop the thread
		
	

	pThreadInfo->ThreadIsFinished();

}
void ThreadWorkDivideIsRunningSingle(size_t startpos, size_t endpos) //Single Thread aka no new threads made
{
	size_t sizeStart = startpos;
	size_t sizeEndPos = endpos;

	boost::mutex m_mutex;

	std::vector<CHuman*>* pvecHumans = country.GetHumanVectorPointer();
	
	CHuman* pHuman = nullptr;

	m_mutex.lock();
	static boost::posix_time::ptime starttime = glb.m_startTime;
	m_mutex.unlock();
	static long lDurationSimulation = glb.propertyBag.GetSimulationDuration();
	static uint64_t u64Cycletime = (uint64_t) glb.propertyBag.GetCycleTime(); 
	


	static boost::posix_time::ptime currenttime = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration diff = currenttime - starttime;

	MOVESTATUS eCurrentPosition = HOME;
	MOVESTATUS eOldPosition = HOME;


	bool bThreadIsRunning = true;


	uint64_t u64OddOrEven = round((diff.total_seconds()) / u64Cycletime);

	while (bThreadIsRunning == true)
	{
		//Check first we have to update the moving state for our objects.

		u64OddOrEven = round((diff.total_seconds()) / u64Cycletime);
		if (u64OddOrEven % 2 == 0)
		{
			//It's an even number
			eCurrentPosition = HOME;
		}
		else
		{
			//it's an odd number
			eCurrentPosition = WORK;
		}

		if (eCurrentPosition != eOldPosition)
		{
			for (size_t idxStartPos = sizeStart; idxStartPos < sizeEndPos+1; idxStartPos++)
			{
				m_mutex.lock();
				pHuman = pvecHumans->at(idxStartPos);
				pHuman->SetMovingStatus(eCurrentPosition);
				m_mutex.unlock();
			}
			eOldPosition = eCurrentPosition;
		}


		//Now that we are sure the moving status is handled; We are going to call the CallOnThreadFunction in the human objects;

		for (size_t idxStartPosHealth = sizeStart; idxStartPosHealth < sizeEndPos+1; idxStartPosHealth++)
			{
				pHuman = pvecHumans->at(idxStartPosHealth);
				pHuman->OnThreadCall(pvecHumans);
			}

		//If running time is longer than the simulation simulation stop the thread
		currenttime = boost::posix_time::second_clock::local_time();
		diff = currenttime - starttime;
		if (diff.total_seconds() > lDurationSimulation)
		{
			bThreadIsRunning = false;
		}
	}

	//We are done.

}

void ThreadProcessSickness(CZombieSimThreadSickness* pThreadInfo) //multi thread
{
	boost::mutex m_mutex;
	//Loop Over amount human objects within borders
	//Check first if object is HEALTHY, if not ignore
	//If Healthy check if there is overlap
	//If overlap -> Set that object as GetSick.
	static std::random_device RNG;  // Will be used to obtain a seed for the random number engine
	static std::mt19937 Seed(RNG()); // Standard mersenne_twister_engine seeded with RNG()
	static std::uniform_real_distribution<> disInfectionRate(0, 100);
	static float fInfectionRate = (float)glb.propertyBag.GetInfectionRate();

	CHuman* pHumanSource = pThreadInfo->GetHuman();

	float fXCurrentPos = 0.0;
	float fYCurrentPos = 0.0;

	pHumanSource->GetCurrentPos(&fXCurrentPos, &fYCurrentPos);
	
	static float fRCurrentPos = 2.0; //only once since this is always the same. In the future make it dynamic, but not now.

	float fXTargetPos = 0.0;
	float fYTargetPos = 0.0;
	static float fRTargetPos = 1.0; //only once since this is always the same. In the future make it dynamic, but not now.


	CHuman* pHuman = nullptr;
	std::vector<CHuman*> * pvecHumans = country.GetHumanVectorPointer();

	LIVINGSTATUS eLivingStatus = HEALTHY;

	float fCurrentInfectionRate = 0.0;

	bool bOverlap = false;

	size_t startPos = pThreadInfo->GetStartPos();
	size_t endPos = pThreadInfo->GetEndPos();

	for (size_t sizeHumanVectorStart = startPos; sizeHumanVectorStart < endPos - 1; sizeHumanVectorStart++)
	{
		m_mutex.lock();
		pHuman = pvecHumans->at(sizeHumanVectorStart);
		m_mutex.unlock();
		eLivingStatus = pHuman->GetLivingStatus(); //locks itself
		
		if (eLivingStatus == HEALTHY)
		{
			//Check if overlap
			pHuman->GetCurrentPos(&fXTargetPos, &fYTargetPos);
			bOverlap = DoHumansOverlap(fXCurrentPos, fYCurrentPos, fRCurrentPos, fXTargetPos, fYTargetPos, fRTargetPos);

			//If Overlap is true we are going to determine if the human get infected or not
			if (bOverlap == true)
			{
				fCurrentInfectionRate = disInfectionRate(Seed);
				if (fCurrentInfectionRate <= fInfectionRate) //if the pulled rate (true distribution) is lower than the determined infection rate. 
					pHuman->SetLivingStatus(GETSICK); //The person gets infected

				//Else the person doesn't get Sick
			}
		}
		


	}
}

void ThreadProcessSicknessSingle(size_t startPos, size_t endPos, CHuman * pHumanparameter) //single thread
{
	boost::mutex m_mutex;
	//Loop Over amount human objects within borders
	//Check first if object is HEALTHY, if not ignore
	//If Healthy check if there is overlap
	//If overlap -> Set that object as GetSick.
	static std::random_device RNG;  // Will be used to obtain a seed for the random number engine
	static std::mt19937 Seed(RNG()); // Standard mersenne_twister_engine seeded with RNG()
	static std::uniform_real_distribution<> disInfectionRate(0, 100);
	static float fInfectionRate = (float)glb.propertyBag.GetInfectionRate();

	CHuman* pHumanSource = pHumanparameter;

	float fXCurrentPos = 0.0;
	float fYCurrentPos = 0.0;

	pHumanSource->GetCurrentPos(&fXCurrentPos, &fYCurrentPos);
	
	static float fRCurrentPos = 2.0; //only once since this is always the same. In the future make it dynamic, but not now.

	float fXTargetPos = 0.0;
	float fYTargetPos = 0.0;
	static float fRTargetPos = 1.0; //only once since this is always the same. In the future make it dynamic, but not now.


	CHuman* pHuman = nullptr;
	std::vector<CHuman*> * pvecHumans = country.GetHumanVectorPointer();

	LIVINGSTATUS eLivingStatus = HEALTHY;

	float fCurrentInfectionRate = 0.0;

	bool bOverlap = false;

	for (size_t sizeHumanVectorStart = startPos; sizeHumanVectorStart < endPos - 1; sizeHumanVectorStart++)
	{
		m_mutex.lock();
		pHuman = pvecHumans->at(sizeHumanVectorStart);
		m_mutex.unlock();
		eLivingStatus = pHuman->GetLivingStatus(); //locks itself
		
		if (eLivingStatus == HEALTHY)
		{
			//Check if overlap
			pHuman->GetCurrentPos(&fXTargetPos, &fYTargetPos);
			bOverlap = DoHumansOverlap(fXCurrentPos, fYCurrentPos, fRCurrentPos, fXTargetPos, fYTargetPos, fRTargetPos);

			//If Overlap is true we are going to determine if the human get infected or not
			if (bOverlap == true)
			{
				fCurrentInfectionRate = disInfectionRate(Seed);
				if (fCurrentInfectionRate <= fInfectionRate) //if the pulled rate (true distribution) is lower than the determined infection rate. 
					pHuman->SetLivingStatus(GETSICK); //The person gets infected

				//Else the person doesn't get Sick
			}
		}
		


	}
}

void ThreadForMovingHumans(CZombieSimThreadMoving* pThreadInfo)
{


	MOVESTATUS eCurrentPosition = HOME;
	MOVESTATUS eOldPosition = HOME;
	MOVESTATUS eMovingStatus = HOME;
	boost::mutex m_mutex;
	m_mutex.lock();
	static boost::posix_time::ptime starttime = glb.m_startTime;

	static long lDurationSimulation = glb.propertyBag.GetSimulationDuration();
	uint64_t u64Cycletime = (uint64_t)glb.propertyBag.GetCycleTime();
	m_mutex.unlock();



	static boost::posix_time::ptime currenttime = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration diff = currenttime - starttime;

	std::vector<CHuman*> *pvecHumans = country.GetHumanVectorPointer();
	size_t sizeHumanVector = pvecHumans->size();

	bool bThreadIsRunning = true;



	uint64_t u64OddOrEven = round((diff.total_seconds()) / u64Cycletime);
	CHuman* pHuman = nullptr;
	while (bThreadIsRunning == true)
	{
		currenttime = boost::posix_time::second_clock::local_time();
		diff = currenttime - starttime;
		if ((diff.total_seconds()) > lDurationSimulation)
			bThreadIsRunning = false;



		//Check first we have to update the moving state for our objects.

		u64OddOrEven = round((diff.total_seconds()) / u64Cycletime);
		if (u64OddOrEven % 2 == 0)
		{
			//It's an even number
			eCurrentPosition = HOME;
		}
		else
		{
			//it's an odd number
			eCurrentPosition = WORK;
		}

		if (eCurrentPosition != eOldPosition)
		{
			for (size_t idxStartPos = 0; idxStartPos < sizeHumanVector; idxStartPos++)
			{
				m_mutex.lock();
				pHuman = pvecHumans->at(idxStartPos);
				m_mutex.unlock();
				pHuman->SetMovingStatus(eCurrentPosition); //Has it's own mutex
				pHuman->SetMoveStartTime();

			}
			eOldPosition = eCurrentPosition;
		}

		for (size_t idxMove = 0; idxMove < sizeHumanVector; idxMove++)
		{
			m_mutex.lock();
			pHuman = pvecHumans->at(idxMove);
			m_mutex.unlock();
			eMovingStatus = pHuman->GetMoveStatus();

			switch (eMovingStatus)
			{
			case STATIC:
				//Do nothing we are being static aka stay at the same location
				break;
			case WORK:
				pHuman->MoveToWork();
				break;
			case HOME:
				pHuman->MoveToHome();
				break;
			default:
				printf("Could not get the right movingstatus %s", EOL);
				break;


			}

		}
		
		boost::this_thread::sleep_for(boost::chrono::milliseconds(250)); //It goes so fast that the frames are already shifting before
	}
	pThreadInfo->ThreadIsFinished();
}