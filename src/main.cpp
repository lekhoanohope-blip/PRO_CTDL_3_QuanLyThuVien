#include <fstream>
#include <iostream>
#include <string>

#include "AppController.h"
#include "LibraryService.h"

/// Tim thu muc data/ chua file Admin.txt, thu tu thu muc hien tai roi thu muc cha.
/// @return "data" neu tim thay o thu muc hien tai, "../data" neu tim thay o
/// thu muc cha, chuoi rong neu khong tim thay o dau.
static std::string resolveDataDirectory() {
    std::ifstream adminFile("data/Admin.txt");
    if (adminFile.good()) {
        return "data";
    }
    std::ifstream parentAdminFile("../data/Admin.txt");
    if (parentAdminFile.good()) {
        return "../data";
    }
    return "";
}

int main() {
    const std::string dataDirectory = resolveDataDirectory();
    if (dataDirectory.empty()) {
        std::cerr << "Khong tim thay thu muc data/. "
                  << "Hay chay chuong trinh tu thu muc goc cua du an." << '\n';
        return 1;
    }
    LibraryService service(dataDirectory);
    if (!service.loadAll()) {
        std::cerr << "Khong the tai du lieu tu thu muc data. "
                  << "Kiem tra lai cac file du lieu." << '\n';
        return 1;
    }
    AppController controller(service);
    controller.run();
    return 0;
}
