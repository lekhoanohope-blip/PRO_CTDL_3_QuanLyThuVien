# Đặc Tả Kỹ Thuật (SPEC): PRO_CTDL_3 — Hệ Thống Quản Lý Thư Viện (Console C++)

## 1. Mục tiêu & Tiêu chí chấm điểm

Xây dựng chương trình Console C++ quản lý hoạt động hằng ngày của thư viện theo đề bài **PROJECT 3 (`PRO_CTDL_3`, trang 194–196)** và tài liệu **[Yêu cầu thực hiện Project.pdf](Y%C3%AAu%20c%E1%BA%A7u%20th%E1%BB%B1c%20hi%E1%BB%87n%20Project.pdf)**.

### Phân bổ điểm theo yêu cầu giảng viên (Tổng 10 điểm)
| Hạng mục | Trọng số | Yêu cầu bắt buộc để đạt điểm tối đa |
|---|---|---|
| **Hướng đối tượng (OOP) & Template** | **2 điểm** | Toàn bộ hệ thống cài đặt theo chuẩn OOP (đóng gói, kế thừa, đa hình, `const`-correctness) và có sử dụng **Class Template + Function Template** thực chất. Làm theo hướng khác bị trừ 2 điểm. |
| **Tuân thủ C++ Coding Standard** | **1 điểm** | Thống nhất quy ước đặt tên, header guard, không `using namespace std;` toàn cục, tránh magic number, tách rõ `.h`/`.cpp`, biên dịch 0 warnings. Không tuân thủ bị trừ 1 điểm. |
| **Chức năng & Ràng buộc dữ liệu** | **5 điểm** | Thực hiện đầy đủ mọi chức năng và ràng buộc dữ liệu (đăng nhập, quản lý sách, quản lý bạn đọc, mượn/trả sách, tìm kiếm sách, lưu file đồng bộ, xử lý lỗi & màu sắc). Thiếu bị trừ 0.5–1 điểm/mục. |
| **Báo cáo Word & Video báo cáo** | **2 điểm** | Báo cáo Word (`.docx`, 1 điểm) + Video demo/báo cáo project nhóm 2–5 sinh viên (1 điểm). |

### Quy ước ngôn ngữ hiển thị
- **Tài liệu (Spec, Plan, Todo, README, Báo cáo Word):** Tiếng Việt **có dấu** đầy đủ.
- **Mã nguồn C++ & Giao diện Console chạy thực tế:** Tiếng Việt **không dấu** (giống hình mẫu `DANG NHAP HE THONG` trong đề bài) nhằm đảm bảo hiển thị an toàn 100%, không lỗi font trên mọi máy chấm bài Windows.

---

## 2. Công nghệ & Kiến trúc OOP + Template

- **Ngôn ngữ:** C++17 (viết theo chuẩn portable, tương thích `g++` MSYS2 UCRT64, Visual Studio MSVC và Dev-C++).
- **Thiết kế Hướng đối tượng (OOP):**
  - **Lớp cơ sở trừu tượng (`Entity`):** Khai báo hủy ảo (`virtual ~Entity() = default;`), phương thức thuần ảo `virtual std::string serialize() const = 0;` và `virtual std::string getEntityType() const = 0;`.
  - **Các lớp thực thể kế thừa `Entity`:** `Admin`, `Book`, `Reader`, `Loan` — đóng gói toàn bộ dữ liệu trong `private` (tiền tố `m_`), cung cấp constructor, getter/setter có kiểm tra hợp lệ, phương thức `getKey()`, `serialize()`, và `static bool fromFileLine(...)`.
  - **Lớp giá trị `Date`:** Đóng gói `m_day`, `m_month`, `m_year`; tự động kiểm tra hợp lệ lịch thực (kể cả năm nhuận); hỗ trợ `Date::today()`, `addDays(int)`, `toString()`, `fromString()`, cùng các toán tử so sánh (`==`, `<=`, `<`) để kiểm tra ràng buộc ngày.
  - **Tách biệt tầng rõ ràng:** `LibraryService` (nghiệp vụ) tách hoàn toàn khỏi `ConsoleUI` và `AppController` (giao diện Console) để có thể kiểm thử tự động độc lập.
