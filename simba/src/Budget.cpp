#include "include/Budget.hpp"

#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

#include "nlohmann/json_fwd.hpp"

namespace simba {

Budget::Budget() noexcept : cents_{0}, transaction_history_{} {}

Budget::Budget(Budget&& other) noexcept
    : cents_{other.cents_}, transaction_history_{other.transaction_history_} {}

Budget::Budget(const nlohmann::json& json)
    : cents_{json.at("cents")},
      transaction_history_(
          json.at("transaction_history").get<std::vector<Transaction>>()) {}

Budget& Budget::operator=(Budget&& other) noexcept {
    cents_ = other.cents_;
    transaction_history_ = std::move(other.transaction_history_);
    return *this;
}

void Budget::ToJson(nlohmann::json& json) const {
    json = {
        {"cents", cents_},
        {"transaction_history", transaction_history_},
    };
}

void Budget::AddTransaction() {}

Budget Budget::FromFile(const std::string& path) {
    try {
        std::ifstream file{path};
        nlohmann::json j{};
        file >> j;
        return j.get<Budget>();
    } catch (...) {
        return {};
    }
}

void Budget::SaveToFile(const std::string& path) {
    try {
        std::ofstream file{path};
        const nlohmann::json j(*this);
        file << j;
    } catch (...) {
    }
}

Budget::Transaction::Transaction() noexcept : date{}, time{}, amount{} {}

Budget::Transaction::Transaction(const nlohmann::json& json)
    : date{}, time{}, amount{json.at("amount")} {
    const std::string date_string{json.at("date")};
    std::istringstream date_stream{date_string};

    const std::string time_string{json.at("time")};
    std::istringstream time_stream{time_string};
    std::chrono::seconds aux{};

    std::chrono::from_stream(date_stream, "%F", date);
    std::chrono::from_stream(time_stream, "%T", aux);

    time = std::chrono::hh_mm_ss<decltype(aux)>{aux};
}

void Budget::Transaction::ToJson(nlohmann::json& json) const {
    std::ostringstream date_stream{};
    std::ostringstream time_stream{};

    date_stream << date;
    time_stream << time;

    json = {
        {"amount", amount},
        {"date", date_stream.view()},
        {"time", time_stream.view()},
    };
}

void to_json(nlohmann::json& j, const Budget& b) { b.ToJson(j); }

void from_json(const nlohmann::json& json, Budget& b) { b = Budget{json}; }

void to_json(nlohmann::json& j, const Budget::Transaction& t) { t.ToJson(j); }

void from_json(const nlohmann::json& j, Budget::Transaction& t) {
    t = Budget::Transaction{j};
}

}  // namespace simba
