#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "Budget.hpp"

namespace {

nlohmann::json MakeEmptyBudgetJson() {
    nlohmann::json j;
    j["cents"] = 0;
    j["transaction_history"] = nlohmann::json::array();
    return j;
}

nlohmann::json MakeTransactionJson(const std::string& date,
                                   const std::string& time,
                                   std::int64_t amount) {
    nlohmann::json j;
    j["date"] = date;
    j["time"] = time;
    j["amount"] = amount;
    return j;
}

nlohmann::json MakeBudgetJson(std::size_t cents,
                              const nlohmann::json& transactions) {
    nlohmann::json j;
    j["cents"] = cents;
    j["transaction_history"] = transactions;
    return j;
}

class BudgetFileTest : public ::testing::Test {
 protected:
    std::filesystem::path tmp_path_;

    void SetUp() override {
        tmp_path_ = std::filesystem::temp_directory_path() /
                    ("simba_test_" +
                     std::to_string(std::hash<std::string>{}(
                         ::testing::UnitTest::GetInstance()
                             ->current_test_info()
                             ->name())) +
                     ".json");
    }

    void TearDown() override { std::filesystem::remove(tmp_path_); }
};

// --- Budget default constructor ---

TEST(BudgetTest, DefaultConstructorProducesEmptyCentsAndHistory) {
    const simba::Budget budget{};
    nlohmann::json j{};
    budget.ToJson(j);

    EXPECT_EQ(j.at("cents").get<std::size_t>(), 0u);
    EXPECT_TRUE(j.at("transaction_history").is_array());
    EXPECT_TRUE(j.at("transaction_history").empty());
}

// --- Budget JSON round-trip ---

TEST(BudgetTest, JsonRoundTripNoTransactions) {
    const auto input = MakeEmptyBudgetJson();
    const simba::Budget budget{input};

    nlohmann::json output{};
    budget.ToJson(output);

    EXPECT_EQ(output.at("cents").get<std::size_t>(), 0u);
    EXPECT_TRUE(output.at("transaction_history").empty());
}

TEST(BudgetTest, JsonRoundTripWithTransactions) {
    auto transactions = nlohmann::json::array();
    transactions.push_back(
        MakeTransactionJson("2026-03-15", "14:30:00", -5000));
    transactions.push_back(
        MakeTransactionJson("2026-03-16", "09:00:00", 120000));

    const auto input = MakeBudgetJson(42000, transactions);
    const simba::Budget budget{input};

    nlohmann::json output{};
    budget.ToJson(output);

    EXPECT_EQ(output.at("cents").get<std::size_t>(), 42000u);

    const auto& out_txns = output.at("transaction_history");
    ASSERT_EQ(out_txns.size(), 2u);

    EXPECT_EQ(out_txns[0].at("amount").get<std::int64_t>(), -5000);
    EXPECT_EQ(out_txns[1].at("amount").get<std::int64_t>(), 120000);
}

// --- Budget ADL round-trip via nlohmann conventions ---

TEST(BudgetTest, AdlRoundTrip) {
    const auto input = MakeBudgetJson(999, nlohmann::json::array());
    const nlohmann::json j_in = input;

    const auto budget = j_in.get<simba::Budget>();
    const nlohmann::json j_out = budget;

    EXPECT_EQ(j_out.at("cents").get<std::size_t>(), 999u);
    EXPECT_TRUE(j_out.at("transaction_history").empty());
}

// --- Transaction JSON round-trip ---

TEST(TransactionTest, JsonRoundTrip) {
    const auto input = MakeTransactionJson("2026-03-15", "14:30:00", -5000);
    const simba::Budget::Transaction txn{input};

    nlohmann::json output{};
    txn.ToJson(output);

    EXPECT_EQ(output.at("amount").get<std::int64_t>(), -5000);
    EXPECT_FALSE(output.at("date").get<std::string>().empty());
    EXPECT_FALSE(output.at("time").get<std::string>().empty());
}

TEST(TransactionTest, DateTimeParsing) {
    const auto input = MakeTransactionJson("2026-03-15", "14:30:45", 100);
    const simba::Budget::Transaction txn{input};

    EXPECT_EQ(static_cast<int>(txn.date.year()), 2026);
    EXPECT_EQ(static_cast<unsigned>(txn.date.month()), 3u);
    EXPECT_EQ(static_cast<unsigned>(txn.date.day()), 15u);

    EXPECT_EQ(txn.time.hours().count(), 14);
    EXPECT_EQ(txn.time.minutes().count(), 30);
    EXPECT_EQ(txn.time.seconds().count(), 45);

    EXPECT_EQ(txn.amount, 100);
}

TEST(TransactionTest, SerializedDateIsIso8601) {
    const auto input = MakeTransactionJson("2024-12-25", "08:00:00", 0);
    const simba::Budget::Transaction txn{input};

    nlohmann::json output{};
    txn.ToJson(output);

    EXPECT_EQ(output.at("date").get<std::string>(), "2024-12-25");
    EXPECT_EQ(output.at("time").get<std::string>(), "08:00:00");
}

TEST(TransactionTest, AdlToJson) {
    const auto input = MakeTransactionJson("2025-01-01", "00:00:00", 42);
    const simba::Budget::Transaction txn{input};

    nlohmann::json j_out{};
    to_json(j_out, txn);

    EXPECT_EQ(j_out.at("amount").get<std::int64_t>(), 42);
    EXPECT_EQ(j_out.at("date").get<std::string>(), "2025-01-01");
    EXPECT_EQ(j_out.at("time").get<std::string>(), "00:00:00");
}

// --- File I/O ---

TEST_F(BudgetFileTest, FromFileValid) {
    const auto expected = MakeBudgetJson(500, nlohmann::json::array());
    {
        std::ofstream file{tmp_path_};
        file << expected;
    }

    const auto budget = simba::Budget::FromFile(tmp_path_.string());
    nlohmann::json output{};
    budget.ToJson(output);

    EXPECT_EQ(output.at("cents").get<std::size_t>(), 500u);
    EXPECT_TRUE(output.at("transaction_history").empty());
}

TEST_F(BudgetFileTest, FromFileInvalid) {
    const auto budget =
        simba::Budget::FromFile("/tmp/simba_nonexistent_file_xyz.json");

    nlohmann::json output{};
    budget.ToJson(output);

    EXPECT_EQ(output.at("cents").get<std::size_t>(), 0u);
    EXPECT_TRUE(output.at("transaction_history").empty());
}

TEST_F(BudgetFileTest, FromFileMalformedJson) {
    {
        std::ofstream file{tmp_path_};
        file << "{ not valid json !!!";
    }

    const auto budget = simba::Budget::FromFile(tmp_path_.string());
    nlohmann::json output{};
    budget.ToJson(output);

    EXPECT_EQ(output.at("cents").get<std::size_t>(), 0u);
    EXPECT_TRUE(output.at("transaction_history").empty());
}

TEST_F(BudgetFileTest, SaveAndLoadRoundTrip) {
    auto transactions = nlohmann::json::array();
    transactions.push_back(MakeTransactionJson("2025-06-01", "12:00:00", 1000));

    const auto input = MakeBudgetJson(7500, transactions);
    simba::Budget original{input};
    original.SaveToFile(tmp_path_.string());

    const auto loaded = simba::Budget::FromFile(tmp_path_.string());

    nlohmann::json j_original{};
    nlohmann::json j_loaded{};
    original.ToJson(j_original);
    loaded.ToJson(j_loaded);

    EXPECT_EQ(j_original, j_loaded);
}

TEST_F(BudgetFileTest, SaveAndLoadRoundTripEmpty) {
    simba::Budget original{};
    original.SaveToFile(tmp_path_.string());

    const auto loaded = simba::Budget::FromFile(tmp_path_.string());

    nlohmann::json j_original{};
    nlohmann::json j_loaded{};
    original.ToJson(j_original);
    loaded.ToJson(j_loaded);

    EXPECT_EQ(j_original, j_loaded);
}

}  // namespace
