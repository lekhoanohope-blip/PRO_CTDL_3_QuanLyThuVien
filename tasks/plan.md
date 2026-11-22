# Kế Hoạch Triển Khai: PRO_CTDL_3 — Hệ Thống Quản Lý Thư Viện

## 1. Tổng quan

Xây dựng ứng dụng Console C++17 quản lý thư viện theo đặc tả tại [SPEC.md](../SPEC.md), đáp ứng trọn vẹn đề bài **PROJECT 3 (`PRO_CTDL_3`)** và bộ tiêu chí chấm điểm trong **[Yêu cầu thực hiện Project.pdf](../Y%C3%AAu%20c%E1%BA%A7u%20th%E1%BB%B1c%20hi%E1%BB%87n%20Project.pdf)**:

- **2 điểm:** Cài đặt chuẩn Hướng đối tượng (OOP) và Template.
- **1 điểm:** Tuân thủ nghiêm ngặt C++ Coding Standard.
- **5 điểm:** Đầy đủ mọi chức năng và ràng buộc dữ liệu.
- **2 điểm:** Báo cáo Word (`.docx`) và Kịch bản/Video báo cáo project nhóm.

Dự án được chia thành 4 giai đoạn (10 task nguyên tử) đi theo lát cắt dọc (vertical slices), mỗi giai đoạn có checkpoint kiểm thử rõ ràng. Toàn bộ tài liệu kế hoạch/báo cáo dùng **tiếng Việt có dấu**; riêng giao diện hiển thị trong mã nguồn Console C++ dùng **tiếng Việt không dấu** để tránh lỗi font trên mọi máy Windows.

---

## 2. Quyết định kiến trúc chính

1. **Kiến trúc OOP có kế thừa & đa hình tự nhiên:**
   - Lớp cơ sở trừu tượng `Entity` định nghĩa giao diện chung `serialize() const` và `getEntityType() const`.
   - Các lớp `Admin`, `Book`, `Reader`, `Loan` kế thừa `Entity`, đóng gói thuộc tính `private` (`m_...`), kiểm soát tính hợp lệ qua constructor/setter và hỗ trợ nạp/xuất file `.txt`.
   - Lớp `Date` đóng gói hoàn chỉnh nghiệp vụ ngày tháng (`today()`, `addDays(7)`, kiểm tra năm nhuận, toán tử so sánh `<=`).
2. **Class Template `Repository<T, KeyType>` + Function Templates kết hợp STL:**
   - Thay vì viết lặp lại logic quản lý danh sách và đọc/ghi file cho 4 thực thể, dùng duy nhất **Class Template `Repository<T, KeyType>`** bọc `std::list<T>` (giữ thứ tự, xóa `O(1)` không vô hiệu hóa iterator) và `std::map<KeyType, iterator>` (Red-Black Tree tra cứu `O(log n)`).
   - Cung cấp thêm member template `findWhere<Predicate>` (phục vụ tìm kiếm theo tên sách, tác giả) và function template `parseNumberInRange<T>` (validate mọi kiểu số nguyên).
3. **Tách biệt hoàn toàn Business Logic khỏi Console UI:**
   - `LibraryService` không dùng `std::cin` / `std::cout`, chỉ trả về `ServiceResult` để kiểm thử tự động 100% luồng nghiệp vụ và file I/O bằng `tests/test_runner.cpp`.
   - Mọi lệnh phụ thuộc Windows (`<windows.h>`, `<conio.h>`) được cô lập trong `ConsoleUI.cpp`, giữ mã nguồn sạch, portable giữa `g++`, Visual Studio và Dev-C++.
4. **Đảm bảo tính toàn vẹn dữ liệu & Rollback:**
   - Mọi thao tác mượn sách, trả sách, xóa sách đều giữ bản sao trạng thái trước khi cập nhật đĩa; nếu ghi bất kỳ file nào thất bại thì khôi phục lại RAM và báo lỗi rõ ràng.
5. **Tuân thủ C++ Coding Standard ngay từ dòng code đầu tiên:**
   - Header guards chuẩn, cấm `using namespace std;`, đặt tên nhất quán (`PascalCase` / `camelCase` / `m_` / `UPPER_SNAKE_CASE`), `const`-correctness, không magic numbers, comment Doxygen rõ ràng, biên dịch 0 warnings với `-Wall -Wextra -Wpedantic`.

---

## 3. Sơ đồ phụ thuộc giữa các thành phần

