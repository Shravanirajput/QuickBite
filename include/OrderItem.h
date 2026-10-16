#ifndef ORDER_ITEM_H
#define ORDER_ITEM_H

#include <string>

// Stores a snapshot of one menu item inside an order.
class OrderItem
{
private:
    int itemId;
    std::string itemName;
    double unitPrice;
    int quantity;

public:
    OrderItem(int id, const std::string& name, double price, int itemQuantity);

    double getTotal() const;
    int getItemId() const;
    const std::string& getItemName() const;
    double getUnitPrice() const;
    int getQuantity() const;
};

#endif
