#ifndef SIMBA_SRC_INCLUDE_BUDGET_HPP_
#define SIMBA_SRC_INCLUDE_BUDGET_HPP_

#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>

namespace simba {
class Budget final {
 public:
    Budget() noexcept;
    Budget(const nlohmann::json& json);
    void AddTransaction();
    void ToJson(nlohmann::json&) const;

 public:
    static Budget FromFile(const std::string& path);

 private:
    std::size_t cents_{};
};

void to_json(nlohmann::json& j, const Budget& b);
void from_json(const nlohmann::json& j, Budget& p);

}  // namespace simba

#endif  // SIMBA_SRC_INCLUDE_BUDGET_HPP_
