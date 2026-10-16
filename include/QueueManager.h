#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include "Order.h"
#include <string>
#include <vector>

// A lightweight saved-order view used by the staff dashboard.
struct OrderRecord
{
    int orderId;
    std::string studentName;
    int tableNumber;
    OrderStatus status;
    double total;
    std::string itemSummary;
};

// Loads, displays, updates, and saves the canteen order queue.
class QueueManager
{
private:
    std::vector<OrderRecord> orders;

    static OrderStatus statusFromString(const std::string& status);
    static bool isValidTransition(OrderStatus current, OrderStatus next);

public:
    void load(const std::string& fileName);
    void save(const std::string& fileName) const;
    void displayQueue() const;
    void updateStatus(int orderId, OrderStatus newStatus);
    double getCompletedSales() const;
    int getCompletedOrderCount() const;
    const std::vector<OrderRecord>& getOrders() const;
};

#endif
