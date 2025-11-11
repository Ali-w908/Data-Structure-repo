#include "MarsStation.h"
#include "newRequest.h"
#include "abortRequest.h"
#include "UI.h"
#include <cstdlib>
#include <cassert>
using namespace std;
MarsStation::MarsStation()
{
    time = 0;
    abortedMissions = 0;
	totalMissions = 0;
}

void MarsStation::readInputFile(string filename)
{

    ifstream input(filename);

    if (!input.is_open())
    {
        cout << "Error: Couldn't open input file: " << filename << endl;
        return;
    }

    input >> NO_OF_DRS >> NO_OF_PRS >> NO_OF_NRS;
    input >> SD >> SP >> SN;
    input >> M >> CD >> CP >> CN;
    for(int i=1;i<=NO_OF_NRS;i++)
    {
        Rover* r = new Rover(i, NR, SN, M, CN);
        Avail_NR.enqueue(r);
	}
    for(int i=1;i<=NO_OF_PRS;i++)
    {
        Rover* r = new Rover(NO_OF_NRS + i, PR, SP, M, CP);
        Avail_PR.enqueue(r);
    }
    for(int i=1;i<=NO_OF_DRS;i++)
    {
        Rover* r = new Rover(NO_OF_NRS+NO_OF_PRS+i, DR, SD, M, CD);
        Avail_DR.enqueue(r);
	}
	input >> K;
    for(int i=0;i<K;i++)
    {
        char requesttype;
        input >> requesttype;
        if (requesttype == 'R')
        {
            char mtype;
			int mid, rday, tloc, dur;
			input >> mtype >> rday >> mid >> tloc >> dur;
			newRequest* R = new newRequest(rday,mid,tloc,dur,mtype);
			DailyRequests.enqueue(R);
			totalMissions++;
        }
        else
        {
            int xd, id;
			input >> xd >> id;
			abortRequest* AR = new abortRequest(xd, id);
			DailyRequests.enqueue(AR);
        }
	}
    ui = new UI(this);
}

void MarsStation::simulate()
{
    while(totalMissions > DONE_missions.getCount())
    {
		RequeststoMissions();
		CheckUPtoAvailable();
		BacktoDone();
		ExectoBack();
        ExectoBack();
		OuttoExec();
        assignPM();
		assignDM();
        assignNM();

        if (ui->getMode() == INTERACTIVE) ui->printInterface();
        time++;
        assert(time < 1000);
	}

}

void MarsStation::insertMission(Mission* mission)
{
    switch (mission->getType())
    {
    case NM:
        RDY_NM_list.enqueue(mission);
        break;
    case PM:
        RDY_PM_list.enqueue(mission);
        break;
    case DM:
        RDY_DM_list.enqueue(mission);
        break;
    default:
        break;
	}
}

void MarsStation::RequeststoMissions()
{
    Request* req;
    while (!DailyRequests.isEmpty())
    {
        DailyRequests.peek(req);
        newRequest* NR = dynamic_cast<newRequest*>(req);
        abortRequest* AR = dynamic_cast<abortRequest*>(req);
        if (NR && NR->getRday() == time)
        {
            NR->Operate(this);
            DailyRequests.dequeue(req);
            delete req;
        }
        else if (AR && AR->getXD() == time)
        {
            AR->Operate(this);
            DailyRequests.dequeue(req);
            delete req;
        }
        else
            break;
	}
}

void MarsStation::CheckUPtoAvailable()
{
    int y = rand() % 100;
    Rover* rover=nullptr;
    if (y < 70)
    {
        if (!Checkup_DR.isEmpty())
        {
            Checkup_DR.dequeue(rover);
            Avail_DR.enqueue(rover);
        }
        else if (!Checkup_NR.isEmpty())
        {
            Checkup_NR.dequeue(rover);
            Avail_NR.enqueue(rover);
        }
        else if (!Checkup_PR.isEmpty())
        {

            Checkup_PR.dequeue(rover);
            Avail_PR.enqueue(rover);
		}
    }
}

void MarsStation::BacktoDone()
{
    if(BACK_missions.isEmpty())
		return;
    Mission* m = nullptr;
    int pri;
	BACK_missions.dequeue(m, pri);
	Rover* r = m->getAssignedRover();
	m->setAssignedRover(nullptr);
	DONE_missions.push(m);
    int x = rand() % 100;
    if (x < 20)
    {
        switch (r->getType())
        {
            case NR:
                Checkup_NR.enqueue(r);
				break;
            case PR:
				Checkup_PR.enqueue(r);
                break;
			case DR:
                Checkup_DR.enqueue(r);
                break;
        }
    }
    else
    {
        switch (r->getType())
        {
        case NR:
            Avail_NR.enqueue(r);
            break;
        case PR:
            Avail_PR.enqueue(r);
            break;
        case DR:
            Avail_DR.enqueue(r);
            break;
        }
    }
}

void MarsStation::ExectoBack()
{
    if (EXEC_missions.isEmpty())
        return;
	Mission* m = nullptr;
    int pri;
	EXEC_missions.dequeue(m, pri);
	BACK_missions.enqueue(m, pri);
}

void MarsStation::OuttoExec()
{
    if (OUT_missions.isEmpty())
        return;
    Mission* m = nullptr;
    int pri;
    OUT_missions.dequeue(m, pri);
    EXEC_missions.enqueue(m, pri);

}

void MarsStation::assignPM()
{
    if(RDY_PM_list.isEmpty())
		return;
    Mission* m = nullptr;
    Rover* r = nullptr;
    if (!Avail_PR.isEmpty())
    {
        RDY_PM_list.dequeue(m);
        Avail_PR.dequeue(r);
    }
    else if(!Avail_DR.isEmpty())
    {
        RDY_PM_list.dequeue(m);
        Avail_DR.dequeue(r);
	}
    else if(!Avail_NR.isEmpty())
    {
        RDY_PM_list.dequeue(m);
        Avail_NR.dequeue(r);
	}
    if (r)
    {
        m->setAssignedRover(r);
        OUT_missions.enqueue(m, -m->getLocationArrivalDay(time));
    }
}

void MarsStation::assignDM()
{
    if (RDY_DM_list.isEmpty())
        return;
    Mission* m = nullptr;
    Rover* r = nullptr;
    if (!Avail_DR.isEmpty())
    {
        RDY_DM_list.dequeue(m);
        Avail_DR.dequeue(r);
    }
    else if (!Avail_NR.isEmpty())
    {
        RDY_DM_list.dequeue(m);
        Avail_NR.dequeue(r);
    }
    else if (!Avail_PR.isEmpty())
    {
        RDY_DM_list.dequeue(m);
        Avail_PR.dequeue(r);
    }
    if (r)
    {
        m->setAssignedRover(r);
        OUT_missions.enqueue(m, -m->getLocationArrivalDay(time));
    }
}

void MarsStation::assignNM()
{
    if (RDY_NM_list.isEmpty())
        return;
    Mission* m = nullptr;
    Rover* r = nullptr;
    if (!Avail_NR.isEmpty())
    {
        RDY_NM_list.dequeue(m);
        Avail_NR.dequeue(r);
    }
    else if (!Avail_DR.isEmpty())
    {
        RDY_NM_list.dequeue(m);
        Avail_DR.dequeue(r);
    }
    else if (!Avail_PR.isEmpty())
    {
        RDY_NM_list.dequeue(m);
        Avail_PR.dequeue(r);
    }
    if (r)
    {
        m->setAssignedRover(r);
        OUT_missions.enqueue(m, -m->getLocationArrivalDay(time));
    }
}