- **Thiết kế Template kết hợp STL:**
  - **Class Template `Repository<T, KeyType>`** (`include/Repository.h`): Kho lưu trữ tổng quát dùng chung cho cả 4 thực thể (`Repository<Admin, std::string>`, `Repository<Book, std::string>`, `Repository<Reader, std::string>`, `Repository<Loan, int>`):
    - Bên trong kết hợp `std::list<T>` (Danh sách liên kết kép — giữ đúng thứ tự bản ghi, xóa `O(1)` không làm vô hiệu hóa iterator) và `std::map<KeyType, typename std::list<T>::iterator>` (Cây tìm kiếm cân bằng Red-Black Tree — tra cứu khóa chính `O(log n)`).
    - Cung cấp bộ thao tác tổng quát: `add`, `exists`, `findByKey`, `removeByKey`, `findWhere<Predicate>` (member function template), `loadFromFile`, `saveAllToFile`, `appendToFile`.
  - **Function Templates bổ trợ:**
    - `template <typename T> bool parseNumberInRange(const std::string& raw, T minVal, T maxVal, T& outVal)` trong `Validation.h` (dùng chung cho `int`, `long long`).
    - `template <typename T> bool readValidatedField(...)` trong `ConsoleUI.h` để chuẩn hóa vòng lặp nhập liệu & hủy thao tác an toàn.
- **Thư viện Console Windows:** Đóng gói duy nhất trong `ConsoleUI.cpp` (`<windows.h>` để tô màu, `<conio.h>` dùng `_getch()` che mật khẩu thành `*` và bắt phím `ESC`).

---

## 3. C++ Coding Standard áp dụng (Bắt buộc — 1 điểm)

Toàn bộ mã nguồn trong `include/`, `src/`, `tests/` phải tuân thủ nghiêm ngặt bộ quy chuẩn sau (tham chiếu C++ Core Guidelines & Google C++ Style Guide):

1. **Header Guards:** Mọi file `.h` bắt buộc có `#ifndef LIBRARY_<TEN_FILE>_H` / `#define ...` / `#endif`.
2. **Không ô nhiễm namespace:** Tuyệt đối **KHÔNG** viết `using namespace std;` ở phạm vi toàn cục trong bất kỳ file `.h` hay `.cpp` nào; luôn ghi tường minh `std::`.
3. **Quy ước đặt tên thống nhất 100%:**
   - File & Class / Struct / Enum: `PascalCase` (`LibraryService`, `Repository.h`, `ConsoleColor`).
   - Hàm, phương thức, biến cục bộ, tham số: `camelCase` (`borrowBook`, `maSach`, `soPhieuMuon`).
   - Thuộc tính `private` của class: tiền tố `m_` (`m_maSach`, `m_tinhTrang`, `m_books`).
   - Hằng số `constexpr` & giá trị `enum`: `UPPER_SNAKE_CASE` (`MAX_LOGIN_ATTEMPTS`, `FIELD_DELIMITER`, `LOAN_DURATION_DAYS`).
   - Tham số template: `PascalCase` (`typename T`, `typename KeyType`, `typename Predicate`).
4. **`const`-correctness & truyền tham chiếu:**
   - Mọi getter và phương thức chỉ đọc phải có hậu tố `const`.
   - Truyền đối tượng chuỗi/lớp bằng `const T&` để tránh sao chép thừa.
5. **Không dùng Magic Numbers:** Mọi con số nghiệp vụ (`0`, `1`, `3`, `7`, `27` cho phím ESC, `8` cho Backspace, `1000` năm tối thiểu) đều được đặt thành hằng số `constexpr` có tên rõ nghĩa.
6. **Tổ chức & Định dạng mã nguồn:**
   - Thụt lề chuẩn **4 dấu cách** (không trộn tab/space), mở ngoặc `{` nhất quán.
   - Mỗi hàm thực hiện một nhiệm vụ rõ ràng, độ dài gọn gàng, có comment tài liệu chuẩn `///` (Doxygen) cho mọi class và phương thức public.
   - Bắt buộc biên dịch sạch **0 errors, 0 warnings** với cờ `-Wall -Wextra -Wpedantic`.

