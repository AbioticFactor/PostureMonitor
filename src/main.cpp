#include "/home/pi/mtg-collection-manager/src/interface/MainWindow.h"
#include <QApplication>
#include <iostream>
#include <string>
#include <chrono>
#include "/home/pi/mtg-collection-manager/include/feeder/Feeder.hpp"



int main(int argc, char *argv[])
{
    // // Initialize Qt Application
    // QApplication a(argc, argv);
    Feeder feeder;
    // CardOCR ocr;
    // DatabaseManager dbM("test-collection.db3");
    // dbM.addCard(1, "Black Lotus", 0, "Colorless", "Artifact", "Alpha", "Rare", 0, 0, "Christopher Rush", "imagePath");

    // // Custom initialization code (if any)
    // std::cout << "Initiating boot sequence" << std::endl;

    // // Create and display the main window
    // MainWindow w;
    // w.show();

    // // Start the Qt event loop
    // return a.exec();

    // Open a database file
    // SQLite::Database    db("example.db3");
    
    // // Compile a SQL query, containing one parameter (index 1)
    // SQLite::Statement   query(db, "SELECT * FROM test WHERE size > ?");
    
    // // Bind the integer value 6 to the first parameter of the SQL query
    // query.bind(1, 6);
    
    // // Loop to execute the query step by step, to get rows of result
    // while (query.executeStep())
    // {
    //     // Demonstrate how to get some typed column value
    //     int         id      = query.getColumn(0);
    //     const char* value   = query.getColumn(1);
    //     int         size    = query.getColumn(2);
        
    //     std::cout << "row: " << id << ", " << value << ", " << size << std::endl;
    // }
    return 0;
}
