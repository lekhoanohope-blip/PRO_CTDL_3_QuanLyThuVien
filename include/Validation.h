#ifndef LIBRARY_VALIDATION_H
#define LIBRARY_VALIDATION_H

#include <string>
#include <vector>

namespace Validation {

/// Ky tu phan cach giua cac truong trong file .txt (hop dong du lieu).
constexpr char FIELD_DELIMITER = '|';

/// Ma ASCII cua phim ESC.
constexpr int ESC_KEY_CODE = 27;

/// Ma ASCII cua phim Backspace.
constexpr int BACKSPACE_KEY_CODE = 8;

/// So lan dang nhap sai toi da truoc khi tu dong thoat.
constexpr int MAX_LOGIN_ATTEMPTS = 3;

/// So ngay duoc muon toi da ke tu ngay muon.
constexpr int LOAN_DURATION_DAYS = 7;

/// Xoa khoang trang dau/cuoi va ky tu xuong dong o 2 dau chuoi.
/// @param raw Chuoi can chuan hoa.
/// @return Chuoi da duoc trim.
std::string trim(const std::string& raw);

/// Kiem tra chuoi sau khi trim co rong khong.
/// @param raw Chuoi can kiem tra.
/// @return true neu chuoi chi gom khoang trang.
bool isBlank(const std::string& raw);

/// Kiem tra chuoi co chua ky tu phan cach hop dong du lieu hay khong.
/// @param raw Chuoi can kiem tra.
/// @return true neu chua ky tu '|'.
bool containsDelimiter(const std::string& raw);

/// Chuyen toan bo chuoi sang chu thuong (khong phan biet hoa/thuong
/// khi tim kiem Ten sach, Tac gia).
/// @param raw Chuoi can chuyen.
/// @return Chuoi chu thuong.
std::string toLower(const std::string& raw);

/// Tach 1 dong file thanh cac truong theo FIELD_DELIMITER.
/// Tu dong trim tung truong va loai bo ky tu '\r' (CRLF).
/// @param line Dong file can tach.
/// @return Vector cac truong da trim.
std::vector<std::string> splitLine(const std::string& line);

/// Function Template: chuyen chuoi sang so nguyen co kiem tra.
/// Dung chung cho int, long long. Tu choi: chua chu cai, am
/// (khi minVal >= 0), vuot bien, tran so.
/// @tparam T Kieu so nguyen (int, long long).
/// @param raw Chuoi nguoi dung nhap.
/// @param minVal Gia tri toi thieu hop le (bao gom).
/// @param maxVal Gia tri toi da hop le (bao gom).
/// @param outVal Tham chieu nhan ket qua neu hop le.
/// @return true neu chuyen doi thanh cong va nam trong [minVal, maxVal].
template <typename T>
bool parseNumberInRange(const std::string& raw, T minVal, T maxVal, T& outVal) {
    const std::string cleaned = trim(raw);
    if (cleaned.empty()) {
        return false;
    }

    std::size_t pos = 0;
    if (cleaned[pos] == '-') {
        // Khong cho phep so am khi minVal khong am.
        if (minVal >= 0) {
            return false;
        }
        ++pos;
    } else if (cleaned[pos] == '+') {
        ++pos;
    }

    // Phan con lai phai toan la chu so.
    for (std::size_t i = pos; i < cleaned.size(); ++i) {
        if (cleaned[i] < '0' || cleaned[i] > '9') {
            return false;
        }
    }
    if (pos == cleaned.size()) {
        return false;  // Chi co dau '-'/'+' ma khong co chu so.
    }

    try {
        // std::stoll nem exception khi tran so -> tu choi an toan.
        const long long value = std::stoll(cleaned, nullptr, 10);
        if (value < static_cast<long long>(minVal)
            || value > static_cast<long long>(maxVal)) {
            return false;
        }
        outVal = static_cast<T>(value);
        return true;
    } catch (...) {
        return false;  // std::out_of_range hoac std::invalid_argument.
    }
}

}  // namespace Validation

#endif  // LIBRARY_VALIDATION_H
