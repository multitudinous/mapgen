#ifndef SLEEPSIM_H
#define SLEEPSIM_H

#include <QMutex>
#include <QWaitCondition>

class SleepSim{
     QMutex localMutex;
     QWaitCondition sleepSimulator;
public:
    SleepSim::SleepSim()
    {
        localMutex.lock();
    }
    void sleep(unsigned long sleepMS)
    {
        sleepSimulator.wait(&localMutex, sleepMS);
    }
    void CancelSleep()
    {
        sleepSimulator.wakeAll();
    }
};

/*
QWaitCondition is designed to coordinate mutex waiting between different threads.
But what makes this work is the wait method has a timeout on it.
When called this way, it functions exactly like a sleep function, but it uses Qt's event loop for the timing.
So, no other events or the UI are blocked like normal windows sleep function does.

As a bonus, we added the CancelSleep function to allows another part of the program to cancel the "sleep" function.
What we liked about this is that it lightweight, reusable and is completely self contained.

QMutex: http://doc.qt.nokia.com/4.6/qmutex.html
QWaitCondition: http://doc.qt.nokia.com/4.6/qwaitcondition.html
*/

#endif
