#include "MarsStation.h"
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////
// Constructor implementation
//Initializes the Mars Station.
//Creates available rovers for each type(Normal, Polar, Drill) and enqueues them in their respective lists
MarsStation::MarsStation(int numNR, int numPR, int numDR,
    int spNR, int spPR, int spDR,
    int missionsBeforeCheckup, int checkupDuration_DR, int checkupDuration_PR, int checkupDuration_NR)
{
    // Store the configuration values
    NO_OF_NRS = numNR;
    NO_OF_PRS = numPR;
    NO_OF_DRS = numDR;
    SN = spNR;
    SP = spPR;
    SD = spDR;
    M = missionsBeforeCheckup;
    CD = checkupDuration_DR;
    CP = checkupDuration_PR;
    CN = checkupDuration_NR;

    // === Create and enqueue Normal Rovers ===
    for (int i = 1; i <= numNR; i++) {
        Rover* r = new Rover(i, NR, spNR, missionsBeforeCheckup, CD);
        Avail_NR.enqueue(r);
    }

    // === Create and enqueue Polar Rovers ===
    for (int i = 1; i <= numPR; i++) {
        Rover* r = new Rover(1000 + i, PR, spPR, missionsBeforeCheckup, CP);
        Avail_PR.enqueue(r);
    }

    // === Create and enqueue Drill Rovers ===
    for (int i = 1; i <= numDR; i++) {
        Rover* r = new Rover(2000 + i, DR, spDR, missionsBeforeCheckup, CN);
        Avail_DR.enqueue(r);
    }

    std::cout << "MarsStation initialized with:" << std::endl;
    std::cout << "  Normal Rovers: " << numNR << std::endl;
    std::cout << "  Polar  Rovers: " << numPR << std::endl;
    std::cout << "  Drill  Rovers: " << numDR << std::endl;
}


/////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////
// Set the number of requests
void MarsStation::setNoOfRequests(int num)
{
    if (num < 0) {
        std::cout << "Error: Number of requests cannot be negative." << std::endl;
        return;
    }

    K = num;
  
}

/////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////
  int MarsStation::getNoOfRequests() const
  {
    return K;
   }

  /////////////////////////////////////////////////////////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////////////////
//Reads a command from the user :

//R  Adds a new mission(Normal / Polar / Drill) to its corresponding Ready List.

//X  Aborts a Normal Mission(NM) if it exists in the Ready NM list or in Out missions.

//If successful, moves the mission to the Aborted_missions list.

//Prints a success or failure message
void MarsStation::readUserCommand()
{
    char command;
    cout << "\nEnter Command (R TYPE RDAY ID TLOC DUR) or (X XD ID): ";
    cin >> command;
    command = toupper(command);

    if (command == 'R') {
        char type;   // N, P, D
        int rDay, id, tloc, dur;

        cin >> type >> rDay >> id >> tloc >> dur;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); return; }

        // إنشاء مهمة جديدة
        Mission* mission = new Mission(id,
            (type == 'N' ? NM : (type == 'P' ? PM : DM)),
            rDay, tloc, dur);

        // إضافة المهمة مباشرة للقائمة المناسبة
        switch (type) {
        case 'N':
            RDY_NM_list.enqueue(mission);
            break;
        case 'P':
            RDY_PM_list.enqueue(mission);
            break;
        case 'D':
            RDY_DM_list.enqueue(mission);
            break;
        }

        cout << " Mission added: ID=" << id << endl;
    }
    else if (command == 'X') {
        int xd, id;
        cin >> xd >> id;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); return; }

        Mission* abortedMission = nullptr;

        // 1️⃣ البحث في Ready NM list
        abortedMission = RDY_NM_list.AbortMission(id);

        // 2️⃣ لو مش موجودة في Ready List، البحث في OUT missions
        if (!abortedMission) {
            abortedMission = OUT_missions.AbortMission(id);
        }

        if (abortedMission) {
            abortedMission->setStatus(ABORTED);
            Aborted_missions.enqueue(abortedMission);
            cout << " Normal Mission ID=" << id << " aborted successfully.\n";
        }
        else {
            cout << " Normal Mission ID=" << id << " not found in Ready or Out missions.\n";
        }
    }
    else {
        cout << " Invalid command! Please enter 'R' or 'X'.\n";
    }

    cin.ignore(1000, '\n');
}

/////////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////
//Checks all available rovers in the queues.

//If a rover has completed M missions, moves it to the checkup list(Checkup_NR, Checkup_PR, Checkup_DR).

//Updates its status to "in checkup" and sets the available day.

