#include "Menu.h"
#include <iomanip>
#include <iostream>

void Menu::addItem(const MenuItem& item)
{
    items.push_back(item); //push_back() is a vector function that adds an element to the end of a vector.
}

void Menu::display() const
{
    std::cout << "\n========== QuickBite Menu ==========\n";
    std::cout << std::left << std::setw(8) << "ID"
              << std::setw(22) << "Item"
              << std::setw(10) << "Price"
              << "Stock\n";
    std::cout << "-------------------------------------\n";

    for (const MenuItem& item : items)
    {
        std::cout << std::left << std::setw(8) << item.getId()
                  << std::setw(22) << item.getName()
                  << std::setw(10) << item.getPrice()
                  << item.getStock() << "\n";
    }
}

MenuItem* Menu::findItem(int itemId)
{
    for (MenuItem& item : items)
    {
        if (item.getId() == itemId)
        {
            return &item;
        }
    }

    return nullptr;
}

const std::vector<MenuItem>& Menu::getItems() const //A vector is a dynamic array that can grow or shrink as elements are added or removed.
{
    return items;
}
