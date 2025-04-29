#include "Npc.h"
#include <iostream>

#include <cmath>
#include <random>

Npc::Npc(std::string name) : balance_(0), score_(0), name_(name), INTEREST_RATE(0.05) {
    // Initialize the Npc with a name and default values for balance and score
    balance_ = 10000;    // Set initial balance to 10,000
    score_ = 0;          // Set initial score to 0
    savingsAccount_ = 0; // Initialize savings account to 0
}

Npc::~Npc() {
    // Destructor
    // Clean up any resources if needed
    for (auto& business : ownedBusinesses_) {
        delete business; // Delete the business object
    }
    ownedBusinesses_.clear(); // Clear the vector of owned businesses
}

void Npc::setName(const std::string& name) {
    this->name_ = name;
}

std::string Npc::name() const {
    return name_;
}

int Npc::score() const {
    return score_;
}

int Npc::buyStock(Business* business, int amount) {
    double stockPrice = business->stockPrice();
    if (amount * stockPrice > balance_) {
        std::cout << "Not enough balance to buy " << amount << " stocks of " << business->name() << std::endl;
        return -1; // Not enough balance
    }
    if (stocks_.find(business) == stocks_.end()) {
        stocks_[business] = amount; // Initialize if not already present
    }
    // Create a copy of the business
    stocks_[business] += amount; // Increase the amount of stocks owned

    buyStockPrices_[business] = stockPrice; // Store the price at which the stock was bought

    balance_ -= amount * stockPrice;

    score_ += (int)(2 * (sqrt(stockPrice) * log2(amount)));
    std::cout << "Bought " << amount << " stocks of " << business->name() << std::endl;
    return amount; // Amount of stocks bought
}

int Npc::sellStock(Business* business, int amount) {
    double stockPrice = business->stockPrice();
    if (stocks_.find(business) == stocks_.end()) {
        std::cout << "You don't own any stocks of " << business->name() << std::endl;
        return 0; // No stocks to sell
    }
    if (stocks_[business] < amount) {
        std::cout << "You don't own enough stocks of " << business->name() << std::endl;
        return 0; // Not enough stocks to sell
    }
    stocks_[business] -= amount;
    balance_ += amount * stockPrice;

    score_ += (int)(2 * (sqrt(stockPrice) * log2(amount)));

    std::cout << "Sold " << amount << " stocks of " << business->name() << std::endl;

    if (business->stockPrice() < business->initialStockPrice()) {
        std::cout << "Stock price of " << business->name() << " has decreased." << std::endl;
    }

    if (stocks_[business] == 0) {
        stocks_.erase(business); // Remove business from stocks if no stocks left
        std::cout << "No more stocks of " << business->name() << " left." << std::endl;
    }

    return amount; // Amount of stocks sold
}

double Npc::buy(Business& business, std::string& product, int amount) {
    if (business.supply(product) == 0) {
        std::cout << "Product not available in the business." << std::endl;
        return -1; // Product not available
    }

    if (business.supply(product) < amount) {
        std::cout << "Not enough supply " << product << " in the business." << std::endl;
        return 0; // Not enough supply
    }

    if (amount * business.supply(product) > balance_) {
        std::cout << "Not enough balance to buy " << amount << " of " << product << std::endl;
        return -2; // Not enough balance
    }

    balance_ -= business.supply(product) * amount; // Deduct the cost from the Npc's balance

    business.setSupply(product, business.supply(product) - amount); // Reduce the supply in the business
    business.addBalance(amount * business.supply(product));         // Add the cost to the business balance

    std::cout << "Bought " << amount << " of " << product << " from " << business.name() << std::endl;

    score_ += amount * business.supply(product); // Increase the score by the amount of product
    return amount * business.supply(product);    // Return the total cost of the purchase
}

void Npc::setBalance(int balance) {
    this->balance_ = balance;
}

double Npc::balance() const {
    return balance_;
} // Get the current balance of the Npc

double Npc::savingsAccount() const {
    return savingsAccount_; // Get the savings account balance
}

std::vector<std::string> Npc::getBusinessNames() const {
    std::vector<std::string> businessNames;

    for (const auto& business : ownedBusinesses_) {
        businessNames.push_back(business->name());
    }

    return businessNames;
}

Business* Npc::getBusiness(const std::string& name) const {
    for (const auto business : ownedBusinesses_) {
        if (business->name() == name) {
            return business; // Return the business if found
        }
    }
    return nullptr; // Business not found
}

Business* Npc::createBusiness(std::string& name) {
    if (!(balance_ > 10000)) {
        std::cout << "Not enough balance to create a business." << std::endl;
        // Return the first business if not enough balance
    }
    Business* business = new Business(name); // Create a new business object
    ownedBusinesses_.push_back(business);    // Add to the Npc's list of businesses
    Business& returnBus = *business;
    // Return the created business
    return &returnBus;
}

void Npc::update(std::vector<Business*>& businesses) {
    // Update cycle for the Npc
    // This function can be used to update the Npc's state, such as balance, score, etc.

    savingsAccount_ += savingsAccount_ * INTEREST_RATE; // Update savings account with interest

    std::random_device rd;                          // Random device for generating random numbers
    std::mt19937 gen(rd());                         // Mersenne Twister random number generator
    std::uniform_real_distribution<> dis(0.0, 1.0); // Uniform distribution between 0 and 1

    for (auto it = stocks_.begin(); it != stocks_.end(); ++it) {
        double roll = dis(gen);                           // Generate a random factor
        double stockPrice = it->first->stockPrice();      // Get the stock price of the business
        double initialPrice = buyStockPrices_[it->first]; // Get the initial price of the stock

        /**
         * @brief rolls for a chance to sell stocks
         * only sells if the stock price is higher than the initial price
         * then rolls for a random amount to sell
         */
        if (stockPrice > initialPrice && roll > 0.5) {
            int rollAmount = static_cast<int>(it->second * roll); // Calculate the amount to sell
            if (rollAmount > 0) {
                sellStock(it->first, rollAmount); // Sell the stocks based on the random factor
            }
        }
    }

    for (auto& business : businesses) {
        double roll = dis(gen);                                 // Generate a random factor
        if (roll > 0.5) {                                       // 50% chance to buy stocks
            int amount = static_cast<int>(dis(gen) * balance_ / business->stockPrice()); // Random amount to buy (0-10)
            buyStock(business, amount);                         // Buy the stocks based on the random factor
        }
    }

    double roll = dis(gen); // Generate a random factor
    // 50% chance to buy products from businesses
    if (roll > 0.5) {
        for (auto& business : businesses) {
            int product_num = static_cast<int>(dis(gen) * business->productNames().size()); // Random product number
            int amount = static_cast<int>(dis(gen) * 10);                               // Random amount to buy (0-10)

            std::string product = business->productNames()[product_num]; // Get the product name

            buy(*business, product, amount); // Buy the product from the business
        }
    }

    if (roll > 0.7) { // 30% chance to create a new business
        std::string newBusinessName =
            "Business" + std::to_string(ownedBusinesses_.size() + 1); // Generate a new business name
        createBusiness(newBusinessName);                              // Create a new business

        businesses.push_back(getBusiness(newBusinessName)); // Add the new business to the list of businesses
    }

    if (roll > 0.9) { // 20% chance to sell a business
        if (!ownedBusinesses_.empty()) {
            int business_num = static_cast<int>(dis(gen) * ownedBusinesses_.size()); // Random business number
            Business* business = ownedBusinesses_[business_num];                     // Get the business
            ownedBusinesses_.erase(ownedBusinesses_.begin() + business_num); // Remove the business from the 
        }
    }
}