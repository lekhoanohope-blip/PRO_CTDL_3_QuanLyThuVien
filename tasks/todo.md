# Danh Sách Công Việc Chi Tiết (Todo): PRO_CTDL_3

Theo dõi từng task nguyên tử căn cứ vào [SPEC.md](../SPEC.md), [tasks/plan.md](plan.md) và bảng phân công 4 thành viên tại [tasks/team-assignments.md](team-assignments.md).

---

## Giai đoạn 1: Nền tảng OOP, Template & Lưu trữ (Foundation)

## Task 1: Xây dựng lớp `Date`, `Validation` (kèm Function Template) và khung Test Runner

**Phụ trách:** Nhật (`Date`) + Hiếu (`Validation`, `test_runner.cpp`) — *Khoa khởi tạo khung repo & review*

**Mô tả:** Cài đặt lớp OOP `Date` (đóng gói ngày/tháng/năm, kiểm tra hợp lệ lịch thực kể cả năm nhuận, lấy ngày hệ thống `Date::today()`, cộng ngày `addDays(int)`, định dạng `dd/mm/yyyy`, toán tử so sánh `==`/`<=`), bộ tiện ích `Validation` (chuẩn hóa chuỗi `trim`, chặn ký tự `|`, chuyển chữ thường `toLower`, và **Function Template** `parseNumberInRange<T>` dùng chung cho `int`/`long long`), đồng thời tạo khung kiểm thử `tests/test_runner.cpp`.

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] `Date::fromString` nhận diện chính xác ngày hợp lệ và từ chối ngày sai (`29/02/2025`, `31/04/2026`, `00/12/2026`, chuỗi rác), chấp nhận `29/02/2024`.
- [ ] `addDays(7)` tính đúng qua cuối tháng và chuyển năm (`28/12/2026 -> 04/01/2027`, `25/02/2024 -> 03/03/2024`, `25/02/2026 -> 04/03/2026`).
- [ ] `parseNumberInRange<int>` và `parseNumberInRange<long long>` từ chối chuỗi chứa chữ cái, số âm, vượt biên hoặc tràn số; `Validation` chặn ký tự `|`.
- [ ] Tuân thủ đầy đủ C++ Coding Standard (header guard, không `using namespace std;`, tiền tố `m_`, `const`-correctness).

**Kiểm chứng (Verification):**
- [ ] Build & test pass 0 warnings: `g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/Date.cpp src/Validation.cpp tests/test_runner.cpp -o build/test_runner.exe && .\build\test_runner.exe`

**Phụ thuộc:** Không

**File dự kiến chạm tới:**
- `include/Date.h`, `src/Date.cpp`
- `include/Validation.h`, `src/Validation.cpp`
- `tests/test_runner.cpp`

**Quy mô ước tính:** Nhỏ–Trung bình (5 file)

---

## Task 2: Xây dựng lớp trừu tượng `Entity` và 4 lớp thực thể OOP (`Admin`, `Book`, `Reader`, `Loan`)

**Phụ trách:** Khoa (`Entity.h`) + Nhật (`Book`, `Loan`) + Hiếu (`Admin`, `Reader`)

**Mô tả:** Tạo lớp cơ sở trừu tượng `Entity` (hủy ảo, phương thức thuần ảo `serialize() const` và `getEntityType() const`) và 4 lớp kế thừa `Admin`, `Book`, `Reader`, `Loan` cùng `ServiceResult`. Mỗi lớp đóng gói thuộc tính `private`, cung cấp getter/setter an toàn, `getKey()`, `serialize()`, và `static bool fromFileLine(...)` có kiểm tra ràng buộc miền giá trị.

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] Cả 4 lớp `Admin`, `Book`, `Reader`, `Loan` kế thừa công khai `Entity`, cài đặt `override` đúng chuẩn cho `serialize()` và `getEntityType()`.
- [ ] Round-trip `serialize()` -> `fromFileLine()` giữ nguyên 100% giá trị mọi trường của cả 4 thực thể.
- [ ] `fromFileLine()` trả về `false` an toàn (không crash / không throw exception chưa bắt) khi gặp dòng thiếu cột, sai kiểu số hoặc sai định dạng ngày.

