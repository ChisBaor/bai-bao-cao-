#pragma once
#include "../../core/Models.h"
#include <QVector>

// THANH VIEN 2: CRUD va tim kiem; module khac phai goi cac ham nay.
class ContactService {
public:
    static QVector<Contact> list(int userId, const QString &search = {});
    static bool get(int userId, int contactId, Contact *out);
    static bool add(Contact contact, QString *error = nullptr);
    static bool update(int userId, Contact contact, QString *error = nullptr);
    static bool remove(int userId, int contactId, QString *error = nullptr);
    static bool ownedBy(int userId, int contactId);
};
