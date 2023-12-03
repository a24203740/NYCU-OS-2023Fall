#include <iostream>
#include <vector>
#include <utility>

using namespace std;

int main() {
    int blockSize;
    int setSize;
    int requestCount;
    cin >> blockSize >> setSize >> requestCount;
    int setCount = blockSize / setSize;
    vector<pair<int,int>> block(blockSize, {-1,-1});
    
    int cacheMiss = 0;
    for(int address = 0; address < requestCount; address++) 
    {
        int request;
        cin >> request;
        for(int i = 0; i < blockSize; i++) 
        {
            if(block[i].first == request) 
            {
                block[i].second = address;
                break;
            }
            if(i == blockSize - 1) 
            {
                int corrspondingSet = address % setCount;
                int setStart = corrspondingSet * setSize;
                int setEnd = setStart + setSize;
                int oldest = block[setStart].second;
                int oldestIndex = setStart;
                for(int j = setStart; j < setEnd; j++) 
                {
                    if(block[j].first == -1) 
                    {
                        oldestIndex = j;
                        break;
                    }
                    if(block[j].second < oldest) 
                    {
                        oldest = block[j].second;
                        oldestIndex = j;
                    }
                }
                block[oldestIndex] = {request, address};
                cacheMiss++;
            }
        }
    }
    cout << "Total Cache Misses:" << cacheMiss;
}