#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>

struct process
{
    int processID;
    int arrivalTime;
    int burstTime;
    int remainBurstTime;
    int belongQueueLevel;
    int remainTimeQuantum;
    int queueTime;
    //int waitingTime;
    //int turnaroundTime;
    int exitTime;

    process(int _processID, int _arrivalTime, int _burstTime)
    {
        processID = _processID;
        arrivalTime = _arrivalTime; 
        burstTime = _burstTime;  

        remainBurstTime = burstTime;
        exitTime=-1;
        queueTime=0;
    }

    bool burstDone()
    {
        return remainBurstTime <= 0;
    }
    bool usedOutTimeQuantum()
    {
        return remainTimeQuantum == 0;
    }

    // we want smaller remainBurstTime to be in front, then smaller arrivalTime
    struct remainingTimeCompareStruct{
        bool operator()(process* a, process* b)
        {
            if(a->remainBurstTime == b->remainBurstTime)
            {
                return a->arrivalTime > b->arrivalTime;
            }
            else
            {
                return a->remainBurstTime > b->remainBurstTime;
            }
        }
    };
};

std::vector<process> processList;

enum queueType
{
    RR,
    FCFS,
    SRTF
};

class baseQueue
{
    int timeQuantum;
    int level;
    queueType type;
public:

    // get/set
    int getTimeQuantum()
    {
        return timeQuantum;
    }
    void setTimeQuantum(int _timeQuantum)
    {
        timeQuantum = _timeQuantum;
    }
    queueType getType()
    {
        return type;
    }
    void setType(queueType _type)
    {
        type = _type;
    }
    int getLevel()
    {
        return level;
    }
    void setLevel(int _level)
    {
        level = _level;
    }

    // constructor
    baseQueue()
    {
        timeQuantum = 0;
        type = RR;
    }
    baseQueue(int _timeQuantum, queueType _type, int _level)
    {
        timeQuantum = _timeQuantum;
        type = _type;
        level = _level;
    }

    // utility function
    bool hasBetterLevel(process* p)
    {
        return p == nullptr || level < p->belongQueueLevel || p->belongQueueLevel == -1;
    }
    bool hasEqualLevel(process* p)
    {
        return p != nullptr && level == p->belongQueueLevel;
    }
    bool hasWorseLevel(process* p)
    {
        return p != nullptr && level > p->belongQueueLevel && p->belongQueueLevel != -1;
    }

    // virtual function
    virtual void addProcess(process* p, int curTime) = 0;
    virtual process* popProcess() = 0;
    // virtual process* frontProcess() = 0;
    virtual bool empty() = 0;
    virtual bool shouldPreempt(process* p) = 0;
    virtual void debug() = 0;
};

class FCFSQueue : public baseQueue
{
    std::queue<process*> queue;
    FCFSQueue()
    {
        setTimeQuantum(0);
        setLevel(0);
        setType(FCFS);
    }
public:
    FCFSQueue(int _timeQuantum, queueType _type, int _level) : baseQueue(_timeQuantum, _type, _level)
    {
    }
    void addProcess(process* p, int curTime)
    {
        if(!p)return;

        queue.push(p);
        p->remainTimeQuantum = getTimeQuantum();
        p->belongQueueLevel = getLevel();
        p->queueTime = curTime;
    }
    process* popProcess()
    {
        if(empty())
        {
            return nullptr;
        }

        process* p = queue.front();
        queue.pop();
        return p;
    }
    bool empty()
    {
        return queue.empty();
    }
    bool shouldPreempt(process* p)
    {
       return !queue.empty() && hasBetterLevel(p);
    }
    void debug()
    {
        std::cout << "FCFSQueue" << std::endl;
        auto cp = queue;
        while(!cp.empty())
        {
            std::cout << cp.front()->processID << " ";
            cp.pop();
        }
    }
};

