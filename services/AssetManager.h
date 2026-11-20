
#pragma once

#include <iostream>
#include <vector>

#include "../models/Asset.h"
#include "../models/LifecycleRecord.h"
#include "../models/MaintenanceRecord.h"

using namespace std;

class AssetManager
{

private:
    int nextAssetID = 1001;

    vector<Asset> assets;

    vector<LifecycleRecord> history;

    vector<MaintenanceRecord> maintenanceRecords;

public:
    bool registerAsset(
        const string &assetType,
        const string &brand,
        const string &model,
        const string &serialNumber,
        const string &purchaseDate,
        double purchaseCost,
        const string &warrantyEndDate,
        const string &location,
        const string &department,
        const string &assignedEmployee,
        int &generatedAssetID,
        string &errorMessage);

    const vector<Asset> &getAssets() const;

    // Web API support
    bool allocateAssetForWeb(
        int assetID,
        const string &employee,
        const string &date,
        string &errorMessage);

    bool transferAssetForWeb(
        int assetID,
        const string &newEmployee,
        const string &date,
        string &errorMessage);

    const vector<LifecycleRecord> &getLifecycleHistory() const;

    bool addMaintenanceForWeb(
        int assetID,
        const string &problem,
        const string &repairDate,
        double repairCost,
        const string &technician,
        const string &repairStatus,
        string &errorMessage);

    const vector<MaintenanceRecord> &getMaintenanceRecords() const;

    void addAsset();

    void viewAllAssets() const;

    void searchAsset() const;

    void loadExistingAssets();

    void allocateAsset();

    void transferAsset();

    void updateAssetStatus();

    void retireAsset();

    void viewLifecycleHistory() const;

    void addMaintenanceRecord();

    void viewMaintenanceRecords() const;

    void searchMaintenanceHistory() const;

    void calculateMaintenanceCost() const;

    void identifyRepeatedFailures() const;

    void checkWarrantyAlerts() const;

    void generateTotalAssetReport() const;

    void generateCategoryReport() const;

    void generateStatusReport() const;

    void generateDepartmentReport() const;

    void generateAssetAgeReport() const;

    void generateMaintenanceReport() const;

    void generateRetiredAssetReport() const;

    void generateReplacementReport() const;

    void showAnalyticsDashboard() const;
};