**Kiểm chứng (Verification):**
- [ ] Build & test pass 0 warnings: `g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/Date.cpp src/Validation.cpp src/Models.cpp tests/test_runner.cpp -o build/test_runner.exe && .\build\test_runner.exe`

**Phụ thuộc:** Task 1

**File dự kiến chạm tới:**
- `include/Entity.h`
- `include/Models.h`, `src/Models.cpp`
- `tests/test_runner.cpp`

**Quy mô ước tính:** Nhỏ (4 file)

---

## Task 3: Xây dựng Class Template `Repository<T, KeyType>` và bộ dữ liệu mẫu `data/*.txt`

**Phụ trách:** Khoa (`Repository.h`) + Hiếu (`data/*.txt`, unit tests)

**Mô tả:** Cài đặt **Class Template `Repository<T, KeyType>`** trong `include/Repository.h` kết hợp `std::list<T>` và `std::map<KeyType, typename std::list<T>::iterator>` với đầy đủ thao tác CRUD, member template `findWhere<Predicate>`, cùng các hàm đọc/ghi đè/ghi nối file (`loadFromFile`, `saveAllToFile`, `appendToFile`). Tạo bộ 4 file dữ liệu mẫu hợp lệ trong `data/` và bổ sung unit tests chi tiết cho `Repository<T, KeyType>`.

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] `Repository<T, KeyType>` hoạt động đúng cho cả khóa `std::string` (`Admin`, `Book`, `Reader`) và khóa `int` (`Loan`): thêm không trùng khóa, tra cứu `O(log n)`, xóa `O(1)`, lọc bằng `findWhere`.
- [ ] `appendToFile` tự động thêm `\n` nếu file không rỗng và chưa có ký tự xuống dòng ở cuối; `loadFromFile` xử lý tốt CRLF/LF và dòng trống.
- [ ] Tạo sẵn dữ liệu mẫu phong phú, nhất quán tình trạng sách và phiếu mượn trong `data/Admin.txt`, `data/Sach.txt`, `data/BanDoc.txt`, `data/PhieuMuon.txt`.

**Kiểm chứng (Verification):**
- [ ] Build & test pass 0 warnings: `g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/Date.cpp src/Validation.cpp src/Models.cpp tests/test_runner.cpp -o build/test_runner.exe && .\build\test_runner.exe`

**Phụ thuộc:** Task 2

**File dự kiến chạm tới:**
- `include/Repository.h`
- `data/Admin.txt`, `data/Sach.txt`, `data/BanDoc.txt`, `data/PhieuMuon.txt`
- `tests/test_runner.cpp`

**Quy mô ước tính:** Trung bình (2 file code + 4 file dữ liệu mẫu)

---

## Checkpoint 1: Sau Tasks 1–3
- [ ] Toàn bộ unit tests cho `Date`, `Validation`, `Entity`/`Models`, `Repository<T, KeyType>` pass 100%.
- [ ] Biên dịch sạch 0 warnings với `-Wall -Wextra -Wpedantic`.

---

## Giai đoạn 2: Các lát cắt nghiệp vụ chính (Core Vertical Slices)

## Task 4: Xây dựng `ConsoleUI` và luồng Đăng nhập (`Admin.txt`)

**Phụ trách:** Nhật (`ConsoleUI`, màn hình `DANG NHAP HE THONG`) + Khoa (`LibraryService::loadAll`, `authenticate`) + Hiếu (unit tests)

