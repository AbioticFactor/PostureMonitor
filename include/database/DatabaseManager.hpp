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

    void addCard(const std::string& name, int mana_cost, 
                                const std::string& color, const std::string& card_type, 
                                const std::string& rarity, const std::string& image_path, 
                                bool isUserCollection); 

    void fetchCard(const std::string& name);

    void deleteCard(int card_id);
    std::vector<CardInfo> searchCards(const std::vector<std::string>& rarities,
                                                                  const std::vector<std::string>& types, 
                                                                  const std::vector<int>& manaCosts, 
                                                                  const std::vector<std::string>& colors,
                                                                  bool isUserCollection);
    CardInfo getCardDetails(const std::string& cardName);
    std::vector<CardInfo> getAllCards(bool isUserCollection);
    void downloadAndSaveImage(const std::string& imageUrl, std::string& savedImagePath);



private:
    std::string dbName;
    SQLite::Database db;

    std::string join(const std::vector<std::string>& elements, const std::string& delimiter);
    int levenshteinDistance(const std::string &s1, const std::string &s2);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
        size_t newLength = size * nmemb;
        try {
            s->append((char*)contents, newLength);
        } catch(std::bad_alloc &e) {
            // Handle memory problem
            return 0;
        }
        return newLength;
    };

    std::string mapColorAbbreviationToFullName(const std::string& abbreviation);



};


