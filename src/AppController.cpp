#include "AppController.h"

#include <list>
#include <string>
#include <vector>

#include "ConsoleUI.h"
#include "Date.h"
#include "Models.h"
#include "Validation.h"

namespace {

// Gioi han lua chon cua menu chinh.
constexpr int MAIN_MENU_MIN = 0;
constexpr int MAIN_MENU_MAX = 3;
// Gioi han lua chon cua menu Quan li sach.
constexpr int BOOK_MENU_MIN = 0;
constexpr int BOOK_MENU_MAX = 4;
// Gioi han lua chon cua menu Quan li phieu muon.
constexpr int LOAN_MENU_MIN = 0;
constexpr int LOAN_MENU_MAX = 3;
// Gioi han lua chon cua menu Quan li ban doc.
constexpr int READER_MENU_MIN = 0;
constexpr int READER_MENU_MAX = 2;
// Gioi han lua chon cua menu tim kiem sach.
constexpr int SEARCH_MENU_MIN = 1;
constexpr int SEARCH_MENU_MAX = 3;

// Rang buoc mien gia tri khi nhap sach (MIN_PUBLISH_YEAR va
// BOOK_STATUS_AVAILABLE lay tu LibraryService.h).
constexpr int MAX_PAGE_COUNT = 100000;
constexpr long long MIN_PRICE = 1LL;
constexpr long long MAX_PRICE = 1000000000LL;
// Rang buoc mien gia tri khi nhap so phieu muon.
constexpr int MIN_LOAN_NUMBER = 1;
constexpr int MAX_LOAN_NUMBER = 1000000000;

}  // namespace

AppController::AppController(LibraryService& service) : m_service(service) {
}

void AppController::run() {
    if (!loginFlow()) {
        return;
    }
    mainMenuLoop();
    ConsoleUI::printInfo("Tam biet!");
}

bool AppController::loginFlow() {
    int failedAttempts = 0;
    while (failedAttempts < MAX_LOGIN_ATTEMPTS) {
        ConsoleUI::clearScreen();
        ConsoleUI::printHeader("DANG NHAP HE THONG");
        std::string user;
        if (!ConsoleUI::readLine("User: ", user)) {
            return false;
        }
        std::string password;
        if (!ConsoleUI::readPassword("Password: ", password)) {
            return false;
        }
        if (m_service.authenticate(user, password)) {
            ConsoleUI::printSuccess("Dang nhap thanh cong!");
            ConsoleUI::pause();
            return true;
        }
        ++failedAttempts;
        ConsoleUI::printError("Sai user hoac password. Vui long nhap lai.");
        ConsoleUI::pause();
    }
    ConsoleUI::printError("Nhap sai " + std::to_string(MAX_LOGIN_ATTEMPTS) +
                          " lan. Thoat chuong trinh.");
    return false;
}

void AppController::mainMenuLoop() {
    while (!m_exitRequested) {
        ConsoleUI::clearScreen();
        ConsoleUI::printHeader("MENU CHINH");
        ConsoleUI::printInfo("1. Quan li sach");
        ConsoleUI::printInfo("2. Quan li phieu muon");
        ConsoleUI::printInfo("3. Quan li ban doc");
        ConsoleUI::printInfo("0. Thoat chuong trinh");
        int choice = 0;
        if (!ConsoleUI::readValidatedField<int>(
                "Lua chon: ", "Lua chon khong hop le. Vui long nhap tu 0 den 3.",
                MAIN_MENU_MIN, MAIN_MENU_MAX, choice)) {
            return;
        }
        switch (choice) {
            case 1:
                bookMenu();
                break;
            case 2:
                loanMenu();
                break;
            case 3:
                readerMenu();
                break;
            case 0:
                return;
            default:
                break;
        }
    }
}

void AppController::bookMenu() {
    while (!m_exitRequested) {
        ConsoleUI::clearScreen();
        ConsoleUI::printHeader("QUAN LI SACH");
        ConsoleUI::printInfo("1. Hien thi danh sach sach");
        ConsoleUI::printInfo("2. Them sach");
        ConsoleUI::printInfo("3. Xoa sach");
        ConsoleUI::printInfo("4. Tim kiem sach");
        ConsoleUI::printInfo("0. Quay lai");
        int choice = 0;
        if (!ConsoleUI::readValidatedField<int>(
                "Lua chon: ", "Lua chon khong hop le. Vui long nhap tu 0 den 4.",
                BOOK_MENU_MIN, BOOK_MENU_MAX, choice)) {
            return;
        }
        if (choice == 0) {
            return;
        }
        switch (choice) {
            case 1:
                showAllBooks();
                break;
            case 2:
                addBookFlow();
                break;
            case 3:
                removeBookFlow();
                break;
            case 4:
                searchBookFlow();
                break;
            default:
                break;
        }
        if (!askContinueOrExit()) {
            return;
        }
    }
}

