# PRO_CTDL_3 — Hệ Thống Quản Lý Thư Viện (Console C++17)

Chương trình Console quản lý hoạt động hằng ngày của thư viện: đăng nhập nhân
viên, quản lý sách, quản lý bạn đọc, mượn/trả sách, tìm kiếm sách và lưu dữ
liệu đồng bộ xuống file `.txt`. Đề bài: **PROJECT 3 (`PRO_CTDL_3`)**, thực hiện
theo đặc tả kỹ thuật trong [`SPEC.md`](SPEC.md).

## Yêu cầu hệ thống

| Công cụ | Yêu cầu tối thiểu |
|---|---|
| Trình biên dịch `g++` | Hỗ trợ C++17 (khuyên dùng MSYS2 UCRT64 trên Windows; Linux/macOS dùng `g++` hệ thống) |
| Visual Studio | 2019 trở lên, workload **Desktop development with C++**, chuẩn ngôn ngữ đặt thành **C++17** |
| Dev-C++ | Phiên bản dùng trình biên dịch TDM-GCC/MinGW-w64 hỗ trợ C++17 (khuyên dùng bản 6.3 trở lên) |

Chương trình chính dùng `<windows.h>` và `<conio.h>` (tô màu Console, che mật
khẩu bằng `*`, bắt phím `ESC`) nên **chỉ biên dịch và chạy trên Windows**
(MSYS2 UCRT64, Visual Studio hoặc Dev-C++). Bộ kiểm thử tự động
`tests/test_runner.cpp` không phụ thuộc Windows API nên biên dịch và chạy được
cả trên Linux/macOS.

## Cách biên dịch và chạy với `g++` (MSYS2 UCRT64 / PowerShell)

Chạy toàn bộ lệnh dưới đây **từ thư mục gốc của dự án** (thư mục chứa
`SPEC.md`). Lệnh copy-paste là chạy được ngay.

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

Trên MSYS2/Git Bash, thay `.\build\library_app.exe` bằng
`./build/library_app.exe` (lệnh `g++` giữ nguyên). Bộ kiểm thử phải in ra
dòng cuối `Tong: 80 test, Pass: 80, Fail: 0` và thoát với mã `0`.

## Cách mở trên Visual Studio

1. Mở Visual Studio, chọn **Create a new project → Empty Project**, đặt tên
   (ví dụ `QuanLyThuVien`), chọn vị trí là thư mục gốc của dự án.
2. Chuột phải vào project trong **Solution Explorer → Add → Existing Item**,
   thêm toàn bộ file trong `include/` (`.h`) và `src/` (`.cpp`).
3. Chuột phải vào project → **Properties → C/C++ → Language → C++ Language
   Standard**, chọn **ISO C++17 Standard (/std:c++17)**.
4. Trong cùng cửa sổ Properties, mục **C/C++ → General → Additional Include
   Directories**, thêm đường dẫn tới thư mục `include` của dự án.
5. Nhấn **Ctrl+Shift+B** để build (phải đạt **0 errors, 0 warnings**), rồi
   **Ctrl+F5** để chạy không cần debug.
6. Chương trình tự tìm thư mục `data/` (kể cả khi chạy từ `Debug/` hay
   `Release/`), nhưng nên chạy lần đầu từ thư mục gốc để kiểm tra dữ liệu mẫu.

## Cách mở trên Dev-C++

1. Mở Dev-C++, chọn **File → New → Project → Console Application**, chọn
   ngôn ngữ **C++**, đặt tên project và lưu file `.dev` vào thư mục gốc dự
   án.
2. Chọn **Project → Add to Project**, thêm toàn bộ file trong `include/`
   (`.h`) và `src/` (`.cpp`).
3. Chọn **Project → Project Options → Parameters → C++ compiler**, thêm cờ
   `-std=c++17` vào ô tham số.
4. Chọn **Project → Project Options → Directories → Include Directories**,
   thêm đường dẫn tới thư mục `include` của dự án.
