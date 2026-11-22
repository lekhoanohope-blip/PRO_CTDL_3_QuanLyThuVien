#ifndef LIBRARY_REPOSITORY_H
#define LIBRARY_REPOSITORY_H

#include <cstddef>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Validation.h"

/// Kho luu tru tong quat ket hop Danh sach lien ket kep (std::list)
/// va Cay tim kiem can bang (std::map) de tra cuu khoa chinh O(log n).
/// - std::list giu dung thu tu ban ghi, xoa O(1) khong lam vo hieu
///   hoa iterator cua cac phan tu khac.
/// - std::map (Red-Black Tree) anh xa khoa chinh -> iterator trong list.
/// Dung chung cho ca 4 thuc the: Admin, Book, Reader (khoa string)
/// va Loan (khoa int). Moi T phai co getKey() tra KeyType, serialize()
/// const va static bool fromFileLine(const std::string&, T&).
template <typename T, typename KeyType>
class Repository {
public:
    /// Kiem tra kho co rong khong.
    bool empty() const {
        return m_items.empty();
    }

    /// So ban ghi hien co trong kho.
    std::size_t size() const {
        return m_items.size();
    }

    /// Toan bo ban ghi theo dung thu tu trong file (chi doc).
    const std::list<T>& items() const {
        return m_items;
    }

    /// Kiem tra khoa da ton tai trong kho chua (O(log n)).
    bool exists(const KeyType& key) const {
        return m_index.find(key) != m_index.end();
    }

    /// Tra cuu khoa chinh O(log n); tra ve nullptr neu khong co.
    const T* findByKey(const KeyType& key) const {
        const auto found = m_index.find(key);
        if (found == m_index.end()) {
            return nullptr;
        }
        return &(*(found->second));
    }

    /// Ban ghi sua duoc theo khoa; tra ve nullptr neu khong co.
    T* findByKey(const KeyType& key) {
        const auto found = m_index.find(key);
        if (found == m_index.end()) {
            return nullptr;
        }
        return &(*(found->second));
    }

    /// Bi danh cua findByKey (ban sua duoc), giu tuong thich voi
    /// code da dung ten nay trong LibraryService.
    T* findMutable(const KeyType& key) {
        return findByKey(key);
    }

    /// Them ban ghi moi; tra ve false neu khoa da ton tai (khong ghi de).
    bool add(const T& item) {
        const KeyType key = item.getKey();
        if (exists(key)) {
            return false;
        }
        m_items.push_back(item);
        auto it = m_items.end();
        --it;
        m_index[key] = it;
        return true;
    }

    /// Ghi de ban ghi co cung khoa, giu nguyen vi tri;
    /// tra ve false neu khoa chua co.
    bool update(const T& item) {
        const auto found = m_index.find(item.getKey());
        if (found == m_index.end()) {
            return false;
        }
        *(found->second) = item;
        return true;
    }

    /// Xoa theo khoa O(1) khoi ca list va map; false neu khong co.
    bool removeByKey(const KeyType& key) {
        const auto found = m_index.find(key);
        if (found == m_index.end()) {
            return false;
        }
        m_items.erase(found->second);
        m_index.erase(found);
        return true;
    }

    /// Loc ban ghi theo dieu kien tuy y (member function template).
    /// Dung cho tim kiem theo TenSach / TacGia (chuoi con, khong
    /// phan biet hoa/thuong). Tra ve con tro chi doc.
    template <typename Predicate>
    std::vector<const T*> findWhere(Predicate predicate) const {
        std::vector<const T*> results;
        for (const T& item : m_items) {
            if (predicate(item)) {
                results.push_back(&item);
            }
        }
        return results;
    }

    /// Loc ban ghi theo dieu kien tuy y, tra ve con tro sua duoc.
    template <typename Predicate>
    std::vector<T*> findWhereMutable(Predicate predicate) {
        std::vector<T*> results;
        for (T& item : m_items) {
            if (predicate(item)) {
                results.push_back(&item);
            }
        }
        return results;
    }

    /// Khoa lon nhat hien co (chi dung khi KeyType la so);
    /// tra ve KeyType() neu kho rong. Dung de sinh SoPhieuMuon = max + 1.
    KeyType maxKey() const {
        if (m_index.empty()) {
            return KeyType();
        }
        return m_index.rbegin()->first;
    }

    /// Xoa toan bo ban ghi khoi RAM.
    void clear() {
        m_items.clear();
        m_index.clear();
    }

    /// Nap toan bo file: tu dong bo qua dong trong, chiu ca CRLF
    /// (\r\n) va LF (\n); dong sai dinh dang bi bo qua va doc tiep.
    /// Tra ve false chi khi khong mo duoc file.
    bool loadFromFile(const std::string& filePath) {
        std::ifstream input(filePath);
        if (!input.is_open()) {
            return false;
        }
        clear();
        std::string line;
        while (std::getline(input, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if (Validation::isBlank(line)) {
                continue;
            }
            T item;
            if (T::fromFileLine(line, item)) {
                add(item);
            }
        }
        return true;
    }

    /// Ghi de toan bo kho ra file (moi ban ghi 1 dong).
    /// Tra ve false khi loi mo/ghi file.
    bool saveAllToFile(const std::string& filePath) const {
        std::ofstream output(filePath, std::ios::trunc);
        if (!output.is_open()) {
            return false;
        }
        for (const T& item : m_items) {
            output << item.serialize() << '\n';
            if (!output) {
                return false;
            }
        }
        return static_cast<bool>(output);
    }

    /// Ghi noi 1 ban ghi vao cuoi file. Neu file khong rong va byte
    /// cuoi khac '\n' thi chen '\n' truoc de tranh dinh 2 ban ghi
    /// chung 1 dong. Tra ve false khi loi mo/ghi file.
    bool appendToFile(const std::string& filePath, const T& item) const {
        std::ifstream input(filePath, std::ios::ate);
        bool needsNewline = false;
        if (input.is_open()) {
            const std::streamoff fileSize =
                static_cast<std::streamoff>(input.tellg());
            if (fileSize > 0) {
                constexpr std::streamoff LAST_BYTE_OFFSET = -1;
                input.seekg(LAST_BYTE_OFFSET, std::ios::end);
                const char lastChar = static_cast<char>(input.get());
                needsNewline = (lastChar != '\n');
            }
            input.close();
        }
        std::ofstream output(filePath, std::ios::app);
        if (!output.is_open()) {
            return false;
        }
        if (needsNewline) {
            output << '\n';
        }
        output << item.serialize() << '\n';
        return static_cast<bool>(output);
    }

private:
    std::list<T> m_items;
    std::map<KeyType, typename std::list<T>::iterator> m_index;
};

#endif  // LIBRARY_REPOSITORY_H
