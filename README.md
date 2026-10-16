# QuickBite

QuickBite is a smart canteen ordering and queue management system built primarily in C++.

## Current milestone

The first milestone contains:

- Encapsulated `MenuItem` objects
- A `Menu` class using `std::vector`
- Input validation with standard exceptions
- Stock reduction and restocking
- Menu loading and saving using text file handling
- Interactive student ordering and bill generation
- Order history saved in `data/orders.txt`
- Role-based student and staff screens
- Staff queue management with valid status transitions
- Staff restocking and completed-sales summary

## Build with g++

Run from the `QuickBite` folder:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude src\main.cpp src\Menu.cpp src\MenuItem.cpp src\FileManager.cpp src\Order.cpp src\OrderItem.cpp src\QueueManager.cpp -o QuickBite.exe
.\QuickBite.exe
```

The staff screen currently accepts any non-empty username and password for demonstration purposes.

## Visual interface

The presentation interface is in the `web` folder. Open `web/index.html` in a browser to view the responsive student ordering and staff dashboard screens.

The current web layer is a visual demonstration that mirrors the C++ menu, order queue, stock, and billing workflows. The C++ console application remains the working business-logic implementation. The next integration step would connect the web buttons to the C++ classes through an API.
