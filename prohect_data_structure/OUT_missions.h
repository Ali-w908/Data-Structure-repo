#include "Mission.h"
#include "priQueue.h"

class OUT_missions : public priQueue<Mission*> {
public:
    Mission* AbortMission(int ID);
};

