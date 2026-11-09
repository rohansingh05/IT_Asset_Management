
#pragma once

#include <iostream>
#include <string>
using namespace std;

class Asset {
private:
    int assetID;
    string assetType;
    string brand;
    string model;
    string serialNumber;
    string purchaseDate;
    double purchaseCost;
    string warrantyEndDate;
    string location;
    string department;
    string assignedEmployee;
    string status;

public:
    Asset(int id, string type, string b, string m,
          string serial, string pDate, double cost,
          string warranty, string loc, string dept,
          string employee, string currentStatus);

    void displayAsset() const;

    
    int getAssetID() const;
    string getAssetType() const;
    string getBrand() const;
    string getModel() const;
    string getSerialNumber() const;
    string getPurchaseDate() const;
    double getPurchaseCost() const;
    string getWarrantyEndDate() const;
    string getLocation() const;
    string getDepartment() const;
    string getAssignedEmployee() const;
    string getStatus() const;
    void setStatus(string newStatus);
    void setAssignedEmployee(string employee);

};
