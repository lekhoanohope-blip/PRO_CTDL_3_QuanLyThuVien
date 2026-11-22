# Kịch Bản Quay Video Báo Cáo — PRO_CTDL_3 Hệ Thống Quản Lý Thư Viện

- **Thời lượng:** 6–8 phút (mục tiêu 7 phút 30 giây).
- **Hình thức:** Họp Google Meet, cả 4 thành viên cùng tham gia; luân phiên
  share màn hình (mỗi người share phần mình demo, máy còn lại tắt mic khi
  không nói).
- **Ngôn ngữ:** Tiếng Việt, nói tự nhiên, không đọc thuộc lòng từng chữ —
  lời thoại dưới đây là khung ý chính và câu mở đầu gợi ý cho từng người.

## Phân đoạn 1 — Mở đầu (0:00–1:00) — Vinh

**Màn hình:** Slide tiêu đề (tên đề tài + tên 4 thành viên) hoặc cửa sổ
VS Code mở thư mục dự án.

**Lời thoại (Vinh):**

> "Chào thầy cô, chúng em là nhóm 4 sinh viên thực hiện đồ án PROJECT 3 —
> Hệ thống Quản lý Thư viện viết bằng C++ theo hướng đối tượng, dùng Template
> và STL. Nhóm em gồm Khoa, Nhật, Hiếu và em là Vinh. Trong 7 phút tới, nhóm
> em sẽ trình bày kiến trúc OOP và Template của hệ thống, sau đó demo trực
> tiếp 4 nhóm chức năng: đăng nhập, quản lý sách, quản lý bạn đọc và mượn trả
> sách, kèm theo bộ kiểm thử tự động gồm 80 test case. Dữ liệu của hệ thống
> được lưu trong 4 file text trong thư mục data: Admin, Sách, Bạn đọc và
> Phiếu mượn."

**Thao tác trên màn hình:** Mở thư mục `data/` cho thấy 4 file `Admin.txt`,
`Sach.txt`, `BanDoc.txt`, `PhieuMuon.txt`; mở nhanh 1 file để thấy định dạng
các trường phân cách bằng `|`.

## Phân đoạn 2 — Kiến trúc OOP & Template (1:00–2:45) — Khoa

**Màn hình:** VS Code mở `include/Entity.h`, `include/Repository.h`,
`include/Models.h` và 1 cửa sổ terminal.

**Lời thoại (Khoa):**

> "Về kiến trúc, toàn bộ hệ thống được thiết kế theo hướng đối tượng. Lớp cơ
> sở trừu tượng Entity khai báo 2 phương thức thuần ảo là serialize và
> getEntityType; 4 lớp thực thể Admin, Book, Reader và Loan kế thừa Entity,
> đóng gói toàn bộ dữ liệu ở private và kiểm tra hợp lệ ngay trong setter.
> Lớp Date đóng gói ngày tháng, tự kiểm tra năm nhuận và cộng ngày bằng
> mktime nên tự xử lý đúng qua cuối tháng và cuối năm. Điểm nhấn về Template
> là Class Template Repository, khóa chính và kiểu thực thể đều là tham số
> template: bên trong nó kết hợp std::list để giữ đúng thứ tự bản ghi và xóa
> O(1), với std::map là cây Red-Black để tra cứu mã sách theo O(log n).
> Ngoài ra nhóm dùng 2 Function Template là parseNumberInRange và findWhere.
> Về Coding Standard, nhóm tuân thủ header guard, không dùng using namespace
> std ở phạm vi toàn cục, không magic number, và sau đây em build trực tiếp
> cho thầy cô xem."

**Thao tác trên màn hình:**

1. Cuộn nhanh `Entity.h` (chỉ ra `virtual serialize() = 0`) và `Models.h`
   (chỉ ra `Book : public Entity`, các getter/setter).
2. Mở `Repository.h`, chỉ vào 2 thành viên `std::list<T> m_items` và
   `std::map<KeyType, ...> m_index`, rồi chỉ vào `findWhere<Predicate>`.
