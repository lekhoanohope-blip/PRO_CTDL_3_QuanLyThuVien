/// Bo kiem thu tu dong doc lap cho du an PRO_CTDL_3 — He thong Quan ly Thu vien.
///
/// Pham vi: Date, Validation (ke ca Function Template parseNumberInRange<T>),
/// Models (Admin, Book, Reader, Loan), Class Template Repository<T, KeyType>
/// va LibraryService (kiem thu E2E nghiep vu).
/// Khong phu thuoc ConsoleUI / AppController nen chay duoc tren moi may.
///
/// Cach chay (tu thu muc goc du an):
///   g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/Date.cpp src/Validation.cpp
///       src/Models.cpp src/LibraryService.cpp tests/test_runner.cpp
///       -o build/test_runner.exe
///   ./build/test_runner.exe        (Linux / MSYS2)
///   .\build\test_runner.exe        (PowerShell)
///
/// Du lieu tam: moi lan chay, thu muc "build/test_data" duoc xoa va tao lai;
/// bo test KHONG dung thu muc data/ that cua du an.
///
/// Quy uoc: thong bao tieng Viet khong dau. Moi CHECK la 1 test doc lap.
/// Ket thuc in "Tong: X test, Pass: Y, Fail: Z" va tra 0 neu tat ca pass.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <system_error>
#include <vector>

#include "Date.h"
#include "LibraryService.h"
#include "Models.h"
#include "Repository.h"
#include "Validation.h"

namespace {

/// So ngay toi da duoc muon mot cuon sach (SPEC muc 6.1).
constexpr int kLoanDurationDays = LOAN_DURATION_DAYS;
/// So phieu muon dau tien duoc cap (bat dau tu 1).
constexpr int kFirstLoanNumber = 1;
/// So phieu muon thu hai trong kich ban E2E.
constexpr int kSecondLoanNumber = 2;

int g_passCount = 0;
int g_failCount = 0;

/// Ghi nhan 1 test: in "PASS: <ten>" hoac "FAIL: <ten>".
void check(bool condition, const std::string& testName) {
    if (condition) {
        ++g_passCount;
        std::cout << "PASS: " << testName << '\n';
    } else {
        ++g_failCount;
        std::cout << "FAIL: " << testName << '\n';
    }
}

/// Thu muc tam rieng cua bo test (duoc xoa va tao lai moi lan chay).
const std::filesystem::path kTestDir =
    std::filesystem::path("build") / "test_data";

/// Doc toan bo noi dung file thanh chuoi (che do binary de giu nguyen CRLF).
std::string readWholeFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return std::string();
    }
    return std::string(std::istreambuf_iterator<char>(input),
                       std::istreambuf_iterator<char>());
}

/// Ghi de file bang noi dung cho truoc (che do binary de giu nguyen CRLF).
bool writeTextFile(const std::string& path, const std::string& content) {
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) {
        return false;
    }
    output << content;
    return output.good();
}

/// true neu file ton tai va chua chuoi con needle.
bool fileContains(const std::string& path, const std::string& needle) {
    return readWholeFile(path).find(needle) != std::string::npos;
}

/// Sach mau hop le (ngay nhap kho trong qua khu, tinh trang tuy chon).
Book makeBook(const std::string& maSach, int tinhTrang = 0,
              long long giaBan = 150000) {
    return Book(maSach, "Lap Trinh C++", "Bjarne Stroustrup", "Giao Duc",
                giaBan, 2020, 350, Date(10, 1, 2026), tinhTrang);
}

/// Thu thap serialize() cua toan bo ban ghi trong kho (giu nguyen thu tu).
template <typename T, typename KeyType>
std::vector<std::string> collectSerializes(Repository<T, KeyType>& repo) {
    std::vector<std::string> result;
    for (const T& item : repo.items()) {
        result.push_back(item.serialize());
    }
    return result;
}

}  // namespace

