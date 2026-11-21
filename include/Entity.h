#ifndef LIBRARY_ENTITY_H
#define LIBRARY_ENTITY_H

#include <string>

/// Lop co so truu tuong cho moi thuc the cua he thong thu vien.
/// Dinh nghia giao dien chung cho da hinh: serialize() va getEntityType().
/// Moi lop con (Admin, Book, Reader, Loan) bat buoc phai cai dat
/// serialize(), getEntityType(), getKey() va static fromFileLine().
class Entity {
public:
    /// Huy ao: bat buoc khi co ke thua va da hinh.
    virtual ~Entity() = default;

    /// Chuyen toan bo trang thai thuc the thanh 1 dong file.
    /// Cac truong phan cach bang FIELD_DELIMITER ('|').
    /// @return Chuoi dong file, KHONG chua ky tu xuong dong.
    virtual std::string serialize() const = 0;

    /// Tra ve ten loai thuc the (vi du: "Book", "Loan").
    /// Dung cho thong bao loi va log.
    /// @return Ten loai thuc the.
    virtual std::string getEntityType() const = 0;
};

#endif  // LIBRARY_ENTITY_H