void AppController::loanMenu() {
    while (!m_exitRequested) {
        ConsoleUI::clearScreen();
        ConsoleUI::printHeader("QUAN LI PHIEU MUON");
        ConsoleUI::printInfo("1. Hien thi danh sach phieu muon");
        ConsoleUI::printInfo("2. Muon sach");
        ConsoleUI::printInfo("3. Tra sach");
        ConsoleUI::printInfo("0. Quay lai");
        int choice = 0;
        if (!ConsoleUI::readValidatedField<int>(
                "Lua chon: ", "Lua chon khong hop le. Vui long nhap tu 0 den 3.",
                LOAN_MENU_MIN, LOAN_MENU_MAX, choice)) {
            return;
        }
        if (choice == 0) {
            return;
        }
        switch (choice) {
            case 1:
                showAllLoans();
                break;
            case 2:
                borrowBookFlow();
                break;
            case 3:
                returnBookFlow();
                break;
            default:
                break;
        }
        if (!askContinueOrExit()) {
            return;
        }
    }
}

void AppController::readerMenu() {
    while (!m_exitRequested) {
        ConsoleUI::clearScreen();
        ConsoleUI::printHeader("QUAN LI BAN DOC");
        ConsoleUI::printInfo("1. Hien thi danh sach ban doc");
        ConsoleUI::printInfo("2. Them ban doc");
        ConsoleUI::printInfo("0. Quay lai");
        int choice = 0;
        if (!ConsoleUI::readValidatedField<int>(
                "Lua chon: ", "Lua chon khong hop le. Vui long nhap tu 0 den 2.",
                READER_MENU_MIN, READER_MENU_MAX, choice)) {
            return;
        }
        if (choice == 0) {
            return;
        }
        switch (choice) {
            case 1:
                showAllReaders();
                break;
            case 2:
                addReaderFlow();
                break;
            default:
                break;
        }
        if (!askContinueOrExit()) {
            return;
        }
    }
}

void AppController::showAllBooks() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("DANH SACH SACH");
    ConsoleUI::printBookTable(m_service.getAllBooks());
    ConsoleUI::pause();
}

