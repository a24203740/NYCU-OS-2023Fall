#include <iostream>
#include <thread>

using namespace std;

int n, global_count = 0;
bool global_is_prime;
int threadCount = 1;
thread t[8];


void is_prime(int num, int start) {
    if (num == 1) 
    {
        global_is_prime = false;
        return;
    }
    for (int i = start; i * i <= num && global_is_prime; i+=threadCount) {
        cout << start << " ";
        if (num % i == 0) {
            global_is_prime = false;
            return;
        }
    }
    return;
}

int main(int argc, char* argv[]) {
    cin >> n;
    threadCount = stoi(argv[2]);
    for (int i = 1; i <= n; i++) {
        global_is_prime = true;
        for(int nt = 0; nt < threadCount; nt++)
        {
            t[nt] = thread(is_prime, i, nt+2);
        }
        for(int nt = 0; nt < threadCount; nt++)
        {
            t[nt].join();
        }
        cout << endl;
        if (global_is_prime) global_count++;
    }

    cout << global_count << endl;
    return 0;
}
