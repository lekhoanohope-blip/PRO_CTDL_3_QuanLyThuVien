# QA Checklist — Kiểm Thử Thủ Công (PRO_CTDL_3 Quản Lý Thư Viện)

Hướng dẫn dùng: chạy chương trình từ thư mục gốc, làm theo từng mục, đánh dấu
`[x]` vào ô khi trường hợp đã cho kết quả đúng. Dữ liệu mẫu dùng để kiểm tra:
`admin|123456`, sách `S001` (sẵn sàng), `S002` (đang mượn),
bạn đọc `BD001`–`BD003`, phiếu `1`–`3` (đang mượn).

## 1. Chuẩn bị & môi trường

- [ ] Build thành công với lệnh trong `README.md` (0 errors, 0 warnings).
- [ ] Chạy được `build\library_app.exe` từ thư mục gốc, màn hình chính hiện
      đủ khung tiêu đề và menu.
- [ ] Chạy được `build\test_runner.exe`, kết quả cuối `Tong: 80 test,
      Pass: 80, Fail: 0`, thoát mã `0`.
- [ ] Khôi phục được dữ liệu mẫu: `git checkout -- data/` rồi chạy lại,
      danh sách sách/bạn đọc/phiếu về đúng trạng thái ban đầu.

## 2. Đăng nhập

- [ ] Đăng nhập đúng `admin` / `123456` → vào được menu chính.
- [ ] Đăng nhập sai mật khẩu 1 lần → hiện thông báo lỗi, cho nhập lại.
- [ ] Đăng nhập sai mật khẩu 3 lần liên tiếp → chương trình tự thoát,
      không còn cho nhập nữa.
- [ ] Đăng nhập sai tên người dùng (ví dụ `admin123`) → hiện thông báo
      lỗi, cho nhập lại.
- [ ] Đăng nhập với `User` để trống (nhấn Enter ngay) → bị từ chối, không
      crash.
- [ ] Đăng nhập với `Password` để trống → bị từ chối, không crash.
- [ ] Nhấn phím `ESC` khi đang gõ `User` → chương trình thoát ngay.
- [ ] Nhấn phím `ESC` khi đang gõ `Password` → chương trình thoát ngay.
- [ ] Gõ phím mũi tên (`←`, `→`, `↑`, `↓`) trong ô `Password` → không sinh
      ký tự rác, màn hình vẫn chỉ hiện dấu `*` cho các ký tự đã gõ.
- [ ] Gõ phím mũi tên trong ô `User` → không sinh ký tự rác.
- [ ] Nhập mật khẩu dài hơn 20 ký tự → bị giới hạn/ từ chối đúng quy định,
      không tràn bộ nhớ.
- [ ] Mật khẩu chỉ hiện dấu `*`, không lộ ký tự thật trên màn hình.

## 3. Quản lý sách

- [ ] Danh sách sách hiển thị đủ 9 cột: mã, tên, tác giả, nhà xuất bản, giá,
      năm, số trang, ngày nhập kho, tình trạng.
- [ ] Sách sẵn sàng (tình trạng `0`) hiển thị màu xanh (hoặc màu phân biệt
      với sách đang mượn).
- [ ] Sách đang mượn (tình trạng khác `0`) hiển thị đúng số phiếu mượn.
- [ ] Thêm sách mới hợp lệ → lưu thành công, mở `data\Sach.txt` thấy dòng
      mới đã được ghi nối vào cuối file.
- [ ] Thêm sách trùng `MaSach` (ví dụ `S001`) → bị từ chối, thông báo "Sach
      da ton tai", file không đổi.
- [ ] Nhập chữ vào ô `GiaBan` (ví dụ `abc`) → bị từ chối nhập lại, không
      crash, không lưu giá rác.
- [ ] Nhập chữ vào ô `SoTrang` (ví dụ `12a`) → bị từ chối nhập lại, không
      crash.
- [ ] Nhập giá bán âm (`-5`) → bị từ chối, không lưu sách.
- [ ] Nhập giá bán `0` → bị từ chối (giá phải là số dương).
- [ ] Nhập số trang `0` → bị từ chối (số trang phải là số dương).
- [ ] Nhập năm phát hành `999` → bị từ chối (năm phải thuộc `[1000, năm
      hiện tại]`).
- [ ] Nhập năm phát hành là năm tương lai (ví dụ `2030`) → bị từ chối.
- [ ] Nhập ngày nhập kho tương lai (ví dụ ngày mai) → bị từ chối (ngày nhập
      kho phải `<=` hôm nay).
- [ ] Nhập ngày nhập kho không hợp lệ (`29/02/2025`, `31/04/2026`) → bị
      từ chối, không crash.
- [ ] Nhập ký tự `|` vào tên sách/tác giả → bị từ chối (vì `|` là ký tự phân
      cách file), không làm hỏng file.
- [ ] Nhập tên sách để trống → bị từ chối, không lưu sách rỗng.
- [ ] Xóa sách sẵn sàng (tình trạng `0`) → xóa được, `Sach.txt` đã ghi đè
      lại (dòng sách biến mất khỏi file).
- [ ] Xóa sách đang mượn (ví dụ `S002`) → bị từ chối, thông báo sách đang
      được mượn, file không đổi.
