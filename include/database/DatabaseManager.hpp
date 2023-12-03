#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>
#include <vector>

class DatabaseManager {
public:
    DatabaseManager(const std::string& db_name);
    void createTable();
    void addCard(int id, const std::string& name, int mana_cost, const std::string& color, 
                 const std::string& card_type, const std::string& card_set, const std::string& rarity, 
                 int power, int toughness, const std::string& artist, const std::string& image_path);
    void deleteCard(int card_id);
    void searchCards(const std::string& keywords, const std::vector<std::string>& rarities,
                     const std::vector<std::string>& types, std::vector<int>& manaCosts, 
                     const std::vector<std::string>& colors);

private:
    std::string dbName;
    SQLite::Database db;
    std::string join(const std::vector<std::string>& elements, const std::string& delimiter);
};