---

## 4. Lệnh Biên dịch & Kiểm thử (Commands)

```powershell
# Tạo thư mục build (nếu chưa có)
New-Item -ItemType Directory -Force -Path build | Out-Null

# Biên dịch chương trình chính
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/Date.cpp src/Validation.cpp src/Models.cpp src/LibraryService.cpp src/ConsoleUI.cpp src/AppController.cpp src/main.cpp -o build/library_app.exe

# Chạy chương trình chính
.\build\library_app.exe

# Biên dịch & chạy bộ kiểm thử tự động (Unit + Integration tests)
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/Date.cpp src/Validation.cpp src/Models.cpp src/LibraryService.cpp tests/test_runner.cpp -o build/test_runner.exe
.\build\test_runner.exe
```

---

## 5. Cấu trúc thư mục dự án

```text
Project_Khoa/
├── SPEC.md                     # Tài liệu đặc tả kỹ thuật (Source of Truth)
├── README.md                   # Hướng dẫn biên dịch & sử dụng (g++, Visual Studio, Dev-C++)
├── tasks/
│   ├── plan.md                 # Kế hoạch triển khai & quyết định kiến trúc
│   └── todo.md                 # Danh sách task nguyên tử & checkpoints
├── docs/                       # Tài liệu nộp bài: Báo cáo Word (.docx) & Kịch bản quay video
├── data/
│   ├── Admin.txt               # Tài khoản nhân viên quản lý thư viện
│   ├── Sach.txt                # Danh sách sách trong thư viện
│   ├── BanDoc.txt              # Danh sách bạn đọc đã đăng ký
│   └── PhieuMuon.txt           # Danh sách phiếu mượn / trả sách
├── include/
│   ├── Date.h                  # Lớp Date (OOP: kiểm tra hợp lệ, +7 ngày lịch, so sánh)
│   ├── Validation.h            # Tiện ích chuẩn hóa & Function Template parseNumberInRange<T>
│   ├── Entity.h                # Lớp cơ sở trừu tượng Entity (đa hình)
│   ├── Models.h                # Các lớp OOP: Admin, Book, Reader, Loan, ServiceResult
│   ├── Repository.h            # Class Template Repository<T, KeyType> (STL list + map + file I/O)
│   ├── LibraryService.h        # Lớp nghiệp vụ lõi (Auth, Sách, Bạn đọc, Phiếu mượn)
│   ├── ConsoleUI.h             # Lớp giao diện Console Windows (màu, khung, bảng, pass '*', ESC)
│   └── AppController.h         # Lớp điều phối luồng Đăng nhập, Menu chính & Menu con
├── src/
│   ├── Date.cpp
│   ├── Validation.cpp
│   ├── Models.cpp
│   ├── LibraryService.cpp
│   ├── ConsoleUI.cpp
│   ├── AppController.cpp
│   └── main.cpp                # Điểm vào chương trình
├── tests/
│   └── test_runner.cpp         # Bộ kiểm thử tự động (không phụ thuộc tương tác console)
└── build/                      # Chứa file thực thi .exe sau khi biên dịch
```

---

## 6. Hợp đồng dữ liệu & Ràng buộc dữ liệu (5 điểm)

Toàn bộ dữ liệu lưu dưới dạng file `.txt` trong thư mục `data/`, mỗi bản ghi trên 1 dòng, các trường phân cách bằng ký tự `|` (`FIELD_DELIMITER`), tự động bỏ qua dòng trống và xử lý an toàn cả `CRLF` (`\r\n`) lẫn `LF` (`\n`). Dữ liệu người dùng nhập vào **không được chứa ký tự `|`**.

### 6.1. Chi tiết 4 file dữ liệu & Ràng buộc miền giá trị