- [ ] Xóa sách không tồn tại (ví dụ `S999`) → báo không tìm thấy sách,
      không crash.
- [ ] Tìm sách theo tên (nhập `lap trinh`, chữ thường) → tìm thấy
      "Lap trinh C++ co ban" (không phân biệt hoa/thường).
- [ ] Tìm sách theo tác giả (nhập từ khóa viết hoa/thường lẫn lộn) → trả về
      đúng các sách của tác giả đó.
- [ ] Tìm sách không tồn tại (ví dụ `zzzz`) → trả về danh sách rỗng, không
      crash.
- [ ] Tìm sách theo mã (`S001`) → trả về đúng 1 sách.

## 4. Quản lý bạn đọc

- [ ] Danh sách bạn đọc hiển thị đủ 3 cột: mã, họ tên, ngày đăng ký.
- [ ] Thêm bạn đọc mới hợp lệ → lưu thành công, `BanDoc.txt` có dòng mới
      ghi nối vào cuối file.
- [ ] Thêm bạn đọc trùng `MaBanDoc` (ví dụ `BD001`) → bị từ chối.
- [ ] Thêm bạn đọc với họ tên để trống → bị từ chối.
- [ ] Thêm bạn đọc với ngày đăng ký tương lai → bị từ chối.
- [ ] Nhập ký tự `|` vào họ tên → bị từ chối, không làm hỏng file.
- [ ] Xóa bạn đọc không có phiếu đang mượn → xóa được.
- [ ] Xóa bạn đọc không tồn tại → báo không tìm thấy, không crash.
- [ ] Tìm bạn đọc theo mã (`BD001`) → trả về đúng 1 bạn đọc.

## 5. Mượn / trả sách

- [ ] Mượn sách sẵn sàng (`S001`) cho bạn đọc `BD001` → tạo phiếu mới,
      số phiếu tự tăng đúng (1, 2, 3...).
- [ ] Phiếu mới có `NgayMuon` là ngày hôm nay và `NgayPhaiTra` bằng
      `NgayMuon + 7` ngày.
- [ ] Sau khi mượn, sách chuyển sang tình trạng bằng số phiếu; mở
      `data\Sach.txt` và `data\PhieuMuon.txt` kiểm tra 2 file đồng bộ.
- [ ] Mượn sách đang mượn (ví dụ `S002`, tình trạng khác `0`) → bị từ chối,
      thông báo sách đang được mượn.
- [ ] Mượn sách với mã bạn đọc sai (ví dụ `BD999`) → bị từ chối.
- [ ] Mượn sách với mã sách sai (ví dụ `S999`) → bị từ chối.
- [ ] Trả sách theo số phiếu đang mượn → phiếu và sách đều về tình trạng
      `0`, 2 file đồng bộ.
- [ ] Trả cùng một phiếu lần thứ 2 → bị từ chối (phiếu đã ở tình trạng `0`).
- [ ] Trả phiếu không tồn tại (ví dụ `999`) → báo không tìm thấy phiếu,
      không crash.
- [ ] Sau khi trả sách xong, xóa được sách đó (tình trạng đã về `0`).
- [ ] Mượn sách, đóng app rồi mở lại → phiếu và tình trạng sách vẫn giữ
      nguyên (dữ liệu đã lưu xuống file).

## 6. Lưu trữ & đồng bộ file

- [ ] Tắt chương trình (chọn menu thoát) rồi mở lại → toàn bộ dữ liệu sách,
      bạn đọc, phiếu đều còn nguyên.
- [ ] Mở 4 file `data/*.txt` bằng Notepad sau mỗi thao tác thêm/xóa/mượn/trả
      → nội dung file khớp với màn hình hiển thị.
- [ ] File `data/` có dòng CRLF (`\r\n`) → chương trình đọc đúng, không
      hiện ký tự lạ.
- [ ] Thêm 1 dòng trống vào giữa file `Sach.txt` rồi chạy chương trình →
      dòng trống được bỏ qua, các sách khác vẫn đọc đủ.

## 7. Chạy từ các thư mục khác nhau

- [ ] Chạy từ thư mục gốc: `.\build\library_app.exe` → nạp đủ dữ liệu mẫu.
- [ ] Chạy từ thư mục `build/`: `.\library_app.exe` → vẫn nạp đủ dữ liệu
      mẫu (chương trình tự tìm thư mục `data/`).
- [ ] Chạy `test_runner.exe` từ thư mục gốc → `Pass: 80, Fail: 0`.

## 8. Giao diện Console 80 cột

- [ ] Thu nhỏ cửa sổ Console về đúng 80 cột rồi mở bảng sách → các cột
      không vỡ, không tràn, tiêu đề cột vẫn đọc được.
- [ ] Thu nhỏ cửa sổ Console về đúng 80 cột rồi mở bảng bạn đọc, bảng phiếu
      mượn → các cột không vỡ.
- [ ] Bảng sách hiển thị tên dài (ví dụ > 30 ký tự) → tên được cắt gọn đúng
      độ rộng cột, không làm lệch các cột còn lại.
- [ ] Màu sắc (sách sẵn sàng / đang mượn) vẫn phân biệt được trên nền
      Console tối.
