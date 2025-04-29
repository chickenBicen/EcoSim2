#include "Business.h"
#include "Npc.h"

#include <iostream>
#include <random>
#include <string>
#include <vector>

int main() {
    std::random_device rd;                       // Random device for generating random numbers
    std::mt19937 gen(rd());                      // Mersenne Twister random number generator
    std::uniform_int_distribution<> dis(10, 60); // Uniform distribution for random numbers

    int length = dis(gen); // Generate a random length for the string

    std::vector<Business*> businesses; // Vector to store pointers to Business objects
    std::vector<Npc*> npcs;            // Vector to store pointers to Npc objects

    for (int i = 0; i < length; ++i) {
        // TODO: add core loop logic here
        Business* business = new Business("Business" + std::to_string(i));
        businesses.push_back(business); // Add the business to the vector
        Npc* npc = new Npc("Npc" + std::to_string(i));
        npcs.push_back(npc); // Add the npc to the vector

        for (auto& business : businesses) {
            business->update(); // Update the business
        }

        for (auto& npc : npcs) {
            npc->update(businesses); // Update the NPC with the list of businesses
        }
    }

    for (auto& business : businesses) {
        delete business; // Delete the business object
    }
    for (auto& npc : npcs) {
        delete npc; // Delete the npc object
    }
    return 0; // Return success
}
