# PHONE BOOK MANAGEMENT SYSTEM — STAGE 3 (C++ / Qt 6 / SQLite)

**Muc dich:** Project C++ Qt Widgets co the chia cho **5 thanh vien** lam song song
tren **mot repo GitHub**, khong phai 5 chuong trinh khac nhau.

**Phien ban:** bo khung MVP chuc nang, UI desktop theo huong giao dien Figma
402x900 (chua sao chep 1:1 giao dien, icon va cac man hinh trang thai Figma).

## 1. Cai dat

- Windows 10/11, **Qt Creator**, Qt 6.4+ voi kit Desktop MinGW 64-bit hoac MSVC.
- Module Qt can: **Widgets, SQL (SQLite driver), Network**. CMake va compiler C++17.
- IDE: **Qt Creator**. Co the mo `CMakeLists.txt` truc tiep.

## 2. Mo va chay

1. Giai nen folder `PhoneBookManagement_Stage3_5Members`.
2. Trong Qt Creator: **File → Open File or Project → CMakeLists.txt**.
3. Chon Desktop Qt 6 Kit; nhan **Configure Project**.
4. Build / Run (`Ctrl+R`).
5. O man hinh **Sign up**, dang ky tai khoan USER; sau do **Log in**.
6. Co the tao lien he, tim kiem, sua, xoa, tao tag, danh dau yeu thich va nhap CSV.
7. Database SQLite tu dong duoc tao trong **AppDataLocation** cua ung dung,
   khong nam trong thu muc ma nguon hay repo.

Neu bao loi `QSQLITE driver not loaded`, can cai/kiem tra Qt SQLite SQL plugin
trong bo Qt 6 Desktop kit dang chay.

### Tao tai khoan Admin demo an toan (khong hardcode password)

Mặc định Sign up **chỉ tạo USER**. Để kiểm tra Admin, trước lần chạy đầu khi
chưa có Admin, cấu hình các biến môi trường sau tại
**Qt Creator → Projects → Run → Run Environment** (không ghi mật khẩu vào code):

```
PHONEBOOK_ADMIN_USER=adminlocal
PHONEBOOK_ADMIN_EMAIL=admin@example.local
PHONEBOOK_ADMIN_PASSWORD=<mat-khau-manh-do-ban-tu-dat>
```

Khởi động ứng dụng. Admin chỉ được tạo khi chưa có Admin và cả 3 biến đầy đủ.
Sau lần tạo thành công, **xóa các biến môi trường chứa mật khẩu** trong phần
Run Environment. Không đưa mật khẩu thật lên GitHub. Nếu lần chạy trước đã tạo
USER tên `adminlocal`, hãy dùng username mới cho Admin.

> Chú ý: đây là tiện ích khởi tạo tài khoản quản trị cho đồ án desktop local.
> Trước khi triển khai thật cần đánh giá lại bảo mật và cấp quyền.

## 3. Năm module của nhóm

| Thành viên | Branch | Module / folder | Các chức năng |
|---|---|---|---|
| 1 | `feature/auth` | `src/modules/auth/` | Sign up, log in, log out, profile, password PBKDF2 |
| 2 | `feature/contacts` | `src/modules/contacts/` | Danh bạ, tìm kiếm, add/edit/delete |
| 3 | `feature/tags-favourites` | `src/modules/tags/` | Tag, Contact_Tag, Favourites |
| 4 | `feature/import-stats` | `src/modules/importstats/` | Import CSV, statistics Admin |
| 5 | `feature/admin-system` | `src/modules/admin/`, `src/core/`, `src/ui/`, `src/main.cpp`, `CMakeLists.txt` | Accounts, lock/unlock/delete, DB, app shell, integration |

**QUAN TRONG:** Cac contract public trong `*.h` da duoc thong nhat. Khong tu y
thay doi ten ham, kieu tham so hay DB schema khi chua trao doi ca nhom.

## 4. File du lieu va quan he

5 bang trong `database/schema.sql` tuong ung voi Data Model nhom:
`USER`, `CONTACT`, `TAG`, `Contact_Tag`, `FAVOURITE_LIST`.

- `ContactID`, `UserID`, `TagID`: SQLite INTEGER, tuong ung `int` trong C++.
- `CONTACT.UserID` tham chieu `USER.UserID`.
- `TAG.UserID` tham chieu `USER.UserID`.
- `Contact_Tag` lien ket Tag/Contact, PK(`TagID`,`ContactID`).
- `FAVOURITE_LIST` chi co 1 danh sach yeu thich mac dinh cho moi nguoi dung;
  `ContactID` la PK/FK, dung nhu ERD (khong co nhieu Favourite List name).
