#ifndef LIBRARY_MODELS_H
#define LIBRARY_MODELS_H

#include <string>

#include "Date.h"
#include "Entity.h"

/// Ket qua tra ve cua mot thao tac nghiep vu: thanh cong hay that bai
/// kem theo thong bao tieng Viet khong dau de hien thi cho nguoi dung.
struct ServiceResult {
    /// true khi thao tac thanh cong, false khi that bai.
    bool success = false;

    /// Thong bao ket qua (thanh cong hoac ly do that bai).
    std::string message;

    /// Constructor mac dinh: that bai, khong co thong bao.
    ServiceResult() = default;

    /// Constructor tuong thich: LibraryService dang dung dang
    /// ServiceResult(true/false, "thong bao") (giong ban Models.h
    /// cu). Khong thay doi API hop dong ok()/fail() o duoi.
    ServiceResult(bool ok, const std::string& text);

    /// Tao ket qua thanh cong kem thong bao.
    static ServiceResult ok(const std::string& message);

    /// Tao ket qua that bai kem thong bao ly do.
    static ServiceResult fail(const std::string& message);
};

/// Tai khoan nhan vien quan ly thu vien.
/// Dong file: user|password
class Admin : public Entity {
public:
    /// Tao tai khoan rong.
    Admin();

    /// Tao tai khoan tu user/password cho truoc (chi cat khoang trang).
    /// Gia tri khong hop le van duoc giu nguyen de isValid() kiem tra.
    Admin(const std::string& user, const std::string& password);

    /// Lay ten dang nhap.
    const std::string& getUser() const;

    /// Lay mat khau.
    const std::string& getPassword() const;

    /// Dat ten dang nhap. Tra ve false khi rong hoac chua '|'.
    bool setUser(const std::string& user);

    /// Dat mat khau. Tra ve false khi rong hoac chua '|'.
    bool setPassword(const std::string& password);

    /// Khoa chinh: user (duy nhat trong Admin.txt).
    std::string getKey() const;

    /// Kiem tra ca user va password deu hop le.
    bool isValid() const;

    /// Xuat thanh dong "user|password" (khong them '\n').
    std::string serialize() const override;

    /// Tra ve "Admin".
    std::string getEntityType() const override;

    /// Doc 1 dong file thanh Admin. Tra ve false khi thieu cot
    /// hoac truong khong hop le; khong doi outAdmin khi loi.
    static bool fromFileLine(const std::string& line, Admin& outAdmin);

private:
    std::string m_user;
    std::string m_password;
};

/// Sach trong thu vien.
/// Dong file: MaSach|TenSach|TacGia|NhaXuatBan|GiaBan|NamPhatHanh|
/// SoTrang|NgayNhapKho|TinhTrang
/// TinhTrang = 0: san sang; = SoPhieuMuon: dang duoc muon.
class Book : public Entity {
public:
    /// Tao sach rong (gia 0, nam 0, so trang 0, TinhTrang 0).
    Book();

    /// Tao sach day du 9 truong (chi cat khoang trang o chuoi).
    /// Gia tri khong hop le van duoc giu nguyen de isValid() kiem tra.
    Book(const std::string& maSach, const std::string& tenSach,
         const std::string& tacGia, const std::string& nhaXuatBan,
         long long giaBan, int namPhatHanh, int soTrang,
         const Date& ngayNhapKho, int tinhTrang);

    /// Lay ma sach.
    const std::string& getMaSach() const;

    /// Lay ten sach.
    const std::string& getTenSach() const;

    /// Lay tac gia.
    const std::string& getTacGia() const;

    /// Lay nha xuat ban.
    const std::string& getNhaXuatBan() const;

    /// Lay gia ban.
    long long getGiaBan() const;

    /// Lay nam phat hanh.
    int getNamPhatHanh() const;

    /// Lay so trang.
    int getSoTrang() const;

    /// Lay ngay nhap kho.
    const Date& getNgayNhapKho() const;

    /// Lay tinh trang (0: san sang, > 0: so phieu dang muon).
    int getTinhTrang() const;

    /// Dat ma sach. Tra ve false khi rong hoac chua '|'.
    bool setMaSach(const std::string& maSach);

    /// Dat ten sach. Tra ve false khi rong hoac chua '|'.
    bool setTenSach(const std::string& tenSach);

    /// Dat tac gia. Tra ve false khi rong hoac chua '|'.
    bool setTacGia(const std::string& tacGia);

    /// Dat nha xuat ban. Tra ve false khi rong hoac chua '|'.
    bool setNhaXuatBan(const std::string& nhaXuatBan);

    /// Dat gia ban. Tra ve false khi gia <= 0.
    bool setGiaBan(long long giaBan);

    /// Dat nam phat hanh. Tra ve false khi ngoai [1000, nam hien tai].
    bool setNamPhatHanh(int namPhatHanh);

    /// Dat so trang. Tra ve false khi so trang <= 0.
    bool setSoTrang(int soTrang);

    /// Dat ngay nhap kho. Tra ve false khi ngay khong hop le
    /// hoac lon hon ngay hien tai.
    bool setNgayNhapKho(const Date& ngayNhapKho);

    /// Dat tinh trang. Tra ve false khi tinh trang < 0.
    bool setTinhTrang(int tinhTrang);

    /// Khoa chinh: MaSach (duy nhat trong Sach.txt).
    std::string getKey() const;

    /// Kiem tra day du rang buoc mien gia tri (SPEC muc 6.1).
    bool isValid() const;

    /// Xuat thanh dong 9 truong noi bang '|' (khong them '\n').
    std::string serialize() const override;

