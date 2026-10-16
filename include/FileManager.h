#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "Menu.h"
#include "Order.h"
#include <string>

// Handles permanent storage for QuickBite data files.
class FileManager
{
public:
    static void loadMenu(const std::string& fileName, Menu& menu);
    static void saveMenu(const std::string& fileName, const Menu& menu);
    static void appendOrder(const std::string& fileName, const Order& order);
    static int getNextOrderId(const std::string& fileName);
};

#endif
