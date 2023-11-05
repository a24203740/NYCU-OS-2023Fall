#include <iostream>
#include <thread>
#include <string>

using namespace std;

int n, global_count = 0;
int threadCount = 1;
thread t[8];
int localCount[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int loading[8] = {0,0,0,0,0,0,0,0};

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
    for(int i = tid+1; i <= n; i+=threadCount)
    {
        if(is_prime(i, tid))
        {
            localCount[tid]++;
        }

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

    for(int tid = 0; tid < threadCount; tid++)
    {
        t[tid] = thread(parallelIsPrime, tid);
    }
    for(int tid = 0; tid < threadCount; tid++)
    {
        t[tid].join();
        global_count += localCount[tid];
        cout << "loading of " << tid << " is " << loading[tid] << endl;
    }
    

    cout << global_count << endl;
    return 0;
}