void AppController::addBookFlow() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("THEM SACH MOI");
    std::string maSach;
    if (!ConsoleUI::readLine("Ma sach: ", maSach)) {
        ConsoleUI::printInfo("Da huy thao tac them sach.");
        return;
    }
    maSach = Validation::trim(maSach);
    if (maSach.empty()) {
        ConsoleUI::printError("Ma sach khong duoc de trong.");
        return;
    }
    std::string tenSach;
    if (!ConsoleUI::readLine("Ten sach: ", tenSach)) {
        ConsoleUI::printInfo("Da huy thao tac them sach.");
        return;
    }
    tenSach = Validation::trim(tenSach);
    if (tenSach.empty()) {
        ConsoleUI::printError("Ten sach khong duoc de trong.");
        return;
    }
    std::string tacGia;
    if (!ConsoleUI::readLine("Tac gia: ", tacGia)) {
        ConsoleUI::printInfo("Da huy thao tac them sach.");
        return;
    }
    tacGia = Validation::trim(tacGia);
    if (tacGia.empty()) {
        ConsoleUI::printError("Tac gia khong duoc de trong.");
        return;
    }
    std::string nhaXuatBan;
    if (!ConsoleUI::readLine("Nha xuat ban: ", nhaXuatBan)) {
        ConsoleUI::printInfo("Da huy thao tac them sach.");
        return;
    }
    nhaXuatBan = Validation::trim(nhaXuatBan);
    if (nhaXuatBan.empty()) {
        ConsoleUI::printError("Nha xuat ban khong duoc de trong.");
        return;
    }
    long long giaBan = 0;
    if (!ConsoleUI::readValidatedField<long long>(
            "Gia ban: ", "Gia ban phai la so nguyen tu 1 den 1000000000.", MIN_PRICE,
            MAX_PRICE, giaBan)) {
        ConsoleUI::printInfo("Da huy thao tac them sach.");
        return;
    }
    const int currentYear = Date::today().getYear();
    int namPhatHanh = 0;
    if (!ConsoleUI::readValidatedField<int>(
            "Nam phat hanh: ", "Nam phat hanh phai tu 1000 den nam hien tai.",
            MIN_PUBLISH_YEAR, currentYear, namPhatHanh)) {
        ConsoleUI::printInfo("Da huy thao tac them sach.");
        return;
    }
    int soTrang = 0;
    if (!ConsoleUI::readValidatedField<int>(
            "So trang: ", "So trang phai la so nguyen tu 1 den 100000.", 1,
            MAX_PAGE_COUNT, soTrang)) {
        ConsoleUI::printInfo("Da huy thao tac them sach.");
        return;
    }
    std::string rawNgayNhap;
    if (!ConsoleUI::readLine("Ngay nhap kho (dd/mm/yyyy, Enter = hom nay): ",
                             rawNgayNhap)) {
        ConsoleUI::printInfo("Da huy thao tac them sach.");
        return;
    }
    Date ngayNhapKho = Date::today();
    if (!Validation::trim(rawNgayNhap).empty()) {
        if (!Date::fromString(Validation::trim(rawNgayNhap), ngayNhapKho)) {
            ConsoleUI::printError("Ngay nhap kho khong hop le (dinh dang dd/mm/yyyy).");
            return;
        }
        if (Date::today() < ngayNhapKho) {
            ConsoleUI::printError("Ngay nhap kho khong duoc lon hon ngay hien tai.");
            return;
        }
    }
    const Book book(maSach, tenSach, tacGia, nhaXuatBan, giaBan, namPhatHanh, soTrang,
                    ngayNhapKho, BOOK_STATUS_AVAILABLE);
    const ServiceResult result = m_service.addBook(book);
    if (result.success) {
        ConsoleUI::printSuccess(result.message);
    } else {
        ConsoleUI::printError(result.message);
    }
}

void AppController::removeBookFlow() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("XOA SACH");
    std::string maSach;
    if (!ConsoleUI::readLine("Ma sach can xoa: ", maSach)) {
        ConsoleUI::printInfo("Da huy thao tac xoa sach.");
        return;
    }
    maSach = Validation::trim(maSach);
    if (maSach.empty()) {
        ConsoleUI::printError("Ma sach khong duoc de trong.");
        return;
    }
    const ServiceResult result = m_service.removeBook(maSach);
    if (result.success) {
        ConsoleUI::printSuccess(result.message);
    } else {
        ConsoleUI::printError(result.message);
    }
}

void AppController::searchBookFlow() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("TIM KIEM SACH");
    ConsoleUI::printInfo("1. Tim theo Ma sach");
    ConsoleUI::printInfo("2. Tim theo Ten sach");
    ConsoleUI::printInfo("3. Tim theo Tac gia");
    int choice = 0;
    if (!ConsoleUI::readValidatedField<int>(
            "Lua chon: ", "Lua chon khong hop le. Vui long nhap tu 1 den 3.",
            SEARCH_MENU_MIN, SEARCH_MENU_MAX, choice)) {
        ConsoleUI::printInfo("Da huy thao tac tim kiem sach.");
        return;
    }
    if (choice == 1) {
        std::string maSach;
        if (!ConsoleUI::readLine("Nhap Ma sach: ", maSach)) {
            ConsoleUI::printInfo("Da huy thao tac tim kiem sach.");
            return;
        }
        const Book* found = m_service.findBookByCode(Validation::trim(maSach));
        if (found == nullptr) {
            ConsoleUI::printInfo("Khong tim thay sach nao.");
            return;
        }
        const std::list<Book> singleBook{*found};
        ConsoleUI::printBookTable(singleBook);
        return;
    }
    std::string keyword;
    const std::string prompt =
        (choice == 2) ? "Nhap Ten sach: " : "Nhap Tac gia: ";
    if (!ConsoleUI::readLine(prompt, keyword)) {
        ConsoleUI::printInfo("Da huy thao tac tim kiem sach.");
        return;
    }
    const std::vector<const Book*> results =
        (choice == 2) ? m_service.searchBooksByTitle(Validation::trim(keyword))
                      : m_service.searchBooksByAuthor(Validation::trim(keyword));
    if (results.empty()) {
        ConsoleUI::printInfo("Khong tim thay sach nao.");
        return;
    }
    std::list<Book> books;
    for (const Book* book : results) {
        books.push_back(*book);
    }
    ConsoleUI::printBookTable(books);
}

