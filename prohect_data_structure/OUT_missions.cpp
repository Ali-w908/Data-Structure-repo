#include "OUT_missions.h"

Mission* OUT_missions::AbortMission(int ID) {
    priQueue<Mission*> temp;
    Mission* target = nullptr;
    int pri;

    while (!this->isEmpty()) {
        Mission* m;
        this->dequeue(m, pri);

        if (m->getID() == ID) {
            target = m; // A BORTED  MISSION
        }
        else {
            temp.enqueue(m, pri);
        }
    }

    //BACK ALL MISSIONS WITHOUT TARGET
    while (!temp.isEmpty()) {
        Mission* m;
        temp.dequeue(m, pri);
        this->enqueue(m, pri);
    }

    return target; // nullptr IF ID DIDNT FIND
}