/// Mini-framework: 1 macro duy nhat, in "PASS/FAIL: <ten test>".
#define CHECK(condition, testName) check((condition), (testName))

void testDate() {
    std::cout << "--- Date ---\n";
    CHECK(Date::isLeapYear(2024), "Date: 2024 la nam nhuan");
    CHECK(!Date::isLeapYear(2025), "Date: 2025 khong phai nam nhuan");
    CHECK(Date(29, 2, 2024).isValid(), "Date: 29/02/2024 hop le");
    CHECK(!Date(29, 2, 2025).isValid(), "Date: 29/02/2025 khong hop le");
    CHECK(!Date(31, 4, 2026).isValid(), "Date: 31/04/2026 khong hop le");
    CHECK(!Date(0, 12, 2026).isValid(), "Date: 00/12/2026 khong hop le");
    CHECK(Date::daysInMonth(2, 2024) == 29 &&
              Date::daysInMonth(2, 2025) == 28,
          "Date: thang 2 nam nhuan 29 ngay, nam thuong 28 ngay");

    Date parsed;
    CHECK(Date::fromString("29/02/2024", parsed) &&
              parsed == Date(29, 2, 2024),
          "Date: fromString 29/02/2024 dung");
    CHECK(!Date::fromString("29/02/2025", parsed),
          "Date: fromString tu choi 29/02/2025");
    CHECK(!Date::fromString("31/04/2026", parsed),
          "Date: fromString tu choi 31/04/2026");
    CHECK(!Date::fromString("chuoi rac", parsed),
          "Date: fromString tu choi chuoi rac");

    CHECK(Date(29, 2, 2024).toString() == "29/02/2024",
          "Date: toString 29/02/2024");
    CHECK(Date(28, 12, 2026).addDays(7) == Date(4, 1, 2027),
          "Date: 28/12/2026 + 7 = 04/01/2027");
    CHECK(Date(25, 2, 2024).addDays(7) == Date(3, 3, 2024),
          "Date: 25/02/2024 + 7 = 03/03/2024 (nam nhuan)");
    CHECK(Date(25, 2, 2026).addDays(7) == Date(4, 3, 2026),
          "Date: 25/02/2026 + 7 = 04/03/2026");

    CHECK(Date(1, 1, 2026) == Date(1, 1, 2026),
          "Date: operator== dung khi cung ngay");
    CHECK(Date(1, 1, 2026) <= Date(1, 1, 2026),
          "Date: operator<= dung khi bang nhau");
    CHECK(Date(1, 1, 2026) <= Date(2, 1, 2026),
          "Date: operator<= dung khi ngay truoc nho hon");
    const Date sample(29, 2, 2024);
    CHECK(sample.getDay() == 29 && sample.getMonth() == 2 &&
              sample.getYear() == 2024,
          "Date: getter ngay/thang/nam");
}

