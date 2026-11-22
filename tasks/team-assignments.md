# Phân Công Công Việc Nhóm (4 Thành Viên) — PRO_CTDL_3

Tài liệu phân chia nhiệm vụ chi tiết cho nhóm 4 thành viên thực hiện đồ án **PROJECT 3 (`PRO_CTDL_3`) — Hệ Thống Quản Lý Thư Viện (C++ OOP + Template + STL)** theo [SPEC.md](../SPEC.md) và [tasks/plan.md](plan.md).

---

## 1. Bảng tổng quan phân công thành viên

*(Có thể dùng trực tiếp bảng này cho mục **Bảng phân công công việc** trong Báo cáo Word.)*

| STT | Thành viên | Vai trò chính | Hạng mục phụ trách chính | Mức độ đóng góp |
|---|---|---|---|---|
| 1 | **Khoa** | **Team Lead / Code chính** | Thiết kế kiến trúc OOP & Class Template `Repository<T, KeyType>`, nghiệp vụ Đăng nhập & Quản lý phiếu mượn (Mượn/Trả + đồng bộ 2 file + Rollback), tích hợp `AppController`/`main.cpp`, review C++ Coding Standard & merge code toàn nhóm. | 25% |
| 2 | **Nhật** | **Code phụ (Core & UI)** | Xây dựng lớp OOP `Date` (`+7` ngày, năm nhuận), thực thể `Book` & `Loan`, lớp giao diện `ConsoleUI` (màu sắc, khung, bảng, che mật khẩu `*`, bắt `ESC`), và lát cắt chức năng Quản lý sách (Hiển thị, Thêm, Xóa, Tìm kiếm). | 25% |
| 3 | **Hiếu** | **Code nhẹ & Kiểm thử tự động** | Xây dựng `Validation` & Function Template `parseNumberInRange<T>`, thực thể `Admin` & `Reader`, lát cắt Quản lý bạn đọc, tạo bộ dữ liệu mẫu `data/*.txt`, và viết bộ kiểm thử tự động `tests/test_runner.cpp` phủ đủ mọi ràng buộc dữ liệu. | 25% |
| 4 | **Vinh** | **Code nhẹ, QA & Tài liệu** | Hỗ trợ hoàn thiện bảng hiển thị & luồng hỏi tiếp tục/thoát, lập QA Checklist & kiểm thử thủ công toàn bộ ứng dụng, chụp ảnh minh họa, viết `README.md`, soạn kịch bản quay video (`docs/KichBanVideo.md`) và tổng hợp Báo cáo Word (`docs/BaoCao_PRO_CTDL_3.docx`). | 25% |

> **Cả 4 thành viên (Khoa, Nhật, Hiếu, Vinh)** cùng tham gia thuyết trình và quay **Video báo cáo trên Google Meet**.

---

## 2. Bản đồ sở hữu File (Tránh xung đột Git khi code song song)

Mỗi file chỉ có **1 người chịu trách nhiệm chính** để hạn chế tối đa merge conflict:

| Thành viên | File sở hữu chính | File phối hợp (chỉ sửa qua PR do Khoa duyệt) |
|---|---|---|
| **Khoa** | `include/Entity.h`, `include/Repository.h`, `include/LibraryService.h`, `src/LibraryService.cpp` *(Auth + Loan)*, `include/AppController.h`, `src/main.cpp` | `src/AppController.cpp` *(tích hợp Menu chính & Phiếu mượn)* |
| **Nhật** | `include/Date.h`, `src/Date.cpp`, `include/ConsoleUI.h`, `src/ConsoleUI.cpp` | `include/Models.h`, `src/Models.cpp` *(phần `Book`, `Loan`)*; `src/LibraryService.cpp` *(phần Sách)*; `src/AppController.cpp` *(Menu Sách & Đăng nhập)* |
| **Hiếu** | `include/Validation.h`, `src/Validation.cpp`, `data/Admin.txt`, `data/Sach.txt`, `data/BanDoc.txt`, `data/PhieuMuon.txt`, `tests/test_runner.cpp` | `include/Models.h`, `src/Models.cpp` *(phần `Admin`, `Reader`)*; `src/LibraryService.cpp` *(phần Bạn đọc)*; `src/AppController.cpp` *(Menu Bạn đọc)* |
| **Vinh** | `README.md`, `docs/KichBanVideo.md`, `docs/QA_Checklist.md`, `docs/BaoCao_PRO_CTDL_3.docx` | `src/ConsoleUI.cpp` *(tinh chỉnh căn cột/màu sắc bảng nếu cần)*, `src/AppController.cpp` *(thông báo lỗi thân thiện & luồng hỏi tiếp tục/thoát)* |

