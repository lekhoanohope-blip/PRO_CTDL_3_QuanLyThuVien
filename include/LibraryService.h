#ifndef LIBRARY_LIBRARYSERVICE_H
#define LIBRARY_LIBRARYSERVICE_H

#include <list>
#include <string>
#include <vector>

#include "Date.h"
#include "Models.h"
#include "Repository.h"

/// So ngay toi da duoc muon mot cuon sach (NgayPhaiTra = NgayMuon + 7 ngay lich).
inline constexpr int LOAN_DURATION_DAYS = 7;

/// Nam phat hanh nho nhat cho phep cua mot cuon sach.
inline constexpr int MIN_PUBLISH_YEAR = 1000;

/// Tinh trang sach: 0 = san sang cho muon.
inline constexpr int BOOK_STATUS_AVAILABLE = 0;

/// Tinh trang phieu muon: 1 = dang muon.
inline constexpr int LOAN_STATUS_ACTIVE = 1;

/// Tinh trang phieu muon: 0 = da tra.
inline constexpr int LOAN_STATUS_RETURNED = 0;

/// Ten cac file du lieu trong thu muc du lieu.
inline constexpr const char* ADMIN_FILE_NAME = "Admin.txt";
inline constexpr const char* BOOK_FILE_NAME = "Sach.txt";
inline constexpr const char* READER_FILE_NAME = "BanDoc.txt";
inline constexpr const char* LOAN_FILE_NAME = "PhieuMuon.txt";

/// Ky tu phan cach thu muc trong duong dan file.
/// Dung '/' de duong dan chay duoc ca tren Windows lan Linux.
inline constexpr char PATH_SEPARATOR = '/';

/// Lop nghiep vu loi cua he thong quan ly thu vien.
///
/// LibraryService giu toan bo du lieu trong RAM thong qua Repository
/// va dong bo xuong 4 file .txt trong thu muc du lieu sau moi thay doi.
/// Lop nay khong doc/ghi console, chi tra ve ServiceResult de lop
/// giao dien hien thi thong bao phu hop.
class LibraryService {
public:
    /// Khoi tao dich vu voi thu muc chua 4 file du lieu.
    /// @param dataDirectory Duong dan thu muc du lieu (vi du: "data").
    explicit LibraryService(const std::string& dataDirectory);

    /// Tai toan bo 4 file du lieu (Admin, Sach, BanDoc, PhieuMuon) vao RAM.
    /// @return true neu tai duoc ca 4 file, false neu bat ky file nao khong mo duoc.
    bool loadAll();

    /// Xac thuc tai khoan quan tri.
    /// @param user Ten dang nhap.
    /// @param password Mat khau, so sanh khop tuyet doi.
    /// @return true neu ton tai Admin co dung user va password.
    bool authenticate(const std::string& user, const std::string& password) const;

    /// Them mot cuon sach moi (TinhTrang luon duoc ep ve 0 - san sang).
    /// @param book Sach can them.
    /// @return ServiceResult thanh cong ("Them sach thanh cong") hoac that bai
    ///         (trung MaSach, sai rang buoc mien gia tri, loi ghi file).
    ServiceResult addBook(const Book& book);

    /// Xoa mot cuon sach khoi he thong (chi xoa duoc sach san sang).
    /// @param maSach Ma sach can xoa.
    /// @return ServiceResult thanh cong ("Xoa sach thanh cong") hoac that bai
    ///         (sach khong ton tai, sach dang duoc muon, loi ghi file).
    ServiceResult removeBook(const std::string& maSach);

    /// Tim sach theo ma chinh xac (tra cuu O(log n) qua Repository).
    /// @param maSach Ma sach can tim.
    /// @return Con tro toi sach tim thay, nullptr neu khong ton tai.
    const Book* findBookByCode(const std::string& maSach) const;

    /// Tim sach theo ten (chuoi con, khong phan biet hoa/thuong).
    /// @param keyword Tu khoa can tim; chuoi rong tra ve toan bo sach.
    /// @return Danh sach con tro toi cac sach khop.
    std::vector<const Book*> searchBooksByTitle(const std::string& keyword) const;

