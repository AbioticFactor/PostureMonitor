#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include <nlohmann/json.hpp>

class DatabaseManager {
public:
    struct CardInfo {
        std::string name;
        int mana_cost;
        std::string color;
        std::string card_type;
        std::string rarity;
        std::string imagePath;
    };

    DatabaseManager(const char* db_name);
    void createTable();
    void bulkUpdate();
    void findAndAddMostSimilarCard(const std::string& inputCardName, const std::string& imagePath);

    void addCard(const std::string& name, int mana_cost, 
                                const std::string& color, const std::string& card_type, 
                                const std::string& rarity, const std::string& image_path, 
                                bool isUserCollection); 

    void deleteCard(int card_id);
    std::vector<CardInfo> searchCards(const std::vector<std::string>& rarities,
                                                                  const std::vector<std::string>& types, 
                                                                  const std::vector<int>& manaCosts, 
                                                                  const std::vector<std::string>& colors,
                                                                  bool isUserCollection);
    CardInfo getCardDetails(const std::string& cardName);
    std::vector<CardInfo> getAllCards(bool isUserCollection);


private:
    std::string dbName;
    SQLite::Database db;

    std::string join(const std::vector<std::string>& elements, const std::string& delimiter);
    int levenshteinDistance(const std::string &s1, const std::string &s2);


};


