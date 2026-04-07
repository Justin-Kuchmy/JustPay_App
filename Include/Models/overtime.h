#pragma once
#include "Models/Primitives/date.h"
#include <string>

struct Overtime
{
    double regular = 0.0;
    double rest_day = 0.0;
    double rest_day_plus = 0.0;
    double legal_holiday = 0.0;
    double legal_holiday_plus = 0.0;
    double special_holiday = 0.0;
    double special_holiday_plus = 0.0;
    double rest_plus_legal = 0.0;
    double rest_plus_special = 0.0;
    double night_shift_diff = 0.0;

    int sumMinutes() const
    {
        double sum = 0.0;
        sum += regular;
        sum += rest_day;
        sum += rest_day_plus;
        sum += legal_holiday;
        sum += legal_holiday_plus;
        sum += special_holiday;
        sum += special_holiday_plus;
        sum += rest_plus_legal;
        sum += rest_plus_special;
        sum += night_shift_diff;
        return static_cast<int>(sum);
    }

    double rateMultiplier() const
    {
        double sum = 0.0;
        sum += (regular / 60.0 * 1.25);
        sum += (rest_day / 60.0 * 1.30);
        sum += (rest_day_plus / 60.0 * 1.69);
        sum += (legal_holiday / 60.0 * 2.00);
        sum += (legal_holiday_plus / 60.0 * 2.60);
        sum += (special_holiday / 60.0 * 1.30);
        sum += (special_holiday_plus / 60.0 * 1.69);
        sum += (rest_plus_legal / 60.0 * 2.60);
        sum += (rest_plus_special / 60.0 * 3.38);
        sum += (night_shift_diff / 60.0 * 0.10);
        // this value is multiplied by the rate of pay
        return sum;
    }

    static double calculatePay(const std::string &json)
    {
        Overtime ot = fromJson(json);
        return ot.rateMultiplier();
    }

    static double extractNumber(const std::string &json, const std::string &key)
    {
        auto pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos)
            return 0.0;

        pos = json.find(":", pos);
        if (pos == std::string::npos)
            return 0.0;

        std::stringstream ss(json.substr(pos + 1));
        double value = 0;
        ss >> value;
        return value;
    }

    static Overtime fromJson(const std::string &json)
    {
        Overtime ot;
        ot.regular = extractNumber(json, "regular");
        ot.rest_day = extractNumber(json, "rest_day");
        ot.rest_day_plus = extractNumber(json, "rest_day_plus");
        ot.legal_holiday = extractNumber(json, "legal_holiday");
        ot.legal_holiday_plus = extractNumber(json, "legal_holiday_plus");
        ot.special_holiday = extractNumber(json, "special_holiday");
        ot.special_holiday_plus = extractNumber(json, "special_holiday_plus");
        ot.rest_plus_legal = extractNumber(json, "rest_plus_legal");
        ot.rest_plus_special = extractNumber(json, "rest_plus_special");
        ot.night_shift_diff = extractNumber(json, "night_shift_diff");
        return ot;
    }

    std::string to_string() const
    {
        return std::format(
            "{{'regular':{},'rest_day':{},'rest_day_plus':{},"
            "'legal_holiday':{},'legal_holiday_plus':{},"
            "'special_holiday':{},'special_holiday_plus':{},"
            "'rest_plus_legal':{},'rest_plus_special':{},"
            "'night_shift_diff':{}}}",
            regular,
            rest_day,
            rest_day_plus,
            legal_holiday,
            legal_holiday_plus,
            special_holiday,
            special_holiday_plus,
            rest_plus_legal,
            rest_plus_special,
            night_shift_diff);
    }
};

struct AttendanceLog
{
    int logId{};
    std::string employeeId{};
    Date logDate{};
    int lateByMinute{};
    int underTimeByMinute{};
    int overTimeByMinute{};
    bool isAbsent{};
    std::string overtimeJson{};
    Overtime overtimeObj{};
    std::string notes{};

    double getRateMultiplier() const
    {
        return overtimeObj.rateMultiplier();
    }

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "logId: " << logId
            << "\n employeeId: " << employeeId
            << "\n logDate: " << logDate.to_string()
            << "\n lateByMinute: " << lateByMinute
            << "\n underTimeByMinute: " << underTimeByMinute
            << "\n overTimeByMinute: " << overTimeByMinute
            << "\n isAbsent: " << ((isAbsent == 1) ? "True" : "False")
            << "\n Notes: " << notes;

        return oss.str();
    }
};