**Mô tả:** Cài đặt lớp `ConsoleUI` hỗ trợ tô màu Windows Console (`enum class ConsoleColor`), vẽ khung tiêu đề, in thông báo thành công/lỗi/cảnh báo, in bảng căn cột, đọc chuỗi/số (kèm Function Template đọc & validate đầu vào), và đọc input có bắt phím `ESC` + che mật khẩu bằng dấu `*` (xử lý `Backspace`, bỏ qua phím mũi tên/phím chức năng). Khởi tạo lớp `LibraryService` (tải 4 file qua `Repository`, xác thực `authenticate`) và dựng màn hình `DANG NHAP HE THONG` (sai 3 lần thoát, `ESC` thoát).

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] Màn hình đăng nhập hiển thị khung dấu `*` với tiêu đề `DANG NHAP HE THONG`, 2 dòng `User:` và `Password:` giống hình mẫu trang 195 (tiếng Việt không dấu, có màu dễ nhìn).
- [ ] Khi nhập mật khẩu hiển thị `*`, phím `Backspace` xóa đúng ký tự, phím mũi tên/F1–F12 không sinh ký tự rác, phím `ESC` ở cả `User` lẫn `Password` thoát ngay lập tức.
- [ ] `LibraryService::authenticate` xác thực chính xác cặp `(user, password)` trong `Admin.txt`; sai 3 lần tự động thoát chương trình.

**Kiểm chứng (Verification):**
- [ ] Unit test pass cho `LibraryService::loadAll()` và `authenticate()` (đúng, sai mật khẩu, sai user, file thiếu).
- [ ] Kiểm thử thủ công trên Windows Console: test `*`, `Backspace`, phím mũi tên, `ESC`, nhập sai 3 lần.

**Phụ thuộc:** Task 3

**File dự kiến chạm tới:**
- `include/ConsoleUI.h`, `src/ConsoleUI.cpp`
- `include/LibraryService.h`, `src/LibraryService.cpp`
- `tests/test_runner.cpp`

**Quy mô ước tính:** Trung bình (5 file)

---

## Task 5: Lát cắt Quản lý sách (Hiển thị, Thêm, Xóa, Tìm kiếm)

**Phụ trách:** Nhật (nghiệp vụ & UI Quản lý sách) + Hiếu (unit tests) + Vinh (tinh chỉnh bảng 9 cột)

**Mô tả:** Cài đặt đầy đủ nghiệp vụ Quản lý sách trong `LibraryService` và giao diện menu con trong `AppController`: hiển thị toàn bộ danh sách sách (9 cột), thêm sách mới (validate mọi ràng buộc miền giá trị, chống trùng `MaSach`, đặt `TinhTrang = 0`, ghi nối vào cuối `Sach.txt`), xóa sách (chỉ xóa khi tồn tại và `TinhTrang == 0`, ghi đè `Sach.txt`, rollback nếu lỗi ghi file), và tìm kiếm sách theo `MaSach` / `TenSach` / `TacGia`.

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] Hiển thị bảng đầy đủ 9 trường của tất cả sách, phân màu rõ sách sẵn sàng (`0`) và đang mượn (`> 0`).
- [ ] Thêm sách trùng `MaSach` báo lỗi *"Sach da ton tai"* và không thêm; chặn giá bán `<= 0`, số trang `<= 0`, năm phát hành ngoài `[1000, năm hiện tại]`, ngày nhập kho sai hoặc ở tương lai; thêm thành công ghi nối cuối `Sach.txt`.
- [ ] Xóa sách không tồn tại báo lỗi; xóa sách có `TinhTrang != 0` báo *"Khong the xoa sach do sach dang duoc muon"*; xóa sách có `TinhTrang == 0` xóa khỏi RAM và ghi đè `Sach.txt`.
- [ ] Tìm kiếm hoạt động chính xác theo `MaSach` (`O(log n)`), theo `TenSach` và `TacGia` (chuỗi con không phân biệt hoa/thường).

**Kiểm chứng (Verification):**
- [ ] Unit tests pass cho toàn bộ ca Thêm/Trùng mã/Validate sai/Xóa(0 & !=0)/Tìm kiếm và kiểm chứng tải lại từ `Sach.txt`.
- [ ] Build sạch 0 warnings.

**Phụ thuộc:** Task 4

