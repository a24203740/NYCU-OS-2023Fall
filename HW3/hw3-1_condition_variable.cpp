#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <vector>
#include <condition_variable>

const int numOfThread = 100;
std::mutex mtx;
std::condition_variable cv;
std::vector<bool> canGetIn;

void count(int index) {
    int num = 1000000;
    while (num--) {}
    {
        std::unique_lock<std::mutex> lock(mtx);
        // cannot use lock guard since it cannot move the ownership of lock
        cv.wait(lock, [index]{ return canGetIn[index]; });
        // the lock must be acquired by this thread.
        // check if the stop_waiting function return true
        //  -> stop function is the second argument
        //  -> if not, release lock, add itself into waiting list
        //      -> get up when notify.
        //  -> when notify, acquire lock, check if stop_waiting true
        //      -> if still not, back to sleep.
        std::cout << "I'm thread " << index << ", local count: 1000000\n";
        if(index + 1 < numOfThread)
        {
            canGetIn[index + 1] = true;
        }
        lock.unlock(); // must unlock before notify.
        cv.notify_all();
    }
}

int main(void) {
    std::thread t[100];
    canGetIn.assign(100, false);
    canGetIn[0] = true;
    for (int i = 0; i < 100; i++)
        t[i] = std::thread(count, i);

    for (int i = 0; i < 100; i++)
        t[i].join();
}
