#include <iostream>
#include "MarsStation.h"
using namespace std;

int main() {
    int numNR, numPR, numDR;
    int spNR, spPR, spDR;
    int M;
    int checkupNR, checkupPR, checkupDR;

    // 1️⃣ إدخال بيانات الروفرات
    cout << "Enter number of Normal, Polar, Drill Rovers: ";
    cin >> numNR >> numPR >> numDR;

    cout << "Enter speed of Normal, Polar, Drill Rovers: ";
    cin >> spNR >> spPR >> spDR;

    cout << "Enter missions before checkup (M): ";
    cin >> M;

    cout << "Enter checkup duration for Normal, Polar, Drill Rovers: ";
    cin >> checkupNR >> checkupPR >> checkupDR;

    // إنشاء MarsStation
    MarsStation station(numNR, numPR, numDR, spNR, spPR, spDR, M,
        checkupDR, checkupPR, checkupNR);

    // 2️⃣ إدخال عدد الطلبات قبل التشغيل
    int numRequests;
    cout << "Enter total number of requests for the simulation: ";
    cin >> numRequests;
    station.setNoOfRequests(numRequests);

    cout << "\nSimulation Started. Enter R (request) or X (abort) commands.\n";

    // 3️⃣ حلقة استقبال الأوامر من المستخدم
    int requestCounter = 0;
    while (requestCounter < station.getNoOfRequests()) {
        cout << "\n--- Command " << (requestCounter + 1) << " of "
            << station.getNoOfRequests() << " ---\n";

        station.readUserCommand();              // المستخدم يدخل R أو X
        station.checkRoversForCheckup();        // فحص الروفرات اللي وصلت للـ M
        station.checkMissionsForAbort(requestCounter); // فحص مهمات Polar الطويلة
        station.assignReadyMissions();          // توزيع المهمات على الروفرات المتاحة

        requestCounter++;
    }

    cout << "\nAll requests processed. Simulation ended.\n";

    return 0;
}