void testValidation() {
    std::cout << "--- Validation ---\n";
    CHECK(Validation::trim("  abc  ") == "abc" &&
              Validation::trim("abc") == "abc" &&
              Validation::trim("   ") == "" && Validation::trim("") == "",
          "Validation: trim bo khoang trang 2 dau, giu chuoi thuong, chuoi trang ve rong");
    CHECK(Validation::isBlank("") && Validation::isBlank("   ") &&
              !Validation::isBlank("a"),
          "Validation: isBlank nhan dien chuoi rong hoac toan khoang trang");
    CHECK(Validation::containsDelimiter("a|b") &&
              !Validation::containsDelimiter("abc") &&
              !Validation::containsDelimiter("") &&
              FIELD_DELIMITER == '|',
          "Validation: containsDelimiter phat hien ky tu phan cach | (FIELD_DELIMITER)");
    CHECK(Validation::toLower("AbC") == "abc" &&
              Validation::toLower("ABC123") == "abc123",
          "Validation: toLower chuyen ve chu thuong");

    const std::vector<std::string> fields =
        Validation::splitLine("a|b|c");
    CHECK(fields.size() == 3U && fields[0] == "a" && fields[1] == "b" &&
              fields[2] == "c",
          "Validation: splitLine tach dung 3 truong");
    const std::vector<std::string> crlfFields =
        Validation::splitLine("a|b|c\r");
    CHECK(crlfFields.size() == 3U && crlfFields[2] == "c",
          "Validation: splitLine bo \\r cuoi dong (xu ly file CRLF)");
    const std::vector<std::string> emptyFields =
        Validation::splitLine("a||c");
    CHECK(emptyFields.size() == 3U && emptyFields[1].empty(),
          "Validation: splitLine giu truong rong o giua");

    int intValue = 0;
    CHECK(Validation::parseNumberInRange<int>("123", 0, 1000, intValue) &&
              intValue == 123,
          "Validation: parseNumberInRange<int> doc dung 123");
    CHECK(!Validation::parseNumberInRange<int>("12a", 0, 1000, intValue),
          "Validation: parseNumberInRange<int> tu choi chu cai");
    CHECK(!Validation::parseNumberInRange<int>("-5", 0, 100, intValue),
          "Validation: parseNumberInRange<int> tu choi so am ngoai bien");
    CHECK(!Validation::parseNumberInRange<int>("9999999999999999999999",
                                               0, 1000000, intValue),
          "Validation: parseNumberInRange<int> tu choi so tran");
    CHECK(Validation::parseNumberInRange<int>("1000", 0, 1000, intValue) &&
              intValue == 1000,
          "Validation: parseNumberInRange<int> chap nhan bien tren 1000");
    CHECK(!Validation::parseNumberInRange<int>("", 0, 100, intValue),
          "Validation: parseNumberInRange<int> tu choi chuoi rong");

    long long longValue = 0;
    CHECK(Validation::parseNumberInRange<long long>(
              "1234567890123", 0LL, 9999999999999LL, longValue) &&
              longValue == 1234567890123LL,
          "Validation: parseNumberInRange<long long> doc dung so lon");
    CHECK(!Validation::parseNumberInRange<long long>(
              "9999999999999999999999", 0LL, 9999999999999LL, longValue),
          "Validation: parseNumberInRange<long long> tu choi so tran");
}

