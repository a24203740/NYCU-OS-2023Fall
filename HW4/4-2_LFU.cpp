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
                block[i].second++;
                break;
            }
            if(i == blockSize - 1) 
            {
                int corrspondingSet = address % setCount;
                int setStart = corrspondingSet * setSize;
                int setEnd = setStart + setSize;
                int leastFreq = block[setStart].second;
                int leastFreqIndex = setStart;
                for(int j = setStart; j < setEnd; j++) 
                {
                    if(block[j].first == -1) 
                    {
                        leastFreqIndex = j;
                        break;
                    }
                    if(block[j].second < leastFreq) 
                    {
                        leastFreq = block[j].second;
                        leastFreqIndex = j;
                    }
                }
                block[leastFreqIndex] = {request, 0};
                cacheMiss++;
            }
        }
    }
    cout << "Total Cache Misses:" << cacheMiss;
}