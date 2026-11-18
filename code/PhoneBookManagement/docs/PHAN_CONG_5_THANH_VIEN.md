# PHAN CONG 5 THANH VIEN VA QUY TAC GHEP CODE

## API/Contract chung - da chot truoc khi code

### AuthService (TV1)
- `registerUser(username, email, password, error)` -> bool
- `login(username, password, error)` -> `Session{ userId,username,email,role }`

### ContactService (TV2)
- `list(userId, search)` -> `QVector<Contact>`
- `get(userId, contactId, Contact*)` -> bool
- `add(Contact, error)` -> bool (ImportService TV4 su dung)
- `update(userId, Contact, error)` -> bool
- `remove(userId, contactId, error)` -> bool
- `ownedBy(userId, contactId)` -> bool (TagService TV3 su dung)

### TagFavouriteService (TV3)
- `tags`, `createTag`, `renameTag`, `deleteTag`, `assignTag`, `unassignTag`
- `assignments(userId)`, `favourites(userId)`, `addFavourite`, `removeFavourite`

### ImportStatsService (TV4)
- `importCsv(userId, path, ImportSummary*, error)` -> bool
- `statistics(adminId, SystemStatistics*, error)` -> bool

### AdminService (TV5)
- `isAdmin(userId)`, `accounts(adminId, search)`, `setAccountLocked`, `deleteAccount`

## Thu tu build merge

1. `main` ban dau chua 5 module toi thieu + SQLite.
2. TV1 va TV2 chu dong phat trien, TV3/TV4 goi dung API TV2.
3. TV5 phu trach `MainWindow` va compile ban `main` moi lan merge.
4. Sau moi PR: checkout branch, build, smoke test, roi moi merge.

## Vi du dong du lieu

User log in -> Session.userId = 7 -> ContactsPage.setUserId(7)
-> ContactService.list(7) -> SQLite `WHERE UserID=7`.

Import CSV -> ImportStatsService.importCsv(7, file) ->
ContactService.add(Contact{ userId=7, ... }) -> CONTACT table.

Tag -> TagFavouriteService.assignTag(7,tagId,contactId) ->
kiem tra ca Tag va Contact deu co owner UserID=7.

Admin -> AdminService.isAdmin(adminId) -> truoc khi lay danh sach user/
khoa/mo khoa/xoa tai khoan.

## Không nên làm

- 5 người tự tạo 5 Qt Creator project khác nhau.
- Mỗi người tự tạo một kiểu SQL/database khác nhau.
- Tự thêm/xóa tham số API chung trước khi thống nhất.
- Merge code không build được vào `main`.
- Commit file `*.db`, các folder `build`, hoặc mật khẩu admin.
