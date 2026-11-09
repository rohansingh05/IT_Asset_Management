#include "LifecycleRecord.h"
#include <iostream>

using namespace std;

LifecycleRecord::LifecycleRecord(
    int id, string act, string d,
    string emp, string st) {

    assetID = id;
    action = act;
    date = d;
    employee = emp;
    status = st;
}

void LifecycleRecord::displayRecord() const {

    cout << "\nAsset ID: " << assetID;
    cout << "\nAction: " << action;
    cout << "\nDate: " << date;
    cout << "\nEmployee: " << employee;
    cout << "\nStatus: " << status << endl;
}

int LifecycleRecord::getAssetID() const {
    return assetID;
}

string LifecycleRecord::getAction() const {
    return action;
}

string LifecycleRecord::getDate() const {
    return date;
}

string LifecycleRecord::getEmployee() const {
    return employee;
}

string LifecycleRecord::getStatus() const {
    return status;
}