#ifndef LIBRARY_REPOSITORY_H
#define LIBRARY_REPOSITORY_H

#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Validation.h"

/// Class Template: Kho luu tru tong quat cho moi thuc thi cua he thong.
///
/// Ket hop 2 cau truc du lieu STL bu tru diem yeu cho nhau:
///  - std::list<T>: danh sach lien ket kep, giu nguyen thu tu ghi file,
///    chen/xoa O(1) va KHONG lam vo hieu hoa iterator dang ton tai.
///  - std::map<KeyType, iterator>: cay Red-Black Tree, tra cuu khoa chinh
///    O(log n) thay vi duyet tuyen tinh O(n).
///
/// Dieu kien cua kieu T:
///  - T::getKey() const -> KeyType
///  - T::serialize() const -> std::string   (ke thua tu Entity)
///  - static bool T::fromFileLine(const std::string& line, T& out)
///
/// Vi du khoi tao:
///  Repository<Book, std::string> books;
///  Repository<Loan, int> loans;
template <typename T, typename KeyType>
class Repository {
public:
    using Iterator = typename std::list<T>::iterator;
    using ConstIterator = typename std::list<T>::const_iterator;

    /// So luong ban ghi dang luu trong kho.
    /// @return So phan tu cua danh sach.
    std::size_t size() const {
        return m_items.size();
    }

    /// Kiem tra kho co rong khong.
    /// @return true neu chua co ban ghi nao.
    bool isEmpty() const {
        return m_items.empty();
    }

    /// Kiem tra khoa da ton tai chua (tra cuu O(log n)).
    /// @param key Khoa can kiem tra.
    /// @return true neu khoa da ton tai.
    bool exists(const KeyType& key) const {
        return m_index.find(key) != m_index.end();
    }

    /// Them ban ghi moi. Tu choi neu trung khoa chinh.
    /// @param item Ban ghi can them.
    /// @return true neu them thanh cong, false neu trung khoa.
    bool add(const T& item) {
        const KeyType key = item.getKey();
        if (exists(key)) {
            return false;
        }
        m_items.push_back(item);
        Iterator it = m_items.end();
        --it;
        m_index[key] = it;
        return true;
    }

    /// Tim ban ghi theo khoa chinh (tra cuu O(log n)).
    /// Con tro tra ve on dinh vi std::list khong doi chuoi bo nho
    /// khi chen/xoa phan tu khac.
    /// @param key Khoa can tim.
    /// @return Con tro toi ban ghi, hoac nullptr neu khong co.
    const T* findByKey(const KeyType& key) const {
        const auto it = m_index.find(key);
        if (it == m_index.end()) {
            return nullptr;
        }
        return &(*(it->second));
    }

    /// Tra ve con tro co the sua doi (dung khi cap nhat TinhTrang...).
    /// @param key Khoa can tim.
    /// @return Con tro toi ban ghi, hoac nullptr neu khong co.
    T* findByKeyMutable(const KeyType& key) {
        const auto it = m_index.find(key);
        if (it == m_index.end()) {
            return nullptr;
        }
        return &(*(it->second));
    }

    /// Xoa ban ghi theo khoa (xoa O(1) tren danh sach lien ket).
    /// @param key Khoa cua ban ghi can xoa.
    /// @return true neu xoa thanh cong, false neu khong tim thay.
    bool removeByKey(const KeyType& key) {
        const auto it = m_index.find(key);
        if (it == m_index.end()) {
            return false;
        }
        m_items.erase(it->second);  // Xoa node khoi list.
        m_index.erase(it);          // Xoa khoi cay tim kiem.
        return true;
    }

    /// Lay toan bo ban ghi (tham chieu, khong sao chep du lieu).
    /// @return Vector con tro toi tat ca ban ghi, dung thu tu file.
    std::vector<const T*> getAll() const {
        std::vector<const T*> results;
        results.reserve(m_items.size());
        for (ConstIterator it = m_items.begin(); it != m_items.end(); ++it) {
            results.push_back(&(*it));
        }
        return results;
    }