    /// Tim sach theo tac gia (chuoi con, khong phan biet hoa/thuong).
    /// @param keyword Tu khoa can tim; chuoi rong tra ve toan bo sach.
    /// @return Danh sach con tro toi cac sach khop.
    std::vector<const Book*> searchBooksByAuthor(const std::string& keyword) const;

    /// Lay toan bo danh sach sach trong RAM (chi doc).
    const std::list<Book>& getAllBooks() const;

    /// Them mot ban doc moi.
    /// @param reader Ban doc can them.
    /// @return ServiceResult thanh cong ("Them ban doc thanh cong") hoac that bai
    ///         (trung MaBanDoc, sai rang buoc mien gia tri, loi ghi file).
    ServiceResult addReader(const Reader& reader);

    /// Tim ban doc theo ma chinh xac.
    /// @param maBanDoc Ma ban doc can tim.
    /// @return Con tro toi ban doc tim thay, nullptr neu khong ton tai.
    const Reader* findReaderByCode(const std::string& maBanDoc) const;

    /// Lay toan bo danh sach ban doc trong RAM (chi doc).
    const std::list<Reader>& getAllReaders() const;

    /// Lap phieu muon sach moi.
    /// @param maSach Ma sach muon muon.
    /// @param maBanDoc Ma ban doc muon sach.
    /// @param[out] outSoPhieuMuon So phieu muon vua tao (chi gan khi thanh cong).
    /// @return ServiceResult thanh cong ("Muon sach thanh cong") hoac that bai
    ///         (sach/ban doc khong ton tai, sach dang duoc muon, loi ghi file).
    ServiceResult borrowBook(const std::string& maSach, const std::string& maBanDoc, int& outSoPhieuMuon);

    /// Tra sach theo so phieu muon.
    /// @param soPhieuMuon So phieu muon can tra.
    /// @return ServiceResult thanh cong ("Tra sach thanh cong") hoac that bai
    ///         (phieu khong ton tai, phieu da duoc tra truoc do, loi ghi file).
    ServiceResult returnBook(int soPhieuMuon);

    /// Tim phieu muon theo so phieu chinh xac.
    /// @param soPhieuMuon So phieu muon can tim.
    /// @return Con tro toi phieu tim thay, nullptr neu khong ton tai.
    const Loan* findLoanByNumber(int soPhieuMuon) const;

    /// Lay toan bo danh sach phieu muon trong RAM (chi doc).
    const std::list<Loan>& getAllLoans() const;

    /// Lay thu muc du lieu da cau hinh.
    const std::string& getDataDirectory() const;

private:
    std::string m_dataDirectory;
    Repository<Admin, std::string> m_admins;
    Repository<Book, std::string> m_books;
    Repository<Reader, std::string> m_readers;
    Repository<Loan, int> m_loans;

    /// Duong dan day du toi file Admin.txt.
    std::string adminFilePath() const;

    /// Duong dan day du toi file Sach.txt.
    std::string bookFilePath() const;

    /// Duong dan day du toi file BanDoc.txt.
    std::string readerFilePath() const;

    /// Duong dan day du toi file PhieuMuon.txt.
    std::string loanFilePath() const;

    /// Kiem tra mot ngay co hop le lich thuc khong.
    /// @param date Ngay can kiem tra.
    /// @return true neu ngay doc lai dung dinh dang dd/mm/yyyy va hop le lich.
    bool isValidDate(const Date& date) const;

    /// Kiem tra cac rang buoc mien gia tri cua sach (tru tinh duy nhat cua MaSach).
    /// @param book Sach can kiem tra.
    /// @param[out] errorMessage Thong bao loi tieng Viet khong dau khi kiem tra that bai.
    /// @return true neu moi rang buoc deu thoa.
    bool validateBook(const Book& book, std::string& errorMessage) const;

    /// Kiem tra cac rang buoc mien gia tri cua ban doc (tru tinh duy nhat cua MaBanDoc).
    /// @param reader Ban doc can kiem tra.
    /// @param[out] errorMessage Thong bao loi tieng Viet khong dau khi kiem tra that bai.
    /// @return true neu moi rang buoc deu thoa.
    bool validateReader(const Reader& reader, std::string& errorMessage) const;
};

#endif  // LIBRARY_LIBRARYSERVICE_H
