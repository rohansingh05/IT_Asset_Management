# IT Asset Lifecycle Management System

A web-based **IT Asset Lifecycle Management System** developed using **C++17, HTML, CSS and JavaScript**.

The application helps organizations manage IT assets throughout their lifecycle, from registration and employee allocation to maintenance, warranty monitoring, reporting and retirement.

The frontend communicates with a C++ backend through REST APIs, while asset and maintenance information is stored locally using CSV files.

---

## 1. Project Overview

Organizations use different IT assets such as laptops, desktops, printers, servers and other hardware devices.

Managing these assets manually can make it difficult to track their location, assigned employees, maintenance history, warranty expiry and replacement requirements.

The IT Asset Lifecycle Management System provides a centralized application to manage these activities.

The system supports:

- Automatic Asset ID generation.
- Asset registration and management.
- Employee allocation and asset transfer.
- Maintenance and repair tracking.
- Warranty expiry alerts.
- Asset lifecycle history.
- Reports and analytics.
- CSV-based data persistence.

---

## 2. Key Features

### 2.1 Dashboard

- View the total number of registered assets.
- View assets currently in use.
- View available assets in stock.
- View assets under repair.
- View retired assets.
- View warranty alerts.
- View recently registered assets.

### 2.2 Asset Registration and Management

- Register new IT assets.
- Automatically generate unique Asset IDs through the C++ backend.
- Store asset information including:
  - Asset ID
  - Asset Type
  - Brand
  - Model
  - Serial Number
  - Purchase Date
  - Purchase Cost
  - Warranty End Date
  - Location
  - Department
  - Assigned Employee
  - Asset Status
- View all registered assets.
- Search assets by ID, brand, model and other details.
- Filter assets by type and status.
- Prevent duplicate serial numbers.

### 2.3 Asset Allocation and Transfer

- Allocate available assets to employees.
- Transfer assets between employees.
- Display the current assigned employee.
- Maintain allocation and transfer history.
- Automatically update asset status.
- Store lifecycle activities in `lifecycle.csv`.

**Business Rules:**

| Operation | Condition | Result |
|---|---|---|
| Allocate | Asset is In Stock | Status becomes In Use |
| Transfer | Asset is In Use | Employee assignment changes |
| Allocate | Asset is not In Stock | Operation rejected |
| Transfer | Asset is not In Use | Operation rejected |

### 2.4 Maintenance Management

- Register asset breakdowns and repair activities.
- Record problem descriptions.
- Store repair dates.
- Record repair costs.
- Store technician information.
- Track Pending and Completed maintenance records.
- Calculate total maintenance expenditure.
- View and filter maintenance history.
- Automatically update asset status after repair.

**Maintenance Status Rules:**

| Repair Status | Asset Status |
|---|---|
| Pending | Under Repair |
| Completed | In Use, if previously Under Repair |

### 2.5 Warranty Management

- Monitor warranty expiry dates.
- Identify expired warranties.
- Identify warranties expiring within 30 days.
- Identify warranties expiring within 31–60 days.
- Display affected Asset IDs.
- Filter and search warranty alerts.
- Refresh warranty information using the C++ backend.

Retired assets are excluded from active warranty alerts.

### 2.6 Reports and Analytics

The system generates the following reports:

- Total Asset Report.
- Status-wise Asset Report.
- Category-wise Asset Report.
- Department-wise Asset Report.
- Asset Age Report.
- Maintenance Expenditure Report.
- Retired Asset Report.
- Replacement Recommendations.
- Repeated Failure Analysis.

**Replacement Recommendation Rules:**

An active asset is recommended for replacement review when either:

1. Its age is at least 5 years.
2. Its accumulated maintenance cost is at least 30% of its purchase cost.

These are project-defined rules for educational purposes, not universal industry replacement standards.

---

## 3. Technologies Used

| Technology | Purpose |
|---|---|
| C++17 | Backend business logic |
| HTML5 | Frontend structure |
| CSS3 | Frontend styling and responsive layout |
| JavaScript | Frontend functionality and API communication |
| cpp-httplib | C++ HTTP server and REST API |
| nlohmann/json | JSON parsing and responses |
| STL Vector | Store assets and records |
| STL Map | Generate reports and analytics |
| CSV File Handling | Local data persistence |
| GCC / MinGW | C++ compilation |
| VS Code | Development environment |
| Live Server | Run the frontend locally |

---

