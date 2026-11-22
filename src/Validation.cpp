#include "Validation.h"

#include <cctype>

std::string trim(const std::string& text) {
    std::size_t first = 0;
    while (first < text.size() &&
           std::isspace(static_cast<unsigned char>(text[first]))) {
        ++first;
    }
    std::size_t last = text.size();
    while (last > first &&
           std::isspace(static_cast<unsigned char>(text[last - 1]))) {
        --last;
    }
    return text.substr(first, last - first);
}

bool isBlank(const std::string& text) {
    return trim(text).empty();
}

bool containsDelimiter(const std::string& text) {
    return text.find(FIELD_DELIMITER) != std::string::npos;
}

std::string toLower(const std::string& text) {
    std::string lowered = text;
    for (char& ch : lowered) {
        ch = static_cast<char>(
            std::tolower(static_cast<unsigned char>(ch)));
    }
    return lowered;
}

std::vector<std::string> splitLine(const std::string& line,
                                   char delimiter) {
    // Loai bo ky tu '\r' cuoi dong (file CRLF) truoc khi tach de
    // truong cuoi cung khong dinh '\r'.
    const std::string cleaned =
        (!line.empty() && line.back() == '\r')
            ? line.substr(0, line.size() - 1)
            : line;
    std::vector<std::string> fields;
    std::size_t start = 0;
    while (true) {
        const std::size_t delimiterPos = cleaned.find(delimiter, start);
        if (delimiterPos == std::string::npos) {
            fields.push_back(cleaned.substr(start));
            break;
        }
        fields.push_back(cleaned.substr(start, delimiterPos - start));
        start = delimiterPos + 1;
    }
    return fields;
}
