#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <QDate>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Models/Primitives/enums.h"

struct Date
{
    int year{};
    int month{};
    int day{};

    static Date fromQDate(const QDate &d)
    {
        Date date{};
        date.year = d.year();
        date.month = d.month();
        date.day = d.day();
        return date;
    }

    Date &operator=(const QDate &qtDate)
    {
        this->year = qtDate.year();
        this->month = qtDate.month();
        this->day = qtDate.day();
        return *this;
    }

    static Date fromString(const std::string &s)
    {
        Date d;
        char dash;
        std::istringstream iss(s);
        iss >> d.year >> dash >> d.month >> dash >> d.day;
        return d;
    }

    std::string toMonthYearString() const
    {
        std::ostringstream oss;
        oss << this->year << " " << std::setw(2) << std::setfill('0') << this->month;
        return oss.str();
    }
    std::string to_string() const
    {
        std::ostringstream oss;
        oss << std::setw(4) << std::setfill('0') << year << '-'
            << std::setw(2) << std::setfill('0') << month << '-'
            << std::setw(2) << std::setfill('0') << day;
        return oss.str();
    }

    static Date getTodayDate()
    {

        const auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&t);

        return Date{
            localTime.tm_year + 1900,
            localTime.tm_mon + 1,
            localTime.tm_mday};
    }

    bool operator>(const Date &rhs) const
    {

        return (year > rhs.year) ||
               (year == rhs.year && month > rhs.month) ||
               (year == rhs.year && month == rhs.month && day > rhs.day);
    }

    bool operator<=(const Date &rhs) const
    {
        return !((*this) > rhs);
    }

    bool operator==(const Date &rhs) const
    {
        return year == rhs.year && month == rhs.month && day == rhs.day;
    }

    int daysUntil(const Date &other) const
    {
        auto toTimePoint = [](const Date &d)
        {
            std::tm tm{};
            tm.tm_year = d.year - 1900;
            tm.tm_mon = d.month - 1;
            tm.tm_mday = d.day;
            std::time_t t = std::mktime(&tm);
            return std::chrono::system_clock::from_time_t(t);
        };

        auto tp1 = toTimePoint(*this);
        auto tp2 = toTimePoint(other);
        auto diff = std::chrono::duration_cast<std::chrono::hours>(tp2 - tp1).count();
        return static_cast<int>(diff / 24);
    }

    Date addMonths(int months) const
    {
        int newMonth = month + months;
        int newYear = year + (newMonth - 1) / 12;
        newMonth = ((newMonth - 1) % 12) + 1;
        return Date{newYear, newMonth, day};
    }
};

struct BudgetPeriod
{
    int id{};
    std::string label{};
    int year{};
    int half{};

    Date startDate{};
    Date endDate{};
};