#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>
#include <database/DatabaseManager.hpp>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <QtConcurrent/QtConcurrent>


DatabaseManager::DatabaseManager(const char* db_name) : dbName(db_name), db(dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
    createTable();
}

void DatabaseManager::createTable() {
    SQLite::Database db(dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    std::string createTableQuery_ref = 
    "CREATE TABLE IF NOT EXISTS reference_cards ("
    "name TEXT NOT NULL, "
    "mana_cost INTEGER, "
    "color TEXT, "
    "card_type TEXT, "
    "rarity TEXT, "
    "image_path TEXT"
    ");";

    std::string createTableQuery_user = 
    "CREATE TABLE IF NOT EXISTS user_cards ("
    "name TEXT NOT NULL, "
    "mana_cost INTEGER, "
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
    std::string insertQueryRef = "INSERT INTO reference_cards (name, mana_cost, color, card_type, rarity, image_path) VALUES (?, ?, ?, ?, ?, ?)";
    SQLite::Statement queryInsertRef(db, insertQueryRef);
    queryInsertRef.bind(1, name);
    queryInsertRef.bind(2, mana_cost);
    queryInsertRef.bind(3, color);
    queryInsertRef.bind(4, card_type);
    queryInsertRef.bind(5, rarity);
    queryInsertRef.bind(6, image_path);
    queryInsertRef.exec();

    // If isUserCollection is true, also insert into user_cards
    if (isUserCollection) {
        std::string insertQueryUser = "INSERT INTO user_cards (name, mana_cost, color, card_type, rarity, image_path) VALUES (?, ?, ?, ?, ?, ?)";
        SQLite::Statement queryInsertUser(db, insertQueryUser);
        queryInsertUser.bind(1, name);
        queryInsertUser.bind(2, mana_cost);
        queryInsertUser.bind(3, color);
        queryInsertUser.bind(4, card_type);
        queryInsertUser.bind(5, rarity);
        queryInsertUser.bind(6, image_path);
        queryInsertUser.exec();
    }
}



void DatabaseManager::deleteCard(int card_id) {

    return;
}



std::vector<DatabaseManager::CardInfo> DatabaseManager::searchCards(const std::vector<std::string>& rarities,
                                                                  const std::vector<std::string>& types, 
                                                                  const std::vector<int>& manaCosts, 
                                                                  const std::vector<std::string>& colors,
                                                                  bool isUserCollection) {
    std::vector<DatabaseManager::CardInfo> cards;

    std::string query = isUserCollection
        ? "SELECT name, mana_cost, color, card_type, rarity, image_path FROM user_cards WHERE "
        : "SELECT name, mana_cost, color, card_type, rarity, image_path FROM reference_cards WHERE ";

    std::vector<std::string> conditions;

    if (!rarities.empty()) {
        conditions.push_back("rarity IN ('" + join(rarities, "', '") + "')");
    }

    if (!types.empty()) {
        std::string typeCondition;
        for (size_t i = 0; i < types.size(); ++i) {
            if (i > 0) {
                typeCondition += " OR ";
            }
            typeCondition += "card_type LIKE '%" + types[i] + "%'";
        }
        conditions.push_back("(" + typeCondition + ")");
    }

    // Add mana cost filter
    if (!manaCosts.empty()) {
        std::string manaCostCondition = "mana_cost IN (";
        for (size_t i = 0; i < manaCosts.size(); ++i) {
            manaCostCondition += std::to_string(manaCosts[i]);
            if (i < manaCosts.size() - 1) {
                manaCostCondition += ", ";
            }
        }
        manaCostCondition += ")";
        conditions.push_back(manaCostCondition);
    }

    // Add color filter
    if (!colors.empty()) {
        conditions.push_back("color IN ('" + join(colors, "', '") + "')");
    }

    // Combine all conditions with AND logic
    if (!conditions.empty()) {
        query += join(conditions, " AND ");
    } else {
        query += "1 = 1"; // Default condition if no filters
    }
    

    SQLite::Statement queryExec(db, query);
    while (queryExec.executeStep()) {
        DatabaseManager::CardInfo card;
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

std::string DatabaseManager::mapColorAbbreviationToFullName(const std::string& abbreviation) {
    if (abbreviation == "W") return "White";
    if (abbreviation == "U") return "Blue";
    if (abbreviation == "B") return "Black";
    if (abbreviation == "R") return "Red";
    if (abbreviation == "G") return "Green";
    return "Unknown";
}

void DatabaseManager::fetchCard(const std::string& name) {
    if (name.empty()) {
        std::cout << "No card was scanned." << std::endl;
        return;
    }

    std::string processedName = name;
    std::replace(processedName.begin(), processedName.end(), ' ', '+');
    std::string url = "https://api.scryfall.com/cards/named?fuzzy=" + processedName;

    CURL* curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        std::string readBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                auto json = nlohmann::json::parse(readBuffer);
                if (json.contains("object") && json["object"] == "error") {
                    std::cout << "No card found for: " << name << std::endl;
                } else {
                    int cmc = json["cmc"];
                    std::string color;
                    if (json["colors"].size() == 0) {
                        color = "Colorless";
                    } else if (json["colors"].size() > 1) {
                        color = "Multicolored";
                    } else {
                        color = mapColorAbbreviationToFullName(json["colors"][0].get<std::string>());
                    }
                    std::string rarity = json["rarity"];
                    std::string imagePath = json["image_uris"]["normal"];

                    std::string savedImagePath;
                    downloadAndSaveImage(imagePath, savedImagePath);

                    std::cout << "CMC: " << cmc << "\nColor: " << color << "\nRarity: " << rarity << "\nImagePath: " << imagePath << std::endl;
                    std::string cardType = json["type_line"];

                    std::cout << "CONFIRMING THAT SAVEDIMAGEPATH IS" << savedImagePath << std::endl;

                    addCard(name, cmc, color, cardType, rarity, savedImagePath, true);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }
}

void DatabaseManager::downloadAndSaveImage(const std::string& imageUrl, std::string& savedImagePath) {
    CURL* curl = curl_easy_init();
    if(curl) {
        CURLcode res;
        std::string imageBuffer;

        // Set up the request for image download
        curl_easy_setopt(curl, CURLOPT_URL, imageUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "Failed to download image: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Generate a unique filename based on the current time
            auto now = std::chrono::system_clock::now();
            auto now_c = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&now_c), "%Y%m%d%H%M%S") << ".jpg";
            savedImagePath = "/home/pi/mtg-collection-manager/src/database/images" + ss.str(); // Modify this path as needed

            // Save the image data to a file
            std::ofstream file(savedImagePath, std::ios::out | std::ios::binary);
            if(file.is_open()) {
                file.write(imageBuffer.c_str(), imageBuffer.size());
                file.close();
                std::cout << "Image downloaded and saved to '" << savedImagePath << "'" << std::endl;
            } else {
                std::cerr << "Unable to open file for writing image data." << std::endl;
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
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

void DatabaseManager::fetchCardAsync(const std::string& name) {
    QtConcurrent::run([=]() {
        fetchCard(name);
    });
}
