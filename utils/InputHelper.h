
#pragma once

#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include <sstream>
#include <ctime>

using namespace std;

// Read a valid integer
inline int readInt(const string& message) {

    int value;

    while (true) {

        cout << message;

        if (cin >> value) {

            return value;
        }

        cout << "Invalid input! Enter a number.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }
}

// Read a valid positive amount
inline double readPositiveAmount(const string& message) {

    double value;

    while (true) {

        cout << message;

        if (cin >> value && value >= 0) {

            return value;
        }

        cout << "Invalid amount! Enter a valid non-negative number.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }
}



// Validate DD-MM-YYYY format
inline bool isValidDate(const string& date) {

    istringstream ss(date);

    int day, month, year;
    char dash1, dash2;

    if (!(ss >> day >> dash1 >> month
             >> dash2 >> year)) {
        return false;
    }

    if (dash1 != '-' || dash2 != '-' || !ss.eof()) {
        return false;
    }

    if (year < 1900 || month < 1 || month > 12 ||
        day < 1 || day > 31) {
        return false;
    }

    tm dateInfo = {};

    dateInfo.tm_mday = day;
    dateInfo.tm_mon = month - 1;
    dateInfo.tm_year = year - 1900;
    dateInfo.tm_hour = 12;

    time_t result = mktime(&dateInfo);

    if (result == -1) {
        return false;
    }

    return dateInfo.tm_mday == day &&
           dateInfo.tm_mon == month - 1 &&
           dateInfo.tm_year == year - 1900;
}

// Read a valid date
inline string readDate(const string& message) {

    string date;

    while (true) {

        cout << message;

        getline(cin, date);

        if (isValidDate(date)) {
            return date;
        }

        cout << "Invalid date! Use DD-MM-YYYY format.\n";
    }
}

