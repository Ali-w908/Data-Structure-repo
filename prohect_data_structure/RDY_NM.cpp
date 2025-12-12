#include "RDY_NM.h"

Mission* RDY_NM::AbortMission(int ID) {
    LinkedQueue<Mission*> temp;     
    Mission* target = nullptr;


    while (!this->isEmpty()) {
        Mission* m;
        this->dequeue(m);

        if (m->getID() == ID) {
            target = m;
        }
        else {
            temp.enqueue(m);
        }
    }

    while (!temp.isEmpty()) {
        Mission* m;
        temp.dequeue(m);
        this->enqueue(m);
    }

    return target;
}
