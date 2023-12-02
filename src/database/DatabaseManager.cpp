#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>

class DatabaseManager {
public:
    // Constructor: Initializes the database and creates a table if it doesn't exist
    DatabaseManager(const std::string& db_name) : dbName(db_name), db(db_name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
        createTable();
    }

    // Method to create a table
    void createTable() {
        std::string createTableQuery = 
            "CREATE TABLE IF NOT EXISTS cards ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT NOT NULL, "
            "mana_cost TEXT, "
            "card_type TEXT, "
            "card_set TEXT, "
            "rarity TEXT, "
            "power INTEGER, "
            "toughness INTEGER, "
            "artist TEXT, "
            "quantity INTEGER DEFAULT 0"
            ");";
        db.exec(createTableQuery);
    }

    // Method to add or update a card in the database
    void addCard(int id, const std::string& name, const std::string& mana_cost, const std::string& card_type, 
                         const std::string& card_set, const std::string& rarity, int power, int toughness, 
                         const std::string& artist) {
        // Check if the card already exists
        SQLite::Statement queryCheck(db, "SELECT quantity FROM cards WHERE id = ?");
        queryCheck.bind(1, id);
        int quantity = 0;
        if (queryCheck.executeStep()) {
            quantity = queryCheck.getColumn(0).getInt();
            // Update the existing card
            SQLite::Statement queryUpdate(db, "UPDATE cards SET quantity = ? WHERE id = ?");
            queryUpdate.bind(1, quantity + 1);
            queryUpdate.bind(2, id);
            queryUpdate.exec();
        } else {
            // Insert a new card
            SQLite::Statement queryInsert(db, "INSERT INTO cards (id, name, mana_cost, card_type, card_set, rarity, power, toughness, artist, quantity) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, 1)");
            queryInsert.bind(1, id);
            queryInsert.bind(2, name);
            queryInsert.bind(3, mana_cost);
            queryInsert.bind(4, card_type);
            queryInsert.bind(5, card_set);
            queryInsert.bind(6, rarity);
            queryInsert.bind(7, power);
            queryInsert.bind(8, toughness);
            queryInsert.bind(9, artist);
            queryInsert.exec();
        }
    }

    // Method to delete a card from the database
    void deleteCard(int card_id) {
        SQLite::Statement query(db, "DELETE FROM cards WHERE id = ?");
        query.bind(1, card_id);
        query.exec();
    }

private:
    std::string dbName;
    SQLite::Database db;
};

/*
Example code: 
int main() {
    try {
        DatabaseManager dbManager("mtg_collection.db");

        // Add or update a card
        dbManager.addCard(1, "Black Lotus", "0", "Artifact", "Alpha", "Rare", 0, 0, "Christopher Rush");
        // Closing the database handled by SQLite::Database destructor
    } catch (const std::exception& e) {
        std::cerr << "SQLite exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
*/

