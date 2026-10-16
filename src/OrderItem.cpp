#include "OrderItem.h"
#include <stdexcept>

OrderItem::OrderItem(int id, const std::string& name, double price, int itemQuantity)
    : itemId(id), itemName(name), unitPrice(price), quantity(itemQuantity)
{
    // An order item stores the price and name at the time of ordering.
    if (id <= 0 || name.empty() || price < 0 || itemQuantity <= 0)
    {
        throw std::invalid_argument("Invalid order item details.");
    }
}

double OrderItem::getTotal() const
{
    // This line-item total is used to calculate the complete order bill.
    return unitPrice * quantity;
}

int OrderItem::getItemId() const
{
    return itemId;
}

const std::string& OrderItem::getItemName() const
{
    return itemName;
}

double OrderItem::getUnitPrice() const
{
    return unitPrice;
}

int OrderItem::getQuantity() const
{
    return quantity;
}
