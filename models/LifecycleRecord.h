#pragma once

#include <string>
using namespace std;

class LifecycleRecord {

private:
    int assetID;
    string action;
    string date;
    string employee;
    string status;

public:

    LifecycleRecord(int id, string act,
                    string d, string emp,
                    string st);

    void displayRecord() const;

    int getAssetID() const;

    string getAction() const;
    string getDate() const;
    string getEmployee() const;
    string getStatus() const;
};