class SRTFQueue : public baseQueue
{
    std::priority_queue<process*, std::vector<process*>, process::remainingTimeCompareStruct> queue;
    SRTFQueue()
    {
        setTimeQuantum(0);
        setLevel(0);
        setType(SRTF);
    }
public:
    SRTFQueue(int _timeQuantum, queueType _type, int _level) : baseQueue(_timeQuantum, _type, _level)
    {
    }
    void addProcess(process* p, int curTime)
    {
        if(!p)return;

        queue.push(p);
        p->remainTimeQuantum = getTimeQuantum();
        p->belongQueueLevel = getLevel();
        p->queueTime = curTime;
    }
    process* popProcess()
    {
        if(empty())
        {
            return nullptr;
        }

        process* p = queue.top();
        queue.pop();
        return p;
    }
    bool empty()
    {
        return queue.empty();
    }
    bool shouldPreempt(process* p)
    {
        if(queue.empty())
        {
            return false;
        }    

        if(hasBetterLevel(p))
        {
            return true;
        }
        else if(hasEqualLevel(p)) // ==
        {
            return p->remainBurstTime > queue.top()->remainBurstTime;
        }        

        return false;
    }
    void debug()
    {
        std::cout << "SRTFQueue" << std::endl;
        auto cp = queue;
        while(!cp.empty())
        {
            std::cout << cp.top()->processID << "(" << cp.top()->remainBurstTime << ")" << " ";
            cp.pop();
        }
    }
};

class RRQueue : public baseQueue
{
    std::vector<process*> queue;
    RRQueue()
    {
        setTimeQuantum(0);
        setLevel(0);
        setType(RR);
    }
public:
    RRQueue(int _timeQuantum, queueType _type, int _level) : baseQueue(_timeQuantum, _type, _level)
    {
    }
    void addProcess(process* p, int curTime)
    {
        if(!p)return;

        queue.push_back(p);
        p->remainTimeQuantum = getTimeQuantum();
        p->belongQueueLevel = getLevel();
        p->queueTime = curTime;

    }
    process* popProcess()
    {
        if(empty())
        {
            return nullptr;
        }

        for(int i = 1; i < queue.size(); i++)
        {
            if(queue[i]->queueTime != queue[0]->queueTime)
            {
                break;
            }
            if(queue[i]->arrivalTime > queue[0]->arrivalTime)
            {
                std::swap(queue[i], queue[0]);
            }
        }
        process* p = queue.front();
        queue.erase(queue.begin());
        return p;
    }
    bool empty()
    {
        return queue.empty();
    }
    bool shouldPreempt(process* p)
    {
        if(queue.empty())
        {
            return false;
        }    

        if(hasBetterLevel(p))
        {
            return true;
        }
        else if(hasEqualLevel(p)) // ==
        {
            return p->remainTimeQuantum == 0;
        }        

        return false;
    }
    void debug()
    {
        std::cout << "RRQueue" << std::endl;
        for(int i = 0; i < queue.size(); i++)
        {
            std::cout << queue[i]->processID << " ";
        }
    }
};

class Scheduler
{
    int processCount;
    int queueCount;
    
    int completeProcessCount;

    int timeQuantum;
    int currentTime;
    std::vector<baseQueue*> multilevelQueue;

    process* currentProcess;


