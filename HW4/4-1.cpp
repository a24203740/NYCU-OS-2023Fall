#include <iostream>
#include <vector>

using namespace std;

int main() {
    int blockSize;
    int requestCount;
    cin >> blockSize >> requestCount;
    vector<int> block(blockSize, -1);
    int cacheMiss = 0;
    for(int address = 0; address < requestCount; address++) 
    {
        int request;
        cin >> request;
        for(int i = 0; i < blockSize; i++) 
        {
            if(block[i] == request) 
            {
                break;
            }
            if(i == blockSize - 1) 
            {
                int corrspondingBlock = address % blockSize;
                block[corrspondingBlock] = request;
                cacheMiss++;
            }
        }
    }
    cout << "Total Cache Misses:" << cacheMiss;
}