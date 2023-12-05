#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>




// using json = nlohmann::json;

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

    DatabaseManager(const std::string& db_name) : dbName(db_name), db(db_name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
        createTable();
    }

    void createTable() {
        std::string createTableQuery = 
            "CREATE TABLE IF NOT EXISTS cards ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT NOT NULL, "
            "mana_cost INTEGER, "
            "color TEXT, "
            "card_type TEXT, "
            "card_set TEXT, "
            "rarity TEXT, "
            "power INTEGER, "
            "toughness INTEGER, "
            "artist TEXT, "
            "quantity INTEGER DEFAULT 0, "
            "image_path TEXT"
            ");";
        db.exec(createTableQuery);
    }

    void bulkUpdate() {
        
    }


    void addCard(int id, const std::string& name, int mana_cost, const std::string& color, 
                 const std::string& card_type, const std::string& card_set, const std::string& rarity, 
                 int power, int toughness, const std::string& artist, const std::string& image_path) {
        SQLite::Statement queryCheck(db, "SELECT quantity FROM cards WHERE id = ?");
        queryCheck.bind(1, id);
        int quantity = 0;
        if (queryCheck.executeStep()) {
            quantity = queryCheck.getColumn(0).getInt();
            SQLite::Statement queryUpdate(db, "UPDATE cards SET quantity = ?, image_path = ? WHERE id = ?");
            queryUpdate.bind(1, quantity + 1);
            queryUpdate.bind(2, image_path);
            queryUpdate.bind(3, id);
            queryUpdate.exec();
        } else {
            SQLite::Statement queryInsert(db, "INSERT INTO cards (id, name, mana_cost, color, card_type, card_set, rarity, power, toughness, artist, quantity, image_path) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 1, ?)");
            queryInsert.bind(1, id);
            queryInsert.bind(2, name);
            queryInsert.bind(3, mana_cost);
            queryInsert.bind(4, color);
            queryInsert.bind(5, card_type);
            queryInsert.bind(6, card_set);
            queryInsert.bind(7, rarity);
            queryInsert.bind(8, power);
            queryInsert.bind(9, toughness);
            queryInsert.bind(10, artist);
            queryInsert.bind(11, image_path); // Bind the image path
            queryInsert.exec();
        }
    }


    void deleteCard(int card_id) {
        // First, check the current quantity of the card
        SQLite::Statement queryCheck(db, "SELECT quantity FROM cards WHERE id = ?");
        queryCheck.bind(1, card_id);

        if (queryCheck.executeStep()) {
            int quantity = queryCheck.getColumn(0).getInt();

            if (quantity > 1) {
                // If more than one, reduce the quantity by one
                SQLite::Statement queryUpdate(db, "UPDATE cards SET quantity = ? WHERE id = ?");
                queryUpdate.bind(1, quantity - 1);
                queryUpdate.bind(2, card_id);
                queryUpdate.exec();
            } else {
                // If exactly one, delete the card
                SQLite::Statement queryDelete(db, "DELETE FROM cards WHERE id = ?");
                queryDelete.bind(1, card_id);
                queryDelete.exec();
            }
        } else {
            std::cerr << "Card not found with ID: " << card_id << std::endl;
        }
    }



    std::vector<CardInfo> searchCards(const std::string& keywords, 
                                      const std::vector<std::string>& rarities,
                                      const std::vector<std::string>& types, 
                                      const std::vector<int>& manaCosts, 
                                      const std::vector<std::string>& colors) {
        std::vector<CardInfo> cards;
        std::string query = "SELECT id, name, mana_cost, color, card_type, card_set, rarity, power, toughness, artist, quantity, image_path FROM cards WHERE 1 = 1";

        // Add keyword filter
        if (!keywords.empty()) {
            query += " AND name LIKE '%" + keywords + "%'";
        }

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
            CardInfo card;
            card.id = queryExec.getColumn(0).getInt();
            card.name = queryExec.getColumn(1).getString();
            card.mana_cost = queryExec.getColumn(2).getInt();
            card.color = queryExec.getColumn(3).getString();
            card.card_type = queryExec.getColumn(4).getString();
            card.card_set = queryExec.getColumn(5).getString();
            card.rarity = queryExec.getColumn(6).getString();
            card.power = queryExec.getColumn(7).getInt();
            card.toughness = queryExec.getColumn(8).getInt();
            card.artist = queryExec.getColumn(9).getString();
            card.quantity = queryExec.getColumn(10).getInt();
            card.imagePath = queryExec.getColumn(11).getString();
            cards.push_back(card);
        }
        return cards;
    }



private:
    std::string dbName;
    SQLite::Database db;

    std::string join(const std::vector<std::string>& elements, const std::string& delimiter) {
        std::string result;
        for (size_t i = 0; i < elements.size(); ++i) {
            result += elements[i];
            if (i < elements.size() - 1) {
                result += delimiter;
            }
        }
        return result;
    }
};

/*
Example code: 
int main() {
    try {
        DatabaseManager dbManager("mtg_collection.db");

        // Example usage
        dbManager.addCard(1, "Black Lotus", 0, "Artifact", "Alpha", "Rare", 0, 0, "Christopher Rush");
        
        // Example filter usage
        dbManager.filterCards("Lotus", "Rare", -1, "", "");

        // Note: Closing the database is handled by SQLite::Database destructor
    } catch (const std::exception& e) {
        std::cerr << "SQLite exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
*/

