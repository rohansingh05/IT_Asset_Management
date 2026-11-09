#include "../utils/InputHelper.h"
#include <map>
#include "FileManager.h"
#include "AssetManager.h"
#include <iomanip>
#include <ctime>
#include <sstream>

// Register new asset
void AssetManager::addAsset()
{

  int id;
  string type, brand, model;
  string serial, pDate, warranty;
  string location, department, employee;
  double cost;

  cout << "\n===== REGISTER NEW ASSET =====\n";

  id = readInt("Enter Asset ID: ");

  if (id <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  // Check duplicate Asset ID
  bool duplicate = false;

  for (const Asset& a : assets) {

      if (a.getAssetID() == id) {

          duplicate = true;
          break;
      }
  }

  if (duplicate) {

      cout << "Asset ID already exists!\n";
      return;
  }

  cin.ignore();

  cout << "Enter Asset Type: ";
  getline(cin, type);

  cout << "Enter Brand: ";
  getline(cin, brand);

  cout << "Enter Model: ";
  getline(cin, model);

  cout << "Enter Serial Number: ";
  getline(cin, serial);

  pDate = readDate("Enter Purchase Date: ");

  cost = readPositiveAmount("Enter Purchase Cost: ");

  cin.ignore();

  pDate = readDate("Enter Warranty End Date: ");

  cout << "Enter Location: ";
  getline(cin, location);

  cout << "Enter Department: ";
  getline(cin, department);

  cout << "Enter Employee Name (or None): ";
  getline(cin, employee);

  // Create Asset object
  Asset newAsset(
      id, type, brand, model,
      serial, pDate, cost,
      warranty, location,
      department, employee,
      "In Stock");

  // Store object in vector
  assets.push_back(newAsset);
  if (!FileManager::saveAssets(assets)) {
    cout << "Error: Asset data could not be saved!\n";
  }

  cout << "\nAsset Registered Successfully!\n";
}

void AssetManager::viewAllAssets() const
{

  cout << "\n===== ALL REGISTERED ASSETS =====\n";

  if (assets.empty())
  {
    cout << "No assets registered yet.\n";
    return;
  }

  for (const Asset &a : assets)
  {
    a.displayAsset();
  }
}

void AssetManager::searchAsset() const
{

  int searchID;

  searchID = readInt("Enter Asset ID to Search: ");

  if (searchID <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  for (const Asset &a : assets)
  {

    if (a.getAssetID() == searchID)
    {

      cout << "\nAsset Found!\n";

      a.displayAsset();

      return;
    }
  }

  cout << "\nAsset Not Found!\n";
}

// Load previously saved assets
void AssetManager::loadExistingAssets()
{

  FileManager::loadAssets(assets);

  FileManager::loadHistory(history);

  FileManager::loadMaintenance(maintenanceRecords);
}

// Allocate asset to employee
void AssetManager::allocateAsset()
{

  int id;
  string employee, date;

  id = readInt("Enter Asset ID: ");

  if (id <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  for (Asset &a : assets)
  {

    if (a.getAssetID() == id)
    {

      if (a.getStatus() != "In Stock")
      {

        cout << "Asset is not available!\n";
        return;
      }

      cin.ignore();

      cout << "Enter Employee Name: ";
      getline(cin, employee);

      date = readDate("Enter Allocation Date: ");

      if (employee.empty() || date.empty())
      {
        cout << "Invalid input!\n";
        return;
      }

      a.setAssignedEmployee(employee);
      a.setStatus("In Use");

      LifecycleRecord record(
          id,
          "Allocated",
          date,
          employee,
          "In Use");

      history.push_back(record);

      if (!FileManager::saveAssets(assets)) {

        cout << "Error: Asset data could not be saved!\n";
      }
      FileManager::saveHistory(history);

      cout << "\nAsset Allocated Successfully!\n";

      return;
    }
  }

  cout << "Asset Not Found!\n";
}

void AssetManager::transferAsset()
{

  int id;
  string newEmployee, date;

  id = readInt("Enter Asset ID: ");

  if (id <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  for (Asset &a : assets)
  {

    if (a.getAssetID() == id)
    {

      if (a.getStatus() != "In Use")
      {

        cout << "Only In Use assets can be transferred!\n";
        return;
      }

      cout << "Current Employee: "
           << a.getAssignedEmployee() << endl;

      cin.ignore();

      cout << "Enter New Employee Name: ";
      getline(cin, newEmployee);

      date = readDate("Enter Transfer Date: ");

      if (newEmployee.empty() || date.empty())
      {
        cout << "Invalid input!\n";
        return;
      }

      a.setAssignedEmployee(newEmployee);

      LifecycleRecord record(
          id,
          "Transferred",
          date,
          newEmployee,
          "In Use");

      history.push_back(record);

      if (!FileManager::saveAssets(assets)) {

          cout << "Error: Asset data could not be saved!\n";
      }
      FileManager::saveHistory(history);

      cout << "\nAsset Transferred Successfully!\n";

      return;
    }
  }

  cout << "Asset Not Found!\n";
}

void AssetManager::updateAssetStatus()
{

  int id, choice;
  string newStatus, date;

  id = readInt("Enter Asset ID: ");

  if (id <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  for (Asset &a : assets)
  {

    if (a.getAssetID() == id)
    {

      cout << "\nCurrent Status: "
           << a.getStatus() << endl;

      if (a.getStatus() == "Retired")
      {
        cout << "Retired asset cannot be updated!\n";
        return;
      }

      cout << "\n1. Send for Repair\n";
      cout << "2. Mark as Repaired\n";

      cout << "Enter Choice: ";
      cin >> choice;

      if (choice == 1 &&
          a.getStatus() == "In Use")
      {

        newStatus = "Under Repair";
      }
      else if (choice == 2 &&
               a.getStatus() == "Under Repair")
      {

        newStatus = "In Use";
      }
      else
      {

        cout << "Invalid status transition!\n";
        return;
      }

      cin.ignore();

      date = readDate("Enter Date: ");

      if (date.empty())
      {
        cout << "Invalid date!\n";
        return;
      }

      a.setStatus(newStatus);

      LifecycleRecord record(
          id,
          "Status Updated",
          date,
          a.getAssignedEmployee(),
          newStatus);

      history.push_back(record);

      if (!FileManager::saveAssets(assets)) {

        cout << "Error: Asset data could not be saved!\n";
      }
      FileManager::saveHistory(history);

      cout << "Status Updated Successfully!\n";

      return;
    }
  }

  cout << "Asset Not Found!\n";
}

void AssetManager::retireAsset()
{

  int id;
  string date;

  id = readInt("Enter Asset ID: ");

  if (id <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  for (Asset &a : assets)
  {

    if (a.getAssetID() == id)
    {

      if (a.getStatus() == "Retired")
      {

        cout << "Asset is already retired!\n";
        return;
      }

      cout << "Current Status: "
           << a.getStatus() << endl;

      cin.ignore();

      date = readDate("Enter Retirement Date: ");

      if (date.empty())
      {
        cout << "Invalid date!\n";
        return;
      }

      a.setStatus("Retired");
      a.setAssignedEmployee("None");

      LifecycleRecord record(
          id,
          "Retired",
          date,
          "None",
          "Retired");

      history.push_back(record);

      if (!FileManager::saveAssets(assets)) {

          cout << "Error: Asset data could not be saved!\n";
      }
      FileManager::saveHistory(history);

      cout << "\nAsset Retired Successfully!\n";

      return;
    }
  }

  cout << "Asset Not Found!\n";
}

void AssetManager::viewLifecycleHistory() const
{

  int id;
  bool found = false;

  id = readInt("Enter Asset ID: ");

  if (id <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  cout << "\n===== ASSET LIFECYCLE HISTORY =====\n";

  for (const LifecycleRecord &record : history)
  {

    if (record.getAssetID() == id)
    {

      record.displayRecord();

      cout << "--------------------------\n";

      found = true;
    }
  }

  if (!found)
  {
    cout << "No lifecycle history found!\n";
  }
}

void AssetManager::addMaintenanceRecord()
{

  int id;
  double cost;

  string problem;
  string date;
  string technician;
  string repairStatus;

  cout << "\n===== ADD MAINTENANCE RECORD =====\n";

  id = readInt("Enter Asset ID: ");

  if (id <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  // Find asset
  Asset *selectedAsset = nullptr;

  for (Asset &a : assets)
  {

    if (a.getAssetID() == id)
    {
      selectedAsset = &a;
      break;
    }
  }

  if (selectedAsset == nullptr)
  {
    cout << "Asset Not Found!\n";
    return;
  }

  // Check asset status
  if (selectedAsset->getStatus() != "Under Repair")
  {

    cout << "Asset must be Under Repair first!\n";
    return;
  }

  cin.ignore();

  cout << "Enter Problem: ";
  getline(cin, problem);

  date = readDate("Enter Repair Date (DD-MM-YYYY): ");

  cout << "Enter Repair Cost: ";
  cin >> cost;

  cin.ignore();

  cout << "Enter Technician Name: ";
  getline(cin, technician);

  cout << "Enter Repair Status (Pending/Completed): ";
  getline(cin, repairStatus);

  // Input validation
  if (problem.empty() || date.empty() ||
      technician.empty() || cost < 0 ||
      (repairStatus != "Pending" &&
       repairStatus != "Completed"))
  {

    cout << "Invalid maintenance details!\n";
    return;
  }

  // Create maintenance record
  MaintenanceRecord record(
      id,
      problem,
      date,
      cost,
      technician,
      repairStatus);

  maintenanceRecords.push_back(record);

  // Update asset after successful repair
  if (repairStatus == "Completed")
  {

    selectedAsset->setStatus("In Use");

    LifecycleRecord historyRecord(
        id,
        "Maintenance Completed",
        date,
        selectedAsset->getAssignedEmployee(),
        "In Use");

    history.push_back(historyRecord);

    if (!FileManager::saveAssets(assets)) {

        cout << "Error: Asset data could not be saved!\n";
    }
    FileManager::saveHistory(history);
  }

  FileManager::saveMaintenance(maintenanceRecords);

  cout << "\nMaintenance Record Added Successfully!\n";
}

void AssetManager::viewMaintenanceRecords() const
{

  cout << "\n===== ALL MAINTENANCE RECORDS =====\n";

  if (maintenanceRecords.empty())
  {

    cout << "No maintenance records found!\n";
    return;
  }

  for (const MaintenanceRecord &record : maintenanceRecords)
  {

    record.displayRecord();

    cout << "-------------------------\n";
  }
}

void AssetManager::searchMaintenanceHistory() const
{

  int id;
  bool found = false;

  id = readInt("Enter Asset ID: ");

  if (id <= 0)
  {

    cout << "Asset ID must be positive!\n";
    return;
  }

  cout << "\n===== MAINTENANCE HISTORY =====\n";

  for (const MaintenanceRecord &record : maintenanceRecords)
  {

    if (record.getAssetID() == id)
    {

      record.displayRecord();

      found = true;
    }
  }

  if (!found)
  {
    cout << "No maintenance history found!\n";
  }
}

void AssetManager::calculateMaintenanceCost() const
{

  double totalCost = 0;

  cout << "\n===== MAINTENANCE EXPENDITURE =====\n";

  if (maintenanceRecords.empty())
  {

    cout << "No maintenance expenses found.\n";
    return;
  }

  for (const MaintenanceRecord &record : maintenanceRecords)
  {

    totalCost += record.getRepairCost();
  }

  cout << fixed << setprecision(2);

  cout << "Total Maintenance Cost: Rs. "
       << totalCost << endl;
}

void AssetManager::identifyRepeatedFailures() const
{

  cout << "\n===== REPEATED FAILURE REPORT =====\n";

  bool found = false;

  for (const Asset &asset : assets)
  {

    int failureCount = 0;

    for (const MaintenanceRecord &record : maintenanceRecords)
    {

      if (record.getAssetID() == asset.getAssetID())
      {

        failureCount++;
      }
    }

    if (failureCount >= 3)
    {

      cout << "\nAsset ID: "
           << asset.getAssetID() << endl;

      cout << "Brand: "
           << asset.getBrand() << endl;

      cout << "Total Repairs: "
           << failureCount << endl;

      cout << "Warning: Repeated Failures!\n";

      found = true;
    }
  }

  if (!found)
  {

    cout << "No repeated failure assets found.\n";
  }
}

// Convert DD-MM-YYYY into a valid date
bool parseDate(const string &date, tm &result)
{

  istringstream ss(date);

  int day, month, year;
  char dash1, dash2;

  if (!(ss >> day >> dash1 >> month >> dash2 >> year))
  {
    return false;
  }

  if (dash1 != '-' || dash2 != '-' || !ss.eof())
  {
    return false;
  }

  if (year < 1900 || month < 1 || month > 12 ||
      day < 1 || day > 31)
  {
    return false;
  }

  tm parsed = {};

  parsed.tm_mday = day;
  parsed.tm_mon = month - 1;
  parsed.tm_year = year - 1900;

  parsed.tm_hour = 12;
  parsed.tm_isdst = -1;

  time_t timestamp = mktime(&parsed);

  if (timestamp == -1)
  {
    return false;
  }

  // Check that the date was not normalized
  if (parsed.tm_mday != day ||
      parsed.tm_mon != month - 1 ||
      parsed.tm_year != year - 1900)
  {
    return false;
  }

  result = parsed;

  return true;
}

void AssetManager::checkWarrantyAlerts() const
{

  time_t now = time(nullptr);

  tm currentDate = {};

  tm *current = localtime(&now);

  if (current == nullptr)
  {
    cout << "Unable to read system date.\n";
    return;
  }

  currentDate = *current;

  currentDate.tm_hour = 12;
  currentDate.tm_min = 0;
  currentDate.tm_sec = 0;
  currentDate.tm_isdst = -1;

  time_t today = mktime(&currentDate);

  bool found = false;

  cout << "\n===== WARRANTY EXPIRY REPORT =====\n";

  for (const Asset &asset : assets)
  {

    tm expiryDate = {};

    if (!parseDate(
            asset.getWarrantyEndDate(),
            expiryDate))
    {

      cout << "\nAsset ID: "
           << asset.getAssetID()
           << " - Invalid warranty date!\n";

      continue;
    }

    time_t expiry = mktime(&expiryDate);

    double seconds = difftime(expiry, today);

    int daysRemaining =
        static_cast<int>(seconds / 86400.0);

    if (daysRemaining < 0)
    {

      cout << "\nAsset ID: "
           << asset.getAssetID();

      cout << " - Warranty Expired!\n";

      found = true;
    }

    else if (daysRemaining <= 30)
    {

      cout << "\nAsset ID: "
           << asset.getAssetID();

      cout << " - Warranty expires in "
           << daysRemaining << " days!\n";

      found = true;
    }

    else if (daysRemaining <= 60)
    {

      cout << "\nAsset ID: "
           << asset.getAssetID();

      cout << " - Warranty expires within 60 days!\n";

      found = true;
    }
  }

  if (!found)
  {

    cout << "No warranty alerts found.\n";
  }
}

void AssetManager::generateTotalAssetReport() const
{

  cout << "\n===== TOTAL ASSET REPORT =====\n";

  cout << "Total Registered Assets: "
       << assets.size() << endl;

  int inStock = 0;
  int inUse = 0;
  int underRepair = 0;
  int retired = 0;

  for (const Asset &a : assets)
  {

    if (a.getStatus() == "In Stock")
    {
      inStock++;
    }

    else if (a.getStatus() == "In Use")
    {
      inUse++;
    }

    else if (a.getStatus() == "Under Repair")
    {
      underRepair++;
    }

    else if (a.getStatus() == "Retired")
    {
      retired++;
    }
  }

  cout << "\nIn Stock: " << inStock;
  cout << "\nIn Use: " << inUse;
  cout << "\nUnder Repair: " << underRepair;
  cout << "\nRetired: " << retired << endl;
}

void AssetManager::generateCategoryReport() const
{

  cout << "\n===== CATEGORY-WISE ASSET REPORT =====\n";

  map<string, int> categoryCount;

  for (const Asset &a : assets)
  {

    categoryCount[a.getAssetType()]++;
  }

  for (const auto &item : categoryCount)
  {

    cout << item.first
         << " : "
         << item.second
         << endl;
  }
}

void AssetManager::generateStatusReport() const
{

  cout << "\n===== STATUS-WISE ASSET REPORT =====\n";

  map<string, int> statusCount;

  for (const Asset &a : assets)
  {

    statusCount[a.getStatus()]++;
  }

  for (const auto &item : statusCount)
  {

    cout << item.first
         << " : "
         << item.second
         << endl;
  }
}

void AssetManager::generateDepartmentReport() const
{

  cout << "\n===== DEPARTMENT-WISE REPORT =====\n";

  map<string, int> departmentCount;

  for (const Asset &a : assets)
  {

    if (a.getStatus() != "Retired")
    {

      departmentCount[a.getDepartment()]++;
    }
  }

  for (const auto &item : departmentCount)
  {

    cout << "Department: " << item.first
         << " | Assets: " << item.second
         << endl;
  }
}

void AssetManager::generateAssetAgeReport() const
{

  cout << "\n===== ASSET AGE REPORT =====\n";

  time_t now = time(nullptr);

  tm currentDate = {};

  tm *current = localtime(&now);

  if (current == nullptr)
  {
    cout << "Unable to read current date.\n";
    return;
  }

  currentDate = *current;
  currentDate.tm_hour = 12;
  currentDate.tm_min = 0;
  currentDate.tm_sec = 0;
  currentDate.tm_isdst = -1;

  time_t today = mktime(&currentDate);

  for (const Asset &a : assets)
  {

    tm purchaseDate = {};

    if (!parseDate(a.getPurchaseDate(), purchaseDate))
    {

      cout << "Invalid purchase date for Asset ID: "
           << a.getAssetID() << endl;

      continue;
    }

    time_t purchase = mktime(&purchaseDate);

    if (purchase > today)
    {

      cout << "Future purchase date for Asset ID: "
           << a.getAssetID() << endl;

      continue;
    }

    double seconds = difftime(today, purchase);

    double ageYears = seconds / (365.25 * 86400.0);

    cout << "\nAsset ID: " << a.getAssetID();

    cout << "\nBrand: " << a.getBrand();

    cout << "\nAsset Age: "
         << fixed << setprecision(1)
         << ageYears << " years\n";
  }
}

void AssetManager::generateMaintenanceReport() const
{

  cout << "\n===== MAINTENANCE EXPENDITURE REPORT =====\n";

  double totalCost = 0;

  map<int, double> assetExpenses;

  for (const MaintenanceRecord &record : maintenanceRecords)
  {

    totalCost += record.getRepairCost();

    assetExpenses[record.getAssetID()] +=
        record.getRepairCost();
  }

  cout << fixed << setprecision(2);

  cout << "\nTotal Maintenance Expenditure: Rs. "
       << totalCost << endl;

  cout << "\nAsset-wise Maintenance Cost:\n";

  for (const auto &item : assetExpenses)
  {

    cout << "Asset ID: " << item.first
         << " | Cost: Rs. " << item.second
         << endl;
  }
}

void AssetManager::generateRetiredAssetReport() const
{

  cout << "\n===== RETIRED ASSET REPORT =====\n";

  int count = 0;

  for (const Asset &a : assets)
  {

    if (a.getStatus() == "Retired")
    {

      a.displayAsset();

      count++;
    }
  }

  cout << "\nTotal Retired Assets: "
       << count << endl;
}

void AssetManager::generateReplacementReport() const
{

  cout << "\n===== REPLACEMENT RECOMMENDATION =====\n";

  time_t now = time(nullptr);

  tm currentDate = {};

  tm *current = localtime(&now);

  if (current == nullptr)
  {
    cout << "Unable to read current date.\n";
    return;
  }

  currentDate = *current;
  currentDate.tm_hour = 12;
  currentDate.tm_min = 0;
  currentDate.tm_sec = 0;
  currentDate.tm_isdst = -1;

  time_t today = mktime(&currentDate);

  bool found = false;

  for (const Asset &a : assets)
  {

    if (a.getStatus() == "Retired")
    {
      continue;
    }

    tm purchaseDate = {};

    if (!parseDate(a.getPurchaseDate(), purchaseDate))
    {
      continue;
    }

    time_t purchase = mktime(&purchaseDate);

    if (purchase > today)
    {
      continue;
    }

    double seconds = difftime(today, purchase);

    double ageYears = seconds / (365.25 * 86400.0);

    double repairCost = 0;

    for (const MaintenanceRecord &record : maintenanceRecords)
    {

      if (record.getAssetID() == a.getAssetID())
      {

        repairCost += record.getRepairCost();
      }
    }

    double costThreshold = a.getPurchaseCost() * 0.30;

    if (ageYears >= 5 || repairCost >= costThreshold)
    {

      cout << "\nAsset ID: " << a.getAssetID();

      cout << "\nBrand: " << a.getBrand();

      cout << "\nAge: "
           << fixed << setprecision(1)
           << ageYears << " years";

      cout << "\nPurchase Cost: Rs. "
           << a.getPurchaseCost();

      cout << "\nMaintenance Cost: Rs. "
           << repairCost;

      cout << "\nRecommendation: REVIEW FOR REPLACEMENT\n";

      found = true;
    }
  }

  if (!found)
  {

    cout << "No replacement candidates found.\n";
  }
}

void AssetManager::showAnalyticsDashboard() const
{

  cout << "\n====================================\n";
  cout << "       IT ASSET ANALYTICS\n";
  cout << "====================================\n";

  cout << "\nTotal Assets: " << assets.size() << endl;

  int inStock = 0;
  int inUse = 0;
  int underRepair = 0;
  int retired = 0;

  double totalMaintenance = 0;

  for (const Asset &a : assets)
  {

    if (a.getStatus() == "In Stock")
      inStock++;

    else if (a.getStatus() == "In Use")
      inUse++;

    else if (a.getStatus() == "Under Repair")
      underRepair++;

    else if (a.getStatus() == "Retired")
      retired++;
  }

  for (const MaintenanceRecord &r : maintenanceRecords)
  {

    totalMaintenance += r.getRepairCost();
  }

  cout << "\n--- ASSET STATUS ---\n";

  cout << "In Stock: " << inStock << endl;

  cout << "In Use: " << inUse << endl;

  cout << "Under Repair: " << underRepair << endl;

  cout << "Retired: " << retired << endl;

  cout << fixed << setprecision(2);

  cout << "\nTotal Maintenance Expenditure: Rs. "
       << totalMaintenance << endl;

  cout << "\nTotal Maintenance Records: "
       << maintenanceRecords.size() << endl;

  cout << "\n====================================\n";
}