    /// Member Function Template: loc ban ghi theo dieu kien tuy y.
    /// Dung cho tim kiem theo TenSach / TacGia (khong phan biet hoa thuong),
    /// loc sach dang muon, loc phieu chua tra...
    /// @tparam Predicate Kieu ham/lambda nhan (const T&) tra ve bool.
    /// @param predicate Dieu kien loc.
    /// @return Vector con tro toi cac ban ghi thoa man.
    template <typename Predicate>
    std::vector<const T*> findWhere(Predicate predicate) const {
        std::vector<const T*> results;
        for (ConstIterator it = m_items.begin(); it != m_items.end(); ++it) {
            if (predicate(*it)) {
                results.push_back(&(*it));
            }
        }
        return results;
    }

    /// Ghi de toan bo kho ra file (1 ban ghi / dong).
    /// @param filePath Duong dan file can ghi.
    /// @return true neu ghi thanh cong.
    bool saveAllToFile(const std::string& filePath) const {
        std::ofstream out(filePath, std::ios::out | std::ios::trunc);
        if (!out.is_open()) {
            return false;
        }
        for (ConstIterator it = m_items.begin(); it != m_items.end(); ++it) {
            out << it->serialize() << '\n';
        }
        out.close();
        return !out.fail();
    }

    /// Ghi noi 1 ban ghi vao cuoi file. Tu dong bo sung '\n' neu
    /// file hien tai khong ket thuc bang ky tu xuong dong (tranh dinh
    /// 2 ban ghi chung 1 dong khi append).
    /// @param item Ban ghi can ghi noi.
    /// @param filePath Duong dan file can ghi noi.
    /// @return true neu ghi thanh cong.
    bool appendToFile(const T& item, const std::string& filePath) const {
        if (!ensureTrailingNewline(filePath)) {
            return false;
        }
        std::ofstream out(filePath, std::ios::out | std::ios::app);
        if (!out.is_open()) {
            return false;
        }
        out << item.serialize() << '\n';
        out.close();
        return !out.fail();
    }

    /// Nap toan bo du lieu tu file vao kho.
    /// Bo qua: dong trong, dong loi dinh dang, dong trung khoa.
    /// Xu ly an toan ca CRLF (\r\n) lan LF (\n).
    /// @param filePath Duong dan file can nap.
    /// @return true neu mo duoc file (file chua ton tai thi khoi tao kho rong).
    bool loadFromFile(const std::string& filePath) {
        clear();
        std::ifstream in(filePath);
        if (!in.is_open()) {
            return false;  // File chua co: he thong khoi tao tu danh sach rong.
        }
        std::string line;
        while (std::getline(in, line)) {
            if (Validation::isBlank(line)) {
                continue;  // Bo qua dong trong.
            }
            T item;
            if (T::fromFileLine(line, item)) {
                add(item);  // add() tu bo qua neu trung khoa.
            }
        }
        in.close();
        return true;
    }

    /// Xoa toan bo du lieu trong kho (giu nguyen file tren dia).
    void clear() {
        m_items.clear();
        m_index.clear();
    }

private:
    /// Dam bao file ket thuc bang '\n' truoc khi ghi noi.
    /// @param filePath Duong dan file can kiem tra.
    /// @return true neu da an toan (hoac da xu ly xong).
    bool ensureTrailingNewline(const std::string& filePath) const {
        // Mo de doc: kiem tra byte cuoi cung.
        std::ifstream in(filePath, std::ios::in | std::ios::binary);
        if (!in.is_open()) {
            return true;  // File chua ton tai: ghi noi se tao file moi.
        }
        in.seekg(0, std::ios::end);
        const std::streamoff length = in.tellg();
        if (length == 0) {
            in.close();
            return true;  // File rong: ghi noi binh thuong.
        }
        in.seekg(-1, std::ios::end);
        char lastChar = '\0';
        in.get(lastChar);
        in.close();
        if (lastChar == '\n') {
            return true;  // Da ket thuc bang xuong dong.
        }
        // Thieu '\n' -> bo sung truoc khi ghi noi ban ghi moi.
        std::ofstream fix(filePath, std::ios::out | std::ios::app);
        if (!fix.is_open()) {
            return false;
        }
        fix << '\n';
        fix.close();
        return !fix.fail();
    }

    std::list<T> m_items;
    std::map<KeyType, typename std::list<T>::iterator> m_index;
};

#endif  // LIBRARY_REPOSITORY_H
