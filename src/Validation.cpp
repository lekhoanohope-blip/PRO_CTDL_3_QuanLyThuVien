#include "Validation.h"

namespace Validation {

std::string trim(const std::string& raw) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = raw.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = raw.find_last_not_of(whitespace);
    return raw.substr(first, last - first + 1);
}

bool isBlank(const std::string& raw) {
    return trim(raw).empty();
}

bool containsDelimiter(const std::string& raw) {
    return raw.find(FIELD_DELIMITER) != std::string::npos;
}

std::string toLower(const std::string& raw) {
    std::string result = raw;
    for (char& ch : result) {
        if (ch >= 'A' && ch <= 'Z') {
            ch = static_cast<char>(ch - 'A' + 'a');
        }
    }
    return result;
}

std::vector<std::string> splitLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    for (const char ch : line) {
        if (ch == '\r' || ch == '\n') {
            continue;  // Loai bo ky tu xuong dong (xu ly ca CRLF lan LF).
        }
        if (ch == FIELD_DELIMITER) {
            fields.push_back(trim(current));
            current.clear();
        } else {
            current += ch;
        }
    }
    fields.push_back(trim(current));
    return fields;
}

}  // namespace Validation
