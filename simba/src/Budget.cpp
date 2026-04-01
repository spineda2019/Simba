#include "include/Budget.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sstream>
#include <string>

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

    std::tm tm{};

    date_stream >> std::get_time(&tm, "%Y-%m-%d");
    date = std::chrono::year{tm.tm_year + 1900} /
           std::chrono::month{static_cast<unsigned>(tm.tm_mon + 1)} /
           std::chrono::day{static_cast<unsigned>(tm.tm_mday)};

    time_stream >> std::get_time(&tm, "%H:%M:%S");
    auto aux = std::chrono::hours{tm.tm_hour} +
               std::chrono::minutes{tm.tm_min} +
               std::chrono::seconds{tm.tm_sec};
    time = std::chrono::hh_mm_ss<std::chrono::seconds>{aux};
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
