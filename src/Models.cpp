#include "Models.h"

#include <limits>
#include <vector>

#include "Validation.h"

namespace {

/// So truong cua moi dong trong cac file du lieu .txt.
constexpr std::size_t ADMIN_FIELD_COUNT = 2;
constexpr std::size_t BOOK_FIELD_COUNT = 9;
constexpr std::size_t READER_FIELD_COUNT = 3;
constexpr std::size_t LOAN_FIELD_COUNT = 6;

/// Gia tri nho nhat cho cac truong so.
constexpr long long MIN_PRICE = 1;
constexpr int MIN_PUBLICATION_YEAR = 1000;
constexpr int MIN_PAGE_COUNT = 1;
constexpr int MIN_STATUS_VALUE = 0;
constexpr int MIN_LOAN_NUMBER = 1;

/// Trang thai phieu muon: 0 = da tra, 1 = dang muon.
constexpr int LOAN_STATUS_RETURNED = 0;
constexpr int LOAN_STATUS_BORROWED = 1;

/// Kiem tra chuoi truong van ban hop le: khong rong va khong chua '|'.
bool isValidTextField(const std::string& value) {
    return !isBlank(value) && !containsDelimiter(value);
}

}  // namespace

// ---------------------------------------------------------------- ServiceResult

ServiceResult::ServiceResult(bool ok, const std::string& text)
    : success(ok), message(text) {
}

ServiceResult ServiceResult::ok(const std::string& message) {
    ServiceResult result;
    result.success = true;
    result.message = message;
    return result;
}

ServiceResult ServiceResult::fail(const std::string& message) {
    ServiceResult result;
    result.success = false;
    result.message = message;
    return result;
}

// ---------------------------------------------------------------- Admin

Admin::Admin() : m_user(""), m_password("") {
}

Admin::Admin(const std::string& user, const std::string& password)
    : m_user(trim(user)), m_password(trim(password)) {
}

const std::string& Admin::getUser() const {
    return m_user;
}

const std::string& Admin::getPassword() const {
    return m_password;
}

bool Admin::setUser(const std::string& user) {
    if (!isValidTextField(user)) {
        return false;
    }
    m_user = user;
    return true;
}

bool Admin::setPassword(const std::string& password) {
    if (!isValidTextField(password)) {
        return false;
    }
    m_password = password;
    return true;
}

std::string Admin::getKey() const {
    return m_user;
}

bool Admin::isValid() const {
    return isValidTextField(m_user) && isValidTextField(m_password);
}

std::string Admin::serialize() const {
    return m_user + FIELD_DELIMITER + m_password;
}

std::string Admin::getEntityType() const {
    return "Admin";
}

bool Admin::fromFileLine(const std::string& line, Admin& outAdmin) {
    const std::vector<std::string> fields = splitLine(line, FIELD_DELIMITER);
    if (fields.size() != ADMIN_FIELD_COUNT) {
        return false;
    }
    const std::string user = trim(fields[0]);
    const std::string password = trim(fields[1]);
    if (!isValidTextField(user) || !isValidTextField(password)) {
        return false;
    }
    outAdmin = Admin(user, password);
    return true;
}

// ---------------------------------------------------------------- Book

Book::Book()
    : m_maSach(""),
      m_tenSach(""),
      m_tacGia(""),
      m_nhaXuatBan(""),
      m_giaBan(0),
      m_namPhatHanh(0),
      m_soTrang(0),
      m_ngayNhapKho(),
      m_tinhTrang(0) {
}

Book::Book(const std::string& maSach, const std::string& tenSach,
           const std::string& tacGia, const std::string& nhaXuatBan,
           long long giaBan, int namPhatHanh, int soTrang,
           const Date& ngayNhapKho, int tinhTrang)
    : m_maSach(trim(maSach)),
      m_tenSach(trim(tenSach)),
      m_tacGia(trim(tacGia)),
      m_nhaXuatBan(trim(nhaXuatBan)),
      m_giaBan(giaBan),
      m_namPhatHanh(namPhatHanh),
      m_soTrang(soTrang),
      m_ngayNhapKho(ngayNhapKho),
      m_tinhTrang(tinhTrang) {
}

const std::string& Book::getMaSach() const {
    return m_maSach;
}

const std::string& Book::getTenSach() const {
    return m_tenSach;
}

const std::string& Book::getTacGia() const {
    return m_tacGia;
}

const std::string& Book::getNhaXuatBan() const {
    return m_nhaXuatBan;
}

