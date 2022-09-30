#include "Helper.h"
#include "Threads.h"

void ThreadWorkDivideIsRunning(CZombieSimThread* pThreadInfo);
void ThreadWorkDivideIsRunningSingle(size_t startpos, size_t endpos);
void ThreadForMovingHumans(CZombieSimThreadMoving* pThreadInfo);


long CreateThreadsAndRun(std::vector<CZombieSimThread*>*pvecThreads, std::vector<CZombieSimThreadMoving*> *pvecThreadsMove) {
    long lReturn = ERR_NOERROR;
    //Get the amount of threads

    size_t sizeThreads = 0;

    lReturn = glb.propertyBag.GetThreadBase(&sizeThreads);
    if (lReturn != ERR_NOERROR)
        return lReturn;

    //We now now how many threads need to be made to divide the workload.

    //We need to know the size of the human vector.
    size_t sizeHumanVector = country.GetHumanVectorSize();
	CZombieSimThread* pThread = nullptr;
    CZombieSimThreadMoving* pThreadMoving = nullptr;

    //Now we have to split the workload. where the last one gets more to do if it's a fraction.
    //E.g we have 3 threads over 100 people. Thread one get 33, thread 2 get 33 thread 3 get 34.
    //aka 0-32, 33-65, 66-99 //array

	pThreadMoving = new CZombieSimThreadMoving(country.GetHumanVectorPointer());
	pThreadMoving->SetThreadProc(new boost::thread(ThreadForMovingHumans, pThreadMoving));
		
	ASSERT(pThreadMoving->GetThreadProc());
	//Give is time to initialize
	boost::this_thread::sleep_for(boost::chrono::milliseconds(250));

	//And add the info to the vector of running threads

    pvecThreadsMove->push_back(pThreadMoving);

    if (sizeThreads < 2)
    {
        //Only one thread do not need to create a new one.
        //ThreadWorkDivideIsRunningSingle(0, sizeHumanVector-1); //for loop. array starts from 0 not from 1.

        //Start Thread

        pThread = new CZombieSimThread(0, sizeHumanVector-1);
        pThread->SetThreadProc(new boost::thread(ThreadWorkDivideIsRunning, pThread));
		ASSERT(pThread->GetThreadProc());
		
		pvecThreads->push_back(pThread);

        return lReturn;
    }

    size_t sizeDivideWorkload = round(sizeHumanVector / (float)sizeThreads);

    size_t sizeStartPos = 0;
    size_t sizeEndPos = 0;
 

    

    for (size_t idxThreadPos = 0; idxThreadPos < sizeThreads; idxThreadPos++)
    {
        sizeStartPos = idxThreadPos * sizeDivideWorkload;
        sizeEndPos = sizeStartPos + sizeDivideWorkload - 1;
        //Start Thread

        if (idxThreadPos == (sizeThreads - 1))
        {
		    //Last thread has to be set on the last pos
            sizeEndPos = sizeHumanVector - 1;
        }


        pThread = new CZombieSimThread(sizeStartPos, sizeEndPos);
        pThread->SetThreadProc(new boost::thread(ThreadWorkDivideIsRunning, pThread));
		ASSERT(pThread->GetThreadProc());

		//Give is time to initialize
		boost::this_thread::sleep_for(boost::chrono::milliseconds(250));


		//And add the info to the vector of running threads
        pvecThreads->push_back(pThread);
    }


    


#ifdef _DEBUG



	printf_s("Now we wait till all processes are done %s", EOL);
#endif // _DEBUG
    
    return lReturn;
}

