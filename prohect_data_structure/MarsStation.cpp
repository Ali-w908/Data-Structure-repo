#include <iostream>
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
    // Run until all missions are done or aborted
    while (DONE_missions.getCount() + abortedMissions < totalMissions)
    {
        // Process today's requests (new missions & aborts)
        RequeststoMissions();
        
        // Auto-abort PM waiting too long (>2x duration)
        autoAbortPM();
        
        // Release rovers from checkup
        CheckUPtoAvailable();
        
        // Complete returning missions
        BacktoDone();
        
        // Finished execution? Start heading back
        ExectoBack();
        
        // Arrived at target? Begin execution
        OuttoExec();
        
        // Assign rovers to missions - PM first (they can auto-abort), then NM, then DM
        bool assigned = true;
        while (assigned)
        {
            assigned = false;
            
            if (!RDY_PM_list.isEmpty() && 
                (!Avail_PR.isEmpty() || !Avail_DR.isEmpty() || !Avail_NR.isEmpty()))
            {
                assignPM();
                assigned = true;
            }
            
            if (!RDY_NM_list.isEmpty() && 
                (!Avail_NR.isEmpty() || !Avail_DR.isEmpty() || !Avail_PR.isEmpty()))
            {
                assignNM();
                assigned = true;
            }
            
            if (!RDY_DM_list.isEmpty() && 
                (!Avail_DR.isEmpty() || !Avail_NR.isEmpty() || !Avail_PR.isEmpty()))
            {
                assignDM();
                assigned = true;
            }
        }
        
        // Show UI in interactive mode
        if (ui->getMode() == INTERACTIVE)
        {
            ui->printInterface();
            cin.get();
        }
        
        time++;
        
        if (time >= MAX_DAYs)
        {
            cout << "Simulation hit " << MAX_DAYs << " days limit." << endl;
            break;
        }
    }
    
    // Done - save output
    writeOutputFile("output.txt");
    
    cout << "\n=== SIMULATION COMPLETE ===" << endl;
    cout << "Days: " << time << " | Done: " << DONE_missions.getCount() 
         << " | Aborted: " << abortedMissions << endl;
    cout << "Output: output.txt" << endl;
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
    // Process checkup rovers for each type - release when checkup is complete
    LinkedQueue<Rover*> tempQueue;
    Rover* rover = nullptr;
    
    // Check NR rovers in checkup
    while (!Checkup_NR.isEmpty())
    {
        Checkup_NR.dequeue(rover);
        if (rover->getAvailableDay() <= time)
        {
            rover->setInCheckup(false);
            Avail_NR.enqueue(rover);
        }
        else
        {
            tempQueue.enqueue(rover);
        }
    }
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(rover);
        Checkup_NR.enqueue(rover);
    }
    
    // Check PR
    while (!Checkup_PR.isEmpty())
    {
        Checkup_PR.dequeue(rover);
        if (rover->getAvailableDay() <= time)
        {
            rover->setInCheckup(false);
            Avail_PR.enqueue(rover);
        }
        else
        {
            tempQueue.enqueue(rover);
        }
    }
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(rover);
        Checkup_PR.enqueue(rover);
    }
    
    // Check DR
    while (!Checkup_DR.isEmpty())
    {
        Checkup_DR.dequeue(rover);
        if (rover->getAvailableDay() <= time)
        {
            rover->setInCheckup(false);
            Avail_DR.enqueue(rover);
        }
        else
        {
            tempQueue.enqueue(rover);
        }
    }
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(rover);
        Checkup_DR.enqueue(rover);
    }
}

void MarsStation::BacktoDone()
{
    
    priQueue<Mission*> tempQueue;
    Mission* m = nullptr;
    int pri;
    
    while (!BACK_missions.isEmpty())
    {
        BACK_missions.dequeue(m, pri);
        if (m->getFday() <= time)
        {
            Rover* r = m->getAssignedRover();
            m->setAssignedRover(nullptr);
            m->setStatus(DONE);
            DONE_missions.push(m);
            
            // Increment rover's mission count and check for checkup
            r->incrementMissions();
            if (r->needsCheckup())
            {
                r->setInCheckup(true);
                r->setAvailableDay(time + r->getCheckupDuration());
                switch (r->getType())
                {
                    case NR: Checkup_NR.enqueue(r); break;
                    case PR: Checkup_PR.enqueue(r); break;
                    case DR: Checkup_DR.enqueue(r); break;
                }
            }
            else
            {
                switch (r->getType())
                {
                    case NR: Avail_NR.enqueue(r); break;
                    case PR: Avail_PR.enqueue(r); break;
                    case DR: Avail_DR.enqueue(r); break;
                }
            }
        }
        else
        {
            tempQueue.enqueue(m, pri);
        }
    }
    
    // Restore missions still in BACK state
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(m, pri);
        BACK_missions.enqueue(m, pri);
    }
}