//Prints a message for each rover moved to checkup.
void MarsStation::checkRoversForCheckup() {
    // Normal Rovers
    int count = Avail_NR.getCount();
    for (int i = 0; i < count; i++) {
        Rover* r;
        Avail_NR.dequeue(r);
        if (r->getMissionsCompleted() >= M) {
            r->setInCheckup(true);            // وضع علامة على Checkup
            r->setAvailableDay(0);            // أو اليوم الحالي لو تحب
            Checkup_NR.enqueue(r);            // نقل للقائمة الخاصة بالصيانة
            cout << " Normal Rover ID=" << r->getID() << " moved to checkup.\n";
        }
        else {
            Avail_NR.enqueue(r);              // لو لسه ما وصلش M، يرجع للقائمة
        }
    }

    // Drill Rovers
    count = Avail_DR.getCount();
    for (int i = 0; i < count; i++) {
        Rover* r;
        Avail_DR.dequeue(r);
        if (r->getMissionsCompleted() >= M) {
            r->setInCheckup(true);
            r->setAvailableDay(0);
            Checkup_DR.enqueue(r);
            cout << " Drill Rover ID=" << r->getID() << " moved to checkup.\n";
        }
        else {
            Avail_DR.enqueue(r);
        }
    }

    // Polar Rovers
    count = Avail_PR.getCount();
    for (int i = 0; i < count; i++) {
        Rover* r;
        Avail_PR.dequeue(r);
        if (r->getMissionsCompleted() >= M) {
            r->setInCheckup(true);
            r->setAvailableDay(0);
            Checkup_PR.enqueue(r);
            cout << " Polar Rover ID=" << r->getID() << " moved to checkup.\n";
        }
        else {
            Avail_PR.enqueue(r);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/*Only checks Polar Missions (PM) in the Ready list.

If a mission has waited more than double its duration:

Aborts it (ABORTED) and moves it to the Aborted_missions list.

Prints a message indicating the mission was aborted.*/

void MarsStation::checkMissionsForAbort(int currentDay) {
    // فقط Polar Missions في Ready List
    int count = RDY_PM_list.getCount();
    LinkedQueue<Mission*> temp;

    for (int i = 0; i < count; i++) {
        Mission* m;
        RDY_PM_list.dequeue(m);

        int waitingDays = currentDay - m->getRday();  // الأيام اللي قعدتها المهمة
        if (waitingDays > 2 * m->getMDUR()) {
            // عمل abort للمهمة
            m->setStatus(ABORTED);
            Aborted_missions.enqueue(m);
            cout << " Polar Mission ID=" << m->getID()
                << " aborted due to exceeding double duration.\n";
        }
        else {
            temp.enqueue(m); 
        }
    }

    while (!temp.isEmpty()) {
        Mission* m;
        temp.dequeue(m);
        RDY_PM_list.enqueue(m);
    }
}
/// /////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/*Selects an available rover based on mission type:

NM: Prefer Normal; if unavailable → Polar.

PM: Prefer Polar; if unavailable → Normal, then Drill.

DM: Drill only.

If a rover is available, assigns the mission, increments rover’s completed missions, and enqueues the mission in EXEC_missions.

Prints assignment info or a message if no rover is available.*/
Rover* MarsStation::assignRoverToMission(Mission* mission) {
    Rover* assignedRover = nullptr;

    switch (mission->getType()) {
    case NM:
        if (!Avail_NR.isEmpty()) {
            Avail_NR.dequeue(assignedRover);
        }
        else if (!Avail_PR.isEmpty()) {
            Avail_PR.dequeue(assignedRover);
        }
        break;

    case PM:
        if (!Avail_PR.isEmpty()) {
            Avail_PR.dequeue(assignedRover);
        }
        else if (!Avail_NR.isEmpty()) {
            Avail_NR.dequeue(assignedRover);
        }
        else if (!Avail_DR.isEmpty()) {
            Avail_DR.dequeue(assignedRover);
        }
        break;

    case DM:
        if (!Avail_DR.isEmpty()) {
            Avail_DR.dequeue(assignedRover);
        }
        break;
    }

    if (assignedRover) {
        cout << "Mission ID=" << mission->getID()
            << " assigned to Rover ID=" << assignedRover->getID() << endl;
        mission->setStatus(IN_EXEC);
        assignedRover->incrementMissions();
        EXEC_missions.enqueue(mission, 0); // 0: يمكن تغييرها حسب الأولوية
    }
    else {
        cout << "No available rover for Mission ID=" << mission->getID() << endl;
    }

    return assignedRover;
}
/// /////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/*Attempts to assign all missions in the Ready lists (RDY_NM_list, RDY_PM_list, RDY_DM_list) to available rovers.

If no rover is available, the mission stays in its Ready List for later assignment.*/
void MarsStation::assignReadyMissions() {
    // Normal Missions
    int count = RDY_NM_list.getCount();
    LinkedQueue<Mission*> temp;
    for (int i = 0; i < count; i++) {
        Mission* m;
        RDY_NM_list.dequeue(m);
        if (!assignRoverToMission(m)) {  
            temp.enqueue(m);
        }
    }
    while (!temp.isEmpty()) {
        Mission* m;
        temp.dequeue(m);
        RDY_NM_list.enqueue(m);
    }

    // Polar Missions
    count = RDY_PM_list.getCount();
    for (int i = 0; i < count; i++) {
        Mission* m;
        RDY_PM_list.dequeue(m);
        if (!assignRoverToMission(m)) {
            temp.enqueue(m);
        }
    }
    while (!temp.isEmpty()) {
        Mission* m;
        temp.dequeue(m);
        RDY_PM_list.enqueue(m);
    }

    // Drill Missions
    count = RDY_DM_list.getCount();
    for (int i = 0; i < count; i++) {
        Mission* m;
        RDY_DM_list.dequeue(m);
        if (!assignRoverToMission(m)) {
            temp.enqueue(m);
        }
    }
    while (!temp.isEmpty()) {
        Mission* m;
        temp.dequeue(m);
        RDY_DM_list.enqueue(m);
    }
}
