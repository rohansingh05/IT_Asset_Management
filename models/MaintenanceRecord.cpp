
#include "MaintenanceRecord.h"
#include <iostream>

using namespace std;

MaintenanceRecord::MaintenanceRecord(
    int id,
    string issue,
    string date,
    double cost,
    string tech,
    string status) {

    assetID = id;
    problem = issue;
    repairDate = date;
    repairCost = cost;
    technician = tech;
    repairStatus = status;
}

void MaintenanceRecord::displayRecord() const {

    cout << "\n----- MAINTENANCE DETAILS -----\n";

    cout << "Asset ID: " << assetID << endl;
    cout << "Problem: " << problem << endl;
    cout << "Repair Date: " << repairDate << endl;
    cout << "Repair Cost: Rs. " << repairCost << endl;
    cout << "Technician: " << technician << endl;
    cout << "Repair Status: " << repairStatus << endl;
}

int MaintenanceRecord::getAssetID() const {
    return assetID;
}

double MaintenanceRecord::getRepairCost() const {
    return repairCost;
}

string MaintenanceRecord::getProblem() const {
    return problem;
}

string MaintenanceRecord::getRepairDate() const {
    return repairDate;
}

string MaintenanceRecord::getTechnician() const {
    return technician;
}

string MaintenanceRecord::getRepairStatus() const {
    return repairStatus;
}