| File | Định dạng dòng | Ràng buộc dữ liệu bắt buộc | Quy tắc ghi file |
|---|---|---|---|
| `data/Admin.txt` | `user\|password` | `user`, `password` không rỗng, không chứa `\|`. `user` duy nhất. | Chỉ đọc khi khởi động. |
| `data/Sach.txt` | `MaSach\|TenSach\|TacGia\|NhaXuatBan\|GiaBan\|NamPhatHanh\|SoTrang\|NgayNhapKho\|TinhTrang` | - `MaSach`: không rỗng, không chứa `\|`, **duy nhất**.<br>- `TenSach`, `TacGia`, `NhaXuatBan`: không rỗng, không chứa `\|`.<br>- `GiaBan`: số nguyên dương (`> 0`).<br>- `NamPhatHanh`: số nguyên thuộc `[1000, năm hiện tại]`.<br>- `SoTrang`: số nguyên dương (`> 0`).<br>- `NgayNhapKho`: `dd/mm/yyyy` hợp lệ lịch thực và `<= ngày hiện tại` (Enter để mặc định hôm nay).<br>- `TinhTrang`: `0` (mới thêm / đã trả), `= SoPhieuMuon` khi đang mượn. | - **Thêm sách:** ghi nối (`append`) vào cuối `Sach.txt`.<br>- **Xóa / Mượn / Trả:** ghi đè (`overwrite`) toàn bộ `Sach.txt`. |
| `data/BanDoc.txt` | `MaBanDoc\|HoTen\|NgayDangKy` | - `MaBanDoc`: không rỗng, không chứa `\|`, **duy nhất**.<br>- `HoTen`: không rỗng, không chứa `\|`.<br>- `NgayDangKy`: `dd/mm/yyyy` hợp lệ và `<= ngày hiện tại` (mặc định hôm nay). | - **Thêm bạn đọc:** ghi nối (`append`) vào cuối `BanDoc.txt`. |
| `data/PhieuMuon.txt` | `SoPhieuMuon\|MaBanDoc\|MaSach\|NgayMuon\|NgayPhaiTra\|TinhTrang` | - `SoPhieuMuon`: nguyên dương bắt đầu từ `1`, tự động tăng (`max + 1`).<br>- `MaBanDoc`: phải tồn tại trong `BanDoc.txt`.<br>- `MaSach`: phải tồn tại trong `Sach.txt` và có `TinhTrang == 0` lúc tạo phiếu.<br>- `NgayMuon`: tự động lấy ngày hệ thống.<br>- `NgayPhaiTra`: tự động `= NgayMuon + 7` ngày lịch thực.<br>- `TinhTrang`: `1` (đang mượn), `0` (đã trả). | - **Mượn / Trả sách:** cập nhật đồng bộ cả `Sach.txt` và `PhieuMuon.txt` (rollback RAM nếu ghi file lỗi). |

### 6.2. Ma trận yêu cầu chức năng & Ràng buộc nghiệp vụ

1. **Đăng nhập hệ thống (`Admin.txt`):**
   - Giao diện khung dấu `*` theo mẫu (`DANG NHAP HE THONG`, `User:`, `Password:`).
   - Mã hóa mật khẩu thành dấu `*`, hỗ trợ `Backspace`, bỏ qua phím mũi tên/phím chức năng (mã quét `0`/`224`).
   - Nhấn phím **`ESC`** tại màn hình đăng nhập (khi đang gõ `User` hoặc `Password`) → thoát chương trình ngay lập tức.
   - Nhập đúng `(user, password)` trong `Admin.txt` mới được vào menu chính; nhập sai báo lỗi và cho nhập lại; **sai 3 lần → tự động thoát**.
2. **Menu chính & Điều hướng:**
   - Gồm: `1. Quan li sach`, `2. Quan li phieu muon`, `3. Quan li ban doc`, `0. Thoat chuong trinh`.
   - Sau khi hoàn thành mỗi thao tác: cho phép người dùng chọn thực hiện chức năng khác hoặc kết thúc chương trình.
   - Khi xảy ra lỗi: thông báo rõ lỗi, **hủy chức năng đang thực hiện** (bảo toàn dữ liệu), và cho phép chọn chức năng khác hoặc thoát.
