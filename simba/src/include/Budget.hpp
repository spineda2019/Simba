#ifndef SIMBA_SRC_INCLUDE_BUDGET_HPP_
#define SIMBA_SRC_INCLUDE_BUDGET_HPP_

#include <chrono>
#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>

#include "meta.hpp"

namespace simba {
class Budget final {
 public:
    Budget() noexcept;
    Budget(const nlohmann::json& json);
    void AddTransaction();
    void ToJson(nlohmann::json&) const;

 public:
    struct Transaction final {
        std::chrono::year_month_day date;
        std::chrono::hh_mm_ss<std::chrono::seconds> time;

        meta::signed_native_word_t amount;
    };

 public:
    static Budget FromFile(const std::string& path);
    void SaveToFile(const std::string& path);

 private:
    std::size_t cents_{};
};

void to_json(nlohmann::json& j, const Budget& b);
void from_json(const nlohmann::json& j, Budget& p);

}  // namespace simba

#endif  // SIMBA_SRC_INCLUDE_BUDGET_HPP_
