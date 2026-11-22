#ifndef LIBRARY_APPCONTROLLER_H
#define LIBRARY_APPCONTROLLER_H

#include "LibraryService.h"

/// Dieu phoi luong dang nhap, menu chinh va cac menu con cua chuong trinh.
/// Lop nay chi phu trach giao tiep console (qua ConsoleUI); moi nghiep vu
/// va luu file deu uy thac cho LibraryService.
class AppController {
public:
    /// @brief Khoi tao bo dieu khien voi dich vu nghiep vu da nap du lieu.
    /// @param service Dich vu nghiep vu dung chung trong suot phien lam viec.
    explicit AppController(LibraryService& service);

    /// @brief Chay chuong trinh: dang nhap roi vao vong lap menu chinh.
    void run();

private:
    /// So lan nhap sai user/password toi da truoc khi tu dong thoat.
    static constexpr int MAX_LOGIN_ATTEMPTS = 3;

    LibraryService& m_service;
    bool m_exitRequested = false;

    /// @brief Luong dang nhap: toi da MAX_LOGIN_ATTEMPTS lan, ESC thoat ngay.
    /// @return true neu dang nhap thanh cong, false neu huy hoac sai qua so lan.
    bool loginFlow();

    /// @brief Vong lap menu chinh: Quan li sach / Phieu muon / Ban doc / Thoat.
    void mainMenuLoop();

    /// @brief Menu con Quan li sach: hien thi, them, xoa, tim kiem sach.
    void bookMenu();

    /// @brief Menu con Quan li phieu muon: hien thi, muon sach, tra sach.
    void loanMenu();

    /// @brief Menu con Quan li ban doc: hien thi, them ban doc.
    void readerMenu();

    /// @brief Hien thi toan bo danh sach sach dang bang.
    void showAllBooks();

    /// @brief Luong nhap lieu them mot cuon sach moi.
    void addBookFlow();

    /// @brief Luong nhap ma sach va xoa sach khoi he thong.
    void removeBookFlow();

    /// @brief Luong tim kiem sach theo ma, ten sach hoac tac gia.
    void searchBookFlow();

    /// @brief Hien thi toan bo danh sach phieu muon dang bang.
    void showAllLoans();

    /// @brief Luong nhap lieu lap phieu muon sach moi.
    void borrowBookFlow();

    /// @brief Luong nhap so phieu va tra sach.
    void returnBookFlow();

    /// @brief Hien thi toan bo danh sach ban doc dang bang.
    void showAllReaders();

    /// @brief Luong nhap lieu them mot ban doc moi.
    void addReaderFlow();

    /// @brief Hoi nguoi dung co tiep tuc chuc nang khac hay thoat chuong trinh.
    /// @return true neu tiep tuc, false neu thoat (dat co m_exitRequested).
    bool askContinueOrExit();
};

#endif  // LIBRARY_APPCONTROLLER_H