---

## 3. Phân chia Task chi tiết cho từng thành viên

### 3.1. Khoa — Team Lead / Code chính

- [ ] **K1 — Khởi tạo khung dự án & Hợp đồng kiến trúc (Ưu tiên làm đầu tiên):**
  - Tạo cấu trúc thư mục (`include/`, `src/`, `data/`, `tests/`, `docs/`), đẩy khung header ban đầu lên nhánh `main` để 3 bạn còn lại có thể bắt tay vào làm ngay.
  - Cài đặt lớp cơ sở trừu tượng `Entity` (`include/Entity.h`).
- [ ] **K2 — Cài đặt Class Template `Repository<T, KeyType>` (`include/Repository.h`):**
  - Kết hợp `std::list<T>` (giữ thứ tự, xóa `O(1)`) và `std::map<KeyType, iterator>` (tra cứu khóa chính `O(log n)`).
  - Cài đặt đầy đủ: `add`, `exists`, `findByKey`, `removeByKey`, member template `findWhere<Predicate>`, `loadFromFile`, `saveAllToFile`, `appendToFile` (tự xử lý thiếu `\n` cuối file, CRLF/LF, bỏ qua dòng trống).
- [ ] **K3 — Nghiệp vụ Đăng nhập & Quản lý Phiếu mượn (`LibraryService`):**
  - Cài đặt khởi nạp 4 kho dữ liệu (`loadAll`) và xác thực `authenticate(user, pass)`.
  - Cài đặt **Mượn sách** (`borrowBook`): kiểm tra sách tồn tại & `TinhTrang == 0`, bạn đọc tồn tại, sinh `SoPhieuMuon = max + 1` (từ `1`), tự tính `NgayMuon = Date::today()` và `NgayPhaiTra = addDays(7)`, cập nhật `Sach.TinhTrang = SoPhieuMuon`, lưu đồng bộ `Sach.txt` & `PhieuMuon.txt` kèm **rollback RAM** nếu ghi lỗi.
  - Cài đặt **Trả sách** (`returnBook`): kiểm tra phiếu tồn tại & `TinhTrang != 0`, đặt `PhieuMuon.TinhTrang = 0` và `Sach.TinhTrang = 0`, lưu đồng bộ 2 file kèm rollback.
- [ ] **K4 — Tích hợp hệ thống & Review C++ Coding Standard (`AppController`, `main.cpp`):**
  - Ghép toàn bộ Menu chính và các Menu con (Đăng nhập, Sách, Phiếu mượn, Bạn đọc).
  - Cài đặt `resolveDataDirectory()` trong `main.cpp` để chạy ổn định dù gọi từ thư mục gốc hay `build/`.
  - Review toàn bộ code của nhóm: kiểm tra header guard, 0 dòng `using namespace std;`, đặt tên chuẩn `m_` / `camelCase` / `PascalCase`, không magic number, biên dịch **0 warnings** với `g++ -std=c++17 -Wall -Wextra -Wpedantic`.

---

### 3.2. Nhật — Code phụ (Lớp `Date`, `ConsoleUI` & Quản lý sách)

- [ ] **N1 — Xây dựng lớp OOP `Date` (`include/Date.h`, `src/Date.cpp`):**
  - Đóng gói `m_day`, `m_month`, `m_year`; kiểm tra hợp lệ ngày tháng (bao gồm năm nhuận 29/02, tháng 30/31 ngày).
  - Cài đặt `Date::today()`, `addDays(int days)` bằng `std::tm`/`std::mktime`, `toString()` (`dd/mm/yyyy`), `fromString(...)`, toán tử so sánh (`==`, `<`, `<=`).
- [ ] **N2 — Cài đặt lớp thực thể `Book` & `Loan` (`Models.h`, `Models.cpp`):**
  - Kế thừa `Entity`, đóng gói toàn bộ thuộc tính `private` (`m_...`), getter/setter hợp lệ, `getKey()`, `serialize()`, `fromFileLine(...)`.
