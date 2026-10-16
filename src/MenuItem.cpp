#include "MenuItem.h"
#include <stdexcept>

MenuItem::MenuItem(int itemId, const std::string& itemName, double itemPrice, int itemStock)
    : id(itemId), name(itemName), price(itemPrice), stock(itemStock), available(itemStock > 0)
{
    // Reject invalid data at construction time so every MenuItem is valid.
    if (itemId <= 0 || itemName.empty() || itemPrice < 0 || itemStock < 0)
    {
        throw std::invalid_argument("Invalid menu item details.");
    }
}

int MenuItem::getId() const
{
    return id;
}

const std::string& MenuItem::getName() const
{
    return name;
}

double MenuItem::getPrice() const
{
    return price;
}

int MenuItem::getStock() const
{
    return stock;
}

bool MenuItem::isAvailable() const
{
    return available && stock > 0;
}

void MenuItem::reduceStock(int quantity)
{
    if (quantity <= 0 || quantity > stock)
    {
        throw std::invalid_argument("Requested quantity is not valid.");
    }

    // Stock is changed only after the requested quantity has been validated.
    stock -= quantity;
    if (stock == 0)
    {
        available = false;
    }
}

void MenuItem::restock(int quantity)
{
    if (quantity <= 0)
    {
        throw std::invalid_argument("Restock quantity must be positive.");
    }

    // Staff use this operation when new canteen inventory arrives.
    stock += quantity;
    available = true;
}

void MenuItem::setAvailability(bool itemAvailability)
{
    available = itemAvailability;
}
