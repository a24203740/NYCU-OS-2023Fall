#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <vector>

const int numOfThread = 100;

class SemWrapper
{
    std::binary_semaphore sem{0};
public:
    inline void acquire()
    {
        sem.acquire();
    }
    inline void release()
    {
        sem.release();
    }
};
SemWrapper semList[numOfThread];

void count(int index) {
    int num = 1000000;
    while (num--) {}
    {
        semList[index].acquire();
        std::cout << "I'm thread " << index << ", local count: 1000000\n";
        semList[index].release();
        if(index < numOfThread - 1)
        {
            semList[index + 1].release();
        }

    }
}

int main(void) {
    std::thread t[100];
    semList[0].release();
    for (int i = 0; i < 100; i++)
        t[i] = std::thread(count, i);

    for (int i = 0; i < 100; i++)
        t[i].join();
}
