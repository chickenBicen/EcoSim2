#pragma once
#ifndef NPC_H
#define NPC_H
#include "Business.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * *@class Npc
 * * @brief Class representing a Non-Player Character (NPC) in the game.
 * * The NPC can own businesses, buy and sell stocks, and manage a balance.
 * * The NPC can also buy products from businesses and has a savings account with an interest rate.
 */
class Npc {
public:
    Npc(std::string name);
    ~Npc();

    /**
     * * @brief Set the name of the NPC.
     * * @param name The name to set for the NPC.
     */
    void setName(const std::string& name);
    std::string name() const;

    /**
     * * @brief Get the score of the NPC.
     * * @return The score of the NPC.
     */
    int score() const;

    /**
     * * @brief Get the balance of the NPC.
     * * @return The balance of the NPC.
     */
    double balance() const;

    /**
     * * @brief Get the savings account balance of the NPC.
     * * @return The savings account balance of the NPC.
     */
    double savingsAccount() const; // Get the savings account balance
    /**
     * @brief Buy stocks from a business.
     * * @param business The business from which to buy stocks.
     * * @param amount The amount of stocks to buy.
     * * @return The amount of stocks bought.
     */
    int buyStock(Business* business, int amount);

    /**
     * @brief Sell stocks of a business.
     * * @param business The business from which to sell stocks.
     * * @param amount The amount of stocks to sell.
     * * @return The amount of stocks sold.
     */
    int sellStock(Business* business, int amount);

    /**
     * @brief Buy a product from a business.
     * * @param business The business from which to buy the product.
     * * @param product The product to buy.
     * * @param amount The amount of the product to buy.
     * * @return The amount of the product bought.
     * * @note If the product is not available, it returns 0.
     * @note If the business does not have enough supply, it returns -1.
     * @note If the NPC does not have enough balance, it returns -2.
     */
    double buy(Business& business, std::string& product, int amount); // Buy a product from a business

    /**
     * @brief Get the names of all businesses owned by the NPC.
     * * @return A vector of strings containing the names of the businesses.
     */
    std::vector<std::string> getBusinessNames() const;
    /**
     * @brief Get a business by its name.
     * * @param name The name of the business to get.
     * * @return A pointer to the business if found, nullptr otherwise.
     */
    Business* getBusiness(const std::string& name) const; // Get business by name
    /**
     * @brief Create a new business and add it to the NPC's list of businesses.
     * * @param name The name of the business to create.
     * * @note This function creates a new business and adds it to the NPC's list of businesses.
     */
    Business* createBusiness(std::string& name); // Add a business to the Npc

    void update(std::vector<Business*>& ActiveBusinesses); // update cycle for the Npc

private:
    std::string name_;
    double balance_;
    int score_;
    double savingsAccount_;     // Savings account balance
    const double INTEREST_RATE; // Interest rate for the savings account

    std::vector<Business*> ownedBusinesses_;
    std::unordered_map<Business*, double> stocks_;         // Business name to amount of stocks
    std::unordered_map<Business*, double> buyStockPrices_; // price of stocks at the time of purchase

    void setBalance(int balance);
};
#endif // Npc_H
