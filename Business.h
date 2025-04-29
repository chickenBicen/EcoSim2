#pragma once
#pragma once
#ifndef BUSINESS_H
#define BUSINESS_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Business {
  public:
    explicit Business(std::string name, std::string description = {});

    ~Business() = default;                              // Destructor
    Business(const Business&) = default;                // Copy constructor
    Business& operator=(Business&&) noexcept = default; // Move constructor
    Business& operator=(const Business&) = default;     // Copy assignment operator

    int* getID() noexcept; // Get the ID of the business

    double stockPrice() const noexcept;
    std::string name() const noexcept;
    std::string description() const noexcept;
    const double initialStockPrice() const noexcept; // Get the initial stock price of the business

    void setDescription(const std::string& description) noexcept;
    void setName(const std::string& name) noexcept;

    const auto& supply() const;                             // Get supply of the business
    int supply(const std::string& product) const;           // Get supply of a specific product
    const auto& demand() const;                             // Get demand of the business
    const auto& resupplyRates() const;                      // Get resupply rates of the business
    const auto& InitialProductPrices() const;               // Get initial product prices of the business
    const auto& products() const;                           // Get products of the business
    void setSupply(const std::string& product, int amount); // Set supply of the business

    std::vector<std::string> productNames() const; // Get product names of the business
    std::vector<double> productPrices() const;     // Get product prices of the business

    void addBalance(double amount); // Add balance to the business

    void addProduct(const std::string& product, double price); // Add a product to the business
    void removeProduct(const std::string& product);            // Remove a product from the business

    void update(); // Update cycle for the business

  private:
    const double INITIAL_STOCK_PRICE = 100.0; // Initial stock price of the business
    double stockPrice_ = INITIAL_STOCK_PRICE; // Current stock price of the business
    std::string name_;
    std::string description_;
    int score_ = 0;            // Score of the business
    double balance_ = 10000.0; // Balance of the business
    double stockDemand_ = 1.0; // Demand for stocks of the business

    int ID;

    static std::unordered_set<int> usedIDs_; // Set of used IDs for businesses

    std::unordered_map<std::string, double> initialProductPrices_; // product prices of the business
    std::unordered_map<std::string, double> products_;             // Stock prices of the business
    std::unordered_map<std::string, int> supply_;                  // Supply of the business
    std::unordered_map<std::string, double> demand_;               // Demand of the business
    std::unordered_map<std::string, double> resupplyRates_;        // Stocks of the business
};

#endif // BUSINESS_H