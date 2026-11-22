#include "LibraryService.h"

#include <string>

#include "Date.h"
#include "Validation.h"

LibraryService::LibraryService(const std::string& dataDirectory)
    : m_dataDirectory(dataDirectory) {
}

bool LibraryService::loadAll() {
    if (!m_admins.loadFromFile(adminFilePath())) {
        return false;
    }
    if (!m_books.loadFromFile(bookFilePath())) {
        return false;
    }
    if (!m_readers.loadFromFile(readerFilePath())) {
        return false;
    }
    if (!m_loans.loadFromFile(loanFilePath())) {
        return false;
    }
    return true;
}

bool LibraryService::authenticate(const std::string& user, const std::string& password) const {
    const Admin* admin = m_admins.findByKey(user);
    if (admin == nullptr) {
        return false;
    }
    return admin->getPassword() == password;
}

ServiceResult LibraryService::addBook(const Book& book) {
    std::string errorMessage;
    if (!validateBook(book, errorMessage)) {
        return ServiceResult(false, errorMessage);
    }
    if (m_books.exists(book.getMaSach())) {
        return ServiceResult(false, "Sach da ton tai");
    }

    Book newBook = book;
    if (!newBook.setTinhTrang(BOOK_STATUS_AVAILABLE)) {
        return ServiceResult(false, "Tinh trang sach khong hop le");
    }
    if (!m_books.add(newBook)) {
        return ServiceResult(false, "Sach da ton tai");
    }
    if (!m_books.appendToFile(bookFilePath(), newBook)) {
        m_books.removeByKey(newBook.getMaSach());
        return ServiceResult(false, "Khong luu duoc file Sach.txt");
    }
    return ServiceResult(true, "Them sach thanh cong");
}

ServiceResult LibraryService::removeBook(const std::string& maSach) {
    const Book* found = m_books.findByKey(maSach);
    if (found == nullptr) {
        return ServiceResult(false, "Sach khong ton tai");
    }
    if (found->getTinhTrang() != BOOK_STATUS_AVAILABLE) {
        return ServiceResult(false, "Khong the xoa sach do sach dang duoc muon");
    }

    const Book backup = *found;
    if (!m_books.removeByKey(maSach)) {
        return ServiceResult(false, "Sach khong ton tai");
    }
    if (!m_books.saveAllToFile(bookFilePath())) {
        m_books.add(backup);
        return ServiceResult(false, "Khong luu duoc file Sach.txt");
    }
    return ServiceResult(true, "Xoa sach thanh cong");
}

const Book* LibraryService::findBookByCode(const std::string& maSach) const {
    return m_books.findByKey(maSach);
}

std::vector<const Book*> LibraryService::searchBooksByTitle(const std::string& keyword) const {
    const std::string loweredKeyword = Validation::toLower(keyword);
    return m_books.findWhere([&loweredKeyword](const Book& book) {
        if (loweredKeyword.empty()) {
            return true;
        }
        return Validation::toLower(book.getTenSach()).find(loweredKeyword) != std::string::npos;
    });
}

std::vector<const Book*> LibraryService::searchBooksByAuthor(const std::string& keyword) const {
    const std::string loweredKeyword = Validation::toLower(keyword);
    return m_books.findWhere([&loweredKeyword](const Book& book) {
        if (loweredKeyword.empty()) {
            return true;
        }
        return Validation::toLower(book.getTacGia()).find(loweredKeyword) != std::string::npos;
    });
}

const std::list<Book>& LibraryService::getAllBooks() const {
    return m_books.items();
}

ServiceResult LibraryService::addReader(const Reader& reader) {
    std::string errorMessage;
    if (!validateReader(reader, errorMessage)) {
        return ServiceResult(false, errorMessage);
    }
    if (m_readers.exists(reader.getMaBanDoc())) {
        return ServiceResult(false, "Ban doc da ton tai");
    }
    if (!m_readers.add(reader)) {
        return ServiceResult(false, "Ban doc da ton tai");
    }
    if (!m_readers.appendToFile(readerFilePath(), reader)) {
        m_readers.removeByKey(reader.getMaBanDoc());
        return ServiceResult(false, "Khong luu duoc file BanDoc.txt");
    }
    return ServiceResult(true, "Them ban doc thanh cong");
}