## 4. System Architecture

The application follows a frontend-backend architecture.

```text
                 USER
                   |
                   v
          WEB FRONTEND
       HTML + CSS + JavaScript
                   |
                   v
              HTTP / JSON
                   |
                   v
            C++ REST API
             server.cpp
                   |
                   v
            ASSET MANAGER
                   |
        +----------+----------+
        |          |          |
        v          v          v
      ASSET     LIFECYCLE   MAINTENANCE
     MANAGEMENT  HISTORY   MANAGEMENT
        |          |          |
        +----------+----------+
                   |
                   v
             FILE MANAGER
                   |
                   v
              CSV STORAGE
       assets.csv
       lifecycle.csv
       maintenance.csv
```

### Frontend

The frontend provides an interactive dashboard, registration forms, asset tables, filters and reports.

### Backend

The C++ backend handles business rules, validation, Asset ID generation, lifecycle transitions, warranty calculations and analytics.

### Data Storage

CSV files store the application's asset, lifecycle and maintenance records.

---

## 5. Project Folder Structure

```text
IT_Asset_Management/
│
├── main.cpp
├── server.cpp
│
├── libs/
│   ├── httplib.h
│   └── nlohmann/
│       └── json.hpp
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
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── app.js
│
└── README.md
```

---

## 6. System Requirements

### Hardware Requirements

- Computer or laptop.
- Minimum 4 GB RAM recommended.
- Sufficient storage for source files and CSV data.

### Software Requirements

- Windows 10 or later for the described Windows build setup.
- Visual Studio Code.
- C++17-compatible GCC compiler.
- MSYS2 UCRT64 with MinGW-w64 GCC.
- Live Server VS Code extension.
- Modern web browser.

---

## 7. External Libraries

The project uses two header-only C++ libraries.

### cpp-httplib

Used to create the HTTP server and handle REST API requests.

Repository: https://github.com/yhirose/cpp-httplib

### nlohmann/json

Used for JSON parsing and generating API responses.

Repository: https://github.com/nlohmann/json

Download and place the files in these locations:

```text
libs/
├── httplib.h
└── nlohmann/
    └── json.hpp
```

---

## 8. How to Run the Project

### Step 1: Open the Project

Open the `IT_Asset_Management` folder in Visual Studio Code.

### Step 2: Install the Required C++ Compiler

For the Windows setup, install MSYS2 and open its UCRT64 terminal.

Install GCC if it is not already installed:

```bash
pacman -Syu
```

