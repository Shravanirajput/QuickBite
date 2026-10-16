#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <string>

// Represents one food item sold by the canteen.
class MenuItem
{
private:
    int id;
    std::string name;
    double price;
    int stock;
    bool available;

public:
    MenuItem(int itemId, const std::string& itemName, double itemPrice, int itemStock);

    int getId() const;
    const std::string& getName() const;
    double getPrice() const;
    int getStock() const;
    bool isAvailable() const;

    void reduceStock(int quantity);
    void restock(int quantity);
    void setAvailability(bool itemAvailability);
};

#endif