void MarsStation::ExectoBack()
{
    // Move missions that have completed execution to BACK state
    priQueue<Mission*> tempQueue;
    Mission* m = nullptr;
    int pri;
    
    while (!EXEC_missions.isEmpty())
    {
        EXEC_missions.dequeue(m, pri);
        Rover* r = m->getAssignedRover();
        // Execution completes after arrival + MDUR days
        int arrivalDay = -pri; // pri is negative of arrival day
        int execEndDay = arrivalDay + m->getMDUR();
        
        if (execEndDay <= time)
        {
            m->setStatus(IN_BACK);
            // Calculate finish day (arrival + MDUR + travel back)
            int travelTime = m->getTLOC() / r->getSpeed();
            m->setFday(execEndDay + travelTime);
            BACK_missions.enqueue(m, -m->getFday()); // Priority by finish day
        }
        else
        {
            tempQueue.enqueue(m, pri);
        }
    }
    
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(m, pri);
        EXEC_missions.enqueue(m, pri);
    }
}

void MarsStation::OuttoExec()
{
    priQueue<Mission*> tempQueue;
    Mission* m = nullptr;
    int pri;
    
    while (!OUT_missions.isEmpty())
    {
        OUT_missions.dequeue(m, pri);
        int arrivalDay = -pri; // pri is negative of arrival day
        
        if (arrivalDay <= time)
        {
            m->setStatus(IN_EXEC);
            EXEC_missions.enqueue(m, pri);
        }
        else
        {
            tempQueue.enqueue(m, pri);
        }
    }
    
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(m, pri);
        OUT_missions.enqueue(m, pri);
    }
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
    if (r && m)
    {
        m->calculateWdays(time);
        m->setStatus(READY);
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
    if (r && m)
    {
        m->calculateWdays(time);
        m->setStatus(READY);
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
    if (r && m)
    {
        m->calculateWdays(time);
        m->setStatus(READY);
        m->setAssignedRover(r);
        OUT_missions.enqueue(m, -m->getLocationArrivalDay(time));
    }
}

void MarsStation::abortMissionTest(int ID)
{
    if (time == 1)
    {
        for (int i = 0; i < 5; i++)
        {
            MissionType mType = NM;
            Mission* m = new Mission(i, mType, 3+i, 2000, 14+i);
            OUT_missions.enqueue(m, -(i*6));
            insertMission(m);
        }
    }
    if (time == 2)
    {
        OUT_missions.AbortMission(ID);
	    RDY_NM_list.AbortMission(ID);
    }
}

void MarsStation::abortMission(int ID)
{
    // Abort is only for Normal Missions
    // First check if mission is in Ready list
    Mission* m = RDY_NM_list.AbortMission(ID);
    if (m)
    {
        m->setStatus(ABORTED);
        Aborted_missions.enqueue(m);
        abortedMissions++;
        return;
    }
    
    // Check if mission is in OUT list (still traveling to target)
    m = OUT_missions.AbortMission(ID);
    if (m && m->getType() == NM)
    {
        // Mission is on the way - rover starts back journey immediately
        m->setStatus(ABORTED);
        Rover* r = m->getAssignedRover();
        
        // Calculate when rover will return (current position back to station)
        // Assume proportional return time based on how far along the journey
        int arrivalDay = m->getLocationArrivalDay(m->getRday() + m->getWdays());
        int elapsedDays = time - (m->getRday() + m->getWdays());
        int travelTime = m->getTLOC() / r->getSpeed();
        int returnTime = (elapsedDays < travelTime) ? elapsedDays : travelTime;
        
        m->setFday(time + returnTime);
        m->setAssignedRover(nullptr);
        
        // Rover goes to available/checkup when back
        r->incrementMissions();
        if (r->needsCheckup())
        {
            r->setInCheckup(true);
            r->setAvailableDay(time + returnTime + r->getCheckupDuration());
            switch (r->getType())
            {
                case NR: Checkup_NR.enqueue(r); break;
                case PR: Checkup_PR.enqueue(r); break;
                case DR: Checkup_DR.enqueue(r); break;
            }
        }
        else
        {
            r->setAvailableDay(time + returnTime);
            switch (r->getType())
            {
                case NR: Avail_NR.enqueue(r); break;
                case PR: Avail_PR.enqueue(r); break;
                case DR: Avail_DR.enqueue(r); break;
            }
        }
        
        Aborted_missions.enqueue(m);
        abortedMissions++;
        return;
    }
    else if (m)
    {
        // Non-NM mission in OUT list - put it back (shouldn't happen per spec)
        OUT_missions.enqueue(m, -m->getLocationArrivalDay(time));
    }
    // Otherwise ignore - mission already executing or completed
}

void MarsStation::autoAbortPM()
{
    // Auto-abort PM missions waiting more than 2x their duration
    LinkedQueue<Mission*> tempQueue;
    Mission* m = nullptr;
    
    while (!RDY_PM_list.isEmpty())
    {
        RDY_PM_list.dequeue(m);
        int waitTime = time - m->getRday();
        
        if (waitTime > 2 * m->getMDUR())
        {
            // Auto-abort this PM
            m->setStatus(ABORTED);
            Aborted_missions.enqueue(m);
            abortedMissions++;
        }
        else
        {
            tempQueue.enqueue(m);
        }
    }
    
    // Restore non-aborted missions
    while (!tempQueue.isEmpty())
    {
        tempQueue.dequeue(m);
        RDY_PM_list.enqueue(m);
    }
}

void MarsStation::writeOutputFile(string filename)
{
    ofstream output(filename);
    if (!output.is_open())
    {
        cout << "Error: Couldn't create output file: " << filename << endl;
        return;
    }
    
    // Collect all done missions (they're in a stack, so already sorted by Fday descending)
    ArrayStack<Mission*> tempStack;
    Mission* m = nullptr;
    
    // Statistics variables
    int totalDone = 0;
    int totalNM = 0, totalPM = 0, totalDM = 0;
    int doneNM = 0, donePM = 0, doneDM = 0;
    double totalWdays = 0, totalMDUR = 0, totalTdays = 0;
    int autoAbortedPM = 0;
    
    // Count mission types from aborted list
    LinkedQueue<Mission*> tempAborted;
    while (!Aborted_missions.isEmpty())
    {
        Aborted_missions.dequeue(m);
        if (m->getType() == NM) totalNM++;
        else if (m->getType() == PM) { totalPM++; autoAbortedPM++; }
        else totalDM++;
        tempAborted.enqueue(m);
    }
    while (!tempAborted.isEmpty())
    {
        tempAborted.dequeue(m);
        Aborted_missions.enqueue(m);
    }
    
    // Header
    output << "Fday\tID\tRday\tWdays\tMDUR\tTdays" << endl;
    
    // Write done missions
    while (!DONE_missions.isEmpty())
    {
        DONE_missions.pop(m);
        
        // Count by type
        if (m->getType() == NM) { totalNM++; doneNM++; }
        else if (m->getType() == PM) { totalPM++; donePM++; }
        else { totalDM++; doneDM++; }
        
        // Calculate Tdays if not set
        if (m->getTdays() == 0 && m->getAssignedRover() == nullptr)
        {
            // Tdays = Fday - Rday - Wdays
            int tdays = m->getFday() - m->getRday() - m->getWdays();
            m->setTdays(tdays);
        }
        
        output << m->getFday() << "\t" << m->getID() << "\t" 
               << m->getRday() << "\t" << m->getWdays() << "\t" 
               << m->getMDUR() << "\t" << m->getTdays() << endl;
        
        totalWdays += m->getWdays();
        totalMDUR += m->getMDUR();
        totalTdays += m->getTdays();
        totalDone++;
        
        tempStack.push(m);
    }
    
    // Restore stack
    while (!tempStack.isEmpty())
    {
        tempStack.pop(m);
        DONE_missions.push(m);
    }
    
    // Write statistics
    output << "...................................................................." << endl;
    output << "...................................................................." << endl;
    output << "Missions: " << totalMissions << "\t\t[N: " << totalNM 
           << ", P: " << totalPM << ", D: " << totalDM << "]\t[" 
           << totalDone << " DONE, " << abortedMissions << " Aborted]" << endl;
    output << "Rovers: " << (NO_OF_NRS + NO_OF_PRS + NO_OF_DRS) 
           << " [N: " << NO_OF_NRS << ", P: " << NO_OF_PRS << ", D: " << NO_OF_DRS << "]" << endl;
    
    double avgWdays = (totalDone > 0) ? totalWdays / totalDone : 0;
    double avgMDUR = (totalDone > 0) ? totalMDUR / totalDone : 0;
    double avgTdays = (totalDone > 0) ? totalTdays / totalDone : 0;
    double wdaysMdurPercent = (avgMDUR > 0) ? (avgWdays / avgMDUR) * 100 : 0;
    double autoAbortPercent = (totalPM > 0) ? ((double)autoAbortedPM / totalPM) * 100 : 0;
    
    output << "Avg Wdays = " << avgWdays << ", Avg MDUR = " << avgMDUR 
           << ", Avg Tdays = " << avgTdays << endl;
    output << "% Avg_Wdays/ Avg_MDUR = " << (int)wdaysMdurPercent 
           << "%, Auto-aborted= " << (int)autoAbortPercent << "%" << endl;
    
    output.close();
}