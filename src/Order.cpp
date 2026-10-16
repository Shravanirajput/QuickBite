#include "Order.h"
#include <iomanip>
#include <iostream>
#include <stdexcept>

// The value is restored from orders.txt when the application starts.
int Order::nextOrderId = 1001;

Order::Order(const std::string& name, int table)
    : orderId(nextOrderId++), studentName(name), tableNumber(table), status(OrderStatus::Placed)
{
    if (name.empty() || table <= 0)
    {
        throw std::invalid_argument("Student name and table number are required.");
    }
}

void Order::setNextOrderId(int id)
{
    if (id > 0)
    {
        nextOrderId = id;
    }
}

void Order::addItem(MenuItem& menuItem, int quantity)
{
    if (!menuItem.isAvailable())
    {
        throw std::runtime_error("The selected item is not available.");
    }

    // Reduce inventory first; an exception prevents an invalid item from entering the order.
    menuItem.reduceStock(quantity);
    items.emplace_back(menuItem.getId(), menuItem.getName(), menuItem.getPrice(), quantity);
}

double Order::getSubtotal() const
{
    double subtotal = 0.0;
    for (const OrderItem& item : items)
    {
        subtotal += item.getTotal();
    }
    return subtotal;
}

double Order::getTax() const
{
    // QuickBite applies a fixed 5 percent tax to the subtotal.
    return getSubtotal() * 0.05;
}

double Order::getTotal() const
{
    return getSubtotal() + getTax();
}

void Order::displayBill() const
{
    std::cout << "\n+------------------------------------------+\n";
    std::cout << "|              QUICKBITE BILL              |\n";
    std::cout << "+------------------------------------------+\n";
    std::cout << "Order ID: " << orderId << "\n";
    std::cout << "Student: " << studentName << "\n";
    std::cout << "Table: " << tableNumber << "\n";
    std::cout << "Status: " << orderStatusToString(status) << "\n\n";

    for (const OrderItem& item : items)
    {
        std::cout << std::left << std::setw(22) << item.getItemName()
                  << " x " << std::setw(3) << item.getQuantity()
                  << " Rs. " << std::fixed << std::setprecision(2)
                  << item.getTotal() << "\n";
    }

    std::cout << "\nSubtotal: Rs. " << getSubtotal() << "\n";
    std::cout << "Tax (5%): Rs. " << getTax() << "\n";
    std::cout << "TOTAL:    Rs. " << getTotal() << "\n";
    std::cout << "+------------------------------------------+\n";
}

int Order::getOrderId() const
{
    return orderId;
}

const std::string& Order::getStudentName() const
{
    return studentName;
}

int Order::getTableNumber() const
{
    return tableNumber;
}

OrderStatus Order::getStatus() const
{
    return status;
}

const std::vector<OrderItem>& Order::getItems() const
{
    return items;
}

void Order::setStatus(OrderStatus newStatus)
{
    status = newStatus;
}

std::string orderStatusToString(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::Placed: return "PLACED";
    case OrderStatus::Preparing: return "PREPARING";
    case OrderStatus::Ready: return "READY";
    case OrderStatus::Completed: return "COMPLETED";
    case OrderStatus::Cancelled: return "CANCELLED";
    }
    return "UNKNOWN";
}
