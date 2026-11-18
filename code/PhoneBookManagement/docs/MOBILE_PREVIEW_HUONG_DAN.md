# Hướng dẫn sử dụng phiên bản giao diện dọc (Qt Widgets, Docker)

## Mục tiêu và giới hạn

Đây là bản **Mobile Preview lần 1** dành cho **desktop Qt Widgets**, không phải ứng dụng APK. So với nguồn Figma, bản này áp dụng hệ màu nền #F8F7F2, xanh #21745B, bố cục 402 px, đăng nhập dọc, danh bạ dạng từng liên hệ và thanh điều hướng dưới. Những màn hình Tag/Favourite, Import, Admin đã được thu gọn để sử dụng trong cửa sổ nhỏ, nhưng **chưa khớp từng chi tiết trong toàn bộ 12 nhóm màn hình Figma**. Biểu tượng Figma gốc chưa được nhúng và trạng thái iOS là minh họa. Hãy tiếp tục phát triển theo phân công 5 thành viên.

## Cài bản mới trên Windows bằng Qt Creator

1. **Giữ nguyên và sao lưu** thư mục PhoneBookManagement_Stage3_5Members đã chạy thành công.
2. Giải nén ZIP MobilePreview thành thư mục mới, ví dụ `C:\Projects\PhoneBookManagement_MobilePreview`.
3. Trong Qt Creator chọn File > Open File or Project > chọn `CMakeLists.txt` của thư mục mới.
4. Chọn Desktop Qt 6.12 MinGW 64-bit (hoặc Qt 6 phù hợp), nhấn Configure Project.
5. Nhấn `Ctrl+B`, sau đó `Ctrl+R`.
6. Xem giao diện dọc; kiểm tra Log in, Contacts, Add/Edit/Delete, Tags, Import và Admin.

**SQLite:** Dữ liệu thực tế được lưu theo AppDataLocation của Qt và cấu hình ứng dụng hiện có. Mở bản mới có thể sử dụng cùng dữ liệu trên Windows nếu appName/orgName giữ nguyên. KHÔNG xóa file cơ sở dữ liệu để đổi giao diện. Sao lưu trước khi thử các chức năng xóa.

## Triển khai lại trên Docker Desktop

1. Giữ Docker Desktop ở trạng thái Engine running.
2. Mở PowerShell ngay tại thư mục bản MobilePreview có `compose.yaml`.
3. Chạy:

```powershell
docker compose down
docker compose up --build -d
docker compose ps
```

4. Mở thủ công `http://localhost:6080/vnc.html` trong trình duyệt.

`docker compose down` không kèm `-v` sẽ giữ nguyên volume SQLite. Tên project Docker Compose thay đổi nếu thư mục nguồn đổi tên: volume khác sẽ là cơ sở dữ liệu khác. Để **giữ volume và tài khoản Docker cũ**, hãy *chép các file mã nguồn và start.sh đã sửa* từ bản MobilePreview vào **thư mục Project Docker cũ** rồi build ở thư mục đó, không xóa volume và không chạy `down -v`.

## Phân chia 5 thành viên

- TV1: `src/modules/auth/AuthPage.cpp` (Login/Sign-up)
- TV2: `src/modules/contacts/ContactsPage.cpp` (Contacts/Add/Edit/Delete)
- TV3: `src/modules/tags/TagFavouritePage.cpp` (Tags/Favourites)
- TV4: `src/modules/importstats/ImportStatsPage.cpp` (Import/Statistics)
- TV5: `src/modules/admin/AdminPage.cpp` và `src/ui/MainWindow.cpp` (Admin/navigation), `src/main.cpp` (theme), `docker/start.sh` (deployment)

Mỗi thành viên làm trên nhánh Git riêng. Các file `src/core`, `*Service.cpp/.h` và schema SQLite không cần đổi nếu chỉ sửa giao diện. Nếu cần thay giao diện có tương tác với dữ liệu thì thảo luận interface với chủ module trước.

## Những việc chưa được kiểm thử trong môi trường tạo bản ZIP

Môi trường tạo bản ZIP không có Qt 6 development và Docker Engine, vì vậy **chưa thể build/run ZIP mới tại đây**. Nhóm cần kiểm tra thực tế bằng Qt Creator và Docker trên Windows. Đây là bản dựa trên mã nguồn trước đó đã chạy trên máy nhóm; sau khi thay đổi GUI vẫn phải biên dịch và kiểm thử lại.