- [ ] **N3 — Xây dựng lớp `ConsoleUI` (`include/ConsoleUI.h`, `src/ConsoleUI.cpp`):**
  - Hệ thống màu sắc Windows Console (`enum class ConsoleColor` dùng `<windows.h>`).
  - Hàm vẽ khung hộp, tiêu đề, thông báo (xanh lá = thành công, đỏ = lỗi, vàng = cảnh báo), in bảng căn cột thẳng hàng (tiếng Việt không dấu).
  - Hàm nhập `User` và `Password` dùng `_getch()`: hiển thị `*` khi nhập mật khẩu, hỗ trợ `Backspace`, bỏ qua phím mũi tên/phím F (`0`/`224`), và nhấn **`ESC` thoát ngay** ở cả 2 trường.
  - Hàm template `readValidatedField<T>` hỗ trợ nhập liệu an toàn và cho phép hủy giữa chừng.
- [ ] **N4 — Lát cắt Quản lý sách & Màn hình Đăng nhập (`LibraryService` + `AppController`):**
  - Dựng màn hình `DANG NHAP HE THONG` đúng khung `*` trang 195 (sai 3 lần tự thoát, `ESC` thoát).
  - Nghiệp vụ & giao diện Quản lý sách:
    - Hiển thị danh sách sách đủ 9 cột (tô màu trạng thái `0 - San sang` / `<SoPhieu> - Dang muon`).
    - Thêm sách mới (chặn trùng `MaSach`, validate giá/số trang/năm/ngày nhập kho `<= hôm nay`, ghi nối cuối `Sach.txt`).
    - Xóa sách (chặn khi `TinhTrang != 0` với thông báo *"Khong the xoa sach do sach dang duoc muon"*, xóa thành công ghi đè `Sach.txt`).
    - Tìm kiếm sách theo `MaSach` (`O(log n)`), theo `TenSach` và `TacGia` (không phân biệt hoa/thường).

---

### 3.3. Hiếu — Code nhẹ, Dữ liệu mẫu & Kiểm thử tự động

- [ ] **H1 — Xây dựng `Validation` & Function Template (`include/Validation.h`, `src/Validation.cpp`):**
  - Cài đặt `trim`, `isBlank`, `containsDelimiter` (chặn `|`), `toLower`, `splitLine`.
  - Cài đặt **Function Template** `template <typename T> bool parseNumberInRange(const std::string& raw, T minVal, T maxVal, T& outVal)` dùng chung cho `int` và `long long`.
- [ ] **H2 — Cài đặt lớp thực thể `Admin` & `Reader` (`Models.h`, `Models.cpp`) + Bộ dữ liệu mẫu `data/*.txt`:**
  - Hoàn thiện 2 lớp `Admin` và `Reader` kế thừa `Entity` (serialize/deserialize khớp định dạng `|`).
  - Tạo bộ dữ liệu mẫu hợp lý và nhất quán giữa 4 file (`data/Admin.txt` ≥ 2 tài khoản, `data/Sach.txt` ≥ 8 đầu sách gồm cả sách sẵn sàng lẫn đang mượn, `data/BanDoc.txt` ≥ 5 bạn đọc, `data/PhieuMuon.txt` ≥ 3 phiếu khớp mã sách đang mượn).
- [ ] **H3 — Lát cắt Quản lý bạn đọc (`LibraryService` + `AppController`):**
  - Hiển thị bảng danh sách bạn đọc (`MaBanDoc`, `HoTen`, `NgayDangKy`).
  - Thêm bạn đọc mới: kiểm tra trùng `MaBanDoc`, validate họ tên không rỗng & không chứa `|`, ngày đăng ký hợp lệ `<= hôm nay` (Enter để lấy hôm nay), ghi nối vào `BanDoc.txt`.
- [ ] **H4 — Viết & duy trì bộ Kiểm thử tự động (`tests/test_runner.cpp`):**
  - Viết test cho `Date` (năm nhuận, `+7` ngày qua cuối tháng & 31/12).
  - Viết test cho `Validation` & `parseNumberInRange<T>`.
  - Viết test cho `Repository<T, KeyType>` (CRUD, `findWhere`, `appendToFile` thiếu `\n`, CRLF).
  - Viết test E2E cho `LibraryService`: Đăng nhập -> Thêm bạn đọc -> Thêm sách -> Chặn trùng mã -> Mượn sách -> Chặn mượn trùng -> Chặn xóa sách đang mượn -> Trả sách -> Chặn trả lần 2 -> Xóa được sách -> Tải lại từ file khớp 100%.

---

### 3.4. Vinh — Code nhẹ (UX), QA Thủ công, README & Báo cáo Word