3. **Quản lý sách:**
   - **Hiển thị thông tin sách:** In bảng đầy đủ 9 cột kèm màu phân biệt tình trạng (`0 - San sang` / `<SoPhieu> - Dang muon`).
   - **Thêm sách:** Kiểm tra trùng `MaSach` (trùng → báo *"Sach da ton tai"* và hủy), validate đủ các trường, đặt `TinhTrang = 0`, ghi nối cuối `Sach.txt`.
   - **Xóa sách:** Kiểm tra tồn tại và `TinhTrang == 0`. Nếu `TinhTrang != 0` → báo *"Khong the xoa sach do sach dang duoc muon"*. Xóa thành công → ghi đè toàn bộ `Sach.txt`.
   - **Tìm kiếm sách:** Tìm theo `MaSach` (chính xác `O(log n)` qua `std::map`), theo `TenSach` và `TacGia` (tìm chuỗi con không phân biệt hoa/thường qua `Repository::findWhere`).
4. **Quản lý phiếu mượn:**
   - **Hiển thị các phiếu mượn:** In bảng đủ 6 cột kèm màu phân biệt trạng thái (`1 - Dang muon` / `0 - Da tra`).
   - **Mượn sách:** Nhập `MaSach`, `MaBanDoc`; kiểm tra sách tồn tại & `TinhTrang == 0`, bạn đọc tồn tại; tạo phiếu mới (`SoPhieuMuon` tăng dần từ `1`, `NgayMuon = hôm nay`, `NgayPhaiTra = +7 ngày`, `TinhTrang = 1`), cập nhật `Sach.TinhTrang = SoPhieuMuon`, lưu đồng bộ `Sach.txt` và `PhieuMuon.txt`.
   - **Trả sách:** Nhập `SoPhieuMuon`; kiểm tra phiếu tồn tại & `TinhTrang != 0`; cập nhật `PhieuMuon.TinhTrang = 0` và `Sach.TinhTrang = 0`, lưu đồng bộ `Sach.txt` và `PhieuMuon.txt`.
5. **Quản lý bạn đọc (mở rộng phục vụ kiểm thử & đối chiếu `BanDoc.txt`):**
   - **Hiển thị bạn đọc:** In bảng `MaBanDoc`, `HoTen`, `NgayDangKy`.
   - **Thêm bạn đọc:** Kiểm tra trùng `MaBanDoc`, validate dữ liệu, ghi nối vào `BanDoc.txt`.

---

## 7. Ví dụ mẫu Phong cách mã nguồn (OOP + Template + Coding Standard)

```cpp
#ifndef LIBRARY_REPOSITORY_H
#define LIBRARY_REPOSITORY_H

#include <list>
#include <map>
#include <string>
#include <vector>

/// Kho luu tru tong quat ket hop Danh sach lien ket kep (std::list)
/// va Cay tim kiem can bang (std::map) de tra cuu khoa chinh O(log n).
template <typename T, typename KeyType>
class Repository {
public:
    bool exists(const KeyType& key) const {
        return m_index.find(key) != m_index.end();
    }

    bool add(const T& item) {
        const KeyType key = item.getKey();
        if (exists(key)) {
            return false;
        }
        m_items.push_back(item);
        auto it = m_items.end();
        --it;
        m_index[key] = it;
        return true;
    }

    template <typename Predicate>
    std::vector<const T*> findWhere(Predicate predicate) const {
        std::vector<const T*> results;
        for (const T& item : m_items) {
            if (predicate(item)) {
                results.push_back(&item);
            }
        }
        return results;
    }

private:
    std::list<T> m_items;
    std::map<KeyType, typename std::list<T>::iterator> m_index;
};

#endif  // LIBRARY_REPOSITORY_H
```

---

## 8. Chiến lược kiểm thử (Testing Strategy)

