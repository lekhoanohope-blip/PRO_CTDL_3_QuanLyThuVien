#include "ConsoleUI.h"

#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace {

// Chieu rong khung tieu de (bao gom 2 dau '*' hai ben).
constexpr int HEADER_WIDTH = 30;
// Chieu rong phan chu ben trong khung tieu de.
constexpr int HEADER_INNER_WIDTH = 28;
// Thoi gian cho (ms) de phan biet phim ESC don voi chuoi phim mui ten (ESC [ A...).
constexpr int ESCAPE_SEQUENCE_TIMEOUT_MS = 50;
// Khoang ma ASCII cua ky tu in duoc.
constexpr int PRINTABLE_MIN = 32;
constexpr int PRINTABLE_MAX = 126;

// Do rong cac cot cua bang sach.
constexpr int BOOK_CODE_WIDTH = 8;
constexpr int BOOK_TITLE_WIDTH = 22;
constexpr int BOOK_AUTHOR_WIDTH = 16;
constexpr int BOOK_PUBLISHER_WIDTH = 12;
constexpr int BOOK_PRICE_WIDTH = 10;
constexpr int BOOK_YEAR_WIDTH = 6;
constexpr int BOOK_PAGE_WIDTH = 9;
constexpr int BOOK_DATE_WIDTH = 12;
constexpr int BOOK_STATUS_WIDTH = 16;

// Do rong cac cot cua bang ban doc.
constexpr int READER_CODE_WIDTH = 10;
constexpr int READER_NAME_WIDTH = 30;
constexpr int READER_DATE_WIDTH = 12;

// Do rong cac cot cua bang phieu muon.
constexpr int LOAN_NUMBER_WIDTH = 9;
constexpr int LOAN_READER_WIDTH = 12;
constexpr int LOAN_BOOK_WIDTH = 10;
constexpr int LOAN_BORROW_DATE_WIDTH = 12;
constexpr int LOAN_DUE_DATE_WIDTH = 13;
constexpr int LOAN_STATUS_WIDTH = 12;

/// Cat chuoi ve toi da width ky tu de giu bang khong vo cot.
std::string fitText(const std::string& text, std::size_t width) {
    if (text.size() <= width) {
        return text;
    }
    return text.substr(0, width);
}

/// In duong gach ngang dai bang tong do rong cac cot cong khoang phan cach.
void printSeparator(const std::vector<int>& widths) {
    int totalWidth = 0;
    for (const int width : widths) {
        totalWidth += width;
    }
    totalWidth += 3 * (static_cast<int>(widths.size()) - 1);
    std::cout << std::string(static_cast<std::size_t>(totalWidth), '-') << '\n';
}

}  // namespace