3. Gõ lệnh build trong terminal (lệnh trong `README.md`), dừng màn hình ở
   kết quả **0 errors, 0 warnings**.

## Phân đoạn 3 — Demo đăng nhập & quản lý sách (2:45–4:30) — Nhật

**Màn hình:** Cửa sổ Console chạy `build\library_app.exe` (cỡ chữ 18–20pt,
nền tối).

**Lời thoại (Nhật):**

> "Em demo phần đăng nhập và quản lý sách. Màn hình đăng nhập hiển thị khung
> dấu sao theo đúng mẫu đề bài. Em nhập sai mật khẩu trước: mật khẩu chỉ
> hiện dấu sao, bấm phím mũi tên không sinh ký tự rác. Nhấn ESC thì chương
> trình thoát ngay. Giờ em đăng nhập lại bằng tài khoản mẫu admin, mật khẩu
> 123456. Vào menu chính, em chọn quản lý sách: bảng hiển thị đủ 9 cột, sách
> sẵn sàng hiện màu xanh, sách đang mượn hiện màu khác kèm số phiếu. Em thêm
> một cuốn sách mới, rồi mở file Sach.txt cho thấy dòng mới đã được ghi nối
> vào cuối file. Cuối cùng em tìm kiếm theo tên và theo tác giả, tìm không
> phân biệt hoa thường."

**Thao tác trên màn hình:**

1. Chạy `.\build\library_app.exe`; nhập sai mật khẩu 1 lần (chỉ thấy `*`);
   bấm vài phím mũi tên trong ô Password (không có ký tự rác); nhấn `ESC`
   (chương trình thoát).
2. Chạy lại, đăng nhập `admin` / `123456` thành công.
3. Menu chính → `1. Quan li sach` → hiển thị bảng 9 cột.
4. Thêm sách mới (ví dụ mã `S009`, giá `99000`, năm `2023`, `100` trang) →
   mở `data\Sach.txt` bằng Notepad/VS Code, dòng `S009` đã nằm cuối file.
5. Tìm kiếm: nhập từ khóa `lap trinh` (chữ thường) → vẫn tìm thấy sách
   "Lap trinh C++ co ban".

## Phân đoạn 4 — Demo bạn đọc, ràng buộc & kiểm thử (4:30–6:00) — Hiếu

**Màn hình:** Cửa sổ Console (tiếp tục phiên demo) rồi chuyển sang terminal
chạy `test_runner.exe`.

**Lời thoại (Hiếu):**

> "Tiếp theo là quản lý bạn đọc và phần kiểm chứng ràng buộc dữ liệu. Em mở
> danh sách bạn đọc rồi thêm một bạn đọc mới, bạn này dùng được ngay để mượn
> sách. Sau đó em cố tình nhập sai để cho thấy hệ thống chặn đúng luật: thêm
> sách trùng mã thì báo sách đã tồn tại; nhập giá âm thì bị từ chối; xóa cuốn
> sách đang được mượn thì hệ thống báo không thể xóa do sách đang được mượn.
> Cuối cùng em chạy bộ kiểm thử tự động: 80 test bao phủ lớp Date, Validation,
> Models, Repository và toàn bộ luồng nghiệp vụ mượn trả của LibraryService.
> Kết quả pass 100 phần trăm, không cần thao tác tay."

**Thao tác trên màn hình:**

1. Menu chính → `3. Quan li ban doc` → hiển thị bảng → thêm bạn đọc mới
   (ví dụ mã `BD006`).
2. Thử thêm sách trùng mã `S001` → thông báo "Sach da ton tai".
3. Thử thêm sách giá `-5` (hoặc nhập chữ vào ô giá bán) → bị từ chối, quay
   lại menu mà không mất dữ liệu.
4. Thử xóa sách đang mượn (ví dụ `S002`, tình trạng khác `0`) → thông báo
   "Khong the xoa sach do sach dang duoc muon".
