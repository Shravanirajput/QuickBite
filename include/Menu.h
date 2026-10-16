#ifndef MENU_H
#define MENU_H

#include "MenuItem.h"
#include <vector>

// Manages all food items available in QuickBite.
class Menu
{
private:
    std::vector<MenuItem> items;

public:
    void addItem(const MenuItem& item);
    void display() const;
    MenuItem* findItem(int itemId);
    const std::vector<MenuItem>& getItems() const;
};

#endif
