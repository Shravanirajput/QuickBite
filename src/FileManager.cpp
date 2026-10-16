#include "FileManager.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

void FileManager::loadMenu(const std::string& fileName, Menu& menu)
{
    // Each menu line uses: id|name|price|stock|availability.
    std::ifstream inputFile(fileName);
    if (!inputFile)
    {
        throw std::runtime_error("Unable to open menu file: " + fileName);
    }

    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream lineStream(line);
        std::string idText;
        std::string name;
        std::string priceText;
        std::string stockText;
        std::string availabilityText;

        if (!std::getline(lineStream, idText, '|') ||
            !std::getline(lineStream, name, '|') ||
            !std::getline(lineStream, priceText, '|') ||
            !std::getline(lineStream, stockText, '|') ||
            !std::getline(lineStream, availabilityText, '|'))
        {
            throw std::runtime_error("Invalid menu record: " + line);
        }

        int id = std::stoi(idText);
        double price = std::stod(priceText);
        int stock = std::stoi(stockText);

        MenuItem item(id, name, price, stock);
        item.setAvailability(availabilityText == "1");
        menu.addItem(item);
    }
}

void FileManager::saveMenu(const std::string& fileName, const Menu& menu)
{
    // Rewriting the menu file preserves the latest stock after an order or restock.
    std::ofstream outputFile(fileName);
    if (!outputFile)
    {
        throw std::runtime_error("Unable to save menu file: " + fileName);
    }

    outputFile << std::fixed << std::setprecision(2);
    for (const MenuItem& item : menu.getItems())
    {
        outputFile << item.getId() << '|'
                   << item.getName() << '|'
                   << item.getPrice() << '|'
                   << item.getStock() << '|'
                   << (item.isAvailable() ? 1 : 0) << '\n';
    }
}

void FileManager::appendOrder(const std::string& fileName, const Order& order)
{
    // Orders are appended so earlier order history is never overwritten.
    std::ofstream outputFile(fileName, std::ios::app);
    if (!outputFile)
    {
        throw std::runtime_error("Unable to open order history file: " + fileName);
    }

    outputFile << order.getOrderId() << '|'
               << order.getStudentName() << '|'
               << order.getTableNumber() << '|'
               << orderStatusToString(order.getStatus()) << '|'
               << std::fixed << std::setprecision(2) << order.getTotal() << '|';

    // Store a compact item summary because the staff queue reads order records from text.
    for (const OrderItem& item : order.getItems())
    {
        outputFile << item.getItemName() << " x " << item.getQuantity() << ';';
    }
    outputFile << '\n';
}

int FileManager::getNextOrderId(const std::string& fileName)
{
    // Find the highest saved ID so restarting the program never reuses an order number.
    std::ifstream inputFile(fileName);
    if (!inputFile)
    {
        throw std::runtime_error("Unable to open order history file: " + fileName);
    }

    int highestOrderId = 1000;
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream lineStream(line);
        std::string orderIdText;
        if (std::getline(lineStream, orderIdText, '|'))
        {
            highestOrderId = std::max(highestOrderId, std::stoi(orderIdText));
        }
    }

    return highestOrderId + 1;
}
