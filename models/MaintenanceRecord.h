
#pragma once

#include <string>
using namespace std;

class MaintenanceRecord {

private:

    int assetID;
    string problem;
    string repairDate;
    double repairCost;
    string technician;
    string repairStatus;

public:

    MaintenanceRecord(int id, string issue,
                      string date, double cost,
                      string tech, string status);

    void displayRecord() const;

    int getAssetID() const;

    double getRepairCost() const;

    string getProblem() const;

    string getRepairDate() const;

    string getTechnician() const;

    string getRepairStatus() const;
};