void testModels() {
    std::cout << "--- Models ---\n";
    Admin adminParsed;
    CHECK(Admin::fromFileLine("admin|123456", adminParsed) &&
              adminParsed.getKey() == "admin" &&
              adminParsed.getUser() == "admin" &&
              adminParsed.getPassword() == "123456" &&
              adminParsed.serialize() == "admin|123456",
          "Models: Admin round-trip serialize -> fromFileLine");
    CHECK(!Admin::fromFileLine("admin", adminParsed),
          "Models: Admin fromFileLine tu choi thieu cot");

    const Book book("S001", "Lap Trinh C++", "Bjarne Stroustrup",
                    "Giao Duc", 150000, 2020, 350, Date(10, 1, 2026), 0);
    CHECK(book.serialize() ==
              "S001|Lap Trinh C++|Bjarne Stroustrup|Giao Duc|150000|2020|"
              "350|10/01/2026|0",
          "Models: Book serialize dung dinh dang 9 truong");
    Book bookParsed;
    CHECK(Book::fromFileLine(book.serialize(), bookParsed) &&
              bookParsed.getMaSach() == "S001" &&
              bookParsed.getTenSach() == "Lap Trinh C++" &&
              bookParsed.getTacGia() == "Bjarne Stroustrup" &&
              bookParsed.getNhaXuatBan() == "Giao Duc" &&
              bookParsed.getGiaBan() == 150000 &&
              bookParsed.getNamPhatHanh() == 2020 &&
              bookParsed.getSoTrang() == 350 &&
              bookParsed.getNgayNhapKho() == Date(10, 1, 2026) &&
              bookParsed.getTinhTrang() == 0,
          "Models: Book round-trip giu nguyen 9 truong");
    CHECK(!Book::fromFileLine("S001|Lap Trinh C++", bookParsed),
          "Models: Book fromFileLine tu choi thieu cot");
    CHECK(!Book::fromFileLine(
              "S001|Lap Trinh C++|Bjarne Stroustrup|Giao Duc|abc|2020|350|"
              "10/01/2026|0",
              bookParsed),
          "Models: Book fromFileLine tu choi gia sai kieu so");
    CHECK(!Book::fromFileLine(
              "S001|Lap Trinh C++|Bjarne Stroustrup|Giao Duc|150000|2020|"
              "350|32/01/2026|0",
              bookParsed),
          "Models: Book fromFileLine tu choi ngay sai");

    const Reader reader("BD001", "Nguyen Van An", Date(5, 1, 2026));
    Reader readerParsed;
    CHECK(Reader::fromFileLine(reader.serialize(), readerParsed) &&
              readerParsed.getMaBanDoc() == "BD001" &&
              readerParsed.getHoTen() == "Nguyen Van An" &&
              readerParsed.getNgayDangKy() == Date(5, 1, 2026),
          "Models: Reader round-trip giu nguyen 3 truong");
    CHECK(!Reader::fromFileLine("BD001|Nguyen Van An", readerParsed),
          "Models: Reader fromFileLine tu choi thieu cot");

    const Loan loan(1, "BD001", "S001", Date(20, 11, 2026),
                    Date(27, 11, 2026), 1);
    Loan loanParsed;
    CHECK(Loan::fromFileLine(loan.serialize(), loanParsed) &&
              loanParsed.getSoPhieuMuon() == 1 &&
              loanParsed.getMaBanDoc() == "BD001" &&
              loanParsed.getMaSach() == "S001" &&
              loanParsed.getNgayMuon() == Date(20, 11, 2026) &&
              loanParsed.getNgayPhaiTra() == Date(27, 11, 2026) &&
              loanParsed.getTinhTrang() == 1,
          "Models: Loan round-trip giu nguyen 6 truong");
    CHECK(!Loan::fromFileLine("abc|BD001|S001|20/11/2026|27/11/2026|1",
                              loanParsed),
          "Models: Loan fromFileLine tu choi so phieu sai kieu");

    const ServiceResult okResult = ServiceResult::ok("Thanh cong");
    const ServiceResult failResult = ServiceResult::fail("That bai");
    CHECK(okResult.success && okResult.message == "Thanh cong" &&
              !failResult.success && failResult.message == "That bai",
          "Models: ServiceResult ok/fail giu success va message");
}

