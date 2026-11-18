# Triển khai Phone Book Management bằng Docker (Windows + Docker Desktop)

## Vì sao cần noVNC?
Project là ứng dụng C++ dùng Qt Widgets (desktop), **không phải website**. Một Linux container
không tự có màn hình. Bản triển khai này chạy ứng dụng với Xvfb và xem giao diện trong
trình duyệt bằng noVNC. Đây là cách demo cục bộ cho học phần, không phải dịch vụ
Internet bảo mật để sử dụng công khai.

## A. Lập trình trên Windows: dùng Qt Creator
1. Giải nén project rồi mở `CMakeLists.txt` bằng Qt Creator.
2. Chọn **Desktop Qt 6 Kit** (Qt >= 6.4; Widgets, Sql, Network) và Configure Project.
3. Nhấn Ctrl+R để biên dịch/chạy. Docker **không bắt buộc** trong lúc viết code.
4. Các module trong thư mục `src/modules` chia theo thành viên.

## B. Demo bằng Docker
1. Cài Docker Desktop cho Windows, dùng Linux containers/WSL 2 và mở Docker Desktop.
2. Mở PowerShell hoặc Terminal tại thư mục gốc Project (nơi có `compose.yaml`).
3. Chạy:

```powershell
docker compose up --build -d
```

4. Truy cập **http://localhost:6080/vnc.html** trên Chrome/Edge. Nhấn Connect
nếu noVNC hỏi. Đây là giao diện Qt đang chạy trong container Linux.
5. Xem log nếu cần: `docker compose logs -f phonebook`
6. Dừng ứng dụng: `docker compose down` (vẫn giữ volume SQLite).
7. Chạy lại: `docker compose up -d`.

**Mỗi lần sửa code:** cần build lại image để cập nhật code:

```powershell
docker compose up --build -d
```

### Tài khoản Admin demo (tùy chọn)
Sau khi chạy thành công chức năng User, nếu muốn test Admin:
- Tạo file `.env` trong thư mục gốc, file này đã được `.gitignore` loại trừ.
- Chỉ dùng mật khẩu thử nghiệm, đặt các dòng dưới đây với giá trị do nhóm tự chọn:

```dotenv
PHONEBOOK_ADMIN_USER=adminlocal
PHONEBOOK_ADMIN_EMAIL=admin@example.local
PHONEBOOK_ADMIN_PASSWORD=CHON_MAT_KHAU_THU_NGHIEM_DU_MANH
```

- Khởi động lại: `docker compose up -d --force-recreate`.
- Sau khi Admin được tạo, **xóa mật khẩu khỏi `.env`**, rồi recreate container.
- Đừng dùng credentials thật trong .env vì biến môi trường container có thể bị xem trong
Docker metadata. Cơ chế này chỉ phục vụ demo trên máy cá nhân.

### Kiểm tra dữ liệu không mất sau khi đóng Docker
Dữ liệu SQLite nằm tại volume tên `phonebook-data` và được mount vào `/data`.
`docker compose down` sẽ giữ dữ liệu; **không chạy `docker compose down -v`**
nếu muốn bảo toàn danh bạ.

## C. Lưu ý an toàn và giới hạn
- `127.0.0.1:6080:6080` giới hạn trang noVNC cho máy chạy Docker. **Không đổi thành
  `0.0.0.0:6080:6080` hay public port** nếu chưa bổ sung xác thực và TLS.
- Không đưa `.env`, database chứa dữ liệu cá nhân hay mật khẩu lên GitHub.
- Docker này tạo Linux container, **không tạo file .exe Windows**.
- Nếu giảng viên yêu cầu file .exe Windows, đóng gói bổ sung bằng windeployqt sau khi
  biên dịch trên Windows. Đây là con đường triển khai khác với Docker.
- Máy có Docker Desktop mới dùng được cách này; không biến C++ Qt desktop thành web app.
- Bộ Dockerfile chưa được thực tế build trên máy có Docker trong môi trường tạo file.
  Nếu build lỗi, lưu `docker compose logs` và output Build để nhóm sửa đúng thông báo.
