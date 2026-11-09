# IT Asset Lifecycle Management System

A console-based **IT Asset Lifecycle Management System** developed using **C++17**. This project helps an organization manage IT assets throughout their lifecycle, from registration and allocation to maintenance, transfer, and retirement.

## 1. Project Overview

The IT Asset Lifecycle Management System is designed to simplify the management and tracking of organizational IT assets such as laptops, desktops, printers, and other hardware devices.

It maintains asset information, tracks lifecycle activities, records maintenance expenses, checks warranty status, and generates useful reports.

## 2. Features

### Asset Management
- Register new IT assets.
- Store asset details such as ID, type, brand, model, serial number, purchase date, and cost.
- View all registered assets.
- Search assets using Asset ID.
- Prevent duplicate Asset IDs.

### Asset Lifecycle Management
- Allocate assets to employees.
- Transfer assets between employees.
- Update asset status.
- Track asset lifecycle history.
- Retire assets.

### Maintenance Management
- Record asset breakdowns and maintenance activities.
- Store repair details, technician information, and repair costs.
- View maintenance history.
- Calculate total maintenance expenditure.
- Identify assets with repeated failures.

### Warranty Management
- Track warranty end dates.
- Identify expired warranties.
- Generate alerts for warranties expiring within 30 days.
- Generate alerts for warranties expiring within 60 days.

### Reports and Analytics
- Total asset report.
- Category-wise asset report.
- Status-wise asset report.
- Department-wise asset report.
- Asset age report.
- Maintenance expenditure report.
- Retired asset report.
- Asset replacement recommendations.
- Analytics dashboard.

### File Handling
- Store asset information in CSV files.
- Store lifecycle history in CSV files.
- Store maintenance records in CSV files.
- Load previously saved records when the application starts.

## 3. Technologies Used

| Technology | Purpose |
|---|---|
| C++17 | Core programming language |
| STL Vector | Store asset and history records |
| STL Map | Generate analytical reports |
| File Handling | Read and write CSV files |
| VS Code | Development environment |
| GCC / G++ | Compilation |

## 4. Project Folder Structure

```text
IT_Asset_Management/
│
├── main.cpp
│
├── models/
│   ├── Asset.h
│   ├── Asset.cpp
│   ├── LifecycleRecord.h
│   ├── LifecycleRecord.cpp
│   ├── MaintenanceRecord.h
│   └── MaintenanceRecord.cpp
│
├── services/
│   ├── AssetManager.h
│   ├── AssetManager.cpp
│   ├── FileManager.h
│   └── FileManager.cpp
│
├── utils/
│   └── InputHelper.h
│
├── data/
│   ├── assets.csv
│   ├── lifecycle.csv
│   └── maintenance.csv
│
└── README.md
```

## 5. System Requirements

- Operating System: Windows / Linux / macOS
- C++ Compiler: GCC or MinGW supporting C++17
- IDE: Visual Studio Code (recommended)
- C++17 standard support

## 6. How to Run the Project

### Step 1: Clone the Repository

```bash
git clone <your-repository-url>
```

### Step 2: Open the Project Folder

```bash
cd IT_Asset_Management
```

### Step 3: Create the Data Folder

Make sure the `data` folder exists in the project directory. The application uses this folder to store CSV files.

### Step 4: Compile the Project

Run the following command in the VS Code terminal:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp models/Asset.cpp models/LifecycleRecord.cpp models/MaintenanceRecord.cpp services/AssetManager.cpp services/FileManager.cpp -o asset_system
```

### Step 5: Run the Application

**Windows:**

```bash
.\asset_system.exe
```

**Linux / macOS:**

```bash
./asset_system
```

## 7. Application Workflow

1. Start the application.
2. Register a new IT asset.
3. View or search registered assets.
4. Allocate an asset to an employee.
5. Transfer an asset when required.
6. Update asset status during repair.
7. Add maintenance records.
8. Check warranty alerts.
9. Generate reports and analytics.
10. Retire assets when they are no longer required.
11. View lifecycle history.

## 8. Asset Status

The system uses the following asset statuses:

| Status | Description |
|---|---|
| In Stock | Asset is available for allocation |
| In Use | Asset is assigned to an employee |
| Under Repair | Asset is undergoing maintenance |
| Retired | Asset is no longer in active use |

## 9. Data Storage

The project uses CSV files for local data storage.

| File | Stored Information |
|---|---|
| `assets.csv` | Asset details |
| `lifecycle.csv` | Allocation, transfer, status changes, and retirement history |
| `maintenance.csv` | Maintenance and repair records |

Data is loaded when the application starts and saved when records are updated.

**Note:** The current CSV implementation uses simple comma-separated fields and does not fully support commas inside field values.

## 10. C++ Concepts Implemented

- Classes and Objects
- Encapsulation
- Constructors
- Getters and Setters
- Vectors
- Maps
- Functions
- Conditional Statements
- Loops
- File Handling
- Exception Handling
- Input Validation
- Searching and Filtering
- Date Handling
- Modular Programming using Header and Source Files

## 11. Future Enhancements

The following features can be added in future versions:

- Employee Management Module
- User Login and Role-Based Access
- Database Integration using MySQL or SQLite
- Graphical User Interface
- QR Code-Based Asset Identification
- Asset Depreciation Calculation
- Automated Email Notifications for Warranty Expiry
- Advanced CSV Export and Import
- Improved Data Validation and Backup

## 12. Limitations

- The current application is console-based.
- Data is stored locally in CSV files.
- The system does not currently provide multi-user access.
- CSV handling is basic and does not support all special characters.
- Replacement recommendations use simple project-defined rules.

## 13. Author

**Project Name:** IT Asset Lifecycle Management System  
**Language:** C++17  
**Project Type:** Console-Based Application  
**Development Environment:** Visual Studio Code

---