    /// Tra ve "Book".
    std::string getEntityType() const override;

    /// Doc 1 dong file thanh Book. Tra ve false khi thieu cot,
    /// sai kieu so, sai ngay hoac vi pham rang buoc mien gia tri;
    /// khong doi outBook khi loi, khong throw.
    static bool fromFileLine(const std::string& line, Book& outBook);

private:
    std::string m_maSach;
    std::string m_tenSach;
    std::string m_tacGia;
    std::string m_nhaXuatBan;
    long long m_giaBan;
    int m_namPhatHanh;
    int m_soTrang;
    Date m_ngayNhapKho;
    int m_tinhTrang;
};

/// Ban doc da dang ky.
/// Dong file: MaBanDoc|HoTen|NgayDangKy
class Reader : public Entity {
public:
    /// Tao ban doc rong.
    Reader();

    /// Tao ban doc day du 3 truong (chi cat khoang trang o chuoi).
    /// Gia tri khong hop le van duoc giu nguyen de isValid() kiem tra.
    Reader(const std::string& maBanDoc, const std::string& hoTen,
           const Date& ngayDangKy);

    /// Lay ma ban doc.
    const std::string& getMaBanDoc() const;

    /// Lay ho ten.
    const std::string& getHoTen() const;

    /// Lay ngay dang ky.
    const Date& getNgayDangKy() const;

    /// Dat ma ban doc. Tra ve false khi rong hoac chua '|'.
    bool setMaBanDoc(const std::string& maBanDoc);

    /// Dat ho ten. Tra ve false khi rong hoac chua '|'.
    bool setHoTen(const std::string& hoTen);

    /// Dat ngay dang ky. Tra ve false khi ngay khong hop le
    /// hoac lon hon ngay hien tai.
    bool setNgayDangKy(const Date& ngayDangKy);

    /// Khoa chinh: MaBanDoc (duy nhat trong BanDoc.txt).
    std::string getKey() const;

    /// Kiem tra day du rang buoc mien gia tri (SPEC muc 6.1).
    bool isValid() const;

    /// Xuat thanh dong 3 truong noi bang '|' (khong them '\n').
    std::string serialize() const override;

    /// Tra ve "Reader".
    std::string getEntityType() const override;

    /// Doc 1 dong file thanh Reader. Tra ve false khi thieu cot,
    /// sai ngay hoac vi pham rang buoc; khong doi outReader khi loi.
    static bool fromFileLine(const std::string& line, Reader& outReader);

private:
    std::string m_maBanDoc;
    std::string m_hoTen;
    Date m_ngayDangKy;
};

/// Phieu muon / tra sach.
/// Dong file: SoPhieuMuon|MaBanDoc|MaSach|NgayMuon|NgayPhaiTra|TinhTrang
/// TinhTrang = 1: dang muon; = 0: da tra.
/// NgayPhaiTra luon = NgayMuon + 7 ngay lich.
class Loan : public Entity {
public:
    /// Tao phieu rong (so phieu 0, TinhTrang 0).
    Loan();

    /// Tao phieu day du 6 truong (chi cat khoang trang o chuoi).
    /// Gia tri khong hop le van duoc giu nguyen de isValid() kiem tra.
    Loan(int soPhieuMuon, const std::string& maBanDoc,
         const std::string& maSach, const Date& ngayMuon,
         const Date& ngayPhaiTra, int tinhTrang);

    /// Lay so phieu muon.
    int getSoPhieuMuon() const;

    /// Lay ma ban doc.
    const std::string& getMaBanDoc() const;

    /// Lay ma sach.
    const std::string& getMaSach() const;

    /// Lay ngay muon.
    const Date& getNgayMuon() const;

    /// Lay ngay phai tra.
    const Date& getNgayPhaiTra() const;

    /// Lay tinh trang (1: dang muon, 0: da tra).
    int getTinhTrang() const;

    /// Dat so phieu muon. Tra ve false khi so phieu <= 0.
    bool setSoPhieuMuon(int soPhieuMuon);

    /// Dat ma ban doc. Tra ve false khi rong hoac chua '|'.
    bool setMaBanDoc(const std::string& maBanDoc);

    /// Dat ma sach. Tra ve false khi rong hoac chua '|'.
    bool setMaSach(const std::string& maSach);

    /// Dat ngay muon. Tra ve false khi ngay khong hop le.
    bool setNgayMuon(const Date& ngayMuon);

    /// Dat ngay phai tra. Tra ve false khi ngay khong hop le.
    bool setNgayPhaiTra(const Date& ngayPhaiTra);

    /// Dat tinh trang. Tra ve false khi khac 0 va 1.
    bool setTinhTrang(int tinhTrang);

    /// Khoa chinh: SoPhieuMuon (tang dan tu 1).
    int getKey() const;

    /// Kiem tra day du rang buoc mien gia tri (SPEC muc 6.1).
    bool isValid() const;

    /// Xuat thanh dong 6 truong noi bang '|' (khong them '\n').
    std::string serialize() const override;

    /// Tra ve "Loan".
    std::string getEntityType() const override;

    /// Doc 1 dong file thanh Loan. Tra ve false khi thieu cot,
    /// sai kieu so, sai ngay hoac tinh trang khac 0/1;
    /// khong doi outLoan khi loi, khong throw.
    static bool fromFileLine(const std::string& line, Loan& outLoan);

private:
    int m_soPhieuMuon;
    std::string m_maBanDoc;
    std::string m_maSach;
    Date m_ngayMuon;
    Date m_ngayPhaiTra;
    int m_tinhTrang;
};

#endif  // LIBRARY_MODELS_H
