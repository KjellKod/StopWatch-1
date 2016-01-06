/* 
 * File:   AlarmClockSpeedTest.cpp
 * Author: Amanda Carbonari
 * Created: January 6, 2015 9:00am
 *
 * WARNING: This only measures CPU time
 */

#include <ctime>
#include <iostream>
#include <AlarmClock.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
typedef std::chrono::microseconds microseconds;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::seconds seconds;


template<typename T> void WaitForAlarmClockToExpire(AlarmClock<T>& alerter) {
   while(!alerter.Expired());
}

int main(int, const char**) {
   unsigned int us = 389;

   cout << "Creating Alarm Clock" << endl;
   AlarmClock<microseconds> alerter(us);
   // Give some time for the countdown to start
   std::this_thread::sleep_for(chrono::microseconds(20));
   cout << "Starting clock and resetting" << endl;
   high_resolution_clock::time_point start = high_resolution_clock::now();
   alerter.Reset();
   high_resolution_clock::time_point end = high_resolution_clock::now();
   auto reset_time = duration_cast<microseconds>(end - start).count();
   cout << "Waiting for the clock to expire" << endl;
   WaitForAlarmClockToExpire(alerter);

   cout << "Time: " << reset_time << " us" << endl;
}