long long Book::getGiaBan() const {
    return m_giaBan;
}

int Book::getNamPhatHanh() const {
    return m_namPhatHanh;
}

int Book::getSoTrang() const {
    return m_soTrang;
}

const Date& Book::getNgayNhapKho() const {
    return m_ngayNhapKho;
}

int Book::getTinhTrang() const {
    return m_tinhTrang;
}

bool Book::setMaSach(const std::string& maSach) {
    if (!isValidTextField(maSach)) {
        return false;
    }
    m_maSach = maSach;
    return true;
}

bool Book::setTenSach(const std::string& tenSach) {
    if (!isValidTextField(tenSach)) {
        return false;
    }
    m_tenSach = tenSach;
    return true;
}

bool Book::setTacGia(const std::string& tacGia) {
    if (!isValidTextField(tacGia)) {
        return false;
    }
    m_tacGia = tacGia;
    return true;
}

bool Book::setNhaXuatBan(const std::string& nhaXuatBan) {
    if (!isValidTextField(nhaXuatBan)) {
        return false;
    }
    m_nhaXuatBan = nhaXuatBan;
    return true;
}

bool Book::setGiaBan(long long giaBan) {
    if (giaBan < MIN_PRICE) {
        return false;
    }
    m_giaBan = giaBan;
    return true;
}

bool Book::setNamPhatHanh(int namPhatHanh) {
    const int currentYear = Date::today().getYear();
    if (namPhatHanh < MIN_PUBLICATION_YEAR || namPhatHanh > currentYear) {
        return false;
    }
    m_namPhatHanh = namPhatHanh;
    return true;
}

bool Book::setSoTrang(int soTrang) {
    if (soTrang < MIN_PAGE_COUNT) {
        return false;
    }
    m_soTrang = soTrang;
    return true;
}

bool Book::setNgayNhapKho(const Date& ngayNhapKho) {
    if (!ngayNhapKho.isValid() || Date::today() < ngayNhapKho) {
        return false;
    }
    m_ngayNhapKho = ngayNhapKho;
    return true;
}

bool Book::setTinhTrang(int tinhTrang) {
    if (tinhTrang < MIN_STATUS_VALUE) {
        return false;
    }
    m_tinhTrang = tinhTrang;
    return true;
}

std::string Book::getKey() const {
    return m_maSach;
}

bool Book::isValid() const {
    if (!isValidTextField(m_maSach) || !isValidTextField(m_tenSach) ||
        !isValidTextField(m_tacGia) || !isValidTextField(m_nhaXuatBan)) {
        return false;
    }
    if (m_giaBan < MIN_PRICE || m_soTrang < MIN_PAGE_COUNT ||
        m_tinhTrang < MIN_STATUS_VALUE) {
        return false;
    }
    if (m_namPhatHanh < MIN_PUBLICATION_YEAR ||
        m_namPhatHanh > Date::today().getYear()) {
        return false;
    }
    if (!m_ngayNhapKho.isValid() || Date::today() < m_ngayNhapKho) {
        return false;
    }
    return true;
}

std::string Book::serialize() const {
    return m_maSach + FIELD_DELIMITER + m_tenSach + FIELD_DELIMITER +
           m_tacGia + FIELD_DELIMITER + m_nhaXuatBan + FIELD_DELIMITER +
           std::to_string(m_giaBan) + FIELD_DELIMITER +
           std::to_string(m_namPhatHanh) + FIELD_DELIMITER +
           std::to_string(m_soTrang) + FIELD_DELIMITER +
           m_ngayNhapKho.toString() + FIELD_DELIMITER +
           std::to_string(m_tinhTrang);
}

std::string Book::getEntityType() const {
    return "Book";
}