- **Kiểm thử tự động (`tests/test_runner.cpp`):** Biên dịch và chạy độc lập với Windows Console UI:
  1. **Lớp `Date`:** Kiểm tra ngày hợp lệ/không hợp lệ (29/02 năm nhuận & không nhuận, 31/04), cộng `+7` ngày qua cuối tháng và chuyển năm (`28/12/2026 -> 04/01/2027`), so sánh ngày.
  2. **`Validation` & Function Template `parseNumberInRange<T>`:** Trim khoảng trắng, chặn `|`, kiểm tra biên `int`/`long long`, từ chối chữ cái/số âm/tràn số.
  3. **Class Template `Repository<T, KeyType>`:** Thêm/trùng khóa/tìm `O(log n)`/xóa `O(1)`/`findWhere`, đọc file có CRLF & dòng trống, `appendToFile` khi file không có `\n` cuối, `saveAllToFile` và tải lại khớp 100%.
  4. **`LibraryService` (E2E nghiệp vụ):**
     - Đăng nhập đúng/sai/không tồn tại user.
     - Thêm sách (kiểm tra ghi nối `Sach.txt`), chặn trùng `MaSach`, chặn giá/số trang âm, chặn năm phát hành & ngày nhập kho tương lai.
     - Tìm kiếm sách theo `MaSach`, `TenSach`, `TacGia`.
     - Thêm bạn đọc, chặn trùng `MaBanDoc`.
     - Mượn sách (số phiếu tăng `1 -> 2`, `NgayPhaiTra = NgayMuon + 7`, `Sach.TinhTrang = SoPhieuMuon`, tải lại 2 file khớp).
     - Chặn mượn khi sách đang mượn / sai mã bạn đọc / sai mã sách; chặn xóa sách khi `TinhTrang != 0`.
     - Trả sách (cập nhật phiếu & sách về `0`, tải lại 2 file khớp), chặn trả lại phiếu đã trả (`TinhTrang == 0`), sau đó xóa được sách và xác minh `Sach.txt` đã ghi đè.
- **Kiểm thử thủ công trên Console Windows:** Xác minh khung đăng nhập `*`, che mật khẩu `*`, `Backspace`, phím mũi tên không sinh rác, phím `ESC` thoát ngay, sai 3 lần tự thoát, bố cục màu sắc và bảng căn cột rõ ràng.

---

## 9. Ranh giới thực hiện (Boundaries)

- **Luôn luôn (Always):**
  - Tuân thủ 100% C++ Coding Standard đã định nghĩa ở Mục 3; biên dịch 0 warnings với `-Wall -Wextra -Wpedantic`.
  - Chạy pass 100% `test_runner.exe` sau mỗi task.
  - Validate mọi đầu vào và giữ đồng bộ hoàn toàn giữa RAM và file `.txt` (có rollback khi lỗi ghi file).
- **Hỏi trước khi làm (Ask first):**
  - Thay đổi định dạng cột/ký tự phân cách của các file `.txt`.
  - Bổ sung chức năng ngoài Spec (ví dụ: sửa thông tin sách, xóa bạn đọc, tính tiền phạt quá hạn).
- **Không bao giờ (Never):**
  - Không sửa hoặc xóa 3 file ảnh chụp đề bài và file `Yêu cầu thực hiện Project.pdf`.
  - Không dùng `using namespace std;` toàn cục hoặc phá vỡ thiết kế OOP/Template.
  - Không cài đặt thư viện bên thứ ba ngoài STL và Windows API cơ bản (`<windows.h>`, `<conio.h>`).

---

## 10. Tiêu chí nghiệm thu (Success Criteria)

1. Toàn bộ hệ thống cài đặt chuẩn **OOP** (`Entity`, `Date`, `Admin`, `Book`, `Reader`, `Loan`, `LibraryService`, `ConsoleUI`, `AppController`) và **Template** (`Repository<T, KeyType>`, `parseNumberInRange<T>`, `findWhere<Predicate>`).
2. Tuân thủ đầy đủ **C++ Coding Standard**, biên dịch sạch **0 errors, 0 warnings** với `g++ -std=c++17 -Wall -Wextra -Wpedantic`.
3. Đạt 100% chức năng & ràng buộc dữ liệu của `PRO_CTDL_3` (bao gồm cả Tìm kiếm sách và Quản lý bạn đọc); bộ test `test_runner.exe` pass 100%.
4. Có đầy đủ `README.md`, báo cáo Word (`.docx`) và kịch bản quay video demo phục vụ nộp bài nhóm (2–5 thành viên).

---

## 11. Câu hỏi mở (Open Questions)

- Khi bước vào **Phase 4 (Báo cáo Word & Kịch bản Video)**, bạn có thể cung cấp tên lớp, giảng viên hướng dẫn và danh sách thành viên nhóm (2–5 sinh viên + phần việc phân công) để điền thẳng vào trang bìa và bảng phân công của báo cáo Word.