- [ ] **V1 — Hoàn thiện trải nghiệm Console & Luồng xử lý lỗi (Code nhẹ):**
  - Phối hợp cùng Nhật & Khoa tinh chỉnh độ rộng cột bảng hiển thị Sách (9 cột), Bạn đọc (3 cột), Phiếu mượn (6 cột) không bị xuống dòng vỡ khung trên cửa sổ Console chuẩn.
  - Chuẩn hóa luồng *"Yêu cầu khác"* trang 196: sau mỗi chức năng (hoặc khi thao tác bị hủy do lỗi), hiển thị lựa chọn rõ ràng `[1] Tiep tuc chuc nang khac` / `[0] Thoat chuong trinh`.
- [ ] **V2 — Kiểm thử thủ công (QA) & Chụp ảnh báo cáo (`docs/QA_Checklist.md`):**
  - Lập bảng checklist test tay đầy đủ mọi ca thường & ca biên: đăng nhập sai 3 lần, nhấn `ESC` lúc đang gõ User/Password, bấm phím mũi tên trong ô Password, nhập chữ vào giá bán/số trang, nhập ký tự `|`, mượn sách đang bận, xóa sách đang mượn, tắt app mở lại kiểm tra dữ liệu còn nguyên.
  - Chụp ảnh màn hình rõ nét từng chức năng để chèn vào Báo cáo Word; gửi danh sách bug (nếu có) cho nhóm sửa dứt điểm trước khi quay video.
- [ ] **V3 — Viết `README.md` & Kịch bản Video (`docs/KichBanVideo.md`):**
  - Viết hướng dẫn build/chạy trên `g++`, Visual Studio và Dev-C++, kèm tài khoản đăng nhập mẫu.
  - Soạn kịch bản chia lời thoại chi tiết cho cả 4 bạn quay Google Meet (Mục 5 bên dưới).
- [ ] **V4 — Tổng hợp & Định dạng Báo cáo Word (`docs/BaoCao_PRO_CTDL_3.docx`):**
  - Thu thập phần giải thích kỹ thuật từ Khoa (OOP/Template/Coding Standard), Nhật (`Date`/UI/Sách), Hiếu (Validation/Test).
  - Biên tập hoàn chỉnh file `.docx` gồm: Trang bìa, Mục lục, Bảng phân công nhóm, Phân tích yêu cầu & Ràng buộc dữ liệu, Sơ đồ lớp OOP & Thiết kế Template, Cấu trúc dữ liệu STL & Định dạng file `.txt`, Minh chứng tuân thủ C++ Coding Standard, Ảnh chạy thực tế & Kết quả kiểm thử, Kết luận.

---

## 4. Lộ trình phối hợp theo từng mốc (4 Mốc đồng bộ)

| Mốc | Nội dung thực hiện song song | Điểm đồng bộ (Sync Gate) |
|---|---|---|
| **Mốc 1 — Nền tảng** | • **Khoa:** Khung repo + `Entity.h` + `Repository.h`<br>• **Nhật:** `Date.h/.cpp` + `Book`/`Loan`<br>• **Hiếu:** `Validation.h/.cpp` + `Admin`/`Reader` + `data/*.txt`<br>• **Vinh:** Tạo khung `docs/QA_Checklist.md` & dàn ý Báo cáo Word | Merge toàn bộ về `main`, chạy `test_runner.exe` (Checkpoint 1) đạt **0 warnings, pass 100%**. |
| **Mốc 2 — Tính năng cốt lõi** | • **Khoa:** `LibraryService` (Auth + Mượn/Trả + Rollback)<br>• **Nhật:** `ConsoleUI` + Màn hình Đăng nhập + Quản lý sách<br>• **Hiếu:** Quản lý bạn đọc + bổ sung test `LibraryService`<br>• **Vinh:** Tinh chỉnh căn bảng Console + viết `README.md` | Merge về `main`, pass toàn bộ test E2E (Checkpoint 2 & 3). |
| **Mốc 3 — Tích hợp & QA** | • **Khoa:** Ghép `AppController`/`main.cpp` + Audit C++ Coding Standard<br>• **Nhật & Hiếu:** Sửa bug (nếu QA phát hiện) + hoàn thiện test edge-case<br>• **Vinh:** Chạy hết `QA_Checklist.md`, chụp ảnh màn hình chức năng | Đóng băng mã nguồn (Code Freeze), xác nhận đạt **8/8 điểm phần kỹ thuật** (Checkpoint 4). |
| **Mốc 4 — Tài liệu & Video** | • **Vinh:** Hoàn thiện `docs/BaoCao_PRO_CTDL_3.docx` & `docs/KichBanVideo.md`<br>• **Khoa, Nhật, Hiếu:** Đọc duyệt nội dung phần mình phụ trách trong Word<br>• **Cả 4 thành viên:** Họp Google Meet quay Video báo cáo | Kiểm tra đủ file nộp: Source code + `data/*.txt` + Báo cáo Word + Video (Checkpoint 5). |

