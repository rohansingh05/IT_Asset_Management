#include "utils/InputHelper.h"
#include <iostream>
#include "services/AssetManager.h"

using namespace std;

int main() {

    AssetManager manager;

    manager.loadExistingAssets();

    int choice;

    do {

        cout << "\n====================================\n";
        cout << " IT ASSET LIFECYCLE MANAGEMENT SYSTEM\n";
        cout << "====================================\n";

        cout << "\n--- ASSET MANAGEMENT ---\n";

        cout << "1. Register New Asset\n";
        cout << "2. View All Assets\n";
        cout << "3. Search Asset\n";

        cout << "\n--- LIFECYCLE MANAGEMENT ---\n";

        cout << "4. Allocate Asset\n";
        cout << "5. Transfer Asset\n";
        cout << "6. Update Asset Status\n";
        cout << "7. Retire Asset\n";
        cout << "8. View Lifecycle History\n";

        cout << "\n--- MAINTENANCE MANAGEMENT ---\n";

        cout << "9. Add Maintenance Record\n";
        cout << "10. View Maintenance Records\n";
        cout << "11. Search Maintenance History\n";
        cout << "12. Calculate Maintenance Cost\n";
        cout << "13. Identify Repeated Failures\n";
        cout << "14. Check Warranty Alerts\n";

        cout << "\n--- REPORTS AND ANALYTICS ---\n";

        cout << "15. Total Asset Report\n";
        cout << "16. Category-wise Report\n";
        cout << "17. Status-wise Report\n";
        cout << "18. Department-wise Report\n";
        cout << "19. Asset Age Report\n";
        cout << "20. Maintenance Expenditure Report\n";
        cout << "21. Retired Asset Report\n";
        cout << "22. Replacement Recommendation\n";
        cout << "23. Analytics Dashboard\n";

        cout << "\n24. Exit\n";


        choice = readInt("Enter Your Choice: ");

        switch (choice) {

            case 1:
                manager.addAsset();
                break;

            case 2:
                manager.viewAllAssets();
                break;

            case 3:
                manager.searchAsset();
                break;

            case 4:
                manager.allocateAsset();
                break;

            case 5:
                manager.transferAsset();
                break;

            case 6:
                manager.updateAssetStatus();
                break;

            case 7:
                manager.retireAsset();
                break;

            case 8:
                manager.viewLifecycleHistory();
                break;

            case 9:
                manager.addMaintenanceRecord();
                break;

            case 10:
                manager.viewMaintenanceRecords();
                break;

            case 11:
                manager.searchMaintenanceHistory();
                break;

            case 12:
                manager.calculateMaintenanceCost();
                break;

            case 13:
                manager.identifyRepeatedFailures();
                break;

            case 14:
                manager.checkWarrantyAlerts();
                break;

            case 15:
                manager.generateTotalAssetReport();
                break;

            case 16:
                manager.generateCategoryReport();
                break;

            case 17:
                manager.generateStatusReport();
                break;

            case 18:
                manager.generateDepartmentReport();
                break;

            case 19:
                manager.generateAssetAgeReport();
                break;

            case 20:
                manager.generateMaintenanceReport();
                break;

            case 21:
                manager.generateRetiredAssetReport();
                break;

            case 22:
                manager.generateReplacementReport();
                break;

            case 23:
                manager.showAnalyticsDashboard();
                break;

            case 24:
                cout << "Thank you for using the system!\n";
                break;

            default:
                cout << "Invalid Choice! Try Again.\n";
        }

    } while (choice != 24);

    return 0;
}