void AppController::showAllLoans() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("DANH SACH PHIEU MUON");
    ConsoleUI::printLoanTable(m_service.getAllLoans());
    ConsoleUI::pause();
}

void AppController::borrowBookFlow() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("MUON SACH");
    std::string maSach;
    if (!ConsoleUI::readLine("Ma sach: ", maSach)) {
        ConsoleUI::printInfo("Da huy thao tac muon sach.");
        return;
    }
    maSach = Validation::trim(maSach);
    if (maSach.empty()) {
        ConsoleUI::printError("Ma sach khong duoc de trong.");
        return;
    }
    std::string maBanDoc;
    if (!ConsoleUI::readLine("Ma ban doc: ", maBanDoc)) {
        ConsoleUI::printInfo("Da huy thao tac muon sach.");
        return;
    }
    maBanDoc = Validation::trim(maBanDoc);
    if (maBanDoc.empty()) {
        ConsoleUI::printError("Ma ban doc khong duoc de trong.");
        return;
    }
    int soPhieuMuon = 0;
    const ServiceResult result = m_service.borrowBook(maSach, maBanDoc, soPhieuMuon);
    if (result.success) {
        ConsoleUI::printSuccess("Muon sach thanh cong. So phieu muon: " +
                                std::to_string(soPhieuMuon));
    } else {
        ConsoleUI::printError(result.message);
    }
}

void AppController::returnBookFlow() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("TRA SACH");
    int soPhieuMuon = 0;
    if (!ConsoleUI::readValidatedField<int>(
            "So phieu muon: ", "So phieu muon phai la so nguyen tu 1 den 1000000000.",
            MIN_LOAN_NUMBER, MAX_LOAN_NUMBER, soPhieuMuon)) {
        ConsoleUI::printInfo("Da huy thao tac tra sach.");
        return;
    }
    const ServiceResult result = m_service.returnBook(soPhieuMuon);
    if (result.success) {
        ConsoleUI::printSuccess(result.message);
    } else {
        ConsoleUI::printError(result.message);
    }
}

void AppController::showAllReaders() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("DANH SACH BAN DOC");
    ConsoleUI::printReaderTable(m_service.getAllReaders());
    ConsoleUI::pause();
}

void AppController::addReaderFlow() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("THEM BAN DOC MOI");
    std::string maBanDoc;
    if (!ConsoleUI::readLine("Ma ban doc: ", maBanDoc)) {
        ConsoleUI::printInfo("Da huy thao tac them ban doc.");
        return;
    }
    maBanDoc = Validation::trim(maBanDoc);
    if (maBanDoc.empty()) {
        ConsoleUI::printError("Ma ban doc khong duoc de trong.");
        return;
    }
    std::string hoTen;
    if (!ConsoleUI::readLine("Ho ten: ", hoTen)) {
        ConsoleUI::printInfo("Da huy thao tac them ban doc.");
        return;
    }
    hoTen = Validation::trim(hoTen);
    if (hoTen.empty()) {
        ConsoleUI::printError("Ho ten khong duoc de trong.");
        return;
    }
    std::string rawNgayDangKy;
    if (!ConsoleUI::readLine("Ngay dang ky (dd/mm/yyyy, Enter = hom nay): ",
                             rawNgayDangKy)) {
        ConsoleUI::printInfo("Da huy thao tac them ban doc.");
        return;
    }
    Date ngayDangKy = Date::today();
    if (!Validation::trim(rawNgayDangKy).empty()) {
        if (!Date::fromString(Validation::trim(rawNgayDangKy), ngayDangKy)) {
            ConsoleUI::printError("Ngay dang ky khong hop le (dinh dang dd/mm/yyyy).");
            return;
        }
        if (Date::today() < ngayDangKy) {
            ConsoleUI::printError("Ngay dang ky khong duoc lon hon ngay hien tai.");
            return;
        }
    }
    const Reader reader(maBanDoc, hoTen, ngayDangKy);
    const ServiceResult result = m_service.addReader(reader);
    if (result.success) {
        ConsoleUI::printSuccess(result.message);
    } else {
        ConsoleUI::printError(result.message);
    }
}

bool AppController::askContinueOrExit() {
    if (ConsoleUI::askContinue()) {
        return true;
    }
    m_exitRequested = true;
    return false;
}