---

## 5. Kịch bản phân vai quay Video Báo Cáo trên Google Meet (Cả 4 thành viên — 6 đến 8 phút)

| Thời lượng | Người trình bày | Nội dung trình bày & Thao tác trên màn hình |
|---|---|---|
| **0:00 – 1:00** *(1 phút)* | **Vinh** *(Mở đầu)* | Chào thầy/cô, giới thiệu 4 thành viên nhóm, tên đề tài **PRO_CTDL_3 — Quản lý thư viện**, tổng quan 4 yêu cầu chấm điểm và cấu trúc 4 file dữ liệu trong thư mục `data/`. |
| **1:00 – 2:45** *(1 phút 45s)* | **Khoa** *(Kiến trúc OOP & Template)* | Share VS Code: giải thích nhanh sơ đồ OOP (`Entity` -> `Admin`, `Book`, `Reader`, `Loan`, `Date`), giải thích **Class Template `Repository<T, KeyType>`** kết hợp `std::list` + `std::map`, và chứng minh tuân thủ **C++ Coding Standard** (build trực tiếp `g++ -Wall -Wextra -Wpedantic` ra **0 warnings**). |
| **2:45 – 4:30** *(1 phút 45s)* | **Nhật** *(Demo Đăng nhập & Sách)* | Chạy `library_app.exe`: demo màn hình `DANG NHAP HE THONG` (nhập sai mật khẩu có dấu `*`, thử `ESC` thoát, sau đó đăng nhập thành công); demo **Quản lý sách** (Hiển thị bảng 9 cột có màu, Thêm sách mới + mở `Sach.txt` cho thấy đã ghi nối, Tìm kiếm theo Mã/Tên/Tác giả). |
| **4:30 – 6:00** *(1 phút 30s)* | **Hiếu** *(Demo Bạn đọc, Ràng buộc & Test)* | Demo **Quản lý bạn đọc** (Xem & Thêm bạn đọc mới vào `BanDoc.txt`), thử cố tình nhập sai ràng buộc (trùng mã sách, giá âm, xóa sách đang được mượn bị chặn); chạy `test_runner.exe` cho thấy **Pass 100%** mọi ca kiểm thử tự động. |
| **6:00 – 7:30** *(1 phút 30s)* | **Khoa** *(Demo Mượn/Trả)* & **Vinh** *(Kết luận)* | **Khoa** demo Mượn sách (số phiếu tăng tự động, tự tính `+7` ngày) → mở `Sach.txt` & `PhieuMuon.txt` chứng minh đồng bộ → Trả sách → quay lại Xóa thành công cuốn sách vừa trả. **Vinh** tổng kết ưu điểm/hạn chế và cảm ơn thầy/cô. |

### Checklist nhanh trước khi bấm Record trên Google Meet
- [ ] Tăng cỡ chữ Terminal/Console lên **18–20pt**, nền tối để bảng màu hiện rõ trên bản ghi video.
- [ ] Tắt thông báo Zalo/Messenger/Windows trên máy người share màn hình.
- [ ] Reset thư mục `data/` về trạng thái dữ liệu mẫu sạch trước khi bấm quay.
- [ ] Thử mic cả 4 thành viên 10 giây trước khi bắt đầu; chuyển người nói mượt mà (để 1 máy chạy demo chính hoặc luân phiên share màn hình đã mở sẵn).

---

## 6. Quy ước Git cho nhóm 4 người

- Nhánh chính: `main` (luôn biên dịch chạy được, chỉ **Khoa** merge sau khi kiểm tra không lỗi & đúng Coding Standard).
- Nhánh làm việc theo thành viên:
  - `feat/khoa-core-repository-loan`
  - `feat/nhat-date-ui-books`
  - `feat/hieu-validation-readers-tests`
  - `docs/vinh-qa-readme-report`
- Trước khi tạo Pull Request, mỗi thành viên tự chạy lệnh biên dịch + `test_runner.exe` trong [SPEC.md](../SPEC.md) để đảm bảo **0 warnings** và **pass 100% test**.
