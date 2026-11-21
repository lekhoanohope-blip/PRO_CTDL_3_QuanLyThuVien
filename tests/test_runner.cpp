// Smoke test tam thoi cho Moc 1 (Khoa).
// Hieu se thay bang bo unit test day du o Task 1.
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

#include "Entity.h"
#include "Repository.h"
#include "Validation.h"

/// Thuc the gia lap de test Repository<T, KeyType> doc lap,
/// khong phu thuoc Models.h (Nhat/Hieu lam o Task 2).
class FakeBook : public Entity {
public:
    FakeBook() = default;
    FakeBook(const std::string& maSach, const std::string& tenSach)
        : m_maSach(maSach), m_tenSach(tenSach) {}

    std::string getKey() const { return m_maSach; }
    std::string getTenSach() const { return m_tenSach; }

    std::string serialize() const override {
        return m_maSach + "|" + m_tenSach;
    }

    std::string getEntityType() const override {
        return "FakeBook";
    }

    static bool fromFileLine(const std::string& line, FakeBook& out) {
        const std::vector<std::string> f = Validation::splitLine(line);
        if (f.size() != 2 || Validation::isBlank(f[0])) {
            return false;
        }
        out = FakeBook(f[0], f[1]);
        return true;
    }

private:
    std::string m_maSach;
    std::string m_tenSach;
};

int main() {
    // 1. Validation: parseNumberInRange
    int value = 0;
    assert(Validation::parseNumberInRange("42", 1, 100, value) && value == 42);
    assert(!Validation::parseNumberInRange("-5", 0, 100, value));
    assert(!Validation::parseNumberInRange("abc", 0, 100, value));
    assert(!Validation::parseNumberInRange("101", 0, 100, value));
    long long bigValue = 0;
    assert(Validation::parseNumberInRange("9000000000", 0LL, 99999999999LL, bigValue));

    // 2. Repository: CRUD + trung khoa
    Repository<FakeBook, std::string> repo;
    assert(repo.isEmpty());
    assert(repo.add(FakeBook("S001", "Lap Trinh C")));
    assert(!repo.add(FakeBook("S001", "Trung khoa phai that bai")));
    assert(repo.add(FakeBook("S002", "Cau Truc Du Lieu")));
    assert(repo.size() == 2);
    assert(repo.exists("S002"));
    assert(repo.findByKey("S001") != nullptr);
    assert(repo.findByKey("S999") == nullptr);
    assert(!repo.removeByKey("S999"));
    assert(repo.removeByKey("S001"));
    assert(repo.size() == 1);

    // 3. findWhere
    std::vector<const FakeBook*> found = repo.findWhere(
        [](const FakeBook& b) { return b.getTenSach().find("Du Lieu") != std::string::npos; });
    assert(found.size() == 1);

    std::printf("SMOKE TEST MOC 1: PASS 100%%\n");
    return 0;
}
