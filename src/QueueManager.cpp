#include "QueueManager.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

OrderStatus QueueManager::statusFromString(const std::string& status)
{
    if (status == "PLACED") return OrderStatus::Placed;
    if (status == "PREPARING") return OrderStatus::Preparing;
    if (status == "READY") return OrderStatus::Ready;
    if (status == "COMPLETED") return OrderStatus::Completed;
    if (status == "CANCELLED") return OrderStatus::Cancelled;
    throw std::runtime_error("Unknown order status: " + status);
}

bool QueueManager::isValidTransition(OrderStatus current, OrderStatus next)
{
    // Staff must follow the real preparation sequence instead of skipping stages.
    return (current == OrderStatus::Placed && next == OrderStatus::Preparing) ||
           (current == OrderStatus::Preparing && next == OrderStatus::Ready) ||
           (current == OrderStatus::Ready && next == OrderStatus::Completed) ||
           (current != OrderStatus::Completed && current != OrderStatus::Cancelled &&
            next == OrderStatus::Cancelled);
}

void QueueManager::load(const std::string& fileName)
{
    // Queue records use: orderId|student|table|status|total|items.
    std::ifstream inputFile(fileName);
    if (!inputFile)
    {
        throw std::runtime_error("Unable to open order history file: " + fileName);
    }

    orders.clear();
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty()) continue;

        std::stringstream lineStream(line);
        std::string orderIdText;
        std::string studentName;
        std::string tableText;
        std::string statusText;
        std::string totalText;
        std::string itemSummary;

        if (!std::getline(lineStream, orderIdText, '|') ||
            !std::getline(lineStream, studentName, '|') ||
            !std::getline(lineStream, tableText, '|') ||
            !std::getline(lineStream, statusText, '|') ||
            !std::getline(lineStream, totalText, '|') ||
            !std::getline(lineStream, itemSummary))
        {
            throw std::runtime_error("Invalid order record: " + line);
        }

        orders.push_back({std::stoi(orderIdText), studentName, std::stoi(tableText),
                          statusFromString(statusText), std::stod(totalText), itemSummary});
    }
}

void QueueManager::save(const std::string& fileName) const
{
    // Status changes are saved by rewriting all current records to the text file.
    std::ofstream outputFile(fileName);
    if (!outputFile)
    {
        throw std::runtime_error("Unable to save order history file: " + fileName);
    }

    outputFile << std::fixed << std::setprecision(2);
    for (const OrderRecord& order : orders)
    {
        outputFile << order.orderId << '|'
                   << order.studentName << '|'
                   << order.tableNumber << '|'
                   << orderStatusToString(order.status) << '|'
                   << order.total << '|'
                   << order.itemSummary << '\n';
    }
}

void QueueManager::displayQueue() const
{
    std::cout << "\n+---------------------------------------------------------------------+\n";
    std::cout << "|                         LIVE ORDER QUEUE                           |\n";
    std::cout << "+---------------------------------------------------------------------+\n";

    if (orders.empty())
    {
        std::cout << "| No orders have been placed yet.                                    |\n";
        return;
    }

    for (const OrderRecord& order : orders)
    {
        std::cout << "Order #" << order.orderId
                  << " | Table " << order.tableNumber
                  << " | " << order.studentName
                  << " | " << orderStatusToString(order.status)
                  << " | Rs. " << std::fixed << std::setprecision(2)
                  << order.total << "\n";
        std::cout << "  Items: " << order.itemSummary << "\n";
    }
}

void QueueManager::updateStatus(int orderId, OrderStatus newStatus)
{
    for (OrderRecord& order : orders)
    {
        if (order.orderId == orderId)
        {
            if (!isValidTransition(order.status, newStatus))
            {
                throw std::runtime_error("This order status transition is not allowed.");
            }
            // Update only after validating both the order ID and the state transition.
            order.status = newStatus;
            return;
        }
    }

    throw std::runtime_error("Order ID was not found.");
}

double QueueManager::getCompletedSales() const
{
    double sales = 0.0;
    for (const OrderRecord& order : orders)
    {
        if (order.status == OrderStatus::Completed)
        {
            sales += order.total;
        }
    }
    return sales;
}

int QueueManager::getCompletedOrderCount() const
{
    int count = 0;
    for (const OrderRecord& order : orders)
    {
        if (order.status == OrderStatus::Completed) ++count;
    }
    return count;
}

const std::vector<OrderRecord>& QueueManager::getOrders() const
{
    return orders;
}
