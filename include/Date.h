#ifndef LIBRARY_DATE_H
#define LIBRARY_DATE_H

#include <string>

/// Lop gia tri dong goi ngay/thang/nam theo lich thuc.
/// Constructor mac dinh tao 01/01/2000; constructor co tham so giu
/// nguyen gia tri truyen vao (ke ca ngay khong hop le) — dung isValid()
/// de kiem tra truoc khi dung trong nghiep vu.
class Date {
public:
    /// Tao ngay mac dinh 01/01/2000.
    Date();

    /// Tao ngay tu ngay/thang/nam cho truoc.
    Date(int day, int month, int year);

    /// Kiem tra nam co phai nam nhuan khong (chia het cho 4 va
    /// khong chia het cho 100, tru khi chia het cho 400).
    static bool isLeapYear(int year);

    /// Kiem tra ngay co hop le theo lich thuc khong: thang 1..12,
    /// ngay dung so ngay cua thang, 29/02 chi ton tai o nam nhuan.
    static bool isValid(int day, int month, int year);

    /// Kiem tra ngay hien dang chua co hop le theo lich thuc khong.
    bool isValid() const;

    /// Tra ve so ngay cua thang trong nam cho truoc (thang 2 tinh
    /// ca nam nhuan); tra ve 0 neu thang ngoai 1..12.
    static int daysInMonth(int month, int year);

    /// Lay ngay hien tai cua he thong.
    static Date today();

    /// Tra ve ngay moi bang ngay hien tai cong them so ngay cho truoc
    /// (days co the am). Tinh dung qua cuoi thang va chuyen nam.
    Date addDays(int days) const;

    /// Dinh dang ngay thanh "dd/mm/yyyy", co so 0 o truoc khi can.
    std::string toString() const;

    /// Doc ngay tu chuoi "dd/mm/yyyy" (chap nhan ca "1/2/2026").
    /// Tra ve false va khong doi outDate khi chuoi sai dinh dang
    /// hoac ngay khong hop le.
    static bool fromString(const std::string& text, Date& outDate);

    /// Lay ngay trong thang.
    int getDay() const;

    /// Lay thang trong nam.
    int getMonth() const;

    /// Lay nam.
    int getYear() const;

    /// So sanh bang nhau.
    bool operator==(const Date& other) const;

    /// So sanh nho hon (theo thu tu nam, thang, ngay).
    bool operator<(const Date& other) const;

    /// So sanh nho hon hoac bang.
    bool operator<=(const Date& other) const;

private:
    int m_day;
    int m_month;
    int m_year;
};

#endif  // LIBRARY_DATE_H
