#ifndef LIBRARY_CONSOLEUI_H
#define LIBRARY_CONSOLEUI_H

#include <list>
#include <string>

#include "Models.h"
#include "Validation.h"

/// Mau chu hien thi tren console.
enum class ConsoleColor { Default, Green, Red, Yellow, Cyan, White };

/// Phim ESC dung de huy thao tac dang nhap.
inline constexpr int ESC_KEY = 27;
/// Phim Backspace dung de xoa ky tu khi nhap lieu.
inline constexpr int BACKSPACE_KEY = 8;

/// Lop tien ich giao dien console. Toan bo code phu thuoc Windows
/// (mau sac, doc phim) deu duoc dong goi o day; cac lop khac chi goi
/// cac ham tinh nay va khong bao gio cham truc tiep toi Windows API.
class ConsoleUI {
public:
    /// @brief Dat mau chu cho cac dong in tiep theo.
    /// @param color Mau can dat.
    static void setColor(ConsoleColor color);

    /// @brief Tra mau chu ve mac dinh cua console.
    static void resetColor();

    /// @brief Xoa toan bo noi dung man hinh console.
    static void clearScreen();

    /// @brief In tieu de trong khung dau '*' rong 30 ky tu, chu can giua, mau Cyan.
    /// @param title Tieu de can in (tieng Viet khong dau).
    static void printHeader(const std::string& title);

    /// @brief In thong bao thanh cong bang mau xanh la.
    /// @param message Noi dung thong bao.
    static void printSuccess(const std::string& message);

    /// @brief In thong bao loi bang mau do.
    /// @param message Noi dung thong bao.
    static void printError(const std::string& message);

    /// @brief In thong bao canh bao bang mau vang.
    /// @param message Noi dung thong bao.
    static void printWarning(const std::string& message);

    /// @brief In thong bao thong tin bang mau mac dinh.
    /// @param message Noi dung thong bao.
    static void printInfo(const std::string& message);

    /// @brief Hieu ung quyen sach bay tu ke toi ban doc khi muon sach thanh cong.
    /// @param tenSach Ten sach vua muon, in o giua quyen sach (cat ngan neu dai).
    static void playBorrowAnimation(const std::string& tenSach);

    /// @brief Hieu ung quyen sach bay tro ve ke khi tra sach thanh cong.
    /// @param tenSach Ten sach vua tra, in o giua quyen sach (cat ngan neu dai).
    static void playReturnAnimation(const std::string& tenSach);

    /// @brief Tam dung man hinh cho toi khi nguoi dung nhan mot phim bat ky.
    static void pause();

    /// @brief Doc mot dong chuoi tu ban phim, ho tro Backspace va huy bang ESC.
    /// @param prompt Chuoi goi y in truoc khi doc.
    /// @param outValue Chuoi doc duoc (khong bao gom ky tu xuong dong).
    /// @return true neu doc xong bang Enter, false neu nguoi dung nhan ESC.
    static bool readLine(const std::string& prompt, std::string& outValue);

    /// @brief Doc mat khau tu ban phim, hien thi '*' thay cho ky tu that.
    /// @param prompt Chuoi goi y in truoc khi doc.
    /// @param outValue Mat khau doc duoc.
    /// @return true neu doc xong bang Enter, false neu nguoi dung nhan ESC.
    static bool readPassword(const std::string& prompt, std::string& outValue);

    /// @brief Doc mot truong so co kiem tra khoang gia tri, lap lai toi khi hop le.
    /// @tparam T Kieu so can doc (int hoac long long).
    /// @param prompt Chuoi goi y in truoc khi doc.
    /// @param errorMessage Thong bao loi in ra khi gia tri ngoai khoang.
    /// @param minVal Gia tri toi thieu duoc chap nhan.
    /// @param maxVal Gia tri toi da duoc chap nhan.
    /// @param outValue Gia tri so doc duoc.
    /// @return true neu doc duoc gia tri hop le, false neu nguoi dung nhan ESC.
    template <typename T>
    static bool readValidatedField(const std::string& prompt,
                                   const std::string& errorMessage,
                                   T minVal, T maxVal, T& outValue) {
        std::string raw;
        while (true) {
            if (!readLine(prompt, raw)) {
                return false;
            }
            if (Validation::parseNumberInRange(raw, minVal, maxVal, outValue)) {
                return true;
            }
            printError(errorMessage);
        }
    }

    /// @brief In bang danh sach sach gom 9 cot, to mau theo tinh trang.
    /// @param books Danh sach sach can in.
    static void printBookTable(const std::list<Book>& books);

    /// @brief In bang danh sach ban doc gom 3 cot.
    /// @param readers Danh sach ban doc can in.
    static void printReaderTable(const std::list<Reader>& readers);

    /// @brief In bang danh sach phieu muon gom 6 cot, to mau theo tinh trang.
    /// @param loans Danh sach phieu muon can in.
    static void printLoanTable(const std::list<Loan>& loans);

    /// @brief Hoi nguoi dung co muon tiep tuc chuc nang khac hay thoat chuong trinh.
    /// @return true neu chon tiep tuc (1), false neu chon thoat (0) hoac nhan ESC.
    static bool askContinue();

private:
    /// @brief Doc mot phim tu ban phim ma khong can Enter.
    /// @return Ma phim (ky tu in duoc 32..126, Enter chuan hoa ve '\n',
    /// BACKSPACE_KEY, ESC_KEY), hoac -1 neu la phim chuc nang/mui ten can bo qua.
    static int readKey();
};

#endif  // LIBRARY_CONSOLEUI_H
