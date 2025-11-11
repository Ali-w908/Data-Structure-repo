#include "RDY_NM.h"

Mission* RDY_NM::AbortMission(int ID) {
    LinkedQueue<Mission*> temp;     
    Mission* target = nullptr;      // Pointer to the mission to be aborted

    // Search through the queue
    while (!this->isEmpty()) {
        Mission* m;
        this->dequeue(m);           // Remove mission from the front

        if (m->getID() == ID) {
            target = m;             // Found the mission to abort
        }
        else {
            temp.enqueue(m);        // Keep other missions in the temporary queue
        }
    }

    // Restore all remaining missions back to the original queue
    while (!temp.isEmpty()) {
        Mission* m;
        temp.dequeue(m);
        this->enqueue(m);
    }

    return target; // Return the aborted mission, or nullptr if not found
}