    void inputQueue(int qid)
    {
        int _queueType, _timeQuantum;
        std::cin >> _queueType >> _timeQuantum;
        switch (_queueType)
        {
        case 0:
            multilevelQueue.push_back(new FCFSQueue(_timeQuantum, FCFS, qid));
            break;
        case 1:
            multilevelQueue.push_back(new SRTFQueue(_timeQuantum, SRTF, qid));
            break;
        case 2:
            multilevelQueue.push_back(new RRQueue(_timeQuantum, RR, qid));
            break;
        default:
            break;
        }
    }
    void inputProcess(int pid)
    {
        int _arrivalTime, _burstTime, _processID = pid;
        std::cin >> _arrivalTime >> _burstTime;
        processList.push_back(process(_processID, _arrivalTime, _burstTime));
    }
    void updateCurrentProcess()
    {
        if(currentProcess != nullptr)
        {
            currentProcess->remainBurstTime--;
            if(currentProcess->remainTimeQuantum != -1)
            {
                currentProcess->remainTimeQuantum--;
            }
        }
    }
    void killCurProcess()
    {
        if(currentProcess!= nullptr)
        {
            currentProcess->exitTime = currentTime;
            currentProcess = nullptr;
            completeProcessCount++;
        }
    }
    void suspendCurProcess()
    {
        if(currentProcess != nullptr)
        {
            int newLevel = currentProcess->belongQueueLevel + 1;
            if(newLevel >= queueCount)
            {
                newLevel = queueCount - 1;
            }
            multilevelQueue[newLevel]->addProcess(currentProcess, currentTime);
            currentProcess = nullptr;
        }
    }
    void addArrivedProcess()
    {
        for(int i = 0; i < processCount; i++)
        {
            if(processList[i].arrivalTime == currentTime)
            {
                multilevelQueue[0]->addProcess(&processList[i], currentTime);
            }
        }
    }
    bool checkAllProcessFinished()
    {
        return completeProcessCount == processCount;
    }
public:
    void debug()
    {
        std::cout << "current time: " << currentTime << std::endl;
        if(currentProcess != nullptr)
        {
            std::cout << "current process: " << currentProcess->processID << std::endl;
            std::cout << "remain brust time: " << currentProcess->remainBurstTime << std::endl;
            std::cout << "remain time quantum: " << currentProcess->remainTimeQuantum << std::endl;
        }
        else
        {
            std::cout << "current process: none" << std::endl;
        }
        for(int i = 0; i < queueCount; i++)
        {
            std::cout << "queue " << i << ": ";
            if(multilevelQueue[i]->empty())
            {
                std::cout << "empty" << std::endl;
            }
            else
            {
                multilevelQueue[i]->debug();
                std::cout << std::endl;
            }
            std::cout << "---" << std::endl;
        }
        std::cout << "======================" << std::endl;
    }
    bool ElapsedOneStepTime()
    {
        currentTime++;
        updateCurrentProcess();
        if(currentProcess != nullptr)
        {
            if(currentProcess->burstDone())
            {
                killCurProcess();
            }
            else if(currentProcess->usedOutTimeQuantum())
            {
                suspendCurProcess();
            }
        }
        addArrivedProcess();
        for(int i = 0; i < queueCount; i++)
        {
            bool preempt = multilevelQueue[i]->shouldPreempt(currentProcess);
            if(preempt)
            {
                suspendCurProcess();
                currentProcess = multilevelQueue[i]->popProcess();
                break;
            }
        }
        return checkAllProcessFinished();
    }
    void input()
    {
        std::cin >> queueCount >> processCount;
        for(int i = 0; i < queueCount; i++)
        {
            inputQueue(i);
        }

        for(int i = 0; i < processCount; i++)
        {
            inputProcess(i);
        }
    }
    Scheduler()
    {
        currentProcess = nullptr;
        processCount = 0;
        queueCount = 0;
        timeQuantum = 0;
        currentTime = -1;
        completeProcessCount = 0;
        multilevelQueue.clear();
    }
};

int main()
{
    Scheduler scheduler;
    scheduler.input();
    while(!scheduler.ElapsedOneStepTime())
    {
        //scheduler.debug();
    };

    int totalWaitTime = 0;
    int totalTurnTime = 0;
    for(int i = 0; i < processList.size(); i++)
    {
        int waitTime = processList[i].exitTime - processList[i].arrivalTime - processList[i].burstTime;
        int turnTime = processList[i].exitTime - processList[i].arrivalTime;
        totalWaitTime += waitTime;
        totalTurnTime += turnTime;
        std::cout << waitTime << " " << turnTime << std::endl;
    }
    std::cout << totalWaitTime << "\n" << totalTurnTime << std::endl;
    return 0;
}