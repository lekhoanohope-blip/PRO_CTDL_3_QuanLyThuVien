#ifndef LIBRARY_VALIDATION_H
#define LIBRARY_VALIDATION_H

#include <charconv>
#include <string>
#include <system_error>
#include <type_traits>
#include <vector>

/// Ky tu phan cach cac truong trong file du lieu .txt.
inline constexpr char FIELD_DELIMITER = '|';

/// Cat khoang trang o hai dau chuoi.
std::string trim(const std::string& text);

/// Kiem tra chuoi co rong hoac chi gom khoang trang khong.
bool isBlank(const std::string& text);

/// Kiem tra chuoi co chua ky tu phan cach '|' khong.
bool containsDelimiter(const std::string& text);

/// Chuyen chuoi ve chu thuong (dung de tim kiem khong phan biet hoa/thuong).
std::string toLower(const std::string& text);

/// Tach dong thanh cac truong theo ky tu phan cach, giu lai truong rong
/// va loai bo ky tu '\r' o cuoi dong (xu ly file CRLF).
std::vector<std::string> splitLine(const std::string& line, char delimiter);

/// Doc so nguyen tu chuoi va kiem tra nam trong [minVal, maxVal].
/// Tra ve false khi chuoi rong, chua chu cai, co khoang trang o giua,
/// vuot khoi kieu so hoac nam ngoai khoang; khong throw.
template <typename T>
bool parseNumberInRange(const std::string& raw, T minVal, T maxVal,
                        T& outVal) {
    static_assert(std::is_integral_v<T>,
                  "parseNumberInRange chi ho tro kieu so nguyen");
    const std::string text = trim(raw);
    if (text.empty()) {
        return false;
    }
    T value = 0;
    const char* begin = text.data();
    const char* end = text.data() + text.size();
    const std::from_chars_result result = std::from_chars(begin, end, value);
    if (result.ec != std::errc() || result.ptr != end) {
        return false;
    }
    if (value < minVal || value > maxVal) {
        return false;
    }
    outVal = value;
    return true;
}

/// Bi danh tuong thich: cac file khac trong du an (LibraryService,
/// ConsoleUI, AppController, Repository) dang goi cac tien ich nay
/// duoi dang Validation::trim, Validation::toLower... Cac bi danh duoi
/// day tro ve dung cac ham toan cuc o tren, khong doi API hop dong.
namespace Validation {

using ::containsDelimiter;
using ::isBlank;
using ::parseNumberInRange;
using ::toLower;
using ::trim;

/// Tach dong theo ky tu '|' (dang cu chi lay 1 tham so).
inline std::vector<std::string> splitLine(const std::string& line) {
    return ::splitLine(line, ::FIELD_DELIMITER);
}

}  // namespace Validation

#endif  // LIBRARY_VALIDATION_H
