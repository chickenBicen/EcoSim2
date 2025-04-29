#include "Business.h"
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Static member variable to keep track of used IDs
std::unordered_set<int> Business::usedIDs_ = {}; // Initialize the set of used IDs

Business::Business(std::string name, std::string description)
    : name_(name), description_(description), initialProductPrices_({}) {
    // Initialize the business with a name and description
    std::random_device rd;  // Random device for generating random numbers
    std::mt19937 gen(rd()); // Mersenne Twister random number generator
    std::uniform_int_distribution<> dis(100000000, 999999999);

    int newID;

    do {
        newID = dis(gen); // Generate a random ID for the business
    } while (usedIDs_.find(newID = dis(gen)) != usedIDs_.end()); // Generate a unique ID for the business

    usedIDs_.insert(newID); // Insert the new ID into the set of used IDs
    ID = newID;             // Set the ID of the business
}

int* Business::getID() noexcept {
    return &ID; // Return the ID of the business
}

double Business::stockPrice() const noexcept {
    return stockPrice_; // Return the current stock price of the business
}

const double Business::initialStockPrice() const noexcept {
    return INITIAL_STOCK_PRICE; // Return the initial stock price of the business
}

std::string Business::name() const noexcept {
    return name_; // Return the name of the business
}

std::string Business::description() const noexcept {
    return description_; // Return the description of the business
}

void Business::setDescription(const std::string& description) noexcept {
    description_ = description; // Set the description of the business
}

void Business::setName(const std::string& name) noexcept {
    name_ = name; // Set the name of the business
}

const auto& Business::supply() const {
    return supply_; // Return the supply of the business
}

int Business::supply(const std::string& product) const {
    auto it = supply_.find(product); // Find the product in the supply map
    if (it != supply_.end()) {
        return it->second; // Return the supply of the product if found
    }
    return 0; // Return 0 if the product is not found in the supply
}

void Business::setSupply(const std::string& product, int amount) {
    supply_[product] = amount; // Set the supply of the product in the business
}

const auto& Business::demand() const {
    return demand_; // Return the demand of the business
}

const auto& Business::resupplyRates() const {
    return resupplyRates_; // Return the resupply rates of the business
}

const auto& Business::InitialProductPrices() const {
    return initialProductPrices_; // Return the initial product prices of the
                                  // business
}

const auto& Business::products() const {
    return products_; // Return the products of the business
}

void Business::addBalance(double amount) {
    balance_ += amount; // Add the specified amount to the business balance
}

std::vector<std::string> Business::productNames() const {
    std::vector<std::string> names;
    for (const auto& product : products_) {
        names.push_back(product.first); // Add the product name to the vector
    }
    return names; // Return the vector of product names
}

std::vector<double> Business::productPrices() const {
    std::vector<double> prices;
    for (const auto& product : products_) {
        prices.push_back(product.second); // Add the product price to the vector
    }
    return prices; // Return the vector of product prices
}

void Business::addProduct(const std::string& product, double price) {
    if (products_.find(product) != products_.end()) {
        std::cout << "Product already exists in the business." << std::endl;
        return; // Product already exists, do not add it again
    }
    products_[product] = price;             // Add the product and its price to the business
    initialProductPrices_[product] = price; // Set the initial product price
    supply_[product] = 50;                  // Initialize the supply of the product to 50
    demand_[product] = 50;                  // Initialize the demand of the product to 50
    resupplyRates_[product] = 50;           // Initialize the resupply rate of the product to 50
}

void Business::removeProduct(const std::string& product) {
    products_.erase(product);             // Remove the product from the business
    initialProductPrices_.erase(product); // Remove the initial product price
    supply_.erase(product);               // Remove the supply of the product
    demand_.erase(product);               // Remove the demand of the product
    resupplyRates_.erase(product);        // Remove the resupply rate of the product
}

void Business::update() {
    // Update cycle for the business

    std::random_device rd;                      // Random device for generating random numbers
    std::mt19937 gen(rd());                     // Mersenne Twister random number generator
    std::uniform_real_distribution<> dis(0, 1); // Uniform distribution for random numbers

    double roll = dis(gen);                  // Generate a random factor
    double randomFactor = dis(gen) * 10 - 5; // Generate a random factor between -100 and 100

    // update the demand of the stock randomly
    if (roll > 0.2) {
        stockDemand_ += randomFactor * stockDemand_; // Update the stock demand with the random factor
    }

    // Update the stock price based on the demand
    stockPrice_ += stockDemand_; // Update the stock price with the demand
    if (stockDemand_ < 0) {
        stockDemand_ = 0.0;
    }

    // update the product demands
    for (auto& product : demand_) {
        product.second += randomFactor * product.second; // Update the product demand with the random factor
    }

    // update the product prices based on demand and supply
    for (auto it = products_.begin(); it != products_.end(); ++it) {
        auto demandIt = demand_.find(it->first); // Find the product in the demand map
        if (demandIt != demand_.end()) {
            double demand = demandIt->second;        // Get the demand of the product
            double supply = this->supply(it->first); // Get the supply of the product
            if (supply > 0) {
                it->second += (demand - supply) * 0.1; // Update the product price based on demand and supply
            }
        }
    }

    // resupply the products based on the resupply rates and random factor
    for (auto it = products_.begin(); it != products_.end(); ++it) {
        auto resupplyIt = resupplyRates_.find(it->first); // Find the product in the resupply rates map
        if (resupplyIt != resupplyRates_.end()) {
            double resupplyRate = resupplyIt->second;          // Get the resupply rate of the product
            supply_[it->first] += static_cast<int>(resupplyRate * randomFactor); // Update the supply of the product
        }
    }
}