- Cot `USER.Password` cua ERD duoc thay bang `PasswordHash` va `PasswordSalt`
  de tranh luu mat khau ro. Hash PBKDF2-HMAC-SHA256 150000 iterations demo.
- Kiểu `DateOfBirth` được lưu text theo chuẩn `YYYY-MM-DD` do SQLite không
  có một kiểu DATE chuyên biệt.

## 5. File CSV mau

Xem `tests/sample_contacts.csv`.

```csv
FullName,PersonalPhone,PersonalEmail
Nguyen Van A,0912345678,a@example.com
Tran Thi B,0987654321,b@example.com
```

CSV phai co hang tieu de `FullName`; cac cot `PersonalPhone`, `PersonalEmail`
la tuy chon. Ho tro tieu de thay the `Name`, `Phone`, `Email`.
Ho tro dau phay trong truong co nhay kep, gioi han 5MB / 10000 dong.
Dong sai dinh dang se duoc bo qua va bao so dong skipped.

## 6. Quy trinh GitHub (moi nguoi code song song)

Mot nguoi tao repo GitHub **trong** va day bo khung len dau tien:

```bash
git init
git add .
git commit -m "Initial common Qt C++ project"
git branch -M main
git remote add origin https://github.com/USERNAME/PhoneBookManagement.git
git push -u origin main
```

Moi thanh vien clone repo ve may, tao 1 branch tu `main`. Vi du TV2:

```bash
git clone https://github.com/USERNAME/PhoneBookManagement.git
cd PhoneBookManagement
git switch -c feature/contacts
# Sau khi chinh sua va thu chay tren may
git add src/modules/contacts
git commit -m "Implement contact management"
git push -u origin feature/contacts
```

Tao Pull Request va yeu cau it nhat 1 nguoi khac review, build/run truoc
khi merge vao `main`. Thực hiện merge thường xuyên, không đợi đủ 5 người xong.

Cac thanh vien chi sua module cua minh. Muon sua `Models.h`, `Database.cpp`,
`CMakeLists.txt` hoac chu ky ham chung phai tao issue/thong bao truoc.

## 7. Kiem thu thu cong (Smoke test Stage 3)

1. Sign up 2 USER, log in tai khoan 1.
2. Them 2 contacts; tim kiem theo ten; sua va xoa 1 contact.
3. Tao tag `Family`, gan tag vao contact, bo gan; rename / delete tag.
4. Danh dau va bo danh dau favourite.
5. Import `tests/sample_contacts.csv`: 3 dong hop le; xem danh ba.
6. Log out; log in tai khoan 2, khong thay contacts cua tai khoan 1.
7. Dang nhap Admin, xac nhan co trang Admin va thong ke; khoa USER 2.
8. Thu dang nhap USER 2, phai bi tu choi; Admin mo khoa lai.

Đây là checklist thử thủ công, **chưa thay thế unit test của Stage 4**.

## 8. Giới hạn bản giao

- Là **MVP đồ án**: có nghiệp vụ và giao diện desktop cơ bản, chưa sao chép
  chính xác từng màn hình Figma; muốn 1:1 cần bước hoàn thiện Qt layout/style.
- Chưa có email xác minh, đổi mật khẩu, session persistent, phân trang danh bạ,
  log kiểm toán hoặc hệ thống test tự động đầy đủ.
- Ứng dụng desktop offline SQLite (không nhiều máy đồng bộ cùng 1 database).
- Hỗ trợ nhóm phát triển song song trên GitHub, nhưng **merge không bảo đảm
  tự động không lỗi**: mỗi nhánh vẫn phải build và kiểm thử.

## 9. Vấn đề biên dịch

Mã được tổ chức để dùng với Qt 6 Widgets + SQL + Network. Nếu có lỗi biên dịch
trên kit cụ thể của nhóm, chụp **Compiler Output** trong Qt Creator để xác định
chính xác header, compiler/Qt kit hoặc cấu hình cần sửa.

## 10. Docker deployment (bo sung)

- `Dockerfile`, `compose.yaml`, `docker/start.sh`: build Linux Qt app va chay qua noVNC.
- Xem `docker/README_DOCKER.md` de huong dan chi tiet cho Windows.
- Truy cap `http://localhost:6080/vnc.html` sau `docker compose up --build -d`.

## Phiên bản Mobile Preview Qt Widgets
Đây là bản preview giao diện dọc; xem `docs/MOBILE_PREVIEW_HUONG_DAN.md` để chuyển từ bản Desktop, triển khai lại Docker và phân công 5 thành viên. Giao diện chưa hoàn toàn giống tất cả các màn hình Figma.