void ConsoleUI::setColor(ConsoleColor color) {
#ifdef _WIN32
    WORD attribute = 7;
    switch (color) {
        case ConsoleColor::Default:
            attribute = 7;
            break;
        case ConsoleColor::Green:
            attribute = 10;
            break;
        case ConsoleColor::Red:
            attribute = 12;
            break;
        case ConsoleColor::Yellow:
            attribute = 14;
            break;
        case ConsoleColor::Cyan:
            attribute = 11;
            break;
        case ConsoleColor::White:
            attribute = 15;
            break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
#else
    switch (color) {
        case ConsoleColor::Default:
            std::cout << "\033[0m";
            break;
        case ConsoleColor::Green:
            std::cout << "\033[32m";
            break;
        case ConsoleColor::Red:
            std::cout << "\033[31m";
            break;
        case ConsoleColor::Yellow:
            std::cout << "\033[33m";
            break;
        case ConsoleColor::Cyan:
            std::cout << "\033[36m";
            break;
        case ConsoleColor::White:
            std::cout << "\033[37m";
            break;
    }
    std::cout << std::flush;
#endif
}

void ConsoleUI::resetColor() {
    setColor(ConsoleColor::Default);
}

void ConsoleUI::clearScreen() {
#ifdef _WIN32
    (void)std::system("cls");
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}

void ConsoleUI::printHeader(const std::string& title) {
    setColor(ConsoleColor::Cyan);
    const std::string border(static_cast<std::size_t>(HEADER_WIDTH), '*');
    std::cout << border << '\n';
    const std::string text = fitText(title, static_cast<std::size_t>(HEADER_INNER_WIDTH));
    const std::size_t totalPad =
        static_cast<std::size_t>(HEADER_INNER_WIDTH) - text.size();
    const std::size_t leftPad = totalPad / 2;
    const std::size_t rightPad = totalPad - leftPad;
    std::cout << '*' << std::string(leftPad, ' ') << text << std::string(rightPad, ' ')
              << '*' << '\n';
    std::cout << border << '\n';
    resetColor();
}

void ConsoleUI::printSuccess(const std::string& message) {
    setColor(ConsoleColor::Green);
    std::cout << message << '\n';
    resetColor();
}

void ConsoleUI::printError(const std::string& message) {
    setColor(ConsoleColor::Red);
    std::cout << message << '\n';
    resetColor();
}

void ConsoleUI::printWarning(const std::string& message) {
    setColor(ConsoleColor::Yellow);
    std::cout << message << '\n';
    resetColor();
}

void ConsoleUI::printInfo(const std::string& message) {
    std::cout << message << '\n';
}

void ConsoleUI::pause() {
    std::cout << "Nhan phim bat ky de tiep tuc..." << std::flush;
    (void)readKey();
    std::cout << '\n';
}

int ConsoleUI::readKey() {
#ifdef _WIN32
    const int first = _getch();
    if (first == EOF) {
        return ESC_KEY;
    }
    if (first == 0 || first == 224) {
        // Phim chuc nang / phim mui ten: doc them ma quet roi bo qua.
        (void)_getch();
        return -1;
    }
    if (first == 13) {
        return '\n';
    }
    if (first == ESC_KEY || first == BACKSPACE_KEY) {
        return first;
    }
    if (first >= PRINTABLE_MIN && first <= PRINTABLE_MAX) {
        return first;
    }
    return -1;
#else
    struct termios oldSettings {};
    if (tcgetattr(STDIN_FILENO, &oldSettings) != 0) {
        // stdin khong phai terminal (vi du bi pipe): doc kieu dong bo thong thuong.
        const int fallback = std::cin.get();
        if (fallback == EOF) {
            return ESC_KEY;
        }
        if (fallback == '\n') {
            return '\n';
        }
        if (fallback == 127) {
            return BACKSPACE_KEY;
        }
        if (fallback >= PRINTABLE_MIN && fallback <= PRINTABLE_MAX) {
            return fallback;
        }
        return -1;
    }
    struct termios newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    newSettings.c_cc[VMIN] = 1;
    newSettings.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newSettings) != 0) {
        return -1;
    }
    unsigned char byte = 0;
    const ssize_t readCount = ::read(STDIN_FILENO, &byte, 1);
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    if (readCount == 0) {
        return ESC_KEY;
    }
    if (readCount != 1) {
        return -1;
    }
    if (byte == 27) {
        // Co the la ESC don hoac mo dau chuoi phim mui ten (ESC [ A...):
        // cho them mot chut roi kiem tra co byte tiep theo khong.
        struct pollfd pfd {};
        pfd.fd = STDIN_FILENO;
        pfd.events = POLLIN;
        const int ready = poll(&pfd, 1, ESCAPE_SEQUENCE_TIMEOUT_MS);
        if (ready <= 0) {
            return ESC_KEY;
        }
        // Bo het cac byte con lai cua chuoi escape roi bo qua phim nay.
        unsigned char discard = 0;
        while (poll(&pfd, 1, 0) > 0) {
            if (::read(STDIN_FILENO, &discard, 1) != 1) {
                break;
            }
        }
        return -1;
    }
    if (byte == 10) {
        return '\n';
    }
    if (byte == 127) {
        // Phim Backspace tren Linux thuong gui DEL (127).
        return BACKSPACE_KEY;
    }
    if (byte == BACKSPACE_KEY) {
        return BACKSPACE_KEY;
    }
    if (byte >= PRINTABLE_MIN && byte <= PRINTABLE_MAX) {
        return static_cast<int>(byte);
    }
    return -1;
#endif
}

