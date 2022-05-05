#include <iostream>
#include "thread.h"
#include "semaphore.h"
#include "mutex.h"
#include <unistd.h>

using namespace std;

const int DELAY = 10000000;
const int ITERATIONS = 1;
Mutex table;

Semaphore chopstick[5];

int philosopher(int n)
{

    int first = (n < 4) ? n : 0;      
    int second = (n < 4) ? n + 1 : 4; 


    for (int i = 0; i < ITERATIONS; i++)
    {

        table.lock();
        cout << "Philosopher " << n << " thinking ...\n";
        table.unlock();

        for (int i = 0; i < DELAY * 10; i++);
        sleep(1);

        chopstick[first].p();
        chopstick[second].p();

        table.lock();
        cout << "Philosopher " << n << " eating ...\n";
        table.unlock();

        for (int i = 0; i < DELAY; i++);
        sleep(1);

        chopstick[first].v();
        chopstick[second].v();

        cout << "Philosopher " << n << " release forks!\n";
    
    
    }

    return n;

}

int main()
{
    cout << "The Dining-Philosophers Problem\n";

    Thread *phil[5];
    for (int i = 0; i < 5; i++)
        phil[i] = new Thread(&philosopher, i);

    int status;
    for (int i = 0; i < 5; i++)
    {
        phil[i]->join(&status);

        if (status == i)
            cout << "Philosopher " << i << " went to heaven!\n";
        else
            cout << "Philosopher " << i << " went to hell!\n";
    }

    return 0;
}
