#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <semaphore>
#include <thread>
#include <vector>
#include <condition_variable>
#include <iomanip>

using namespace std;

constexpr int MAX_VERTICES = 750;
constexpr ptrdiff_t MAX_THREADS{4};

mutex mutexes[MAX_VERTICES];
counting_semaphore semaphores{MAX_THREADS};

// random number generator
random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> dist(10, 30);

// num of vertices & num of edges
int V, E;
vector<int> adjacent_matrix[MAX_VERTICES];

// vertex_check[i]:
//    0: not checked
//    1: checked
bool vertex_checked[MAX_VERTICES];

// vertex_status[i]:
//    0: not in independent set
//    1: in independent set
bool vertex_status[MAX_VERTICES];

// if every vertex is checked, then the graph is converged
bool is_converged() {
  for (int i = 0; i < V; i++)
    if (!vertex_checked[i])
      return false;
  return true;
}

// if any neighbor is in the set, then leave the set
bool best_response(int v) {
  for (int u : adjacent_matrix[v])
    if (vertex_status[u])
      return false;
  return true;
}

vector<int> numOfNeighborWorking(MAX_VERTICES, 0);
int workingThread = 0;
int niceThread = 0;
std::mutex modifyNeighborWorkingLock;
std::condition_variable cv;

void maximum_independent_set(int v) {
    bool converged = false;

    while (!converged) 
    {
        // then doing what a vertex will do
        if (vertex_checked[v]) 
        {
            std::this_thread::sleep_for(std::chrono::nanoseconds(dist(rng)));
            converged = is_converged();
        } 
        else 
        {
        // first get unique mutex lock, to check if all neighbor are not working.
            {
                unique_lock<mutex> lk(modifyNeighborWorkingLock);
                cv.wait(lk, [v]
                { 
                    return numOfNeighborWorking[v] == 0;
                });
                //workingThread++;
                //cout << "working thread current: " << workingThread << endl;
                for(auto u : adjacent_matrix[v])
                {
                    numOfNeighborWorking[u]++;
                }
                lk.unlock();
                cv.notify_all();
            }
            bool old_response = vertex_status[v];
            // best_response read all neighbor status
            // all neighbor are sleeping, no race condition.
            vertex_status[v] = best_response(v);

            // only this thread will write itself to true
            // other neighbor may write it to false, but they are all sleeping.
            vertex_checked[v] = true;
            if (vertex_status[v] != old_response) 
            {
                for (int u : adjacent_matrix[v])
                // writing all neighbor checked
                // they are all sleeping, so then can only be write to false by their neighbor
                    vertex_checked[u] = false;
            }
            {
                // writing neighbor working again
                unique_lock<mutex> lk(modifyNeighborWorkingLock);
                cv.wait(lk, [v]{ return true; }); // all of its neighbor mustn't working, no need to check.
                                                    // using cv only for it queueing utility. 
                //workingThread--;
                //cout << "working thread current: " << workingThread << endl;
                for(auto u : adjacent_matrix[v])
                {
                    numOfNeighborWorking[u]--;
                }
                lk.unlock();
                cv.notify_all();
            }
            std::this_thread::sleep_for(std::chrono::nanoseconds(dist(rng)));
        }
    }
}

int main(void) {
    //auto start = chrono::high_resolution_clock::now();
    cin >> V >> E;

    thread t[V];

    for (int i = 0; i < V; i++) {
        vertex_checked[i] = false;
        vertex_status[i] = false;
    }

    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        adjacent_matrix[v].push_back(u);
        adjacent_matrix[u].push_back(v);
    }

    for (int i = 0; i < V; i++)
        t[i] = thread(maximum_independent_set, i);

    for (int i = 0; i < V; i++)
        t[i].join();

    for (int i = 0; i < V; i++) {
        if (!vertex_status[i])
        continue;
        cout << i << ' ';
    }
    cout << '\n';
    // auto end = chrono::high_resolution_clock::now();
    // double time_taken = 
    //   chrono::duration_cast<chrono::nanoseconds>(end - start).count();
 
    // time_taken *= 1e-9;
 
    // cout << "Time taken by program is : " << fixed 
    //      << time_taken << setprecision(9);
    // cout << " sec" << endl;
}