bool Book::fromFileLine(const std::string& line, Book& outBook) {
    const std::vector<std::string> fields = splitLine(line, FIELD_DELIMITER);
    if (fields.size() != BOOK_FIELD_COUNT) {
        return false;
    }
    const std::string maSach = trim(fields[0]);
    const std::string tenSach = trim(fields[1]);
    const std::string tacGia = trim(fields[2]);
    const std::string nhaXuatBan = trim(fields[3]);
    if (!isValidTextField(maSach) || !isValidTextField(tenSach) ||
        !isValidTextField(tacGia) || !isValidTextField(nhaXuatBan)) {
        return false;
    }
    long long giaBan = 0;
    if (!parseNumberInRange<long long>(
            trim(fields[4]), MIN_PRICE,
            std::numeric_limits<long long>::max(), giaBan)) {
        return false;
    }
    int namPhatHanh = 0;
    if (!parseNumberInRange<int>(trim(fields[5]), MIN_PUBLICATION_YEAR,
                                 Date::today().getYear(), namPhatHanh)) {
        return false;
    }
    int soTrang = 0;
    if (!parseNumberInRange<int>(trim(fields[6]), MIN_PAGE_COUNT,
                                 std::numeric_limits<int>::max(), soTrang)) {
        return false;
    }
    Date ngayNhapKho;
    if (!Date::fromString(trim(fields[7]), ngayNhapKho)) {
        return false;
    }
    if (Date::today() < ngayNhapKho) {
        return false;
    }
    int tinhTrang = 0;
    if (!parseNumberInRange<int>(trim(fields[8]), MIN_STATUS_VALUE,
                                 std::numeric_limits<int>::max(), tinhTrang)) {
        return false;
    }
    outBook = Book(maSach, tenSach, tacGia, nhaXuatBan, giaBan, namPhatHanh,
                   soTrang, ngayNhapKho, tinhTrang);
    return true;
}

// ---------------------------------------------------------------- Reader

Reader::Reader() : m_maBanDoc(""), m_hoTen(""), m_ngayDangKy() {
}

Reader::Reader(const std::string& maBanDoc, const std::string& hoTen,
               const Date& ngayDangKy)
    : m_maBanDoc(trim(maBanDoc)),
      m_hoTen(trim(hoTen)),
      m_ngayDangKy(ngayDangKy) {
}

const std::string& Reader::getMaBanDoc() const {
    return m_maBanDoc;
}

const std::string& Reader::getHoTen() const {
    return m_hoTen;
}

const Date& Reader::getNgayDangKy() const {
    return m_ngayDangKy;
}

bool Reader::setMaBanDoc(const std::string& maBanDoc) {
    if (!isValidTextField(maBanDoc)) {
        return false;
    }
    m_maBanDoc = maBanDoc;
    return true;
}

bool Reader::setHoTen(const std::string& hoTen) {
    if (!isValidTextField(hoTen)) {
        return false;
    }
    m_hoTen = hoTen;
    return true;
}

bool Reader::setNgayDangKy(const Date& ngayDangKy) {
    if (!ngayDangKy.isValid() || Date::today() < ngayDangKy) {
        return false;
    }
    m_ngayDangKy = ngayDangKy;
    return true;
}

std::string Reader::getKey() const {
    return m_maBanDoc;
}

bool Reader::isValid() const {
    if (!isValidTextField(m_maBanDoc) || !isValidTextField(m_hoTen)) {
        return false;
    }
    if (!m_ngayDangKy.isValid() || Date::today() < m_ngayDangKy) {
        return false;
    }
    return true;
}

std::string Reader::serialize() const {
    return m_maBanDoc + FIELD_DELIMITER + m_hoTen + FIELD_DELIMITER +
           m_ngayDangKy.toString();
}

std::string Reader::getEntityType() const {
    return "Reader";
}

bool Reader::fromFileLine(const std::string& line, Reader& outReader) {
    const std::vector<std::string> fields = splitLine(line, FIELD_DELIMITER);
    if (fields.size() != READER_FIELD_COUNT) {
        return false;
    }
    const std::string maBanDoc = trim(fields[0]);
    const std::string hoTen = trim(fields[1]);
    if (!isValidTextField(maBanDoc) || !isValidTextField(hoTen)) {
        return false;
    }
    Date ngayDangKy;
    if (!Date::fromString(trim(fields[2]), ngayDangKy)) {
        return false;
    }
    if (Date::today() < ngayDangKy) {
        return false;
    }
    outReader = Reader(maBanDoc, hoTen, ngayDangKy);
    return true;
}

// ---------------------------------------------------------------- Loan

Loan::Loan()
    : m_soPhieuMuon(0),
      m_maBanDoc(""),
      m_maSach(""),
      m_ngayMuon(),
      m_ngayPhaiTra(),
      m_tinhTrang(0) {
}

Loan::Loan(int soPhieuMuon, const std::string& maBanDoc,
           const std::string& maSach, const Date& ngayMuon,
           const Date& ngayPhaiTra, int tinhTrang)
    : m_soPhieuMuon(soPhieuMuon),
      m_maBanDoc(trim(maBanDoc)),
      m_maSach(trim(maSach)),
      m_ngayMuon(ngayMuon),
      m_ngayPhaiTra(ngayPhaiTra),
      m_tinhTrang(tinhTrang) {
}

