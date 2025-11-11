#pragma once
#include "Mission.h"
#include "Rover.h"
#include "RDY_NM.h"
#include "LinkedQueue.h"
#include "ArrayStack.h"
#include "PriQueue.h"
#include "Request.h"
#include "OUT_missions.h"

#define MAX_DAYs 1000
class MarsStation{
private:
    int NO_OF_DRS, NO_OF_PRS, NO_OF_NRS;//NOMBER OF EACH TYPE ROVER
    int SD, SP,SN;// SPEED OF EACH TYPE OF ROVER
    int CD, CP, CN;// check up duration 
    int M;//NOMBER OF COMPLETED MISSION BEFOR CHECK UP 
    int K;// no of requests
    //  Ready Missions
    RDY_NM RDY_NM_list;
    LinkedQueue<Mission*> RDY_DM_list;
    LinkedQueue<Mission*> RDY_PM_list;

    //  Executing & Outgoing Missions
    priQueue<Mission*> EXEC_missions;
    OUT_missions OUT_missions;
    //  Completed & Aborted Missions
    ArrayStack<Mission*> DONE_missions;
    LinkedQueue<Mission*> Aborted_missions;

    //  Available Rovers
    LinkedQueue<Rover*> Avail_NR;
    LinkedQueue<Rover*> Avail_DR;
    LinkedQueue<Rover*> Avail_PR;

    //  Rovers in Checkup
    LinkedQueue<Rover*> Checkup_NR;
    LinkedQueue<Rover*> Checkup_DR;
    LinkedQueue<Rover*> Checkup_PR;

    //  Daily Requests
    LinkedQueue<Request*> DailyRequests; 

public:
    //  Constructor that initializes and fills rover queues
    MarsStation(int numNR, int numPR, int numDR,
        int spNR, int spPR, int spDR,
        int missionsBeforeCheckup, int checkupDuration_DR, int checkupDuration_PR, int checkupDuration_NR);
    void setNoOfRequests(int num);
    int getNoOfRequests() const; 
    void readUserCommand();
    void checkRoversForCheckup();
    void checkMissionsForAbort(int currentDay);
    Rover* assignRoverToMission(Mission* mission);
    void assignReadyMissions();

};