#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT 0x0A00
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <cmath>
#include <sstream>

#include "libs/httplib.h"
#include "libs/nlohmann/json.hpp"

#include "services/AssetManager.h"
#include "models/MaintenanceRecord.h"

using namespace std;
using json = nlohmann::json;

// Allow requests from the local VS Code Live Server
void setCorsHeaders(
    const httplib::Request &req,
    httplib::Response &res)
{

    string origin = req.get_header_value("Origin");

    if (origin == "http://127.0.0.1:5500" ||
        origin == "http://localhost:5500")
    {

        res.set_header("Access-Control-Allow-Origin", origin);
    }

    res.set_header("Vary", "Origin");

    res.set_header(
        "Access-Control-Allow-Methods",
        "GET, POST, OPTIONS");

    res.set_header(
        "Access-Control-Allow-Headers",
        "Content-Type");
}

// Read a required string from JSON
string getRequiredString(
    const json &data,
    const string &key)
{

    if (!data.contains(key) || !data.at(key).is_string())
    {
        throw invalid_argument("Missing or invalid field: " + key);
    }

    string value = data.at(key).get<string>();

    if (value.empty())
    {
        throw invalid_argument("Field cannot be empty: " + key);
    }

    return value;
}

int getRequiredAssetID(const json &data)
{

    if (!data.contains("assetID") ||
        !data.at("assetID").is_number_integer())
    {

        throw invalid_argument("Invalid Asset ID.");
    }

    long long id = data.at("assetID").get<long long>();

    if (id <= 0 || id > 2147483647LL)
    {
        throw invalid_argument("Asset ID is out of range.");
    }

    return static_cast<int>(id);
}

// Convert DD-MM-YYYY into a local calendar date
bool parseWarrantyDate(
    const string &date,
    time_t &result)
{

    istringstream input(date);

    int day, month, year;
    char separator1, separator2;

    if (!(input >> day >> separator1 >> month >> separator2 >> year))
    {
        return false;
    }

    if (separator1 != '-' || separator2 != '-')
    {
        return false;
    }

    input >> ws;

    if (!input.eof())
    {
        return false;
    }

    tm dateInfo{};

    dateInfo.tm_mday = day;
    dateInfo.tm_mon = month - 1;
    dateInfo.tm_year = year - 1900;
    dateInfo.tm_hour = 12;
    dateInfo.tm_isdst = -1;

    time_t timestamp = mktime(&dateInfo);

    if (timestamp == -1)
    {
        return false;
    }

    // Reject invalid dates such as 31-02-2027
    if (dateInfo.tm_mday != day ||
        dateInfo.tm_mon != month - 1 ||
        dateInfo.tm_year != year - 1900)
    {

        return false;
    }

    result = timestamp;
    return true;
}

// Get today's date at local noon
time_t getTodayAtNoon()
{

    time_t now = time(nullptr);

    tm currentDate{};

#ifdef _WIN32
    localtime_s(&currentDate, &now);
#else
    localtime_r(&now, &currentDate);
#endif

    currentDate.tm_hour = 12;
    currentDate.tm_min = 0;
    currentDate.tm_sec = 0;
    currentDate.tm_isdst = -1;

    return mktime(&currentDate);
}