int Loan::getSoPhieuMuon() const {
    return m_soPhieuMuon;
}

const std::string& Loan::getMaBanDoc() const {
    return m_maBanDoc;
}

const std::string& Loan::getMaSach() const {
    return m_maSach;
}

const Date& Loan::getNgayMuon() const {
    return m_ngayMuon;
}

const Date& Loan::getNgayPhaiTra() const {
    return m_ngayPhaiTra;
}

int Loan::getTinhTrang() const {
    return m_tinhTrang;
}

bool Loan::setSoPhieuMuon(int soPhieuMuon) {
    if (soPhieuMuon < MIN_LOAN_NUMBER) {
        return false;
    }
    m_soPhieuMuon = soPhieuMuon;
    return true;
}

bool Loan::setMaBanDoc(const std::string& maBanDoc) {
    if (!isValidTextField(maBanDoc)) {
        return false;
    }
    m_maBanDoc = maBanDoc;
    return true;
}

bool Loan::setMaSach(const std::string& maSach) {
    if (!isValidTextField(maSach)) {
        return false;
    }
    m_maSach = maSach;
    return true;
}

bool Loan::setNgayMuon(const Date& ngayMuon) {
    if (!ngayMuon.isValid()) {
        return false;
    }
    m_ngayMuon = ngayMuon;
    return true;
}

bool Loan::setNgayPhaiTra(const Date& ngayPhaiTra) {
    if (!ngayPhaiTra.isValid()) {
        return false;
    }
    m_ngayPhaiTra = ngayPhaiTra;
    return true;
}

bool Loan::setTinhTrang(int tinhTrang) {
    if (tinhTrang != LOAN_STATUS_RETURNED &&
        tinhTrang != LOAN_STATUS_BORROWED) {
        return false;
    }
    m_tinhTrang = tinhTrang;
    return true;
}

int Loan::getKey() const {
    return m_soPhieuMuon;
}

bool Loan::isValid() const {
    if (m_soPhieuMuon < MIN_LOAN_NUMBER) {
        return false;
    }
    if (!isValidTextField(m_maBanDoc) || !isValidTextField(m_maSach)) {
        return false;
    }
    if (!m_ngayMuon.isValid() || !m_ngayPhaiTra.isValid()) {
        return false;
    }
    if (m_tinhTrang != LOAN_STATUS_RETURNED &&
        m_tinhTrang != LOAN_STATUS_BORROWED) {
        return false;
    }
    return true;
}

std::string Loan::serialize() const {
    return std::to_string(m_soPhieuMuon) + FIELD_DELIMITER + m_maBanDoc +
           FIELD_DELIMITER + m_maSach + FIELD_DELIMITER +
           m_ngayMuon.toString() + FIELD_DELIMITER +
           m_ngayPhaiTra.toString() + FIELD_DELIMITER +
           std::to_string(m_tinhTrang);
}

std::string Loan::getEntityType() const {
    return "Loan";
}

bool Loan::fromFileLine(const std::string& line, Loan& outLoan) {
    const std::vector<std::string> fields = splitLine(line, FIELD_DELIMITER);
    if (fields.size() != LOAN_FIELD_COUNT) {
        return false;
    }
    int soPhieuMuon = 0;
    if (!parseNumberInRange<int>(trim(fields[0]), MIN_LOAN_NUMBER,
                                 std::numeric_limits<int>::max(),
                                 soPhieuMuon)) {
        return false;
    }
    const std::string maBanDoc = trim(fields[1]);
    const std::string maSach = trim(fields[2]);
    if (!isValidTextField(maBanDoc) || !isValidTextField(maSach)) {
        return false;
    }
    Date ngayMuon;
    if (!Date::fromString(trim(fields[3]), ngayMuon)) {
        return false;
    }
    Date ngayPhaiTra;
    if (!Date::fromString(trim(fields[4]), ngayPhaiTra)) {
        return false;
    }
    int tinhTrang = 0;
    if (!parseNumberInRange<int>(trim(fields[5]), LOAN_STATUS_RETURNED,
                                 LOAN_STATUS_BORROWED, tinhTrang)) {
        return false;
    }
    outLoan = Loan(soPhieuMuon, maBanDoc, maSach, ngayMuon, ngayPhaiTra,
                   tinhTrang);
    return true;
}
