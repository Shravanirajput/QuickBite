#include "FileManager.h"
#include "Order.h"
#include "QueueManager.h"
#include <exception>
#include <iostream>

const std::string MENU_FILE = "data/menu.txt";
const std::string ORDER_FILE = "data/orders.txt";

void printBanner()
{
    std::cout << "\n+==================================================+\n";
    std::cout << "|             QUICKBITE CANTEEN                  |\n";
    std::cout << "|       Smart Ordering - Faster Service           |\n";
    std::cout << "+==================================================+\n";
}

void studentSession(Menu& quickBiteMenu)
{
    std::string studentName;
    int tableNumber;

    std::cout << "\n--- STUDENT ORDERING ---\n";
    std::cout << "Enter student name: ";
    std::cin.ignore();
    std::getline(std::cin, studentName);
    std::cout << "Enter table number: ";
    std::cin >> tableNumber;

    // The Order object collects the student's items and calculates the bill.
    Order order(studentName, tableNumber);
    while (true)
    {
        quickBiteMenu.display();
        std::cout << "\nEnter item ID to add, or 0 to finish: ";

        int itemId;
        std::cin >> itemId;
        if (itemId == 0) break;

        MenuItem* selectedItem = quickBiteMenu.findItem(itemId);
        if (selectedItem == nullptr)
        {
            std::cout << "Item not found. Please try again.\n";
            continue;
        }

        std::cout << "Enter quantity: ";
        int quantity;
        std::cin >> quantity;

        try
        {
            order.addItem(*selectedItem, quantity);
            std::cout << "[OK] Item added to your order.\n";
        }
        catch (const std::exception& exception)
        {
            std::cout << "[!] Could not add item: " << exception.what() << "\n";
        }
    }

    if (order.getItems().empty())
    {
        std::cout << "\nNo items were selected. Order cancelled.\n";
        return;
    }

    // Save both results of checkout: the new stock and the new order history record.
    FileManager::saveMenu(MENU_FILE, quickBiteMenu);
    FileManager::appendOrder(ORDER_FILE, order);
    order.displayBill();
    std::cout << "[OK] Your order has been placed. Keep your order ID.\n";
}

void staffSession(Menu& quickBiteMenu, QueueManager& queue)
{
    std::string username;
    std::string password;

    std::cout << "\n--- STAFF LOGIN ---\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    // This academic demo accepts any entered credentials; authentication is outside the project scope.
    std::cout << "[OK] Login successful. Welcome, " << username << ".\n";
    int choice = 0;
    while (choice != 6)
    {
        std::cout << "\n+---------------- STAFF DASHBOARD ----------------+\n";
        std::cout << "| 1. View live order queue                       |\n";
        std::cout << "| 2. Update order status                          |\n";
        std::cout << "| 3. Restock menu item                            |\n";
        std::cout << "| 4. View menu and stock                          |\n";
        std::cout << "| 5. View completed sales                         |\n";
        std::cout << "| 6. Logout                                      |\n";
        std::cout << "+-------------------------------------------------+\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        try
        {
            if (choice == 1)
            {
                queue.displayQueue();
            }
            else if (choice == 2)
            {
                int orderId;
                int statusChoice;
                std::cout << "Enter order ID: ";
                std::cin >> orderId;
                std::cout << "1. PREPARING  2. READY  3. COMPLETED  4. CANCELLED\n";
                std::cout << "Choose new status: ";
                std::cin >> statusChoice;

                OrderStatus newStatus;
                if (statusChoice == 1) newStatus = OrderStatus::Preparing;
                else if (statusChoice == 2) newStatus = OrderStatus::Ready;
                else if (statusChoice == 3) newStatus = OrderStatus::Completed;
                else if (statusChoice == 4) newStatus = OrderStatus::Cancelled;
                else throw std::invalid_argument("Invalid status option.");

                queue.updateStatus(orderId, newStatus);
                queue.save(ORDER_FILE);
                std::cout << "[OK] Order status updated and saved.\n";
            }
            else if (choice == 3)
            {
                int itemId;
                int quantity;
                std::cout << "\nSelect an item from the menu below. Use the ID in the first column.\n";
                quickBiteMenu.display();
                std::cout << "Enter menu item ID: ";
                std::cin >> itemId;
                std::cout << "Enter quantity to add: ";
                std::cin >> quantity;

                MenuItem* item = quickBiteMenu.findItem(itemId);
                if (item == nullptr) throw std::runtime_error("Menu item was not found.");
                item->restock(quantity);
                FileManager::saveMenu(MENU_FILE, quickBiteMenu);
                std::cout << "[OK] Stock updated. New stock: " << item->getStock() << "\n";
            }
            else if (choice == 4)
            {
                quickBiteMenu.display();
            }
            else if (choice == 5)
            {
                std::cout << "Completed orders: " << queue.getCompletedOrderCount() << "\n";
                std::cout << "Completed sales: Rs. " << queue.getCompletedSales() << "\n";
            }
            else if (choice != 6)
            {
                std::cout << "Please choose a valid option.\n";
            }
        }
        catch (const std::exception& exception)
        {
            std::cout << "[!] " << exception.what() << "\n";
        }
    }
}

int main()
{
    try
    {
        // These objects coordinate the menu, saved data, orders, and staff queue.
        Menu quickBiteMenu;
        QueueManager queue;
        // The paths are relative to the QuickBite project folder.
        FileManager::loadMenu(MENU_FILE, quickBiteMenu);
        queue.load(ORDER_FILE);
        // Continue numbering from the highest ID already stored on disk.
        Order::setNextOrderId(FileManager::getNextOrderId(ORDER_FILE)); 

        printBanner();
        int roleChoice = 0;
        while (roleChoice != 3)
        {
            std::cout << "\n1. Student ordering\n2. Staff dashboard\n3. Exit\nChoose role: ";
            std::cin >> roleChoice;

            if (roleChoice == 1) studentSession(quickBiteMenu);
            else if (roleChoice == 2) staffSession(quickBiteMenu, queue);
            else if (roleChoice != 3) std::cout << "Please choose a valid option.\n";

            if (roleChoice == 1)
            {
                queue.load(ORDER_FILE);
                Order::setNextOrderId(FileManager::getNextOrderId(ORDER_FILE));
            }
        }

        std::cout << "\nThank you for using QuickBite.\n";
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Error: " << exception.what() << "\n";
        return 1;
    }

    return 0;
}
