#include "include/Budget.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

namespace simba {

Budget::Budget() noexcept : cents_{0} {}
Budget::Budget(const nlohmann::json& json) : cents_{json.at("cents")} {}
void Budget::ToJson(nlohmann::json& json) const {
    json = {
        {"cents", cents_},
    };
}
void Budget::AddTransaction() {}

Budget Budget::FromFile(const std::string& path) {
    std::ifstream file{path};
    return {};
}

void to_json(nlohmann::json& j, const Budget& b) { b.ToJson(j); }

void from_json(const nlohmann::json& json, Budget& b) { b = Budget{json}; }

}  // namespace simba
