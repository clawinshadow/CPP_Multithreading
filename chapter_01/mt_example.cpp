#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>

#include "mt_example.h"

namespace Chapter_01
{

std::mutex values_mtx;
std::mutex cout_mtx;
std::vector<int> values;

using namespace std;

void ThreadFunc(int tid)
{
    cout_mtx.lock();
    cout << "Starting thread " << tid << ".\n";
    cout_mtx.unlock();

    values_mtx.lock();
    int val = values[0];
    values_mtx.unlock();

    int rval = randGen(0, 10);
    val += rval;

    cout_mtx.lock();
    cout << "Thread " << tid << " adding " << rval << ". New value: " << val << ".\n";
    cout_mtx.unlock();

    values_mtx.lock();
    values.push_back(val);
    values_mtx.unlock();
}

/**
 Thread-local storage duration is a term used to refer to data that is seemingly global or static storage duration
 (from the viewpoint of the functions using it) but in actual fact, there is one copy per thread.

 It adds to the current automatic (exists during a block/function),
 static (exists for the program duration) and dynamic (exists on the heap between allocation and deallocation).

 Something that is thread-local is brought into existence at thread creation and disposed of when the thread stops.

 Some examples follow:
 Think of a random number generator where the seed must be maintained on a per-thread basis.
 Using a thread-local seed means that each thread gets its own random number sequence, independent of other threads.
 - If your seed was a local variable within the random function, it would be initialised every time you called it,
   giving you the same number each time.
 - If it was a global, threads would interfere with each other's sequences.

 */
int randGen(const int& min, const int& max)
{
    thread_local static mt19937 generator(hash<thread::id>()(std::this_thread::get_id()));
    uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void Run()
{
    cout << "Running chapter_01 - mt_example \n";
    values.push_back(42);

    thread tr1(ThreadFunc, 1);
    thread tr2(ThreadFunc, 2);
    thread tr3(ThreadFunc, 3);
    thread tr4(ThreadFunc, 4);

    tr1.join();
    tr2.join();
    tr3.join();
    tr4.join();

    cout << "Input: " << values[0] << ", Result 1: " << values[1] << ", Result 2: "
         << values[2] << ", Result 3: " << values[3] << ", Result 4: " << values[4] << "\n";
}

}