Then:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc
```

Check the compiler:

```bash
g++ --version
```

### Step 3: Verify the Project Directory

Navigate to the main project directory.

Example:

```bash
cd /c/Users/YourUsername/Documents/IT_Asset_Management
```

Check the files:

```bash
ls
```

Make sure `server.cpp`, `models`, `services`, `libs` and `frontend` are present.

### Step 4: Compile the C++ Backend

Run the following command from the main project directory:

```bash
g++ -std=c++17 -D_WIN32_WINNT=0x0A00 -Wall -Wextra -pedantic -pthread server.cpp models/Asset.cpp models/LifecycleRecord.cpp models/MaintenanceRecord.cpp services/AssetManager.cpp services/FileManager.cpp -lws2_32 -o asset_api.exe
```

**Important:** Compile `server.cpp` instead of `main.cpp` for the web application. Both files contain their own `main()` function and must be compiled separately.

### Step 5: Start the C++ Backend

Run:

```bash
./asset_api.exe
```

The API server will start at:

```text
http://127.0.0.1:8080
```

Keep this terminal open while using the frontend.

### Step 6: Start the Frontend

Open a second terminal or use VS Code Explorer.

1. Open the `frontend` folder.
2. Right-click `index.html`.
3. Select **Open with Live Server**.
4. Open the displayed local URL in your browser.

The usual Live Server URL is:

```text
http://127.0.0.1:5500
```

The backend CORS configuration must allow the actual Live Server origin.

### Step 7: Test Backend Connection

Open the following URL in your browser:

```text
http://127.0.0.1:8080/api/health
```

Expected response:

```json
{
    "status": "ok",
    "message": "C++ backend is running"
}
```

---

## 9. REST API Endpoints

The C++ backend exposes the following HTTP endpoints:

| Method | Endpoint | Description |
|---|---|---|
| GET | `/api/health` | Check backend availability |
| GET | `/api/assets` | Retrieve all assets |
| POST | `/api/assets` | Register a new asset |
| POST | `/api/assets/allocate` | Allocate an asset |
| POST | `/api/assets/transfer` | Transfer an asset |
| GET | `/api/lifecycle` | Retrieve lifecycle history |
| GET | `/api/maintenance` | Retrieve maintenance records |
| POST | `/api/maintenance` | Add a maintenance record |
| GET | `/api/warranty-alerts` | Retrieve warranty expiry alerts |
| GET | `/api/reports` | Retrieve reports and analytics |

### Automatic Asset ID Generation

Asset IDs are generated by the C++ backend.

The frontend does not send an Asset ID during registration.

Example:

```text
First registered asset:  1001
Second registered asset: 1002
Third registered asset:  1003
```

The next ID is calculated from the highest existing Asset ID when the backend loads saved asset records.

---

## 10. Data Storage

The system uses CSV files for persistent storage.

| File | Description |
|---|---|
| `assets.csv` | Stores asset information |
| `lifecycle.csv` | Stores allocation, transfer and status history |
| `maintenance.csv` | Stores repair and maintenance records |

The application loads existing records when the C++ backend starts and saves updates through the FileManager.

The `data` directory must exist before starting the application.

**Date Format:** The backend uses `DD-MM-YYYY` for stored dates. The frontend converts browser date inputs into this format before submitting API requests.

---

## 11. C++ Concepts Implemented

- Object-Oriented Programming.
- Classes and Objects.
- Encapsulation.
- Constructors.
- Getters and Setters.
- Vectors.
- Maps.
- Functions and modular programming.
- File Handling.
- Exception Handling.
- Input Validation.
- Searching and Filtering.
- Date Handling.
- JSON Processing.
- REST API Integration.

---

## 12. Testing

The application should be tested using the following scenarios.

| Test ID | Test Scenario | Expected Result |
|---|---|---|
| TC01 | Register a new asset | Asset saved with generated ID |
| TC02 | Register duplicate serial number | Registration rejected |
| TC03 | Search an existing asset | Matching record displayed |
| TC04 | Allocate an In Stock asset | Status changes to In Use |
| TC05 | Allocate an already allocated asset | Operation rejected |
| TC06 | Transfer an allocated asset | Employee assignment updated |
| TC07 | Add pending maintenance | Status becomes Under Repair |
| TC08 | Complete maintenance | Status returns to In Use |
| TC09 | View maintenance history | Saved records displayed |
| TC10 | Check expired warranty | Expired alert displayed |
| TC11 | Check upcoming warranty | 30-day or 60-day alert displayed |
| TC12 | Generate reports | Backend report data displayed |
| TC13 | Restart application | Previously saved records loaded |

Record the actual test result as Pass or Fail after executing each test.

---

## 13. Current Limitations

- The application is designed for local use.
- Data is stored in CSV files rather than a relational database.
- The current CSV implementation uses simple comma-separated fields and does not support commas inside field values.
- The application does not currently provide user authentication or role-based access.
- Multiple simultaneous users and concurrent file updates are not fully supported.
- Separate CSV writes do not provide fully atomic multi-file transactions.
- Employee management is not implemented as a separate module.
- Replacement recommendations use simple project-defined thresholds.

---

## 14. Future Enhancements

Possible future improvements include:

- Employee Management Module.
- User Login and Role-Based Access Control.
- MySQL or SQLite database integration.
- PDF and Excel report export.
- Advanced CSV import and export.
- QR Code-Based Asset Identification.
- Asset Depreciation Calculation.
- Automated Email Notifications for Warranty Expiry.
- Data Backup and Restore.
- Advanced graphical analytics.
- Multi-user access and improved security.

---

## 15. Project Information

| Detail | Information |
|---|---|
| Project Name | IT Asset Lifecycle Management System |
| Programming Language | C++17 |
| Frontend | HTML, CSS, JavaScript |
| Backend | C++ REST API |
| Data Storage | CSV Files |
| Development Environment | Visual Studio Code |
| Application Type | Web-Based Application |

## 16. Conclusion

The IT Asset Lifecycle Management System demonstrates how C++ object-oriented programming and file handling can be integrated with a web-based frontend.

It provides a structured approach to registering, allocating, transferring, maintaining and tracking IT assets.

The application also supports warranty monitoring and analytical reports, helping organizations maintain better visibility into their IT asset lifecycle.

---