const Reader* LibraryService::findReaderByCode(const std::string& maBanDoc) const {
    return m_readers.findByKey(maBanDoc);
}

const std::list<Reader>& LibraryService::getAllReaders() const {
    return m_readers.items();
}

ServiceResult LibraryService::borrowBook(const std::string& maSach, const std::string& maBanDoc,
                                         int& outSoPhieuMuon) {
    Book* book = m_books.findMutable(maSach);
    if (book == nullptr) {
        return ServiceResult(false, "Sach khong ton tai");
    }
    if (book->getTinhTrang() != BOOK_STATUS_AVAILABLE) {
        return ServiceResult(false, "Sach dang duoc muon");
    }
    if (!m_readers.exists(maBanDoc)) {
        return ServiceResult(false, "Ban doc khong ton tai");
    }

    const int newLoanNumber = m_loans.maxKey() + 1;
    const Date borrowDate = Date::today();
    const Date dueDate = borrowDate.addDays(LOAN_DURATION_DAYS);
    const Loan newLoan(newLoanNumber, maBanDoc, maSach, borrowDate, dueDate, LOAN_STATUS_ACTIVE);

    if (!m_loans.add(newLoan)) {
        return ServiceResult(false, "Khong tao duoc phieu muon");
    }
    const int previousBookStatus = book->getTinhTrang();
    if (!book->setTinhTrang(newLoanNumber)) {
        m_loans.removeByKey(newLoanNumber);
        return ServiceResult(false, "Tinh trang sach khong hop le");
    }
    if (!m_books.saveAllToFile(bookFilePath())) {
        book->setTinhTrang(previousBookStatus);
        m_loans.removeByKey(newLoanNumber);
        return ServiceResult(false, "Khong luu duoc file Sach.txt");
    }
    if (!m_loans.appendToFile(loanFilePath(), newLoan)) {
        book->setTinhTrang(previousBookStatus);
        m_loans.removeByKey(newLoanNumber);
        return ServiceResult(false, "Khong luu duoc file PhieuMuon.txt");
    }

    outSoPhieuMuon = newLoanNumber;
    return ServiceResult(true, "Muon sach thanh cong");
}

ServiceResult LibraryService::returnBook(int soPhieuMuon) {
    Loan* loan = m_loans.findMutable(soPhieuMuon);
    if (loan == nullptr) {
        return ServiceResult(false, "Phieu muon khong ton tai");
    }
    if (loan->getTinhTrang() == LOAN_STATUS_RETURNED) {
        return ServiceResult(false, "Phieu muon da duoc tra truoc do");
    }
    Book* book = m_books.findMutable(loan->getMaSach());
    if (book == nullptr) {
        return ServiceResult(false, "Sach khong ton tai");
    }

    const int previousLoanStatus = loan->getTinhTrang();
    const int previousBookStatus = book->getTinhTrang();
    if (!loan->setTinhTrang(LOAN_STATUS_RETURNED) || !book->setTinhTrang(BOOK_STATUS_AVAILABLE)) {
        loan->setTinhTrang(previousLoanStatus);
        book->setTinhTrang(previousBookStatus);
        return ServiceResult(false, "Tinh trang khong hop le");
    }
    if (!m_books.saveAllToFile(bookFilePath())) {
        loan->setTinhTrang(previousLoanStatus);
        book->setTinhTrang(previousBookStatus);
        return ServiceResult(false, "Khong luu duoc file Sach.txt");
    }
    if (!m_loans.saveAllToFile(loanFilePath())) {
        loan->setTinhTrang(previousLoanStatus);
        book->setTinhTrang(previousBookStatus);
        return ServiceResult(false, "Khong luu duoc file PhieuMuon.txt");
    }
    return ServiceResult(true, "Tra sach thanh cong");
}

const Loan* LibraryService::findLoanByNumber(int soPhieuMuon) const {
    return m_loans.findByKey(soPhieuMuon);
}

