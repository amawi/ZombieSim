#pragma once
#include "Country.h"

class CZombieSimThreadMoving
{
    friend void ThreadForMovingHumans(CZombieSimThreadMoving* pThreadInfo);
protected:
    bool m_bIsRunning;
    //Thread function pointer.
    boost::thread* m_pThread = nullptr;
    boost::mutex m_mutexIsRunningInThread;
	size_t m_StartPos = 0;
	size_t m_EndPos = 0;
    std::vector<CHuman*>* m_pvecHumans;

public:
    //Constructor
    CZombieSimThreadMoving()
    {

        m_bIsRunning = true;

    };
	CZombieSimThreadMoving(std::vector<CHuman*>* pvecHumans)
    {
        
        m_bIsRunning = true;

    };


    //Destructor
    ~CZombieSimThreadMoving() {
        if(m_pThread)
        delete m_pThread; //We only want to delete a thread pointer if it actually exist!.
    };

    //getters and setters

    boost::thread* GetThreadProc() {
        return m_pThread;
    }

    std::vector<CHuman*>* GetHumanVector() { return m_pvecHumans; }
    void SetThreadProc(boost::thread* pThreadProc) { m_pThread = pThreadProc; }
    size_t GetStartPos() { return m_StartPos; }
    size_t GetEndPos() { return m_EndPos; }
        
    void ThreadIsFinished()
	{
		boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
		m_bIsRunning = false;
	}
	bool IsRunning()
	{
		boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
		return m_bIsRunning;
	}

    //class function
    boost::thread* Start(){
        //this->pThread = new boost::thread(boost::ref(*this));
        return m_pThread;
    }

    

};

class CZombieSimThread 
{
    friend void ThreadWorkDivideIsRunning(CZombieSimThread* pThreadInfo);
protected:
    bool m_bIsRunning;
    //Thread function pointer.
    boost::thread* m_pThread = nullptr;
    boost::mutex m_mutexIsRunningInThread;
	size_t m_StartPos = 0;
	size_t m_EndPos = 0;
    CHuman* m_pHuman = nullptr;

public:
    //Constructor
    CZombieSimThread(size_t startpos, size_t endpos)
    {
        m_StartPos = startpos;
        m_EndPos = endpos;
        m_bIsRunning = true;

    };
	CZombieSimThread(size_t startpos, size_t endpos, CHuman * pHuman)
    {
        m_StartPos = startpos;
        m_EndPos = endpos;
        m_pHuman = pHuman;
        m_bIsRunning = true;

    };


    //Destructor
    ~CZombieSimThread() {
        if(m_pThread)
        delete m_pThread; //We only want to delete a thread pointer if it actually exist!.
    };

    //getters and setters

    boost::thread* GetThreadProc() {
        return m_pThread;
    }

    CHuman* GetHuman() { return m_pHuman; }

    size_t GetStartPos() { return m_StartPos; }
    size_t GetEndPos() { return m_EndPos; }
    void SetEndPos(size_t sizeEndpos) { m_EndPos = sizeEndpos; }
    void SetThreadProc(boost::thread* pThreadProc) { m_pThread = pThreadProc; }
    
    void ThreadIsFinished()
	{
		boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
		m_bIsRunning = false;
	}
	bool IsRunning()
	{
		boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
		return m_bIsRunning;
	}

    //class function
    boost::thread* Start(){
        //this->pThread = new boost::thread(boost::ref(*this));
        return m_pThread;
    }

    

};

class CZombieSimThreadSickness 
{
    friend void ThreadProcessSickness(CZombieSimThreadSickness* pThreadInfo);
protected:
    bool m_bIsRunning;
    //Thread function pointer.
    boost::thread* m_pThread = nullptr;
    boost::mutex m_mutexIsRunningInThread;
	size_t m_StartPos = 0;
	size_t m_EndPos = 0;
    CHuman* m_pHuman = nullptr;

public:
    //Constructor
    CZombieSimThreadSickness(size_t startpos, size_t endpos)
    {
        m_StartPos = startpos;
        m_EndPos = endpos;
        m_bIsRunning = true;

    };
	CZombieSimThreadSickness(size_t startpos, size_t endpos, CHuman * pHuman)
    {
        m_StartPos = startpos;
        m_EndPos = endpos;
        m_pHuman = pHuman;
        m_bIsRunning = true;

    };


    //Destructor
    ~CZombieSimThreadSickness() {
        if(m_pThread)
        delete m_pThread; //We only want to delete a thread pointer if it actually exist!.
    };

    //getters and setters

    boost::thread* GetThreadProc() {
        return m_pThread;
    }

    CHuman* GetHuman() { return m_pHuman; }

    size_t GetStartPos() { return m_StartPos; }
    size_t GetEndPos() { return m_EndPos; }
    void SetThreadProc(boost::thread* pThreadProc) { m_pThread = pThreadProc; }
    
    void ThreadIsFinished()
	{
		boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
		m_bIsRunning = false;
	}
	bool IsRunning()
	{
		boost::lock_guard<boost::mutex> lock{ m_mutexIsRunningInThread };
		return m_bIsRunning;
	}

    //class function
    boost::thread* Start(){
        //this->pThread = new boost::thread(boost::ref(*this));
        return m_pThread;
    }

    

};