int main()
{

    cout << "Starting IT Asset Management API...\n";

    // Create backend manager
    AssetManager manager;

    // Load existing CSV records and calculate next Asset ID
    manager.loadExistingAssets();

    httplib::Server server;

    // ----------------------------------
    // OPTIONS: Browser CORS preflight
    // ----------------------------------

    server.Options("/api/assets",
                   [](const httplib::Request &req, httplib::Response &res)
                   {
                       setCorsHeaders(req, res);
                       res.status = 204;
                   });

    // ----------------------------------
    // GET: Check backend connection
    // ----------------------------------

    server.Get("/api/health",
               [](const httplib::Request &req, httplib::Response &res)
               {
                   setCorsHeaders(req, res);

                   json response = {
                       {"status", "ok"},
                       {"message", "C++ backend is running"}};

                   res.set_content(
                       response.dump(),
                       "application/json");
               });

    // ----------------------------------
    // GET: Retrieve all assets
    // ----------------------------------

    server.Get("/api/assets",
               [&manager](
                   const httplib::Request &req,
                   httplib::Response &res)
               {
                   setCorsHeaders(req, res);

                   json assetList = json::array();

                   for (const Asset &asset : manager.getAssets())
                   {

                       assetList.push_back({{"assetID", asset.getAssetID()},
                                            {"assetType", asset.getAssetType()},
                                            {"brand", asset.getBrand()},
                                            {"model", asset.getModel()},
                                            {"serialNumber", asset.getSerialNumber()},
                                            {"purchaseDate", asset.getPurchaseDate()},
                                            {"purchaseCost", asset.getPurchaseCost()},
                                            {"warrantyEndDate", asset.getWarrantyEndDate()},
                                            {"location", asset.getLocation()},
                                            {"department", asset.getDepartment()},
                                            {"assignedEmployee", asset.getAssignedEmployee()},
                                            {"status", asset.getStatus()}});
                   }

                   res.set_content(
                       assetList.dump(),
                       "application/json");
               });

    // ----------------------------------
    // POST: Register a new asset
    // ----------------------------------

    server.Post("/api/assets",
                [&manager](
                    const httplib::Request &req,
                    httplib::Response &res)
                {
                    setCorsHeaders(req, res);

                    try
                    {

                        // Parse incoming JSON
                        json data = json::parse(req.body);

                        if (!data.is_object())
                        {
                            throw invalid_argument(
                                "Request body must be a JSON object.");
                        }

                        // Read registration details
                        string assetType =
                            getRequiredString(data, "assetType");

                        string brand =
                            getRequiredString(data, "brand");

                        string model =
                            getRequiredString(data, "model");

                        string serialNumber =
                            getRequiredString(data, "serialNumber");

                        string purchaseDate =
                            getRequiredString(data, "purchaseDate");

                        string warrantyEndDate =
                            getRequiredString(data, "warrantyEndDate");

                        string location =
                            getRequiredString(data, "location");

                        string department =
                            getRequiredString(data, "department");

                        // Employee assignment is optional
                        string assignedEmployee = "None";

                        if (data.contains("assignedEmployee") &&
                            data.at("assignedEmployee").is_string())
                        {

                            assignedEmployee =
                                data.at("assignedEmployee").get<string>();

                            if (assignedEmployee.empty())
                            {
                                assignedEmployee = "None";
                            }
                        }

                        // Validate purchase cost type
                        if (!data.contains("purchaseCost") ||
                            !data.at("purchaseCost").is_number())
                        {

                            throw invalid_argument(
                                "Purchase cost must be a number.");
                        }

                        double purchaseCost =
                            data.at("purchaseCost").get<double>();

                        if (!std::isfinite(purchaseCost) ||
                            purchaseCost < 0)
                        {

                            throw invalid_argument(
                                "Purchase cost must be non-negative.");
                        }

                        // Ask C++ backend to register the asset
                        // The ID is generated inside AssetManager.
                        int generatedID = 0;
                        string errorMessage;

                        bool success = manager.registerAsset(
                            assetType,
                            brand,
                            model,
                            serialNumber,
                            purchaseDate,
                            purchaseCost,
                            warrantyEndDate,
                            location,
                            department,
                            assignedEmployee,
                            generatedID,
                            errorMessage);

                        if (!success)
                        {

                            res.status = 400;

                            json errorResponse = {
                                {"success", false},
                                {"message", errorMessage}};

                            res.set_content(
                                errorResponse.dump(),
                                "application/json");

                            return;
                        }

                        // Successful response
                        res.status = 201;

                        json successResponse = {
                            {"success", true},
                            {"message", "Asset registered successfully."},
                            {"assetID", generatedID}};

                        res.set_content(
                            successResponse.dump(),
                            "application/json");

                        cout << "New asset registered. ID: "
                             << generatedID << "\n";
                    }
                    catch (const exception &e)
                    {

                        res.status = 400;

                        json errorResponse = {
                            {"success", false},
                            {"message", e.what()}};

                        res.set_content(
                            errorResponse.dump(),
                            "application/json");
                    }
                });

    server.Options("/api/assets/allocate",
                   [](const httplib::Request &req, httplib::Response &res)
                   {
                       setCorsHeaders(req, res);
                       res.status = 204;
                   });

    server.Options("/api/assets/transfer",
                   [](const httplib::Request &req, httplib::Response &res)
                   {
                       setCorsHeaders(req, res);
                       res.status = 204;
                   });

    server.Options("/api/lifecycle",
                   [](const httplib::Request &req, httplib::Response &res)
                   {
                       setCorsHeaders(req, res);
                       res.status = 204;
                   });

    // ----------------------------------
    // POST: Allocate an asset
    // ----------------------------------

    server.Post("/api/assets/allocate",
                [&manager](
                    const httplib::Request &req,
                    httplib::Response &res)
                {
                    setCorsHeaders(req, res);

                    try
                    {

                        json data = json::parse(req.body);

                        if (!data.is_object())
                        {
                            throw invalid_argument("Invalid request body.");
                        }

                        int assetID = getRequiredAssetID(data);

                        string employee =
                            getRequiredString(data, "employee");

                        string date =
                            getRequiredString(data, "date");

                        string errorMessage;

                        bool success = manager.allocateAssetForWeb(
                            assetID,
                            employee,
                            date,
                            errorMessage);

                        if (!success)
                        {

                            res.status = 400;

                            res.set_content(
                                json({{"success", false},
                                      {"message", errorMessage}})
                                    .dump(),
                                "application/json");

                            return;
                        }

                        res.status = 200;

                        res.set_content(
                            json({{"success", true},
                                  {"message", "Asset allocated successfully."},
                                  {"assetID", assetID},
                                  {"employee", employee}})
                                .dump(),
                            "application/json");
                    }
                    catch (const exception &e)
                    {

                        res.status = 400;

                        res.set_content(
                            json({{"success", false},
                                  {"message", e.what()}})
                                .dump(),
                            "application/json");
                    }
                });
    // ----------------------------------
    // POST: Transfer an asset
    // ----------------------------------

    server.Post("/api/assets/transfer",
                [&manager](
                    const httplib::Request &req,
                    httplib::Response &res)
                {
                    setCorsHeaders(req, res);

                    try
                    {

                        json data = json::parse(req.body);

                        if (!data.is_object())
                        {
                            throw invalid_argument("Invalid request body.");
                        }

                        int assetID = getRequiredAssetID(data);

                        string newEmployee =
                            getRequiredString(data, "newEmployee");

                        string date =
                            getRequiredString(data, "date");

                        string errorMessage;

                        bool success = manager.transferAssetForWeb(
                            assetID,
                            newEmployee,
                            date,
                            errorMessage);

                        if (!success)
                        {

                            res.status = 400;

                            res.set_content(
                                json({{"success", false},
                                      {"message", errorMessage}})
                                    .dump(),
                                "application/json");

                            return;
                        }

                        res.status = 200;

                        res.set_content(
                            json({{"success", true},
                                  {"message", "Asset transferred successfully."},
                                  {"assetID", assetID},
                                  {"newEmployee", newEmployee}})
                                .dump(),
                            "application/json");
                    }
                    catch (const exception &e)
                    {

                        res.status = 400;

                        res.set_content(
                            json({{"success", false},
                                  {"message", e.what()}})
                                .dump(),
                            "application/json");
                    }
                });
    // ----------------------------------
    // GET: View lifecycle history
    // ----------------------------------

    server.Get("/api/lifecycle",
               [&manager](
                   const httplib::Request &req,
                   httplib::Response &res)
               {
                   setCorsHeaders(req, res);

                   json historyList = json::array();

                   for (const LifecycleRecord &record :
                        manager.getLifecycleHistory())
                   {

                       historyList.push_back({{"assetID", record.getAssetID()},
                                              {"action", record.getAction()},
                                              {"date", record.getDate()},
                                              {"employee", record.getEmployee()},
                                              {"status", record.getStatus()}});
                   }

                   res.set_content(
                       historyList.dump(),
                       "application/json");
               });

    server.Options("/api/maintenance",
                   [](const httplib::Request &req, httplib::Response &res)
                   {
                       setCorsHeaders(req, res);

                       res.status = 204;
                   });

    // ----------------------------------
    // GET: Retrieve maintenance records
    // ----------------------------------

    server.Get("/api/maintenance",
               [&manager](
                   const httplib::Request &req,
                   httplib::Response &res)
               {
                   setCorsHeaders(req, res);

                   json maintenanceList = json::array();

                   for (const MaintenanceRecord &record :
                        manager.getMaintenanceRecords())
                   {

                       maintenanceList.push_back({{"assetID", record.getAssetID()},
                                                  {"problem", record.getProblem()},
                                                  {"repairDate", record.getRepairDate()},
                                                  {"repairCost", record.getRepairCost()},
                                                  {"technician", record.getTechnician()},
                                                  {"repairStatus", record.getRepairStatus()}});
                   }

                   res.set_content(
                       maintenanceList.dump(),
                       "application/json");
               });

    // ----------------------------------
    // POST: Add maintenance record
    // ----------------------------------

    server.Post("/api/maintenance",
                [&manager](
                    const httplib::Request &req,
                    httplib::Response &res)
                {
                    setCorsHeaders(req, res);

                    try
                    {

                        json data = json::parse(req.body);

                        if (!data.is_object())
                        {
                            throw invalid_argument("Invalid request body.");
                        }

                        int assetID = getRequiredAssetID(data);

                        string problem =
                            getRequiredString(data, "problem");

                        string repairDate =
                            getRequiredString(data, "repairDate");

                        string technician =
                            getRequiredString(data, "technician");

                        string repairStatus =
                            getRequiredString(data, "repairStatus");

                        if (!data.contains("repairCost") ||
                            !data.at("repairCost").is_number())
                        {

                            throw invalid_argument(
                                "Repair cost must be a number.");
                        }

                        double repairCost =
                            data.at("repairCost").get<double>();

                        if (!std::isfinite(repairCost) || repairCost < 0)
                        {
                            throw invalid_argument(
                                "Repair cost must be non-negative.");
                        }

                        string errorMessage;

                        bool success = manager.addMaintenanceForWeb(
                            assetID,
                            problem,
                            repairDate,
                            repairCost,
                            technician,
                            repairStatus,
                            errorMessage);

                        if (!success)
                        {

                            res.status = 400;

                            res.set_content(
                                json({{"success", false},
                                      {"message", errorMessage}})
                                    .dump(),
                                "application/json");

                            return;
                        }

                        res.status = 201;

                        res.set_content(
                            json({{"success", true},
                                  {"message", "Maintenance record saved successfully."},
                                  {"assetID", assetID},
                                  {"repairStatus", repairStatus}})
                                .dump(),
                            "application/json");

                        cout << "Maintenance record saved for Asset ID: "
                             << assetID << "\n";
                    }
                    catch (const exception &e)
                    {

                        res.status = 400;

                        res.set_content(
                            json({{"success", false},
                                  {"message", e.what()}})
                                .dump(),
                            "application/json");
                    }
                });

    server.Options("/api/warranty-alerts",
                   [](const httplib::Request &req, httplib::Response &res)
                   {
                       setCorsHeaders(req, res);

                       res.status = 204;
                   });

    // ----------------------------------
    // GET: Warranty Expiry Alerts
    // ----------------------------------

    server.Get("/api/warranty-alerts",
               [&manager](
                   const httplib::Request &req,
                   httplib::Response &res)
               {
                   setCorsHeaders(req, res);

                   json alerts = json::array();

                   time_t today = getTodayAtNoon();

                   for (const Asset &asset : manager.getAssets())
                   {

                       // Retired assets do not require active warranty alerts
                       if (asset.getStatus() == "Retired")
                       {
                           continue;
                       }

                       time_t warrantyDate;

                       // Skip invalid warranty dates
                       if (!parseWarrantyDate(
                               asset.getWarrantyEndDate(),
                               warrantyDate))
                       {

                           continue;
                       }

                       // Calculate calendar days remaining
                       int daysRemaining = static_cast<int>(
                           std::lround(
                               std::difftime(warrantyDate, today) / 86400.0));

                       // Only return expired or upcoming warranties
                       if (daysRemaining > 60)
                       {
                           continue;
                       }

                       string warrantyStatus;

                       if (daysRemaining < 0)
                       {
                           warrantyStatus = "Expired";
                       }
                       else if (daysRemaining <= 30)
                       {
                           warrantyStatus = "Within 30 Days";
                       }
                       else
                       {
                           warrantyStatus = "Within 60 Days";
                       }

                       alerts.push_back({

                           {"assetID", asset.getAssetID()},

                           {"assetType", asset.getAssetType()},

                           {"brand", asset.getBrand()},

                           {"model", asset.getModel()},

                           {"serialNumber", asset.getSerialNumber()},

                           {"purchaseDate", asset.getPurchaseDate()},

                           {"purchaseCost", asset.getPurchaseCost()},

                           {"warrantyEndDate", asset.getWarrantyEndDate()},

                           {"location", asset.getLocation()},

                           {"department", asset.getDepartment()},

                           {"assignedEmployee", asset.getAssignedEmployee()},

                           {"status", asset.getStatus()},

                           {"daysRemaining", daysRemaining},

                           {"warrantyStatus", warrantyStatus}

                       });
                   }

                   res.set_content(
                       alerts.dump(),
                       "application/json");
               });

    // ----------------------------------
    // GET: Reports and Analytics
    // ----------------------------------

    server.Get("/api/reports",
               [&manager](
                   const httplib::Request &req,
                   httplib::Response &res)
               {
                   setCorsHeaders(req, res);

                   const auto &allAssets = manager.getAssets();
                   const auto &maintenanceRecords =
                       manager.getMaintenanceRecords();

                   // ----------------------------------
                   // 1. Initialize report counters
                   // ----------------------------------

                   map<string, int> statusCounts;
                   map<string, int> categoryCounts;
                   map<string, int> departmentCounts;
                   map<string, int> ageCounts;

                   map<int, double> maintenanceCosts;
                   map<int, int> maintenanceCounts;
                   map<int, double> assetAges;

                   double totalMaintenanceCost = 0.0;

                   int inUse = 0;
                   int inStock = 0;
                   int underRepair = 0;
                   int retired = 0;
                   int repeatedFailures = 0;

                   // ----------------------------------
                   // 2. Calculate maintenance costs
                   // ----------------------------------

                   for (const MaintenanceRecord &record :
                        maintenanceRecords)
                   {

                       double cost = record.getRepairCost();

                       if (!std::isfinite(cost) || cost < 0)
                       {
                           continue;
                       }

                       int id = record.getAssetID();

                       maintenanceCosts[id] += cost;
                       maintenanceCounts[id]++;

                       totalMaintenanceCost += cost;
                   }

                   // ----------------------------------
                   // 3. Calculate asset distribution
                   // ----------------------------------

                   time_t today = getTodayAtNoon();

                   for (const Asset &asset : allAssets)
                   {

                       string status = asset.getStatus();
                       string category = asset.getAssetType();
                       string department = asset.getDepartment();

                       statusCounts[status]++;
                       categoryCounts[category]++;

                       if (status == "In Use")
                       {
                           inUse++;
                       }
                       else if (status == "In Stock")
                       {
                           inStock++;
                       }
                       else if (status == "Under Repair")
                       {
                           underRepair++;
                       }
                       else if (status == "Retired")
                       {
                           retired++;
                       }

                       // Exclude retired assets from active department report
                       if (status != "Retired")
                       {
                           departmentCounts[department]++;
                       }

                       // Repeated failure count: 3 or more records
                       if (maintenanceCounts[asset.getAssetID()] >= 3)
                       {
                           repeatedFailures++;
                       }

                       // ----------------------------------
                       // 4. Calculate asset age
                       // ----------------------------------

                       time_t purchaseTimestamp;

                       if (!parseWarrantyDate(
                               asset.getPurchaseDate(),
                               purchaseTimestamp))
                       {

                           ageCounts["Unknown"]++;
                           continue;
                       }

                       double ageDays = std::difftime(
                                            today,
                                            purchaseTimestamp) /
                                        86400.0;

                       if (ageDays < 0)
                       {
                           ageCounts["Unknown"]++;
                           continue;
                       }

                       double ageYears = ageDays / 365.25;

                       assetAges[asset.getAssetID()] = ageYears;

                       if (ageYears < 1)
                       {
                           ageCounts["Under 1 Year"]++;
                       }
                       else if (ageYears < 3)
                       {
                           ageCounts["1-3 Years"]++;
                       }
                       else if (ageYears < 5)
                       {
                           ageCounts["3-5 Years"]++;
                       }
                       else
                       {
                           ageCounts["5+ Years"]++;
                       }
                   }

                   // ----------------------------------
                   // 5. Prepare breakdown arrays
                   // ----------------------------------

                   json statusBreakdown = json::array();

                   for (const auto &item : statusCounts)
                   {

                       statusBreakdown.push_back({{"label", item.first},
                                                  {"count", item.second}});
                   }

                   json categoryBreakdown = json::array();

                   for (const auto &item : categoryCounts)
                   {

                       categoryBreakdown.push_back({{"label", item.first},
                                                    {"count", item.second}});
                   }

                   json departmentBreakdown = json::array();

                   for (const auto &item : departmentCounts)
                   {

                       departmentBreakdown.push_back({{"label", item.first},
                                                      {"count", item.second}});
                   }

                   json ageBreakdown = json::array();

                   for (const auto &item : ageCounts)
                   {

                       ageBreakdown.push_back({{"label", item.first},
                                               {"count", item.second}});
                   }

                   // ----------------------------------
                   // 6. Maintenance expenditure report
                   // ----------------------------------

                   json maintenanceByAsset = json::array();

                   for (const auto &item : maintenanceCosts)
                   {

                       int assetID = item.first;

                       for (const Asset &asset : allAssets)
                       {

                           if (asset.getAssetID() == assetID)
                           {

                               maintenanceByAsset.push_back({

                                   {"assetID", assetID},

                                   {"brand", asset.getBrand()},

                                   {"model", asset.getModel()},

                                   {"recordCount", maintenanceCounts[assetID]},

                                   {"totalCost", item.second},

                                   {"purchaseCost", asset.getPurchaseCost()}

                               });

                               break;
                           }
                       }
                   }

                   // ----------------------------------
                   // 7. Retired asset report
                   // ----------------------------------

                   json retiredAssets = json::array();

                   for (const Asset &asset : allAssets)
                   {

                       if (asset.getStatus() == "Retired")
                       {

                           retiredAssets.push_back({

                               {"assetID", asset.getAssetID()},

                               {"assetType", asset.getAssetType()},

                               {"brand", asset.getBrand()},

                               {"model", asset.getModel()},

                               {"purchaseDate", asset.getPurchaseDate()},

                               {"purchaseCost", asset.getPurchaseCost()},

                               {"department", asset.getDepartment()},

                               {"location", asset.getLocation()}

                           });
                       }
                   }

                   // ----------------------------------
                   // 8. Replacement recommendations
                   // ----------------------------------

                   json replacementRecommendations = json::array();

                   for (const Asset &asset : allAssets)
                   {

                       // Retired assets do not need replacement review
                       if (asset.getStatus() == "Retired")
                       {
                           continue;
                       }

                       int assetID = asset.getAssetID();

                       double maintenanceCost =
                           maintenanceCosts[assetID];

                       double purchaseCost =
                           asset.getPurchaseCost();

                       double ageYears = -1;

                       if (assetAges.count(assetID))
                       {
                           ageYears = assetAges[assetID];
                       }

                       json reasons = json::array();

                       // Project-defined age criterion
                       if (ageYears >= 5)
                       {
                           reasons.push_back("Asset age is 5 years or more");
                       }

                       // Project-defined expenditure criterion
                       if (purchaseCost > 0 &&
                           maintenanceCost >= purchaseCost * 0.30)
                       {

                           reasons.push_back(
                               "Maintenance cost is at least 30% of purchase cost");
                       }

                       if (!reasons.empty())
                       {

                           replacementRecommendations.push_back({

                               {"assetID", assetID},

                               {"brand", asset.getBrand()},

                               {"model", asset.getModel()},

                               {"status", asset.getStatus()},

                               {"ageYears", ageYears},

                               {"purchaseCost", purchaseCost},

                               {"maintenanceCost", maintenanceCost},

                               {"reasons", reasons}

                           });
                       }
                   }

                   // ----------------------------------
                   // 9. Final JSON report
                   // ----------------------------------

                   json report = {

                       {"summary", {

                                       {"totalAssets", allAssets.size()},

                                       {"inUse", inUse},

                                       {"inStock", inStock},

                                       {"underRepair", underRepair},

                                       {"retired", retired},

                                       {"totalMaintenanceCost", totalMaintenanceCost},

                                       {"maintenanceRecordCount", maintenanceRecords.size()},

                                       {"replacementCount", replacementRecommendations.size()},

                                       {"repeatedFailureAssets", repeatedFailures}

                                   }},

                       {"statusBreakdown", statusBreakdown},

                       {"categoryBreakdown", categoryBreakdown},

                       {"departmentBreakdown", departmentBreakdown},

                       {"ageBreakdown", ageBreakdown},

                       {"maintenanceByAsset", maintenanceByAsset},

                       {"retiredAssets", retiredAssets},

                       {"replacementRecommendations", replacementRecommendations}

                   };

                   res.set_content(
                       report.dump(),
                       "application/json");
               });

    // ----------------------------------
    // Start HTTP Server
    // ----------------------------------

    cout << "\nAPI Server running at:\n";
    cout << "http://127.0.0.1:8080\n";

    cout << "\nAvailable endpoints:\n";
    cout << "GET  /api/health\n";
    cout << "GET  /api/assets\n";
    cout << "POST /api/assets\n";

    if (!server.listen("127.0.0.1", 8080))
    {
        cerr << "Could not start the API server.\n";
        return 1;
    }

    return 0;
}