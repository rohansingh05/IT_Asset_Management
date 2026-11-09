
#include "Asset.h"

// Constructor definition
Asset::Asset(int id, string type, string b, string m,
             string serial, string pDate, double cost,
             string warranty, string loc, string dept,
             string employee, string currentStatus) {

    assetID = id;
    assetType = type;
    brand = b;
    model = m;
    serialNumber = serial;
    purchaseDate = pDate;
    purchaseCost = cost;
    warrantyEndDate = warranty;
    location = loc;
    department = dept;
    assignedEmployee = employee;
    status = currentStatus;
}

// Display asset details
void Asset::displayAsset() const {

    cout << "\n------ ASSET DETAILS ------\n";

    cout << "Asset ID: " << assetID << endl;
    cout << "Asset Type: " << assetType << endl;
    cout << "Brand: " << brand << endl;
    cout << "Model: " << model << endl;
    cout << "Serial Number: " << serialNumber << endl;
    cout << "Purchase Date: " << purchaseDate << endl;
    cout << "Purchase Cost: Rs. " << purchaseCost << endl;
    cout << "Warranty End Date: " << warrantyEndDate << endl;
    cout << "Location: " << location << endl;
    cout << "Department: " << department << endl;
    cout << "Employee: " << assignedEmployee << endl;
    cout << "Status: " << status << endl;
}

// Return asset ID

int Asset::getAssetID() const {
    return assetID;
}

string Asset::getAssetType() const {
    return assetType;
}

string Asset::getBrand() const {
    return brand;
}

string Asset::getModel() const {
    return model;
}

string Asset::getSerialNumber() const {
    return serialNumber;
}

string Asset::getPurchaseDate() const {
    return purchaseDate;
}

double Asset::getPurchaseCost() const {
    return purchaseCost;
}

string Asset::getWarrantyEndDate() const {
    return warrantyEndDate;
}

string Asset::getLocation() const {
    return location;
}

string Asset::getDepartment() const {
    return department;
}

string Asset::getAssignedEmployee() const {
    return assignedEmployee;
}

string Asset::getStatus() const {
    return status;
}

void Asset::setStatus(string newStatus) {
    status = newStatus;
}

void Asset::setAssignedEmployee(string employee) {
    assignedEmployee = employee;
}