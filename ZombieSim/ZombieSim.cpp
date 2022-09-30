#include "Helper.h"
#include "Country.h"
#include "PhysicalEngine.h"
#include "Threads.h"


CGlobal glb;
CCountry country;
CPhysicsEngine visualField;


long InitProgramVariables(int argc, char* argv[], char* envp[]);
long CreatePlayFieldObjects();
long CreateThreadsAndRun(std::vector<CZombieSimThread*>* pvecThreads, std::vector<CZombieSimThreadMoving*>* pvecThreadsMove);
long WaitForAllThreads(const std::vector<CZombieSimThread*>& pvecThreads);
long WaitForAllThreads(const std::vector<CZombieSimThreadMoving*>& pvecThreads);


//void VisualLayerThreadFunction(CThreadVisualLayer* pThreadInfo);

//make sure to use C++17 (-std=c++17) and not C++11 (standard use)!.


void StartThread()
{
    //visualField.FillRect(0, 0, glb.propertyBag.GetCountrySize(), glb.propertyBag.GetCountrySize(), olc::WHITE);
    visualField.Start();
}


int main(int argc, char** argv, char**envp)
{
    //Order of tasks
    /*
    1) Initialize program variables (read ZombieSim.conf file and ParseCommandLine (in that order).
    2) SetProgramSettings() //Set countries, cities and humans
    3) Create Threads and give them tasks. aka run them
    3.a) Log data or create images every xx time
    
    */
    
    long lReturn = ERR_NOERROR;

    //Init
    //Read conf file + ParseCommand Line
    
    lReturn = InitProgramVariables(argc, argv, envp); //maaike
    CHECK_RETURNVALUE(lReturn);
     
    lReturn = CreatePlayFieldObjects(); //Set country object, cities and humans and place them into one another.
    CHECK_RETURNVALUE(lReturn);
    // SetCountry() (Maaike) //Create Box2D World and run. Maaike

    float fCountrySize =0;
    lReturn = glb.propertyBag.GetCountrySize(&fCountrySize);
    CHECK_RETURNVALUE(lReturn);

    
    if (visualField.Construct((int32_t) round(fCountrySize), (int32_t) round(fCountrySize),1,1) <= 0)
        CHECK_RETURNVALUE(CREATE_VISUAL_CONSTRUCT_FAIL);

	//Set the pointers for the visual layer.
    lReturn = visualField.SetHumanVector(country.GetHumanVectorPointer());
    CHECK_RETURNVALUE(lReturn);

    lReturn = visualField.SetCityVector(country.GetCityVectorPointer());
    CHECK_RETURNVALUE(lReturn);
   
    std::vector<CZombieSimThread*> vecThreads;
    std::vector<CZombieSimThreadMoving*> vecThreadsMoving;
    lReturn = CreateThreadsAndRun(&vecThreads, &vecThreadsMoving);
    
    boost::thread t{ StartThread };
   
    
    t.join();
    

    WaitForAllThreads(vecThreads);
	WaitForAllThreads(vecThreadsMoving);


    vecThreads.clear();


    //yeah we are done :D
      return ERR_NOERROR;
}


long WriteOutPutFile()
{
    std::string strFilePath = std::filesystem::current_path();
}


long WaitForAllThreads(const std::vector<CZombieSimThread *>& pvecThreads) {
    //Set timeout (5s)
	boost::mutex mmutex;
    boost::system_time const timeout = boost::get_system_time()
            + boost::posix_time::seconds(5);

    for(auto pcZombieSim: pvecThreads){
        boost::thread* pThread = pcZombieSim->GetThreadProc();
        //Wait until execution finishes OR time
        while(!pThread->timed_join(timeout)) pThread->interrupt();
    }
#ifdef DEBUG
    mmutex.lock();
    std::cout << "All joined. " << pvecThreads.size() << std::endl;
    mmutex.unlock();
#endif // DEBUG
    return ERR_NOERROR;
}

long WaitForAllThreads(const std::vector<CZombieSimThreadSickness *>& pvecThreads) {
    //Set timeout (5s)
    boost::system_time const timeout = boost::get_system_time()
            + boost::posix_time::seconds(5);

    for(auto pcZombieSim: pvecThreads){
        boost::thread* pThread = pcZombieSim->GetThreadProc();
        //Wait until execution finishes OR time
        while(!pThread->timed_join(timeout)) pThread->interrupt();
    }
#ifdef DEBUG
    mmutex.lock();
    std::cout << "All joined. " << pvecThreads.size() << std::endl;
    mmutex.unlock();
#endif // DEBUG
    return ERR_NOERROR;
}

long WaitForAllThreads(const std::vector<CZombieSimThreadMoving *>& pvecThreads) {
    //Set timeout (5s)
	boost::mutex mmutex;
    boost::system_time const timeout = boost::get_system_time()
            + boost::posix_time::seconds(5);

    for(auto pcZombieSim: pvecThreads){
        boost::thread* pThread = pcZombieSim->GetThreadProc();
        //Wait until execution finishes OR time
        while(!pThread->timed_join(timeout)) pThread->interrupt();
    }
#ifdef DEBUG
    mmutex.lock();
    std::cout << "All joined. " << pvecThreads.size() << std::endl;
    mmutex.unlock();
#endif // DEBUG
    return ERR_NOERROR;
}

