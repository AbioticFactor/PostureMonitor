#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>
#include <database/DatabaseManager.hpp>
#include <nlohmann/json.hpp>



DatabaseManager::DatabaseManager(const char* db_name) : dbName(db_name), db(dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
    createTable();
}

void DatabaseManager::createTable() {
    SQLite::Database db(dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    std::string createTableQuery_ref = 
    "CREATE TABLE IF NOT EXISTS reference_cards ("
    "name TEXT NOT NULL, "
    "mana_cost TEXT, "
    "color TEXT, "
    "card_type TEXT, "
    "rarity TEXT, "
    "image_path TEXT"
    ");";

    std::string createTableQuery_user = 
    "CREATE TABLE IF NOT EXISTS user_cards ("
    "name TEXT NOT NULL, "
    "mana_cost TEXT, "
    "color TEXT, "
    "card_type TEXT, "
    "rarity TEXT, "
    "image_path TEXT"
    ");";

    db.exec(createTableQuery_ref);
    db.exec(createTableQuery_user);

}

#include <fstream>
#include <nlohmann/json.hpp>

void DatabaseManager::bulkUpdate() {
    // std::ifstream inFile("/home/pi/mtg-collection-manager/src/database/oracle-cards-20231207100139.json");
    // nlohmann::json jsonData = nlohmann::json::parse(inFile);

    // for (const auto& cardJson : jsonData) {
    //     int id = cardJson["id"]; // Adjust the field names based on your JSON structure
    //     std::string name = cardJson["name"];
    //     std::string mana_cost = cardJson["mana_cost"];
    //     std::string color = cardJson["color"];
    //     std::string card_type = cardJson["type"];
    //     std::string card_set = cardJson["set"];
    //     std::string rarity = cardJson["rarity"];
    //     // Image path is set to null
    //     std::string image_path = "null"; 
    //     addCard(name, mana_cost, color, card_type, rarity, image_path);
    //     std::cout << id << std::endl;
    // }
}


void DatabaseManager::addCard(const std::string& name, int mana_cost, 
                              const std::string& color, const std::string& card_type, 
                              const std::string& rarity, const std::string& image_path, 
                              bool isUserCollection) {
    std::string insertQuery = isUserCollection
        ? "INSERT INTO user_cards (name, mana_cost, color, card_type, rarity, image_path) VALUES (?, ?, ?, ?, ?, ?)"
        : "INSERT INTO reference_cards (name, mana_cost, color, card_type, rarity, image_path) VALUES (?, ?, ?, ?, ?, ?)";

    SQLite::Statement queryInsert(db, insertQuery);
    queryInsert.bind(1, name);
    queryInsert.bind(2, mana_cost);
    queryInsert.bind(3, color);
    queryInsert.bind(4, card_type);
    queryInsert.bind(5, rarity);
    queryInsert.bind(6, image_path);
    queryInsert.exec();
}



void DatabaseManager::deleteCard(int card_id) {
    // SQLite::Database db(dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    // // First, check the current quantity of the card
    // SQLite::Statement queryCheck(db, "SELECT quantity FROM cards WHERE id = ?");
    // queryCheck.bind(1, card_id);

    // if (queryCheck.executeStep()) {
    //     int quantity = queryCheck.getColumn(0).getInt();

    //     if (quantity > 1) {
    //         // If more than one, reduce the quantity by one
    //         SQLite::Statement queryUpdate(db, "UPDATE cards SET quantity = ? WHERE id = ?");
    //         queryUpdate.bind(1, quantity - 1);
    //         queryUpdate.bind(2, card_id);
    //         queryUpdate.exec();
    //     } else {
    //         // If exactly one, delete the card
    //         SQLite::Statement queryDelete(db, "DELETE FROM cards WHERE id = ?");
    //         queryDelete.bind(1, card_id);
    //         queryDelete.exec();
    //     }
    // } else {
    //     std::cerr << "Card not found with ID: " << card_id << std::endl;
    // }
    return;
    
}



std::vector<DatabaseManager::CardInfo> DatabaseManager::searchCards(const std::vector<std::string>& rarities,
                                                                  const std::vector<std::string>& types, 
                                                                  const std::vector<int>& manaCosts, 
                                                                  const std::vector<std::string>& colors,
                                                                  bool isUserCollection) {
    std::vector<DatabaseManager::CardInfo> cards;

    std::string query = isUserCollection
        ? "SELECT mana_cost, color, card_type, rarity, image_path FROM user_cards WHERE 1 = 1"
        : "SELECT mana_cost, color, card_type, rarity, image_path FROM reference_cards WHERE 1 = 1";


    // Add rarity filter
    if (!rarities.empty()) {
        query += " AND rarity IN ('" + join(rarities, "', '") + "')";
    }

    // Add type filter
    if (!types.empty()) {
        query += " AND card_type IN ('" + join(types, "', '") + "')";
    }

    // Add mana cost filter
    if (!manaCosts.empty()) {
        query += " AND mana_cost IN (";
        for (size_t i = 0; i < manaCosts.size(); ++i) {
            query += std::to_string(manaCosts[i]);
            if (i < manaCosts.size() - 1) {
                query += ", ";
            }
        }
        query += ")";
    }

    // Add color filter
    if (!colors.empty()) {
        query += " AND color IN ('" + join(colors, "', '") + "')";
    }

    SQLite::Statement queryExec(db, query);
    while (queryExec.executeStep()) {
        DatabaseManager::CardInfo card;
        card.name = queryExec.getColumn(0).getString();
        card.mana_cost = queryExec.getColumn(1).getInt(); // Updated to getInt
        card.color = queryExec.getColumn(2).getString();
        card.card_type = queryExec.getColumn(3).getString();
        card.rarity = queryExec.getColumn(4).getString();
        card.imagePath = queryExec.getColumn(5).getString();
        cards.push_back(card);
    }
    return cards;
}


// we added two more functions

std::string DatabaseManager::join(const std::vector<std::string>& elements, const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result += elements[i];
        if (i < elements.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

void DatabaseManager::findAndAddMostSimilarCard(const std::string& inputCardName, const std::string& imagePath) {
    std::vector<std::string> cardNames;
    SQLite::Statement query(db, "SELECT name FROM reference_cards");

    int minDifference = std::numeric_limits<int>::max();
    std::string mostSimilarCardName;

    while (query.executeStep()) {
        std::string cardName = query.getColumn(0).getString();
        int difference = levenshteinDistance(inputCardName, cardName);

        if (difference < minDifference) {
            minDifference = difference;
            mostSimilarCardName = cardName;
        }
    }

    if (!mostSimilarCardName.empty()) {
        // Retrieve full details of the most similar card
        // Assuming getCardDetails function exists
        CardInfo cardDetails = getCardDetails(mostSimilarCardName);
        addCard(cardDetails.name, cardDetails.mana_cost, cardDetails.color, 
                cardDetails.card_type, cardDetails.rarity, cardDetails.imagePath, true);
    }
}

DatabaseManager::CardInfo DatabaseManager::getCardDetails(const std::string& cardName) {
    SQLite::Database db(dbName, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT name, mana_cost, color, card_type, rarity, image_path FROM reference_cards WHERE name = ?");
    query.bind(1, cardName);

    DatabaseManager::CardInfo cardDetails;
    if (query.executeStep()) {
        cardDetails.name = query.getColumn(0).getString();
        cardDetails.mana_cost = query.getColumn(1).getInt();
        cardDetails.color = query.getColumn(2).getString();
        cardDetails.card_type = query.getColumn(3).getString();
        cardDetails.rarity = query.getColumn(4).getString();
        cardDetails.imagePath = query.getColumn(5).getString();
    }

    return cardDetails;
}

// Function to calculate the Levenshtein distance
int DatabaseManager::levenshteinDistance(const std::string &s1, const std::string &s2) {
    const size_t len1 = s1.size(), len2 = s2.size();
    std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));

    d[0][0] = 0;
    for(int i = 1; i <= len1; ++i) d[i][0] = i;
    for(int j = 1; j <= len2; ++j) d[0][j] = j;

    for(int i = 1; i <= len1; ++i)
        for(int j = 1; j <= len2; ++j)
            d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) });

    return d[len1][len2];
}

std::vector<DatabaseManager::CardInfo> DatabaseManager::getAllCards(bool isUserCollection) {
    std::vector<CardInfo> cards;
    
    // Choose the table based on whether it's a user collection or reference cards
    std::string table = isUserCollection ? "user_cards" : "reference_cards";
    
    // SQL query to select all cards
    std::string query = "SELECT name, mana_cost, color, card_type, rarity, image_path FROM " + table;

    SQLite::Statement queryExec(db, query);
    while (queryExec.executeStep()) {
        CardInfo card;
        card.name = queryExec.getColumn(0).getString();
        card.mana_cost = queryExec.getColumn(1).getInt();
        card.color = queryExec.getColumn(2).getString();
        card.card_type = queryExec.getColumn(3).getString();
        card.rarity = queryExec.getColumn(4).getString();
        card.imagePath = queryExec.getColumn(5).getString();
        cards.push_back(card);
    }

    return cards;
}