bool ConsoleUI::readLine(const std::string& prompt, std::string& outValue) {
    outValue.clear();
    std::cout << prompt << std::flush;
    while (true) {
        const int key = readKey();
        if (key == -1) {
            continue;
        }
        if (key == ESC_KEY) {
            std::cout << '\n';
            return false;
        }
        if (key == '\n') {
            std::cout << '\n';
            return true;
        }
        if (key == BACKSPACE_KEY) {
            if (!outValue.empty()) {
                outValue.pop_back();
                std::cout << "\b \b" << std::flush;
            }
            continue;
        }
        outValue.push_back(static_cast<char>(key));
        std::cout << static_cast<char>(key) << std::flush;
    }
}

bool ConsoleUI::readPassword(const std::string& prompt, std::string& outValue) {
    outValue.clear();
    std::cout << prompt << std::flush;
    while (true) {
        const int key = readKey();
        if (key == -1) {
            continue;
        }
        if (key == ESC_KEY) {
            std::cout << '\n';
            return false;
        }
        if (key == '\n') {
            std::cout << '\n';
            return true;
        }
        if (key == BACKSPACE_KEY) {
            if (!outValue.empty()) {
                outValue.pop_back();
                std::cout << "\b \b" << std::flush;
            }
            continue;
        }
        outValue.push_back(static_cast<char>(key));
        std::cout << '*' << std::flush;
    }
}

void ConsoleUI::printBookTable(const std::list<Book>& books) {
    if (books.empty()) {
        printInfo("Chua co sach nao.");
        return;
    }
    const std::vector<int> widths = {BOOK_CODE_WIDTH, BOOK_TITLE_WIDTH,
                                     BOOK_AUTHOR_WIDTH, BOOK_PUBLISHER_WIDTH,
                                     BOOK_PRICE_WIDTH, BOOK_YEAR_WIDTH,
                                     BOOK_PAGE_WIDTH, BOOK_DATE_WIDTH,
                                     BOOK_STATUS_WIDTH};
    std::cout << std::left << std::setw(BOOK_CODE_WIDTH) << "Ma sach" << " | "
              << std::setw(BOOK_TITLE_WIDTH) << "Ten sach" << " | "
              << std::setw(BOOK_AUTHOR_WIDTH) << "Tac gia" << " | "
              << std::setw(BOOK_PUBLISHER_WIDTH) << "NXB" << " | "
              << std::right << std::setw(BOOK_PRICE_WIDTH) << "Gia ban" << " | "
              << std::setw(BOOK_YEAR_WIDTH) << "Nam" << " | "
              << std::setw(BOOK_PAGE_WIDTH) << "So trang" << " | "
              << std::left << std::setw(BOOK_DATE_WIDTH) << "Ngay nhap" << " | "
              << std::setw(BOOK_STATUS_WIDTH) << "Tinh trang" << '\n';
    printSeparator(widths);
    for (const Book& book : books) {
        std::cout << std::left << std::setw(BOOK_CODE_WIDTH)
                  << fitText(book.getMaSach(), static_cast<std::size_t>(BOOK_CODE_WIDTH))
                  << " | " << std::setw(BOOK_TITLE_WIDTH)
                  << fitText(book.getTenSach(), static_cast<std::size_t>(BOOK_TITLE_WIDTH))
                  << " | " << std::setw(BOOK_AUTHOR_WIDTH)
                  << fitText(book.getTacGia(), static_cast<std::size_t>(BOOK_AUTHOR_WIDTH))
                  << " | " << std::setw(BOOK_PUBLISHER_WIDTH)
                  << fitText(book.getNhaXuatBan(),
                             static_cast<std::size_t>(BOOK_PUBLISHER_WIDTH))
                  << " | " << std::right << std::setw(BOOK_PRICE_WIDTH) << book.getGiaBan()
                  << " | " << std::setw(BOOK_YEAR_WIDTH) << book.getNamPhatHanh() << " | "
                  << std::setw(BOOK_PAGE_WIDTH) << book.getSoTrang() << " | " << std::left
                  << std::setw(BOOK_DATE_WIDTH) << book.getNgayNhapKho().toString() << " | ";
        if (book.getTinhTrang() == 0) {
            setColor(ConsoleColor::Green);
            std::cout << std::left << std::setw(BOOK_STATUS_WIDTH) << "San sang";
        } else {
            // In day du "Dang muon (phieu N)" ke ca khi dai hon cot 16 ky tu,
            // de khong lam mat so phieu muon quan trong.
            setColor(ConsoleColor::Yellow);
            std::cout << std::left << "Dang muon (phieu " << book.getTinhTrang() << ")";
        }
        resetColor();
        std::cout << '\n';
    }
}

