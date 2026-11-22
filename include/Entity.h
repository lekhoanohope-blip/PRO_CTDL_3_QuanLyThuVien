#ifndef LIBRARY_ENTITY_H
#define LIBRARY_ENTITY_H

#include <string>

/// Lop co so truu tuong cho moi thuc the luu tru trong he thong.
/// Cung cap giao dien chung de tuan tu hoa va nhan dien loai thuc the,
/// cho phep Repository<T, KeyType> lam viec da hinh voi moi loai.
class Entity {
public:
    /// Huy ao de giai phong dung doi tuong lop dan xuat qua con tro lop co so.
    virtual ~Entity() = default;

    /// Tuan tu hoa thuc the thanh mot dong file, cac truong noi bang '|'.
    virtual std::string serialize() const = 0;

    /// Tra ve ten loai thuc the ("Admin", "Book", "Reader", "Loan").
    virtual std::string getEntityType() const = 0;
};

#endif  // LIBRARY_ENTITY_H