5. Nhấn **F11** (Compile & Run). Lần build đầu tiên Dev-C++ có thể hỏi thư
   mục lưu file `.exe` — chọn thư mục `build/` của dự án.

## Tài khoản đăng nhập mẫu

File `data/Admin.txt` có sẵn 2 tài khoản (mật khẩu đều là `123456`):

| User | Password | Ghi chú |
|---|---|---|
| `admin` | `123456` | Tài khoản quản trị chính |
| `user1` | `123456` | Tài khoản nhân viên |

Nhập sai 3 lần chương trình sẽ tự động thoát. Nhấn phím `ESC` khi đang gõ
`User` hoặc `Password` cũng thoát ngay lập tức.

## Cấu trúc thư mục

```text
Project_Khoa/
├── SPEC.md                     # Đặc tả kỹ thuật (Source of Truth)
├── README.md                   # File này
├── tasks/                      # Kế hoạch, todo, bảng phân công nhóm
├── docs/                       # Kịch bản video, QA checklist, báo cáo Word
├── data/                       # 4 file dữ liệu .txt (xem mục dưới)
├── include/                    # File header (.h)
├── src/                        # File cài đặt (.cpp)
├── tests/
│   └── test_runner.cpp         # Bộ kiểm thử tự động (80 test, không cần console)
└── build/                      # File .exe sau khi biên dịch (không commit)
```

## Thiết kế OOP và Template (tóm tắt)

- **`Entity`** (`include/Entity.h`): lớp cơ sở trừu tượng cho mọi thực thể,
  có hủy ảo và 2 phương thức thuần ảo `serialize()` / `getEntityType()`.
- **`Admin`, `Book`, `Reader`, `Loan`** (`include/Models.h`): kế thừa
  `Entity`, đóng gói dữ liệu `private` (tiền tố `m_`), getter/setter có kiểm
  tra hợp lệ, `getKey()`, `serialize()` và `static fromFileLine()` để đọc 1
  dòng file.
- **`Date`** (`include/Date.h`): lớp giá trị đóng gói ngày/tháng/năm, tự kiểm
  tra hợp lệ lịch thực (kể cả năm nhuận 29/02), `Date::today()`,
  `addDays(int)` (dùng `std::mktime` nên tự xử lý qua cuối tháng/cuối năm),
  `toString()` định dạng `dd/mm/yyyy`, `fromString()` và toán tử so sánh
  `==`, `<`, `<=`.
- **`Repository<T, KeyType>`** (`include/Repository.h`): **Class Template**
  kho lưu trữ tổng quát, kết hợp `std::list<T>` (giữ đúng thứ tự bản ghi,
  xóa `O(1)`) và `std::map<KeyType, iterator>` (cây Red-Black, tra cứu khóa
  chính `O(log n)`). Có member template `findWhere<Predicate>` để lọc theo
  điều kiện bất kỳ, cùng `loadFromFile`, `saveAllToFile`, `appendToFile`
  (tự chèn `\n` khi file thiếu, chịu cả CRLF lẫn LF, bỏ qua dòng trống).
- **`LibraryService`** (`include/LibraryService.h`): lớp nghiệp vụ lõi, tách
  hoàn toàn khỏi giao diện Console nên kiểm thử được độc lập: đăng nhập,
  thêm/xóa/tìm sách, thêm bạn đọc, mượn/trả sách (đồng bộ cả `Sach.txt` và
  `PhieuMuon.txt`, rollback RAM khi ghi file lỗi).
- **Function Template** `parseNumberInRange<T>` (`Validation.h`, dùng chung
  cho `int` và `long long`) và `findWhere<Predicate>` là 2 template còn lại
  của dự án, bên cạnh Class Template `Repository<T, KeyType>`.
- **`ConsoleUI` / `AppController`**: tầng giao diện Console Windows (màu
  sắc, khung, bảng căn cột, che mật khẩu `*`, bắt `ESC`) và điều phối luồng
  menu.

