
#pragma once

#include <iostream>
#include <vector>

#include "../models/Asset.h"
#include "../models/LifecycleRecord.h"
#include "../models/MaintenanceRecord.h"

using namespace std;

class AssetManager {

private:

    vector<Asset> assets;

    vector<LifecycleRecord> history;

    vector<MaintenanceRecord> maintenanceRecords;

public:

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
