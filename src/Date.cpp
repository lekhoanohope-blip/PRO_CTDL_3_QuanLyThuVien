#include "Date.h"

#include <cstdio>
#include <ctime>
#include <vector>

#include "Validation.h"

namespace {

/// Nam nho nhat va lon nhat chap nhan duoc cho mot ngay hop le.
constexpr int MIN_YEAR = 1;
constexpr int MAX_YEAR = 9999;
/// Ngay nho nhat trong thang.
constexpr int MIN_DAY = 1;
/// Thang nho nhat va lon nhat trong nam.
constexpr int MIN_MONTH = 1;
constexpr int MAX_MONTH = 12;
/// Ngay lon nhat co the co trong mot thang.
constexpr int MAX_DAY_IN_MONTH = 31;
/// Chi so thang 2 (thang co so ngay phu thuoc nam nhuan).
constexpr int FEBRUARY = 2;
/// So ngay cua cac thang (chi so theo thang 1..12, chi so 0 khong dung).
constexpr int DAYS_IN_MONTH_TABLE[] = {0, 31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31};
/// So truong cua chuoi ngay "dd/mm/yyyy".
constexpr std::size_t DATE_PART_COUNT = 3;
/// Gio buoi trua dat trong std::tm de tranh lech gio DST khi cong ngay.
constexpr int NOON_HOUR = 12;
/// Cac moc goc cua std::tm: thang bat dau tu 0, nam tinh tu 1900.
constexpr int TM_MONTH_BASE = 1;
constexpr int TM_YEAR_BASE = 1900;

}  // namespace

Date::Date() : m_day(1), m_month(1), m_year(2000) {
}

Date::Date(int day, int month, int year)
    : m_day(day), m_month(month), m_year(year) {
}

bool Date::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool Date::isValid(int day, int month, int year) {
    if (year < MIN_YEAR || year > MAX_YEAR) {
        return false;
    }
    if (month < MIN_MONTH || month > MAX_MONTH) {
        return false;
    }
    if (day < MIN_DAY || day > daysInMonth(month, year)) {
        return false;
    }
    return true;
}

bool Date::isValid() const {
    return isValid(m_day, m_month, m_year);
}

int Date::daysInMonth(int month, int year) {
    if (month < MIN_MONTH || month > MAX_MONTH) {
        return 0;
    }
    if (month == FEBRUARY && isLeapYear(year)) {
        return 29;
    }
    return DAYS_IN_MONTH_TABLE[month];
}

Date Date::today() {
    const std::time_t now = std::time(nullptr);
    const std::tm* localTime = std::localtime(&now);
    if (localTime == nullptr) {
        return Date();
    }
    return Date(localTime->tm_mday, localTime->tm_mon + TM_MONTH_BASE,
                localTime->tm_year + TM_YEAR_BASE);
}

Date Date::addDays(int days) const {
    std::tm timeInfo = {};
    timeInfo.tm_year = m_year - TM_YEAR_BASE;
    timeInfo.tm_mon = m_month - TM_MONTH_BASE;
    timeInfo.tm_mday = m_day + days;
    timeInfo.tm_hour = NOON_HOUR;
    timeInfo.tm_isdst = -1;
    const std::time_t normalized = std::mktime(&timeInfo);
    if (normalized == static_cast<std::time_t>(-1)) {
        return *this;
    }
    return Date(timeInfo.tm_mday, timeInfo.tm_mon + TM_MONTH_BASE,
                timeInfo.tm_year + TM_YEAR_BASE);
}

std::string Date::toString() const {
    char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d", m_day, m_month,
                  m_year);
    return std::string(buffer);
}

bool Date::fromString(const std::string& text, Date& outDate) {
    const std::vector<std::string> parts = splitLine(trim(text), '/');
    if (parts.size() != DATE_PART_COUNT) {
        return false;
    }
    int day = 0;
    int month = 0;
    int year = 0;
    if (!parseNumberInRange<int>(parts[0], MIN_DAY, MAX_DAY_IN_MONTH, day)) {
        return false;
    }
    if (!parseNumberInRange<int>(parts[1], MIN_MONTH, MAX_MONTH, month)) {
        return false;
    }
    if (!parseNumberInRange<int>(parts[2], MIN_YEAR, MAX_YEAR, year)) {
        return false;
    }
    if (!isValid(day, month, year)) {
        return false;
    }
    outDate = Date(day, month, year);
    return true;
}

int Date::getDay() const {
    return m_day;
}

int Date::getMonth() const {
    return m_month;
}

int Date::getYear() const {
    return m_year;
}

bool Date::operator==(const Date& other) const {
    return m_day == other.m_day && m_month == other.m_month &&
           m_year == other.m_year;
}

bool Date::operator<(const Date& other) const {
    if (m_year != other.m_year) {
        return m_year < other.m_year;
    }
    if (m_month != other.m_month) {
        return m_month < other.m_month;
    }
    return m_day < other.m_day;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}
