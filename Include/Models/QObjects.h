#ifndef QOBJECTS_H
#define QOBJECTS_H

#include <QString>
#include <QVector>
#include <QDebug>
#include "./DataObjects.h"

struct MenuOption {
    QString label;
    QString action;
    QString submenu;
};

struct MenuData {
    QString title;
    QVector<MenuOption> options;
};


// inline QDebug operator<<(QDebug debug, const Employee &e)
// {
//     std::ostringstream oss;
//     oss.imbue(std::locale("en_PH.UTF-8"));
//     oss << std::fixed << std::setprecision(2);
//     oss << std::string(80, '=') << "\n";
//     oss << std::right << std::setw(32) << "" << "Employee Record" << std::setw(32) << "" << "\n";
//     oss << std::string(80, '=') << "\n";
//     oss << std::left << std::setw(20) << "EmployeeId: " << std::setw(22) << e.employeeId<< std::setw(25)        << "| Monthly Basic Salary: "   << "₱" << std::setw(15)     << e.monthlyBasicSalary     << '\n';
//     oss << std::left << std::setw(20) << "Full Name: "  << std::setw(22) << e.fullName<< std::setw(25)          << "| Monthly Allowances: "     << "₱" << std::setw(15)     << e.monthlyAllowances      << '\n';
//     oss << std::left << std::setw(20) << "Department: " << std::setw(22) << department_to_string(e.department)  << std::setw(25)                << "| Active: "             << std::setw(15)            << e.isActive           << '\n';
//     oss << std::left << std::setw(20) << "JobLevel: "   << std::setw(22) << JobLevel_to_string(e.jobLevel)      << std::setw(25)                << "|"                      << '\n';
//     oss << std::left << std::setw(20) << "Position: "   << std::setw(22) << e.position<< std::setw(25)          << "|"                          << '\n';
//     oss << std::left << std::setw(20) << "Status: "     << std::setw(22) << Status_to_string(e.status)          << std::setw(25)                << "| TIN: "                << std::setw(15)            << e.tin                << '\n';
//     oss << std::left << std::setw(20) << "Email: "      << std::setw(22) << e.personalEmail                     << std::setw(25)                << "| SSS_Number: "         << std::setw(15)            << e.sssNumber          << '\n';
//     oss << std::left << std::setw(20) << "Hired: "      << std::setw(22) << to_string(e.dateHired)              << std::setw(25)                << "| PhilHealth_Number: "  << std::setw(15)            << e.philHealthNumber   << '\n';
//     oss << std::left << std::setw(20) << "Separation: " << std::setw(22) << to_string(e.dateSeparation)         << std::setw(25)                << "| HDMF_Number: "        << std::setw(15)            << e.hdmfNumber         << '\n';
//     oss << std::left << std::setw(20) << "PhoneNo: "    << std::setw(22) << e.personalMobileNumber                   << std::setw(25)           << "| Bank_Account: "        << std::setw(15)            << e.bankAccountNumber << '\n';
//     oss << std::left << std::setw(20) << "Contact: "    << std::setw(22) << e.emergencyContact.name             << std::setw(25)                << "| Dependent"            << std::setw(15)            << e.dependent.name     << '\n';
//     oss << std::string(80, '=') << "\n";

//     QDebugStateSaver saver(debug);
//     debug.noquote().nospace() << QString::fromStdString(oss.str());
//     return debug;
// }
// inline QDebug operator<<(QDebug debug, const Contact &e)
// {
//  std::ostringstream oss;
//     oss.imbue(std::locale("en_PH.UTF-8"));
//     oss << std::fixed << std::setprecision(2);
//     oss << std::string(30, '=') << "\n";
//     oss << std::right << std::setw(8) << "" << "Contact Record" << std::setw(9) << "" << "\n";
//     oss << std::string(30, '=') << "\n";
//     oss << std::left << std::setw(15) << "Name: " << std::setw(15) << e.name << "\n"; 
//     oss << std::left << std::setw(15) << "Relation: "  << std::setw(15) << e.relation << "\n";       
//     oss << std::left << std::setw(15) << "Contact Number: " << std::setw(15) << e.contactNo<< "\n"; 
//     oss << std::left << std::setw(15) << "Address: "   << std::setw(15) << e.address<< "\n"; 
//     oss << std::string(30, '=') << "\n";

//     QDebugStateSaver saver(debug);
//     debug.noquote().nospace() << QString::fromStdString(oss.str());
//     return debug;
// }

// inline QDebug operator<<(QDebug debug, const Dependent &e)
// {
//  std::ostringstream oss;
//     oss.imbue(std::locale("en_PH.UTF-8"));
//     oss << std::fixed << std::setprecision(2);
//     oss << std::string(30, '=') << "\n";
//     oss << std::right << std::setw(7) << "" << "Dependent Record" << std::setw(7) << "" << "\n";
//     oss << std::string(30, '=') << "\n";
//     oss << std::left << std::setw(15) << "Name: " << std::setw(15)      << e.name<< "\n"; 
//     oss << std::left << std::setw(15) << "Relation: "  << std::setw(15) << e.relation << "\n";         
//     oss << std::left << std::setw(15) << "Birthday: " << std::setw(15)  << to_string(e.birthday)<< "\n"; 
//     oss << std::string(30, '=') << "\n";

//     QDebugStateSaver saver(debug);
//     debug.noquote().nospace() << QString::fromStdString(oss.str());
//     return debug;
// }

#endif