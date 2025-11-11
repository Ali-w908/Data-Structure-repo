#pragma once
#include "UI.h"
#include "MarsStation.h"

UI::UI(MarsStation* s)
{
	station= s;
	cout << "Choose Running Mode\nEnter S For Silent Mode\nEnter I For Interactive Mode\n";
	char input;
	cin >> input;
	while (tolower(input) != 's' && tolower(input) != 'i')
	{
		cout << "\nPlease Enter I or S\n";
		cin >> input;
	}
	switch (tolower(input))
	{
	case 's':
		mode = SILENT;
		break;
	case 'i':
		mode = INTERACTIVE;
		break;
	}
}

UIState UI::getMode()
{
	return mode;
}
void UI::printInterface()
{
	system("cls");
	cout << "Current Day:" << station->time << endl;
	cout << "==============   RequestsList   ==============\n";
	printRequests();
	cout << "==============   Ready List(s)   ==============\n";
	cout << station->RDY_DM_list.getCount() + station->RDY_PM_list.getCount() + station->RDY_NM_list.getCount() << " Missions: ";
	cout << "NMs[";
	station->RDY_NM_list.print();
	cout << "] PMs[";
	station->RDY_PM_list.print();
	cout << "] DMs[";
	station->RDY_DM_list.print();
	cout << "]" << endl;
	cout << "==============   Available Rovers List(s)   ==============\n";
	cout << station->Avail_NR.getCount() + station->Avail_PR.getCount() + station->Avail_DR.getCount() << " Rovers: ";
	cout << "NRs[";
	station->Avail_NR.print();
	cout << "] PRs[";
	station->Avail_PR.print();
	cout << "] DRs[";
	station->Avail_DR.print();
	cout << "]" << endl;
	cout << "==============   OUT List   ==============\n";
	cout << station->OUT_missions.getCount() << " Missions/Rovers: ";
	station->OUT_missions.print();
	cout << endl;
	cout << "==============   EXEC List   ==============\n";
	cout << station->EXEC_missions.getCount() << " Missions/Rovers: ";
	station->EXEC_missions.print();
	cout << endl;
	cout << "==============   BACK List   ==============\n";
	cout << station->BACK_missions.getCount() << " Missions/Rovers: ";
	station->BACK_missions.print();
	cout << endl;
	cout << "==============   Aborted List   ==============\n";
	cout << station->Aborted_missions.getCount() << " Missions: [";
	station->Aborted_missions.print();
	cout << "]" << endl;
	cout << "==============   Checkup List(s)   ==============\n";
	cout << station->Checkup_NR.getCount() + station->Checkup_PR.getCount() + station->Checkup_DR.getCount() << " Rovers: [";
	station->Checkup_NR.print();
	station->Checkup_PR.print();
	station->Checkup_DR.print();
	cout << "]" << endl;
	cout << "==============   Done List   ==============\n";
	cout << station->DONE_missions.getCount() << " Missions: [";
	station->DONE_missions.print();
	cout << "]" << endl;
	cout << "Press Any Key To Display Next Day\n";
	cin.get();
}

void UI::printRequests()
{
	cout << station->DailyRequests.getCount() << " requests remaining: ";
	LinkedQueue<Request*> temp;
	int i = 0;
	while (!station->DailyRequests.isEmpty())
	{
		Request* R = nullptr;
		station->DailyRequests.dequeue(R);
		if (!R)
			break;
		if (i < 10)
			cout << R<< ",";
		i++;
		temp.enqueue(R);
	}
	while (!temp.isEmpty())
	{
		Request* R;
		temp.dequeue(R);
		station->DailyRequests.enqueue(R);
	}
	cout << "..."<< endl;
}