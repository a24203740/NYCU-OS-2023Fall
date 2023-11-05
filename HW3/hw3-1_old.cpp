#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

void count(int index) {
  static mutex io_mutex;
  int num = 1000000;
  while (num--) {}
  {
    lock_guard<mutex> lock(io_mutex);
    cout << "I'm thread " << index << ", local count: 1000000\n";
  }
}

int main(void) {
  thread t[100];

  for (int i = 0; i < 100; i++)
    t[i] = thread(count, i);

  for (int i = 0; i < 100; i++)
    t[i].join();
}
