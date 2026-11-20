#include <fstream>
#include <sstream>
#include "FileManager.h"
#include <iomanip>

// Save all assets into CSV file
bool FileManager::saveAssets(const vector<Asset>& assets) {

    ofstream file("data/assets.csv");

    if (!file.is_open()) {
        return false;
    }

    for (const Asset& asset : assets) {

        file << asset.getAssetID() << ","
             << asset.getAssetType() << ","
             << asset.getBrand() << ","
             << asset.getModel() << ","
             << asset.getSerialNumber() << ","
             << asset.getPurchaseDate() << ","
             << asset.getPurchaseCost() << ","
             << asset.getWarrantyEndDate() << ","
             << asset.getLocation() << ","
             << asset.getDepartment() << ","
             << asset.getAssignedEmployee() << ","
             << asset.getStatus() << "\n";

        if (!file.good()) {
            file.close();
            return false;
        }
    }

    file.close();

    return !file.fail();
}




void FileManager::loadAssets(vector<Asset>& assets) {

    ifstream file("data/assets.csv");

    if (!file.is_open()) {
        cout << "No previous asset records found.\n";
        return;
    }

    assets.clear();

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string id, type, brand, model;
        string serial, pDate, cost;
        string warranty, location, department;
        string employee, status;

        getline(ss, id, ',');
        getline(ss, type, ',');
        getline(ss, brand, ',');
        getline(ss, model, ',');
        getline(ss, serial, ',');
        getline(ss, pDate, ',');
        getline(ss, cost, ',');
        getline(ss, warranty, ',');
        getline(ss, location, ',');
        getline(ss, department, ',');
        getline(ss, employee, ',');
        getline(ss, status, ',');

        try {

            Asset a(
                stoi(id),
                type,
                brand,
                model,
                serial,
                pDate,
                stod(cost),
                warranty,
                location,
                department,
                employee,
                status
            );

            assets.push_back(a);

        }
        catch (const exception& e) {

            cout << "Invalid record skipped: " << line << endl;
        }
    }

    file.close();

    cout << assets.size()
         << " assets loaded successfully!\n";
}


// Save lifecycle history
bool FileManager::saveHistory(
    const vector<LifecycleRecord>& history
) {

    ofstream file("data/lifecycle.csv");

    if (!file.is_open()) {
        return false;
    }

    for (const LifecycleRecord& record : history) {

        file << record.getAssetID() << ","
             << record.getAction() << ","
             << record.getDate() << ","
             << record.getEmployee() << ","
             << record.getStatus() << "\n";

        if (!file.good()) {
            file.close();
            return false;
        }
    }

    file.close();

    return !file.fail();
}

// Load lifecycle history
void FileManager::loadHistory(
    vector<LifecycleRecord>& history) {

    ifstream file("data/lifecycle.csv");

    if (!file.is_open()) {
        cout << "No previous lifecycle history found.\n";
        return;
    }

    history.clear();

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string id, action, date;
        string employee, status;

        getline(ss, id, ',');
        getline(ss, action, ',');
        getline(ss, date, ',');
        getline(ss, employee, ',');
        getline(ss, status);

        try {

            LifecycleRecord record(
                stoi(id),
                action,
                date,
                employee,
                status
            );

            history.push_back(record);
        }
        catch (const exception& e) {

            cout << "Invalid history record skipped.\n";
        }
    }

    file.close();

    cout << history.size()
         << " lifecycle records loaded!\n";
}


// Save maintenance records
bool FileManager::saveMaintenance(
    const vector<MaintenanceRecord>& records
) {

    ofstream file("data/maintenance.csv");

    if (!file.is_open()) {
        return false;
    }

    for (const MaintenanceRecord& record : records) {

        file << record.getAssetID() << ","
             << record.getProblem() << ","
             << record.getRepairDate() << ","
             << record.getRepairCost() << ","
             << record.getTechnician() << ","
             << record.getRepairStatus() << "\n";

        if (!file.good()) {
            file.close();
            return false;
        }
    }

    file.close();

    return !file.fail();
}

// Load maintenance records
void FileManager::loadMaintenance(
    vector<MaintenanceRecord>& records) {

    ifstream file("data/maintenance.csv");

    if (!file.is_open()) {
        cout << "No previous maintenance records found.\n";
        return;
    }

    records.clear();

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string id, problem, date;
        string cost, technician, status;

        getline(ss, id, ',');
        getline(ss, problem, ',');
        getline(ss, date, ',');
        getline(ss, cost, ',');
        getline(ss, technician, ',');
        getline(ss, status);

        try {

            MaintenanceRecord record(
                stoi(id),
                problem,
                date,
                stod(cost),
                technician,
                status
            );

            records.push_back(record);
        }
        catch (const exception& e) {

            cout << "Invalid maintenance record skipped.\n";
        }
    }

    file.close();

    cout << records.size()
         << " maintenance records loaded!\n";
}