void ConsoleUI::printReaderTable(const std::list<Reader>& readers) {
    if (readers.empty()) {
        printInfo("Chua co ban doc nao.");
        return;
    }
    const std::vector<int> widths = {READER_CODE_WIDTH, READER_NAME_WIDTH,
                                     READER_DATE_WIDTH};
    std::cout << std::left << std::setw(READER_CODE_WIDTH) << "Ma ban doc" << " | "
              << std::setw(READER_NAME_WIDTH) << "Ho ten" << " | "
              << std::setw(READER_DATE_WIDTH) << "Ngay dang ky" << '\n';
    printSeparator(widths);
    for (const Reader& reader : readers) {
        std::cout << std::left << std::setw(READER_CODE_WIDTH)
                  << fitText(reader.getMaBanDoc(),
                             static_cast<std::size_t>(READER_CODE_WIDTH))
                  << " | " << std::setw(READER_NAME_WIDTH)
                  << fitText(reader.getHoTen(), static_cast<std::size_t>(READER_NAME_WIDTH))
                  << " | " << std::setw(READER_DATE_WIDTH)
                  << reader.getNgayDangKy().toString() << '\n';
    }
}

void ConsoleUI::printLoanTable(const std::list<Loan>& loans) {
    if (loans.empty()) {
        printInfo("Chua co phieu muon nao.");
        return;
    }
    const std::vector<int> widths = {LOAN_NUMBER_WIDTH, LOAN_READER_WIDTH,
                                     LOAN_BOOK_WIDTH, LOAN_BORROW_DATE_WIDTH,
                                     LOAN_DUE_DATE_WIDTH, LOAN_STATUS_WIDTH};
    std::cout << std::left << std::setw(LOAN_NUMBER_WIDTH) << "So phieu" << " | "
              << std::setw(LOAN_READER_WIDTH) << "Ma ban doc" << " | "
              << std::setw(LOAN_BOOK_WIDTH) << "Ma sach" << " | "
              << std::setw(LOAN_BORROW_DATE_WIDTH) << "Ngay muon" << " | "
              << std::setw(LOAN_DUE_DATE_WIDTH) << "Ngay phai tra" << " | "
              << std::setw(LOAN_STATUS_WIDTH) << "Tinh trang" << '\n';
    printSeparator(widths);
    for (const Loan& loan : loans) {
        std::cout << std::right << std::setw(LOAN_NUMBER_WIDTH) << loan.getSoPhieuMuon()
                  << " | " << std::left << std::setw(LOAN_READER_WIDTH)
                  << fitText(loan.getMaBanDoc(), static_cast<std::size_t>(LOAN_READER_WIDTH))
                  << " | " << std::setw(LOAN_BOOK_WIDTH)
                  << fitText(loan.getMaSach(), static_cast<std::size_t>(LOAN_BOOK_WIDTH))
                  << " | " << std::setw(LOAN_BORROW_DATE_WIDTH)
                  << loan.getNgayMuon().toString() << " | "
                  << std::setw(LOAN_DUE_DATE_WIDTH) << loan.getNgayPhaiTra().toString()
                  << " | ";
        if (loan.getTinhTrang() == 0) {
            setColor(ConsoleColor::Green);
            std::cout << std::left << std::setw(LOAN_STATUS_WIDTH) << "Da tra";
        } else {
            setColor(ConsoleColor::Yellow);
            std::cout << std::left << std::setw(LOAN_STATUS_WIDTH) << "Dang muon";
        }
        resetColor();
        std::cout << '\n';
    }
}

bool ConsoleUI::askContinue() {
    std::cout << "[1] Tiep tuc chuc nang khac  [0] Thoat chuong trinh\n";
    int choice = 0;
    if (!readValidatedField<int>("Lua chon: ",
                                 "Lua chon khong hop le. Vui long nhap 0 hoac 1.", 0, 1,
                                 choice)) {
        return false;
    }
    return choice == 1;
}