void testRepository() {
    std::cout << "--- Repository ---\n";
    Repository<Book, std::string> books;
    CHECK(books.empty() && books.size() == 0U,
          "Repository: kho moi rong");
    CHECK(books.add(makeBook("S001")), "Repository: add sach moi thanh cong");
    CHECK(!books.add(makeBook("S001")),
          "Repository: add trung khoa tra false");
    CHECK(books.add(makeBook("S002", 0, 120000)) && books.size() == 2U,
          "Repository: add sach thu hai, size = 2");
    CHECK(books.exists("S001") && !books.exists("S999") &&
              books.findByKey("S999") == nullptr,
          "Repository: exists dung/sai khoa, findByKey tra nullptr khi khong co");
    const Book* found = books.findByKey("S001");
    CHECK(found != nullptr && found->getTenSach() == "Lap Trinh C++",
          "Repository: findByKey tra dung sach theo khoa");
    const std::vector<const Book*> expensive =
        books.findWhere([](const Book& item) {
            return item.getGiaBan() > 130000;
        });
    CHECK(expensive.size() == 1U &&
              expensive[0]->getMaSach() == "S001",
          "Repository: findWhere loc theo dieu kien");
    Book* mutableFound = books.findMutable("S002");
    CHECK(mutableFound != nullptr &&
              mutableFound->setTinhTrang(kSecondLoanNumber) &&
              books.findByKey("S002") != nullptr &&
              books.findByKey("S002")->getTinhTrang() == kSecondLoanNumber,
          "Repository: findMutable tra con tro sua duoc, thay doi giu lai trong kho");
    CHECK(books.removeByKey("S002") && !books.removeByKey("S002") &&
              books.size() == 1U,
          "Repository: removeByKey xoa 1 lan, lan 2 tra false");
    books.clear();
    CHECK(books.empty() && books.size() == 0U,
          "Repository: clear lam rong kho");

    const std::string crlfPath = (kTestDir / "repo_crlf.txt").string();
    const std::string crlfContent =
        "S001|Lap Trinh C++|Bjarne Stroustrup|Giao Duc|150000|2020|350|"
        "10/01/2026|0\r\n"
        "\r\n"
        "S002|Cau Truc Du Lieu|Robert Lafore|Giao Duc|120000|2019|420|"
        "15/02/2026|0\r\n";
    Repository<Book, std::string> loaded;
    CHECK(writeTextFile(crlfPath, crlfContent) &&
              loaded.loadFromFile(crlfPath) && loaded.size() == 2U &&
              loaded.findByKey("S002") != nullptr &&
              loaded.findByKey("S002")->getTenSach() == "Cau Truc Du Lieu",
          "Repository: loadFromFile xu ly CRLF va dong trong");

    const std::string appendPath = (kTestDir / "repo_append.txt").string();
    Repository<Book, std::string> appender;
    CHECK(writeTextFile(appendPath, makeBook("S001").serialize()) &&
              appender.appendToFile(appendPath, makeBook("S002")),
          "Repository: appendToFile vao file chua co newline cuoi");
    CHECK(fileContains(appendPath, "\nS002|"),
          "Repository: appendToFile tu them newline khi file thieu");
    Repository<Book, std::string> reloaded;
    CHECK(reloaded.loadFromFile(appendPath) && reloaded.size() == 2U,
          "Repository: file sau append doc lai du 2 dong");

    Repository<Book, std::string> saver;
    saver.add(makeBook("S001"));
    saver.add(makeBook("S002"));
    const std::string savePath = (kTestDir / "repo_save.txt").string();
    Repository<Book, std::string> saverReloaded;
    CHECK(saver.saveAllToFile(savePath) &&
              saverReloaded.loadFromFile(savePath) &&
              collectSerializes(saver) == collectSerializes(saverReloaded),
          "Repository: saveAllToFile + load lai khop 100%");

    Repository<Loan, int> loans;
    const Loan loan1(kFirstLoanNumber, "BD001", "S001",
                     Date(20, 11, 2026), Date(27, 11, 2026), 1);
    CHECK(loans.add(loan1) && !loans.add(loan1),
          "Repository<Loan,int>: add phieu moi, trung so phieu tra false");
    const Loan* foundLoan = loans.findByKey(kFirstLoanNumber);
    CHECK(foundLoan != nullptr && foundLoan->getMaSach() == "S001" &&
              loans.removeByKey(kFirstLoanNumber) && loans.empty(),
          "Repository<Loan,int>: findByKey theo so phieu, removeByKey xoa phieu");
}