```text
Entity.h + Date (Task 1) + Validation Templates (Task 1)
                    │
                    ▼
   Models OOP: Admin, Book, Reader, Loan (Task 2)
                    │
                    ▼
   Repository<T, KeyType> + Dữ liệu mẫu data/*.txt (Task 3)
                    │
        ┌───────────┼───────────┬──────────────┐
        ▼           ▼           ▼              ▼
  ConsoleUI +   Quản lý     Quản lý       Quản lý
  Đăng nhập      Sách       Bạn đọc      Phiếu mượn
   (Task 4)    (Task 5)    (Task 6)       (Task 7)
        └───────────┴───────────┴──────────────┘
                    │
                    ▼
      AppController + main.cpp + Kiểm thử toàn diện (Task 8)
                    │
                    ▼
      README.md + Báo cáo Word (.docx) & Kịch bản Video (Task 9–10)
```

---

## 4. Lộ trình triển khai (Task List)

*(Chi tiết Acceptance Criteria, câu lệnh kiểm thử và danh sách file cho từng task nằm tại [tasks/todo.md](todo.md); bảng phân công chi tiết cho 4 thành viên Khoa, Nhật, Hiếu, Vinh nằm tại [tasks/team-assignments.md](team-assignments.md).)*

### Giai đoạn 1: Nền tảng OOP, Template & Lưu trữ (Foundation)
- [ ] **Task 1 (S):** Xây dựng lớp `Date`, `Validation` (kèm Function Template `parseNumberInRange<T>`) và khung kiểm thử `tests/test_runner.cpp`.
- [ ] **Task 2 (S):** Xây dựng lớp cơ sở trừu tượng `Entity` và 4 lớp thực thể OOP (`Admin`, `Book`, `Reader`, `Loan`) + unit tests serialize/deserialize & ràng buộc miền giá trị.
- [ ] **Task 3 (M):** Xây dựng Class Template `Repository<T, KeyType>` (STL `list` + `map` + đọc/ghi đè/ghi nối file an toàn) + tạo bộ dữ liệu mẫu `data/*.txt` + unit tests.

### Checkpoint 1: Nền tảng OOP & Template
- [ ] Biên dịch 0 warnings (`-Wall -Wextra -Wpedantic`), `test_runner.exe` pass 100% cho `Date`, `Validation`, `Models`, `Repository<T, KeyType>`.

### Giai đoạn 2: Các lát cắt nghiệp vụ chính (Core Vertical Slices)
- [ ] **Task 4 (M):** Xây dựng `ConsoleUI` (màu sắc, khung viền, bảng căn cột, nhập mật khẩu `*`, bắt phím `ESC`, template đọc dữ liệu) + luồng **Đăng nhập** (`LibraryService::authenticate` & màn hình `DANG NHAP HE THONG`).
- [ ] **Task 5 (M):** Lát cắt **Quản lý sách** (Hiển thị 9 cột, Thêm sách + ghi nối `Sach.txt`, Xóa sách chặn khi `TinhTrang != 0` + ghi đè `Sach.txt`, Tìm kiếm theo Mã/Tên/Tác giả) + unit tests.

### Checkpoint 2: Giao diện, Đăng nhập & Quản lý sách
- [ ] Build toàn bộ app 0 warnings; pass 100% test Đăng nhập + Quản lý sách; chạy thử màn hình đăng nhập thực tế.

- [ ] **Task 6 (S):** Lát cắt **Quản lý bạn đọc** (Hiển thị danh sách, Thêm bạn đọc + ghi nối `BanDoc.txt`, chống trùng `MaBanDoc`) + unit tests.
- [ ] **Task 7 (M):** Lát cắt **Quản lý phiếu mượn** (Hiển thị phiếu, Mượn sách +7 ngày tự động & tăng số phiếu, Trả sách, cập nhật đồng bộ `Sach.txt` & `PhieuMuon.txt`, rollback khi lỗi) + unit tests E2E.

### Checkpoint 3: Hoàn thiện toàn bộ nghiệp vụ cốt lõi
- [ ] Pass 100% bộ kiểm thử E2E nghiệp vụ & kiểm tra đối chiếu toàn bộ ràng buộc dữ liệu (hạng mục 5 điểm).

### Giai đoạn 3: Tích hợp, Hoàn thiện & Rà soát Coding Standard
- [ ] **Task 8 (S):** Hoàn thiện `AppController`, `main.cpp` (tự động nhận diện `data/`, điều hướng tiếp tục/thoát sau mỗi thao tác và khi có lỗi), rà soát toàn diện **C++ Coding Standard** & kiểm thử thủ công các ca biên.

