#include "Humans.h"
#include "Threads.h"

long WaitForAllThreads(const std::vector<CZombieSimThreadSickness*>& pvecThreads);
void ThreadProcessSickness(CZombieSimThreadSickness* pThreadInfo);
void ThreadProcessSicknessSingle(size_t startPos, size_t endPos, CHuman* pHumanparameter);


void CHuman::ProcessSickness(std::vector<CHuman*>* pvecHumans)
{
	//First we check if the human actually died or not
	static std::random_device RNG;  // Will be used to obtain a seed for the random number engine
	static std::mt19937 Seed(RNG()); // Standard mersenne_twister_engine seeded with RNG()
	static std::uniform_real_distribution<> disDeathRate(0, 100);
	static float fDeathRate = (float)glb.propertyBag.GetDeathRate() * 0.1;
	float fDeathSet = disDeathRate(Seed);


	if (fDeathSet <= fDeathRate)
	{
		Kill(); //Human died :(
		return;
	}

	
	//Now we have to find the overlapping Healthy objects
	static uint64_t u64CycleSicktime = (uint64_t) glb.propertyBag.GetCycleTime()* 3; 

	m_mutex.lock();
	static size_t sizeThreadOverlapCheck = (size_t) glb.propertyBag.GetThreadOverlapCheck();
	static long SizeHumanVector = pvecHumans->size();
	m_mutex.unlock();

	size_t sizeDivideWorkload = (size_t)round(((float)SizeHumanVector / (float)sizeThreadOverlapCheck));
	
    size_t sizeStartPos = 0;
    size_t sizeEndPos = 0;
    CZombieSimThreadSickness* pThread = nullptr;
    std::vector<CZombieSimThreadSickness *> vecThreads;
   
	
    for (size_t idxThreadPos = 0; idxThreadPos < sizeThreadOverlapCheck; idxThreadPos++)
    {
        sizeStartPos = idxThreadPos * sizeDivideWorkload;
        sizeEndPos = sizeStartPos + sizeDivideWorkload - 1;
		if (idxThreadPos == (sizeThreadOverlapCheck - 1))
		{
			sizeEndPos = SizeHumanVector - 1;
		}
        //Start Thread

        pThread = new CZombieSimThreadSickness(sizeStartPos, sizeEndPos, this);
        pThread->SetThreadProc(new boost::thread(ThreadProcessSickness, pThread));
		ASSERT(pThread->GetThreadProc());

		//Give is time to initialize
		boost::this_thread::sleep_for(boost::chrono::milliseconds(250));

		//And add the info to the vector of running threads
        vecThreads.push_back(pThread);
    }

#ifdef _DEBUG
	
	printf_s("Now we wait till all processes are done %s", EOL);

#endif

	WaitForAllThreads(vecThreads);
    //Now we delete them
    ClearVector<CZombieSimThreadSickness>(&vecThreads);

	//SetHealthyOrNot
	boost::posix_time::ptime SickCurrenttime = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration SickDuration = SickCurrenttime - this->GetSickStartTime();

	if (((uint64_t) SickDuration.total_seconds() )> u64CycleSicktime)
		SetLivingStatus(GETHEALTHY);


}

float CHuman::GetCurrentX()
{

	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_fxCurrentPos;

}

float CHuman::GetCurrentY()
{
	
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_fyCurrentPos;
	
}

LIVINGSTATUS CHuman::GetLivingStatus()
{
	
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_eLivingStatus;
	
	return HEALTHY;
}

long CHuman::InitPositions(float fxHome, float fyHome, float fxWork, float fyWork, float fxCurrent, float fyCurrent)
{
	
	m_fxHome = fxHome;
	m_fyHome = fyHome;
	m_fxWork = fxWork;
	m_fyWork = fyWork;
	m_fxCurrentPos = fxCurrent;
	m_fyCurrentPos = fyCurrent;
	

	m_fxDistanceWork = fxHome - fxWork;
	m_fxDistanceHome = fxWork - fxHome;
	m_fyDistanceWork = fyHome - fyWork;
	m_fyDistanceHome = fyWork - fyHome;


	return ERR_NOERROR;
}

void CHuman::GetDistanceHome(float * pfxDistanceHome, float * pfyDistanceHome)
{
	*pfxDistanceHome = m_fxDistanceHome;
	*pfyDistanceHome = m_fyDistanceHome;
	
}
void CHuman::GetDistanceWork(float* pfxDistanceWork, float* pfyDistanceWork)
{
	*pfxDistanceWork = m_fxDistanceWork;
	*pfyDistanceWork = m_fyDistanceWork;

}

void CHuman::MoveToWork()
{
	
	m_mutex.lock();
	//First check if the person is not dead, otherwise we have to do all this code for nothing;
	if (m_eLivingStatus == DEAD)
	{
		m_mutex.unlock();
		return;
	}

	static float fMovingSpeed = glb.propertyBag.GetCycleTime() * 0.10; //10% of your current cycle time.
	float fElapsedTimeObject = glb.fElapsedTimeProperty;

	float fxDistanceToWork = this->GetDistanceWorkX();
	float fyDistanceToWork = this->GetDistanceWorkY();

	boost::posix_time::ptime CurrentTime = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration TimeDiff = CurrentTime - m_MoveStarttime;
	
	m_fxCurrentPos = m_fxCurrentPos + ((m_fxWork - m_fxCurrentPos) * (fElapsedTimeObject / fMovingSpeed));
	m_fyCurrentPos = m_fyCurrentPos + ((m_fyWork - m_fyCurrentPos) * (fElapsedTimeObject / fMovingSpeed));

	if(((float)TimeDiff.total_seconds()) >= fMovingSpeed)
	{
		m_fxCurrentPos = m_fxWork;
		m_fyCurrentPos = m_fyWork;
		m_eMoveStatus = STATIC;
	}

	m_mutex.unlock();
	
	
}