5. Chuyển sang terminal, chạy `.\build\test_runner.exe`, dừng màn hình ở
   dòng cuối `Tong: 80 test, Pass: 80, Fail: 0`.

## Phân đoạn 5 — Demo mượn/trả & kết luận (6:00–7:30) — Khoa & Vinh

**Màn hình:** Cửa sổ Console (Khoa), sau đó quay lại slide kết thúc (Vinh).

**Lời thoại (Khoa):**

> "Phần cuối về nghiệp vụ, em demo mượn và trả sách. Em mượn cuốn S001 cho
> bạn đọc BD001: hệ thống tự sinh số phiếu là số tiếp theo, ngày mượn là hôm
> nay và ngày phải trả tự tính bằng ngày mượn cộng 7 ngày. Mở file Sach.txt
> và PhieuMuon.txt cho thấy 2 file đã đồng bộ: sách chuyển sang tình trạng
> bằng số phiếu, phiếu mới có tình trạng đang mượn. Giờ em trả sách: cả phiếu
> và sách đều về tình trạng 0. Và vì sách đã về trạng thái sẵn sàng nên giờ
> em xóa được nó, file Sach.txt đã được ghi đè lại."

**Thao tác trên màn hình (Khoa):**

1. Menu chính → `2. Quan li phieu muon` → mượn sách `S001` cho `BD001` →
   ghi nhớ số phiếu hiện trên màn hình.
2. Mở `data\Sach.txt` và `data\PhieuMuon.txt`: chỉ ra `S001` có tình trạng
   bằng số phiếu vừa tạo và dòng phiếu mới có `NgayPhaiTra` bằng
   `NgayMuon + 7` ngày.
3. Trả sách theo số phiếu vừa tạo → cả 2 file cập nhật về `0`.
4. Xóa sách `S001` thành công → mở lại `Sach.txt`, dòng `S001` đã biến mất.

**Lời thoại (Vinh — kết luận):**

> "Tóm lại, hệ thống đã đạt đủ 3 tiêu chí kỹ thuật: thiết kế OOP và Template
> đầy đủ, tuân thủ C++ Coding Standard với 0 warnings, và toàn bộ chức năng
> cùng ràng buộc dữ liệu đều chạy đúng, có bộ test tự động 80 test pass 100
> phần trăm. Hạn chế còn lại là chương trình mới chạy trên Windows Console,
> chưa có giao diện đồ họa và chưa tính tiền phạt quá hạn — đó là hướng phát
> triển tiếp theo của nhóm. Cảm ơn thầy cô đã lắng nghe, nhóm em sẵn sàng trả
> lời câu hỏi."

## Checklist trước khi bấm Record trên Google Meet

- [ ] Tăng cỡ chữ Terminal/Console lên **18–20pt**, nền tối để bảng màu hiện
      rõ trên bản ghi video.
- [ ] Tắt thông báo Zalo/Messenger/Windows trên máy người share màn hình.
- [ ] Reset thư mục `data/` về trạng thái dữ liệu mẫu sạch trước khi bấm quay
      (`git checkout -- data/`).
- [ ] Build trước cả 2 file `.exe` (`library_app.exe` và `test_runner.exe`)
      để lúc quay chỉ việc chạy, không chờ biên dịch.
- [ ] Thử mic cả 4 thành viên 10 giây trước khi bắt đầu; chuyển người nói
      mượt mà (để 1 máy chạy demo chính hoặc luân phiên share màn hình đã mở
      sẵn).
- [ ] Tắt toàn bộ ứng dụng không liên quan, đóng các tab trình duyệt nhạy cảm.
- [ ] Chuẩn bị sẵn 1 file `Sach.txt` dự phòng nếu demo thêm sách bị lỗi giữa
      chừng (copy lại từ Git).
- [ ] Bấm Record, đợi 3 giây rồi mới bắt đầu nói câu đầu tiên.