const std::list<Loan>& LibraryService::getAllLoans() const {
    return m_loans.items();
}

const std::string& LibraryService::getDataDirectory() const {
    return m_dataDirectory;
}

std::string LibraryService::adminFilePath() const {
    return m_dataDirectory + PATH_SEPARATOR + ADMIN_FILE_NAME;
}

std::string LibraryService::bookFilePath() const {
    return m_dataDirectory + PATH_SEPARATOR + BOOK_FILE_NAME;
}

std::string LibraryService::readerFilePath() const {
    return m_dataDirectory + PATH_SEPARATOR + READER_FILE_NAME;
}

std::string LibraryService::loanFilePath() const {
    return m_dataDirectory + PATH_SEPARATOR + LOAN_FILE_NAME;
}

bool LibraryService::isValidDate(const Date& date) const {
    return date.isValid();
}

bool LibraryService::validateBook(const Book& book, std::string& errorMessage) const {
    if (Validation::trim(book.getMaSach()).empty()) {
        errorMessage = "Ma sach khong duoc de trong";
        return false;
    }
    if (Validation::containsDelimiter(book.getMaSach())) {
        errorMessage = "Ma sach khong duoc chua ky tu |";
        return false;
    }
    if (Validation::trim(book.getTenSach()).empty()) {
        errorMessage = "Ten sach khong duoc de trong";
        return false;
    }
    if (Validation::containsDelimiter(book.getTenSach())) {
        errorMessage = "Ten sach khong duoc chua ky tu |";
        return false;
    }
    if (Validation::trim(book.getTacGia()).empty()) {
        errorMessage = "Tac gia khong duoc de trong";
        return false;
    }
    if (Validation::containsDelimiter(book.getTacGia())) {
        errorMessage = "Tac gia khong duoc chua ky tu |";
        return false;
    }
    if (Validation::trim(book.getNhaXuatBan()).empty()) {
        errorMessage = "Nha xuat ban khong duoc de trong";
        return false;
    }
    if (Validation::containsDelimiter(book.getNhaXuatBan())) {
        errorMessage = "Nha xuat ban khong duoc chua ky tu |";
        return false;
    }
    if (book.getGiaBan() <= 0) {
        errorMessage = "Gia ban phai lon hon 0";
        return false;
    }
    const int currentYear = Date::today().getYear();
    if (book.getNamPhatHanh() < MIN_PUBLISH_YEAR || book.getNamPhatHanh() > currentYear) {
        errorMessage = "Nam phat hanh khong hop le";
        return false;
    }
    if (book.getSoTrang() <= 0) {
        errorMessage = "So trang phai lon hon 0";
        return false;
    }
    if (!isValidDate(book.getNgayNhapKho())) {
        errorMessage = "Ngay nhap kho khong hop le";
        return false;
    }
    if (Date::today() < book.getNgayNhapKho()) {
        errorMessage = "Ngay nhap kho khong duoc lon hon ngay hien tai";
        return false;
    }
    return true;
}

bool LibraryService::validateReader(const Reader& reader, std::string& errorMessage) const {
    if (Validation::trim(reader.getMaBanDoc()).empty()) {
        errorMessage = "Ma ban doc khong duoc de trong";
        return false;
    }
    if (Validation::containsDelimiter(reader.getMaBanDoc())) {
        errorMessage = "Ma ban doc khong duoc chua ky tu |";
        return false;
    }
    if (Validation::trim(reader.getHoTen()).empty()) {
        errorMessage = "Ho ten khong duoc de trong";
        return false;
    }
    if (Validation::containsDelimiter(reader.getHoTen())) {
        errorMessage = "Ho ten khong duoc chua ky tu |";
        return false;
    }
    if (!isValidDate(reader.getNgayDangKy())) {
        errorMessage = "Ngay dang ky khong hop le";
        return false;
    }
    if (Date::today() < reader.getNgayDangKy()) {
        errorMessage = "Ngay dang ky khong duoc lon hon ngay hien tai";
        return false;
    }
    return true;
}