**File dự kiến chạm tới:**
- `include/LibraryService.h`, `src/LibraryService.cpp`
- `include/AppController.h`, `src/AppController.cpp`
- `tests/test_runner.cpp`

**Quy mô ước tính:** Trung bình (5 file)

---

## Checkpoint 2: Sau Tasks 4–5
- [ ] Toàn bộ unit tests cho Foundation + Đăng nhập + Quản lý sách pass 100%.
- [ ] Biên dịch sạch 0 warnings, luồng Đăng nhập -> Quản lý sách chạy ổn định.

---

## Task 6: Lát cắt Quản lý bạn đọc (Hiển thị, Thêm bạn đọc)

**Phụ trách:** Hiếu (nghiệp vụ, UI Quản lý bạn đọc & unit tests) — *Khoa review*

**Mô tả:** Cài đặt nghiệp vụ Quản lý bạn đọc trong `LibraryService` (dùng `Repository<Reader, std::string>`) và menu con Quản lý bạn đọc trong `AppController`: hiển thị danh sách bạn đọc và thêm bạn đọc mới (kiểm tra trùng `MaBanDoc`, validate họ tên và ngày đăng ký mặc định hôm nay, ghi nối vào `BanDoc.txt`).

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] Hiển thị bảng đầy đủ `MaBanDoc`, `HoTen`, `NgayDangKy` của tất cả bạn đọc.
- [ ] Thêm bạn đọc trùng `MaBanDoc` hoặc thiếu họ tên / ngày đăng ký sai bị từ chối với thông báo rõ ràng.
- [ ] Thêm bạn đọc thành công ghi nối vào `BanDoc.txt` và sử dụng được ngay để tạo phiếu mượn trong cùng phiên làm việc.

**Kiểm chứng (Verification):**
- [ ] Unit tests pass cho `addReader`, chặn trùng mã, validate sai và kiểm chứng tải lại `BanDoc.txt`.
- [ ] Build sạch 0 warnings.

**Phụ thuộc:** Task 5

**File dự kiến chạm tới:**
- `include/LibraryService.h`, `src/LibraryService.cpp`
- `src/AppController.cpp`
- `tests/test_runner.cpp`

**Quy mô ước tính:** Nhỏ (4 file)

---

## Task 7: Lát cắt Quản lý phiếu mượn (Hiển thị, Mượn sách, Trả sách)

**Phụ trách:** Khoa (nghiệp vụ Mượn/Trả, đồng bộ 2 file, Rollback & UI) + Hiếu (E2E tests)

**Mô tả:** Cài đặt nghiệp vụ Quản lý phiếu mượn trong `LibraryService` (dùng `Repository<Loan, int>`) và menu con Quản lý phiếu mượn trong `AppController`: hiển thị toàn bộ phiếu mượn, lập phiếu mượn mới (kiểm tra sách tồn tại & `TinhTrang == 0`, bạn đọc tồn tại, sinh `SoPhieuMuon = max + 1` bắt đầu từ `1`, `NgayMuon = hôm nay`, `NgayPhaiTra = +7` ngày lịch, cập nhật `Sach.TinhTrang = SoPhieuMuon`, lưu đồng bộ 2 file), và trả sách (kiểm tra phiếu tồn tại & `TinhTrang != 0`, đặt phiếu và sách về `0`, lưu đồng bộ 2 file, rollback RAM khi ghi lỗi).

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] Mượn sách tạo đúng `SoPhieuMuon` tăng dần từ `1`, `NgayMuon = hôm nay`, `NgayPhaiTra = NgayMuon + 7`, `PhieuMuon.TinhTrang = 1`, `Sach.TinhTrang = SoPhieuMuon`.
- [ ] Từ chối mượn khi sách không tồn tại, sách đã được mượn (`TinhTrang != 0`), hoặc bạn đọc không tồn tại.
- [ ] Trả sách chỉ thành công khi phiếu tồn tại và `TinhTrang != 0`; đặt cả `PhieuMuon.TinhTrang = 0` và `Sach.TinhTrang = 0`.
- [ ] Sau cả mượn và trả, `Sach.txt` và `PhieuMuon.txt` được cập nhật đồng bộ (xác minh bằng cách khởi tạo một `LibraryService` mới đọc lại từ đĩa khớp 100%).