Quy chuẩn code (SPEC mục 3): header guard cho mọi file `.h`, tuyệt đối không
`using namespace std;` ở phạm vi toàn cục, đặt tên thống nhất
(`PascalCase` cho class/file, `camelCase` cho hàm/biến, tiền tố `m_` cho
thuộc tính `private`, `UPPER_SNAKE_CASE` cho hằng `constexpr`), không magic
number, thụt lề 4 dấu cách, biên dịch sạch 0 warnings với
`-Wall -Wextra -Wpedantic`.

## Định dạng 4 file dữ liệu

Mọi file nằm trong `data/`, mỗi bản ghi 1 dòng, các trường phân cách bằng ký
tự `|` (không được xuất hiện trong dữ liệu người dùng nhập). Chương trình tự
bỏ qua dòng trống và xử lý cả `CRLF` (`\r\n`) lẫn `LF` (`\n`).

| File | Định dạng 1 dòng | Ví dụ |
|---|---|---|
| `Admin.txt` | `user\|password` | `admin\|123456` |
| `Sach.txt` | `MaSach\|TenSach\|TacGia\|NhaXuatBan\|GiaBan\|NamPhatHanh\|SoTrang\|NgayNhapKho\|TinhTrang` | `S001\|Lap trinh C++ co ban\|Nguyen Van A\|NXB Giao Duc\|120000\|2020\|350\|10/01/2024\|0` |
| `BanDoc.txt` | `MaBanDoc\|HoTen\|NgayDangKy` | `BD001\|Nguyen Van An\|10/01/2024` |
| `PhieuMuon.txt` | `SoPhieuMuon\|MaBanDoc\|MaSach\|NgayMuon\|NgayPhaiTra\|TinhTrang` | `1\|BD001\|S002\|01/11/2026\|08/11/2026\|1` |

Ràng buộc chính (đầy đủ trong SPEC mục 6.1):

- `MaSach` / `MaBanDoc` duy nhất, không rỗng, không chứa `|`.
- `GiaBan`, `SoTrang` là số nguyên dương; `NamPhatHanh` thuộc
  `[1000, năm hiện tại]`; `NgayNhapKho` / `NgayDangKy` hợp lệ và
  `<= ngày hiện tại`.
- `TinhTrang` của sách: `0` là sẵn sàng, `= SoPhieuMuon` khi đang được mượn.
- Phiếu mượn: `SoPhieuMuon` tự tăng từ `1`; `NgayMuon` là hôm nay;
  `NgayPhaiTra = NgayMuon + 7` ngày; `TinhTrang` `1` là đang mượn,
  `0` là đã trả.

Quy tắc ghi file: thêm sách/bạn đọc thì **ghi nối** (append) vào cuối file;
xóa, mượn, trả thì **ghi đè** toàn bộ file tương ứng.

## Lưu ý khi chạy

- **Luôn chạy từ thư mục gốc của dự án** (`.\build\library_app.exe`). Chương
  trình tự tìm thư mục `data/` cả khi gọi từ trong `build/`, nhưng chạy từ
  thư mục gốc là cách chắc chắn nhất để dữ liệu mẫu được nạp đúng.
- **Đừng sửa tay định dạng cột hay ký tự phân cách** của 4 file `data/`
  (xem SPEC mục 9): đổi định dạng sẽ làm `fromFileLine()` bỏ qua dòng đó và
  dữ liệu "biến mất" khỏi chương trình.
- Muốn quay lại dữ liệu mẫu ban đầu: khôi phục 4 file trong `data/` từ Git
  (`git checkout -- data/`) rồi chạy lại chương trình.
- Bộ kiểm thử tự động ghi dữ liệu tạm vào `build/test_data/` và xóa thư mục
  này mỗi lần chạy, **không động vào `data/` thật** nên chạy test bao nhiêu
  lần cũng an toàn.