void CHuman::MoveToHome()
{
	
	m_mutex.lock();
	//First check if the person is not dead, otherwise we have to do all this code for nothing;
	if (m_eLivingStatus == DEAD)
	{
		m_mutex.unlock();
		return;
	}

	static float fMovingSpeed = glb.propertyBag.GetCycleTime() * 0.10; //10% of your current cycle time.
	float fElapsedTimeObject = glb.fElapsedTimeProperty;

	float fxDistanceToHome = this->GetDistanceHomeX();
	float fyDistanceToWork = this->GetDistanceHomeY();

	boost::posix_time::ptime CurrentTime = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration TimeDiff = CurrentTime - m_MoveStarttime;

	m_fxCurrentPos = m_fxCurrentPos + ((m_fxHome - m_fxCurrentPos) * (fElapsedTimeObject / fMovingSpeed));
	m_fyCurrentPos = m_fyCurrentPos + ((m_fyHome - m_fyCurrentPos) * (fElapsedTimeObject / fMovingSpeed));

	if(((float)TimeDiff.total_seconds()) >= fMovingSpeed)
	{
		m_fxCurrentPos = m_fxHome;
		m_fyCurrentPos = m_fyHome;
		m_eMoveStatus = STATIC;
	}

	m_mutex.unlock();
	

}

void CHuman::OnThreadCall(std::vector<CHuman*> * pvecHumans)
{

	//Second Check LivingStatus
	m_mutex.lock();
	LIVINGSTATUS eLivingstatus = m_eLivingStatus;
	m_mutex.unlock();

	switch (eLivingstatus)
	{
	case HEALTHY: //We don't have to do anything.
		break;
	case SICK:
		ProcessSickness(pvecHumans);
		break;
	case GETSICK:
		SetLivingStatus(SICK); //purposely not calling IsSick() function. Since it takes a while before someone get sick
		SetStartTimeSickness();
		break;
	case GETHEALTHY:
		SetLivingStatus(HEALTHY); //A little respite for the program so that not too much done, creates a slight delay. //In the future we plan a immunity timer with this.
		break;
	case DEAD:
		break;
	default:
		printf("Error could not get the right LivingStatus %s", EOL);
		break;
	}

	
	
}

void CHuman::SetLivingStatus(LIVINGSTATUS eLivingStatus)
{
	
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	m_eLivingStatus = eLivingStatus;
	
}
	
long CHuman::GetWork(float* pfWorkX, float* pfWorkY)
{
	
	m_mutex.lock();
	*pfWorkX = m_fxWork;
	*pfWorkY = m_fyWork;
	m_mutex.unlock();
	
	return ERR_NOERROR;
}

long CHuman::SetWork(float fWorkX, float fWorkY)
{
	
	m_mutex.lock();
	m_fxWork = fWorkX;
	m_fyWork = fWorkY;
	m_mutex.unlock();
	return ERR_NOERROR;
	
}

long CHuman::GetHome(float* fpHomeX, float * fpHomeY)
{
	
	m_mutex.lock();
	*fpHomeX = m_fxHome;
	*fpHomeY = m_fyHome;
	m_mutex.unlock();
	
	return ERR_NOERROR;
}

long CHuman::SetHome(float fHomeX, float fHomeY)
{
	
	m_mutex.lock();
	m_fxHome = fHomeX;
	m_fyHome = fHomeY;
	m_mutex.unlock();

	
	return ERR_NOERROR;

}

long CHuman::SetCurrentPos(float fCurrentX, float fCurrentY)
{
	
	m_mutex.lock();
	m_fxCurrentPos = fCurrentX;
	m_fyCurrentPos = fCurrentY;
	m_mutex.unlock();
	
	return ERR_NOERROR;
}

long CHuman::GetCurrentPos(float* fpCurrentX, float* fpCurrentY)
{
	
	m_mutex.lock();
	*fpCurrentX = m_fxCurrentPos;
	*fpCurrentY = m_fyCurrentPos;
	m_mutex.unlock();
	
	return ERR_NOERROR;
}

long CHuman::SetCityPointer(CCity* pCity) //init function
{
	
	if (pCity == nullptr)
		return SET_CITY_POINTER; // Error Handling

	m_pCity = pCity;
	
	return ERR_NOERROR;
}

void CHuman::SetMovingStatus(MOVESTATUS eMoveStatus) { m_eMoveStatus = eMoveStatus; }

void CHuman::SetStartTimeSickness()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	m_SickStarttime = boost::posix_time::second_clock::local_time();
	
}

CCity* CHuman::GetCity()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_pCity;
}

boost::posix_time::ptime CHuman::GetSickStartTime()
{
	
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_SickStarttime;
}

boost::posix_time::ptime CHuman::GetMoveStartTime()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	return m_MoveStarttime;

}

void CHuman::SetMoveStartTime()
{
	boost::lock_guard<boost::mutex> lock{ m_mutex };
	m_MoveStarttime = boost::posix_time::second_clock::local_time();
}


void CHuman::Kill()
{
	//Human is dying now!.
	m_mutex.lock();
	m_eLivingStatus = DEAD;
	m_mutex.unlock();

	//Human is not going to move anymore so we set the home pos and work pos ont he current pos.
	m_mutex.lock();
	m_fxHome = m_fxCurrentPos;
	m_fxWork = m_fxCurrentPos;
	m_fyHome = m_fxCurrentPos;
	m_fyWork = m_fyCurrentPos;
	
	m_mutex.unlock();

}