### Checkpoint 4: Sẵn sàng mã nguồn
- [ ] Mã nguồn đạt 100% tiêu chí OOP + Template (2đ), C++ Coding Standard (1đ) và Chức năng + Ràng buộc dữ liệu (5đ).

### Giai đoạn 4: Tài liệu nộp bài nhóm (2 điểm)
- [ ] **Task 9 (S):** Viết `README.md` hướng dẫn build/chạy trên `g++`, Visual Studio và Dev-C++; soạn **Kịch bản quay Video demo** chi tiết từng cảnh (5–7 phút) để cả nhóm dễ quay.
- [ ] **Task 10 (M):** Lập **Báo cáo Word (`.docx`)** chuyên nghiệp (trang bìa, bảng phân công nhóm, phân tích yêu cầu & ràng buộc dữ liệu, sơ đồ lớp OOP & thiết kế Template, giải thích CTDL/File, quy chuẩn Coding Standard, kết quả kiểm thử).

### Checkpoint 5: Hoàn tất toàn bộ đồ án (Complete)
- [ ] Đủ 4 hạng mục chấm điểm (10/10), sẵn sàng nộp lên Google Classroom.

---

## 5. Rủi ro và Phương án giảm thiểu

| Rủi ro | Mức độ | Phương án giảm thiểu |
|---|---|---|
| Bị trừ 2 điểm nếu giảng viên đánh giá "thiếu OOP hoặc thiếu Template thực chất" | **Cao** | Thiết kế rõ kế thừa đa hình từ `Entity`, đóng gói lớp `Date` + 4 lớp thực thể, và cài đặt Class Template `Repository<T, KeyType>` + Function Templates thực sự gánh trách nhiệm lõi của hệ thống. |
| Bị trừ 1 điểm do vi phạm C++ Coding Standard (quên `const`, dùng `using namespace std;`, magic number, warning) | **Cao** | Bật `-Wall -Wextra -Wpedantic` ở mọi lần build; có bước audit Coding Standard riêng ở Task 8 kiểm tra từng file `.h`/`.cpp`. |
| File `Sach.txt` / `BanDoc.txt` không kết thúc bằng `\n` khiến ghi nối (`append`) bị dính 2 bản ghi chung dòng | **Cao** | Phương thức `Repository::appendToFile` kiểm tra byte cuối cùng của file; nếu file không rỗng và byte cuối khác `\n`, tự động ghi `\n` trước khi nối dòng mới. |
| Phím mũi tên / phím F1–F12 tạo ký tự rác khi đọc mật khẩu bằng `_getch()` | **Trung bình** | Bắt mã tiền tố `0` và `224` rồi gọi `_getch()` thứ hai để bỏ qua; chỉ chấp nhận ký tự ASCII in được (`32..126`), `Enter`, `Backspace`, `ESC`. |
| Cộng `+7` ngày sai vào cuối tháng hoặc năm nhuận | **Cao** | Dùng `std::tm` + `std::mktime` trong lớp `Date` và viết unit test phủ đủ tháng 28/29/30/31 ngày và chuyển năm 31/12. |
| Lệch dữ liệu giữa `Sach.txt` và `PhieuMuon.txt` khi ghi file gặp lỗi | **Cao** | Lưu trạng thái dự phòng trước khi lưu đĩa; nếu ghi một trong hai file thất bại thì rollback RAM và trả lỗi rõ ràng. |
| Chương trình chạy từ thư mục `build/` không tìm thấy `data/` | **Trung bình** | Hàm `resolveDataDirectory()` trong `main.cpp` tự động kiểm tra `data/` và `../data/`, báo lỗi dễ hiểu nếu thiếu `Admin.txt`. |

---

## 6. Điểm cần người dùng xác nhận

- Bạn kiểm tra lại [SPEC.md](../SPEC.md), kế hoạch này và [tasks/todo.md](todo.md) — đặc biệt là cấu trúc OOP (`Entity`, `Date`, `Admin`, `Book`, `Reader`, `Loan`) và Class Template (`Repository<T, KeyType>`).
- Khi bạn xác nhận duyệt, mình sẽ bắt đầu triển khai tuần tự từ **Task 1** (Phase 1–3 cho mã nguồn trước, sau đó sang Phase 4 làm báo cáo Word & kịch bản video khi bạn cung cấp thông tin thành viên nhóm).
