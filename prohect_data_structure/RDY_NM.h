#include "LinkedQueue.h"
#include "Mission.h"


class RDY_NM : public LinkedQueue<Mission*> {
public:
    Mission* AbortMission(int ID);
};

