#ifndef ORDER_H
#define ORDER_H

#include "Menu.h"
#include "OrderItem.h"
#include <string>
#include <vector>

// The allowed stages in an order's preparation lifecycle.
enum class OrderStatus
{
    Placed,
    Preparing,
    Ready,
    Completed,
    Cancelled
};

// Represents a student's complete food order and bill.
class Order
{
private:
    static int nextOrderId;
    int orderId;
    std::string studentName;
    int tableNumber;
    OrderStatus status;
    std::vector<OrderItem> items;

public:
    Order(const std::string& name, int table);
    static void setNextOrderId(int id);

    void addItem(MenuItem& menuItem, int quantity);
    double getSubtotal() const;
    double getTax() const;
    double getTotal() const;
    void displayBill() const;

    int getOrderId() const;
    const std::string& getStudentName() const;
    int getTableNumber() const;
    OrderStatus getStatus() const;
    const std::vector<OrderItem>& getItems() const;
    void setStatus(OrderStatus newStatus);
};

std::string orderStatusToString(OrderStatus status);

#endif