**Kiểm chứng (Verification):**
- [ ] Full E2E test pass: Thêm bạn đọc -> Thêm sách -> Mượn -> Chặn mượn trùng -> Chặn xóa sách đang mượn -> Trả -> Chặn trả lần 2 -> Xóa được sách -> Đọc lại 4 file khớp hoàn toàn.
- [ ] Build sạch 0 warnings.

**Phụ thuộc:** Task 6

**File dự kiến chạm tới:**
- `include/LibraryService.h`, `src/LibraryService.cpp`
- `src/AppController.cpp`
- `tests/test_runner.cpp`

**Quy mô ước tính:** Trung bình (4 file)

---

## Checkpoint 3: Sau Tasks 6–7
- [ ] Toàn bộ bộ kiểm thử tự động `test_runner.exe` pass 100%.
- [ ] Đã kiểm chứng toàn bộ ràng buộc dữ liệu giữa RAM và 4 file `data/*.txt`.

---

## Giai đoạn 3: Tích hợp, Hoàn thiện & Rà soát C++ Coding Standard

## Task 8: Hoàn thiện `AppController`, `main.cpp` và Rà soát C++ Coding Standard

**Phụ trách:** Khoa (`AppController`, `main.cpp`, audit C++ Coding Standard) + Vinh (luồng tiếp tục/thoát, `QA_Checklist.md`, kiểm thử thủ công & chụp ảnh)

**Mô tả:** Hoàn thiện `src/main.cpp` (tự động nhận diện thư mục `data/` hoặc `../data/`), hoàn chỉnh luồng điều hướng Menu chính/Menu con trong `AppController` theo đúng *"Yêu cầu khác"* trang 196 (sau mỗi thao tác thành công hoặc khi có lỗi bị hủy, người dùng luôn chọn được tiếp tục chức năng khác hoặc thoát chương trình), và tiến hành rà soát **C++ Coding Standard** trên toàn bộ các file `.h`/`.cpp`.

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] Sau mọi thao tác (thành công hoặc lỗi), người dùng có lựa chọn tiếp tục chức năng khác hoặc kết thúc chương trình; nhập liệu cho phép hủy giữa chừng an toàn.
- [ ] Chương trình chạy ổn định khi gọi từ thư mục gốc (`.\build\library_app.exe`) lẫn từ trong `build\`.
- [ ] Audit Coding Standard đạt 100%: header guard đầy đủ, 0 dòng `using namespace std;`, đặt tên nhất quán, không magic number, đầy đủ comment `///`, biên dịch 0 warnings với `-Wall -Wextra -Wpedantic`.

**Kiểm chứng (Verification):**
- [ ] Biên dịch full app & test runner 0 warnings, pass 100% test tự động.
- [ ] Chạy thử nghiệm thủ công đủ các kịch bản (đăng nhập sai 3 lần, ESC, thêm/xóa/tìm sách, thêm bạn đọc, mượn/trả sách, kiểm tra bảng màu sắc).

**Phụ thuộc:** Task 7

**File dự kiến chạm tới:**
- `src/main.cpp`
- `include/AppController.h`, `src/AppController.cpp`
- `src/ConsoleUI.cpp`

**Quy mô ước tính:** Nhỏ (4 file)

---

## Checkpoint 4: Hoàn thiện mã nguồn (8/10 điểm phần kỹ thuật)
- [ ] Đạt đầy đủ 3 tiêu chí mã nguồn: OOP & Template (2đ), C++ Coding Standard (1đ), Chức năng & Ràng buộc dữ liệu (5đ).

---

## Giai đoạn 4: Tài liệu nộp bài nhóm (2 điểm)

