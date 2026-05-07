#pragma once
#include "Models/Primitives/enums.h"
#include "Models/Primitives/date.h"

class GovernmentRemittance
{
public:
    int id{};

    int payrollCalculationResultsId{0};
    std::string employeeId{""};
    std::string fullName{""};
    int employeeDepartment{0};
    std::string payPeriodDate{""};
    int payPeriodHalf{1};

    double employee_Contrib{0.0};
    double employer_Contrib{0.0};
    double total_Contrib{0.0};
    RemittanceType contrib_Type = {RemittanceType::SSS};
    double withHoldingTax{0.0};

    RemittanceStatus submissionStatus{RemittanceStatus::PENDING};
    RemittanceStatus withHoldingTaxSubmissionStatus{RemittanceStatus::PENDING};

    Date lastSubmittedDate{Date(1970, 1, 1)};
    int submittedByUserId{0};
    Date dateCreated{Date(1970, 1, 1)};
    Date dateModified{Date(1970, 1, 1)};

    std::string to_string() const
    {

        std::ostringstream oss;
        oss << "GovernmentRemittance { "
            << "\n id: " << id
            << "\n payrollCalculationResultsId: " << payrollCalculationResultsId
            << "\n employeeId: " << employeeId
            << "\n fullName: " << fullName
            << "\n employeeDepartment: " << employeeDepartment
            << "\n payPeriodDate: " << payPeriodDate
            << "\n payPeriodHalf: " << payPeriodHalf
            << "\n employee_Contrib: " << employee_Contrib
            << "\n employer_Contrib: " << employer_Contrib
            << "\n total_Contrib: " << total_Contrib
            << "\n contrib_Type: " << static_cast<int>(contrib_Type)
            << "\n withHoldingTax: " << withHoldingTax
            << "\n submissionStatus: " << RemittanceStatus_to_string(submissionStatus)
            << "\n withHoldingTaxSubmissionStatus: " << RemittanceStatus_to_string(withHoldingTaxSubmissionStatus)
            << "\n lastSubmittedDate: " << lastSubmittedDate.to_string()
            << "\n dateCreated: " << dateCreated.to_string()
            << "\n dateModified: " << dateModified.to_string()
            << " }";
        return oss.str();
    };
};
