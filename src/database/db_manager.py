import sqlite3

class DatabaseManager:
    # create table on initlization
    def __init__(self, db_name):
        self.name = db_name
        self.conn = sqlite3.connect(db_name)
        self.cursor = self.conn.cursor()
        self.create_table()

    # create db table
    def create_table(self):
        create_table_query = '''
        CREATE TABLE IF NOT EXISTS cards (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            mana_cost TEXT,
            card_type TEXT,
            card_set TEXT,
            rarity TEXT,
            power INTEGER,
            toughness INTEGER,
            artist TEXT
        );
        '''
        self.cursor.execute(create_table_query)
        self.conn.commit()

    def add_card(self, card_data):
        self.cursor.execute('''
        INSERT INTO cards (name, mana_cost, card_type, card_set, rarity, power, toughness, artist)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        ''', card_data)
        self.conn.commit()

    def delete_card(self, card_id):
        self.cursor.execute('''
        DELETE FROM cards WHERE id = ?
        ''', (card_id,))
        self.conn.commit()

    def close(self):
        self.conn.close()


# Example on How to use:

# db = DatabaseManager('mtg_collection.db')
# db.add_card(("Black Lotus", "0", "Artifact", "Alpha", "Rare", None, None, "Christopher Rush"))
# db.close()
