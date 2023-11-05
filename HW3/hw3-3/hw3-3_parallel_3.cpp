#include <iostream>
#include <thread>
#include <string>
#include <vector>

using namespace std;

int n, global_count = 0;
int threadCount = 1;
thread t[8];
int localCount[8] = {0, 0, 0, 0, 0, 0, 0, 0};
long long loading[8] = {0,0,0,0,0,0,0,0};
const int offset[8] = {1,3,5,7,9,11,13,15};

bool is_prime(int num, int tid) {
    if (num == 1) return false;
    int i;
    for (i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            loading[tid] += i;
            return false;
        }
    }
    loading[tid] += i;
    return true;
}
void parallelIsPrime(int tid)
{
    int stepIndex = tid;
    for(int i = tid+1; i <= n; i+=offset[stepIndex])
    {
        if(is_prime(i, tid))
        {
            localCount[tid]++;
        }
        stepIndex = threadCount - 1 - stepIndex;
    }
}

int main(int argc, char* argv[]) {
    cin >> n;
    if(argc < 3 || string(argv[1]) != "-t")
    {
        threadCount = 1;
    }
    else
    {
        threadCount = stoi(argv[2]);
    }
    //step.resize(threadCount);

    for(int tid = 0; tid < threadCount; tid++)
    {
        t[tid] = thread(parallelIsPrime, tid);
    }
    for(int tid = 0; tid < threadCount; tid++)
    {
        t[tid].join();
        global_count += localCount[tid];
        cout << "loading of " << tid << " is " << loading[tid] << endl;
        // for(auto x : step[tid])
        // {
        //     cout << x << " ";
        // }
        // cout << endl;
    }
    

    cout << global_count << endl;
    return 0;
}