## Task 9: Viết `README.md` và Kịch bản quay Video báo cáo (1 điểm)

**Phụ trách:** Vinh (soạn `README.md`, `docs/KichBanVideo.md`) + **Cả 4 thành viên** (Khoa, Nhật, Hiếu, Vinh cùng họp Google Meet quay video)

**Mô tả:** Viết file `README.md` hướng dẫn chi tiết cách biên dịch và chạy dự án trên `g++` (MSYS2/VS Code), Visual Studio và Dev-C++, kèm thông tin tài khoản đăng nhập mẫu và cấu trúc thư mục. Soạn file `docs/KichBanVideo.md` chia cảnh quay demo rõ ràng (5–7 phút) bao phủ đủ 4 hạng mục chấm điểm để nhóm thuyết trình và quay video trôi chảy.

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] `README.md` có lệnh build/test chạy được ngay bằng copy-paste, danh sách tài khoản mẫu và giải thích cấu trúc OOP/Template.
- [ ] `docs/KichBanVideo.md` có kịch bản từng phân đoạn (Giới thiệu nhóm & đề tài -> Giải thích kiến trúc OOP/Template & Coding Standard -> Demo trực tiếp các chức năng + ràng buộc lỗi + kiểm chứng file `.txt` -> Kết luận).

**Kiểm chứng (Verification):**
- [ ] Chạy lại đúng các câu lệnh ghi trong `README.md` để xác nhận hoạt động chính xác.

**Phụ thuộc:** Task 8

**File dự kiến chạm tới:**
- `README.md`
- `docs/KichBanVideo.md`

**Quy mô ước tính:** Nhỏ (2 file)

---

## Task 10: Xây dựng Báo cáo Word `.docx` hoàn chỉnh (1 điểm)

**Phụ trách:** Vinh (biên tập chính `docs/BaoCao_PRO_CTDL_3.docx`) + Khoa, Nhật, Hiếu (cung cấp phần kỹ thuật mình phụ trách & duyệt bài)

**Mô tả:** Soạn và xuất file báo cáo Word chuyên nghiệp (`docs/BaoCao_PRO_CTDL_3.docx`) trình bày đầy đủ: Trang bìa, Bảng phân công thành viên nhóm (2–5 sinh viên), Phân tích bài toán & Ràng buộc dữ liệu, Thiết kế Hướng đối tượng (sơ đồ lớp) & Template (`Repository<T, KeyType>`, `Date`), Cấu trúc dữ liệu STL & Định dạng file `.txt`, Tuân thủ C++ Coding Standard, Kết quả kiểm thử (unit test + giao diện chạy thực tế), và Kết luận.

**Tiêu chí nghiệm thu (Acceptance Criteria):**
- [ ] File `docs/BaoCao_PRO_CTDL_3.docx` mở chuẩn trên Microsoft Word, bố cục mục lục/tiêu đề/bảng biểu/đoạn code rõ ràng, tiếng Việt có dấu chuẩn xác.
- [ ] Nội dung bám sát 100% mã nguồn thực tế đã cài đặt (tên lớp, tên hàm, định dạng file khớp hoàn toàn).

**Kiểm chứng (Verification):**
- [ ] Kiểm tra trực quan các trang báo cáo Word (render ảnh từng trang kiểm tra không vỡ bảng, không lỗi font).

**Phụ thuộc:** Task 9 (và thông tin tên nhóm/thành viên từ người dùng)

**File dự kiến chạm tới:**
- `docs/BaoCao_PRO_CTDL_3.docx`

**Quy mô ước tính:** Trung bình (1 file tài liệu chính)

---

## Checkpoint 5: Hoàn tất toàn bộ đồ án (Complete)
- [ ] Đạt 100% Success Criteria trong `SPEC.md` và đủ 4 tiêu chí chấm điểm trong `Yêu cầu thực hiện Project.pdf`.
- [ ] Review lần cuối cùng người dùng trước khi đóng gói nộp Google Classroom.