void testLibraryService() {
    std::cout << "--- LibraryService (E2E) ---\n";
    const std::filesystem::path dataDir = kTestDir / "library";
    std::filesystem::create_directories(dataDir);
    const std::string dirString = dataDir.string();
    const std::string pastDate = Date::today().addDays(-30).toString();

    std::string bookContent =
        "S001|Lap Trinh C++|Bjarne Stroustrup|Giao Duc|150000|2020|350|";
    bookContent += pastDate;
    bookContent +=
        "|0\nS002|Cau Truc Du Lieu|Robert Lafore|Giao Duc|120000|2019|420|";
    bookContent += pastDate;
    bookContent +=
        "|0\nS003|Giai Tich 1|Nguyen Dinh Tri|Giao Duc|90000|2021|300|";
    bookContent += pastDate;
    bookContent += "|0\n";
    std::string readerContent = "BD001|Nguyen Van An|";
    readerContent += pastDate;
    readerContent += "\nBD002|Tran Thi Binh|";
    readerContent += pastDate;
    readerContent += "\n";
    CHECK(writeTextFile((dataDir / "Admin.txt").string(),
                        "admin|123456\n") &&
              writeTextFile((dataDir / "Sach.txt").string(), bookContent) &&
              writeTextFile((dataDir / "BanDoc.txt").string(),
                            readerContent) &&
              writeTextFile((dataDir / "PhieuMuon.txt").string(), ""),
          "LibraryService: ghi 4 file mau vao thu muc tam");

    LibraryService service(dirString);
    CHECK(service.loadAll(),
          "LibraryService: loadAll doc 4 file mau thanh cong");
    CHECK(service.authenticate("admin", "123456") &&
              !service.authenticate("admin", "sai mat khau") &&
              !service.authenticate("khong co", "123456"),
          "LibraryService: dang nhap dung cho phep, sai mat khau / sai user bi tu choi");

    const Date today = Date::today();
    const Date futureDate = today.addDays(1);
    const Book newBook("S004", "Nhap Mon Python", "Guido van Rossum",
                       "Giao Duc", 110000, 2022, 280, today, 0);
    CHECK(service.addBook(newBook).success &&
              fileContains((dataDir / "Sach.txt").string(),
                           "S004|Nhap Mon Python") &&
              service.getAllBooks().size() == 4U,
          "LibraryService: addBook ghi noi dong moi vao Sach.txt");
    CHECK(!service.addBook(makeBook("S001")).success,
          "LibraryService: addBook trung MaSach bi chan");
    CHECK(!service.addBook(Book("S101", "Sach Loi", "Tac Gia", "NXB", -5,
                                2020, 100, today, 0))
               .success &&
              !service.addBook(Book("S102", "Sach Loi", "Tac Gia", "NXB",
                                    100, 2020, 0, today, 0))
                   .success,
          "LibraryService: addBook chan gia am va so trang 0");
    CHECK(!service.addBook(Book("S103", "Sach Loi", "Tac Gia", "NXB", 100,
                                MIN_PUBLISH_YEAR - 1, 100, today, 0))
               .success &&
              !service.addBook(Book("S104", "Sach Loi", "Tac Gia", "NXB",
                                    100, 2020, 100, futureDate, 0))
                   .success,
          "LibraryService: addBook chan nam 999 va ngay nhap kho tuong lai");
    CHECK(service.findBookByCode("S001") != nullptr &&
              service.findBookByCode("S999") == nullptr,
          "LibraryService: findBookByCode tim dung / tra nullptr khi khong co");
    CHECK(service.searchBooksByTitle("lap trinh").size() == 1U &&
              service.searchBooksByTitle("LAP TRINH").size() == 1U &&
              service.searchBooksByAuthor("bjarnE").size() == 1U &&
              service.searchBooksByTitle("khong co sach nay").empty(),
          "LibraryService: tim theo ten/tac gia khong phan biet hoa thuong; khong thay tra rong");
    CHECK(service.addReader(Reader("BD003", "Le Van Cuong", today)).success &&
              !service.addReader(Reader("BD001", "Trung Ma", today)).success,
          "LibraryService: addReader thanh cong, trung MaBanDoc bi chan");

    int soPhieuMuon = 0;
    CHECK(service.borrowBook("S001", "BD001", soPhieuMuon).success &&
              soPhieuMuon == kFirstLoanNumber,
          "LibraryService: muon sach tao phieu so 1");
    const Loan* loan1 = service.findLoanByNumber(kFirstLoanNumber);
    const Book* book1 = service.findBookByCode("S001");
    CHECK(loan1 != nullptr && loan1->getTinhTrang() == 1 &&
              loan1->getNgayMuon() == today &&
              loan1->getNgayPhaiTra() == today.addDays(kLoanDurationDays) &&
              book1 != nullptr &&
              book1->getTinhTrang() == kFirstLoanNumber,
          "LibraryService: phieu 1 co NgayPhaiTra = NgayMuon + 7, sach chuyen sang tinh trang 1");
    int soPhieuMuon2 = 0;
    CHECK(service.borrowBook("S002", "BD002", soPhieuMuon2).success &&
              soPhieuMuon2 == kSecondLoanNumber &&
              !service.borrowBook("S001", "BD002", soPhieuMuon2).success &&
              !service.borrowBook("S003", "BD999", soPhieuMuon2).success &&
              !service.borrowBook("S999", "BD001", soPhieuMuon2).success,
          "LibraryService: phieu thu 2 la so 2; chan muon sach dang muon / sai ma ban doc / sai ma sach");
    CHECK(!service.removeBook("S001").success,
          "LibraryService: chan xoa sach khi tinhTrang != 0");

    LibraryService reloaded(dirString);
    CHECK(reloaded.loadAll() &&
              reloaded.findLoanByNumber(kFirstLoanNumber) != nullptr &&
              reloaded.findLoanByNumber(kFirstLoanNumber)->getTinhTrang() == 1 &&
              reloaded.findLoanByNumber(kSecondLoanNumber) != nullptr &&
              reloaded.findBookByCode("S001") != nullptr &&
              reloaded.findBookByCode("S001")->getTinhTrang() ==
                  kFirstLoanNumber &&
              reloaded.getAllLoans().size() == 2U,
          "LibraryService: service moi doc lai 2 file khop sau khi muon");

    const ServiceResult returnResult =
        service.returnBook(kFirstLoanNumber);
    const Loan* loanAfterReturn =
        service.findLoanByNumber(kFirstLoanNumber);
    const Book* bookAfterReturn = service.findBookByCode("S001");
    CHECK(returnResult.success && loanAfterReturn != nullptr &&
              loanAfterReturn->getTinhTrang() == 0 &&
              bookAfterReturn != nullptr &&
              bookAfterReturn->getTinhTrang() == 0 &&
              !service.returnBook(kFirstLoanNumber).success,
          "LibraryService: tra sach dua phieu va sach ve tinh trang 0; chan tra lan 2");
    LibraryService finalCheck(dirString);
    CHECK(service.removeBook("S001").success && finalCheck.loadAll() &&
              finalCheck.findBookByCode("S001") == nullptr &&
              !fileContains((dataDir / "Sach.txt").string(), "S001|") &&
              finalCheck.findLoanByNumber(kFirstLoanNumber) != nullptr &&
              finalCheck.findLoanByNumber(kFirstLoanNumber)->getTinhTrang() == 0,
          "LibraryService: xoa duoc sach sau khi da tra; Sach.txt da ghi de, phieu 1 van o tinh trang 0");
}

int main() {
    std::error_code error;
    std::filesystem::remove_all(kTestDir, error);
    if (!std::filesystem::create_directories(kTestDir)) {
        std::cout << "Khong tao duoc thu muc tam: " << kTestDir.string()
                  << '\n';
        return 1;
    }

    testDate();
    testValidation();
    testModels();
    testRepository();
    testLibraryService();

    const int totalTests = g_passCount + g_failCount;
    std::cout << "Tong: " << totalTests << " test, Pass: " << g_passCount
              << ", Fail: " << g_failCount << '\n';
    return (g_failCount == 0) ? 0 : 1;
}
