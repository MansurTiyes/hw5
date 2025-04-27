#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Helper function prototypes
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftsWorked,
    size_t day,
    size_t slot);

bool isValid(
    const AvailabilityMatrix& avail,
    const size_t maxShifts,
    const vector<size_t>& shiftsWorked,
    size_t day,
    Worker_T worker);

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched)
{
    if(avail.size() == 0U){
        return false;
    }
    
    sched.clear();
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();
    
    sched = DailySchedule(numDays, vector<Worker_T>(dailyNeed, INVALID_ID));
    vector<size_t> shiftsWorked(numWorkers, 0);
    
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsWorked, 0, 0);
}

// Helper function 
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftsWorked,
    size_t day,
    size_t slot)
{
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();
    
    // Base case
    if (day == numDays) {
        return true;
    }
    
    if (slot == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsWorked, day + 1, 0);
    }
    
    // ASSIGN each worker to the current day and slot
    for (Worker_T worker = 0; worker < numWorkers; worker++) {
        if (isValid(avail, maxShifts, shiftsWorked, day, worker)) {
            bool alreadyScheduled = false;
            for (size_t s = 0; s < slot; s++) {
                if (sched[day][s] == worker) {
                    alreadyScheduled = true;
                    break;
                }
            }
            
            if (!alreadyScheduled) {
                sched[day][slot] = worker;
                shiftsWorked[worker]++;
                
                if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsWorked, day, slot + 1)) {
                    return true;
                }
                
                // BACKTRACK
                sched[day][slot] = INVALID_ID;
                shiftsWorked[worker]--;
            }
        }
    }
    
    // nothing worked:
    return false;
}

bool isValid(
    const AvailabilityMatrix& avail,
    const size_t maxShifts,
    const vector<size_t>& shiftsWorked,
    size_t day,
    Worker_T worker)
{
    if (!avail[day][worker]) {
        return false;
    }
    
    if (shiftsWorked[worker] >= maxShifts) {
        return false;
    }
    
    return true;
}