#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>



class DatabaseManager {
public:

    struct CardInfo {
        int id;
        std::string name;
        int mana_cost;
        std::string color;
        std::string card_type;
        std::string card_set;
        std::string rarity;
        int power;
        int toughness;
        std::string artist;
        int quantity;
        std::string imagePath;
    };

    DatabaseManager(const std::string& db_name);
    void createTable();
    void addCard(int id, const std::string& name, int mana_cost, const std::string& color, 
                 const std::string& card_type, const std::string& card_set, const std::string& rarity, 
                 int power, int toughness, const std::string& artist, const std::string& image_path);
    void deleteCard(int card_id);


    void bulkUpdate();
    std::vector<CardInfo> searchCards(const std::string& keywords, 
                                      const std::vector<std::string>& rarities,
                                      const std::vector<std::string>& types, 
                                      const std::vector<int>& manaCosts, 
                                      const std::vector<std::string>& colors);

private:
    std::string dbName;
    SQLite::Database db;
    std::string join(const std::vector<std::string>& elements, const std::string& delimiter);
};
