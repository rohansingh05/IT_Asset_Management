
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "../models/Asset.h"
#include "../models/LifecycleRecord.h"
#include "../models/MaintenanceRecord.h"

using namespace std;

class FileManager {

public:

    static bool saveAssets(const vector<Asset>& assets);

    static void loadAssets(vector<Asset>& assets);

    static void saveHistory(const vector<LifecycleRecord>& history);

    static void loadHistory(vector<LifecycleRecord>& history);

    static void saveMaintenance(const vector<MaintenanceRecord>& records);

    static void loadMaintenance(vector<MaintenanceRecord>& records);

};
