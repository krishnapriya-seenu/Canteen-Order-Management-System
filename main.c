
#include <stdio.h>
#include <string.h>

#define MAX_ORDERS 5

struct Food {
    int id;
    char name[30];
    float price;
};

struct Order {
    int token;
    char studentName[50];
    char foodItems[300];
    int quantity;
    float total;
};

struct Food menu[] = {
    {1, "Dosa", 40},
    {2, "Idli", 20},
    {3, "Pongal", 35},
    {4, "Vadai", 10},
    {5, "Sandwich", 50},
    {6, "Samosa", 15},
    {7, "Juice", 30},
    {8, "Meals", 80},
    {9, "Poori", 35},
    {10, "Fried Rice", 70}
};

struct Order orders[MAX_ORDERS];

int front = 0;
int rear = -1;
int count = 0;
int nextToken = 1;

int menuSize = sizeof(menu) / sizeof(menu[0]);

void displayMenu() {
    int i;

    printf("\n========== CANTEEN MENU ==========\n");
    printf("ID\tFood Item\tPrice\n");

    for (i = 0; i < menuSize; i++) {
        printf("%d\t%-15s Rs. %.2f\n",
               menu[i].id, menu[i].name, menu[i].price);
    }
}

int searchFood(int id) {
    int i;

    for (i = 0; i < menuSize; i++) {
        if (menu[i].id == id) {
            return i;
        }
    }

    return -1;
}

void placeOrder() {
    struct Order newOrder;
    int foodId, quantity, index;
    int totalQuantity = 0;
    float total = 0;
    char selectedFoods[300] = "";

    if (count == MAX_ORDERS) {
        printf("\nOrder queue is full.\n");
        return;
    }

    printf("\nEnter student name: ");
    scanf(" %49[^\n]", newOrder.studentName);

    displayMenu();

    printf("\nEnter Food ID (0 to finish): ");

    while (1) {
        if (scanf("%d", &foodId) != 1) {
            printf("Invalid input.\n");
            return;
        }

        if (foodId == 0) {
            break;
        }

        index = searchFood(foodId);

        if (index == -1) {
            printf("Invalid Food ID.\n");
            printf("Enter Food ID (0 to finish): ");
            continue;
        }

        printf("Enter quantity for %s: ", menu[index].name);

        if (scanf("%d", &quantity) != 1 || quantity <= 0) {
            printf("Invalid quantity.\n");
            return;
        }

        {
            char item[60];

            snprintf(item, sizeof(item), "%s x%d, ",
                     menu[index].name, quantity);

            if (strlen(selectedFoods) + strlen(item)
                >= sizeof(selectedFoods)) {
                printf("Too many food items in this order.\n");
                return;
            }

            strcat(selectedFoods, item);
        }

        total += menu[index].price * quantity;
        totalQuantity += quantity;

        printf("Item Total: Rs. %.2f\n",
               menu[index].price * quantity);

        printf("Enter next Food ID (0 to finish): ");
    }

    if (totalQuantity == 0) {
        printf("No food selected. Order cancelled.\n");
        return;
    }

    newOrder.token = nextToken++;
    strcpy(newOrder.foodItems, selectedFoods);
    newOrder.quantity = totalQuantity;
    newOrder.total = total;

    rear = (rear + 1) % MAX_ORDERS;
    orders[rear] = newOrder;
    count++;

    printf("\n========== ORDER BILL ==========\n");
    printf("Student Name : %s\n", newOrder.studentName);
    printf("Token Number : %d\n", newOrder.token);
    printf("Food Items   : %s\n", newOrder.foodItems);
    printf("Total Quantity: %d\n", newOrder.quantity);
    printf("Total Bill   : Rs. %.2f\n", newOrder.total);
    printf("Status       : Pending\n");
    printf("Order placed successfully!\n");
}

void displayOrders() {
    int i, index;

    if (count == 0) {
        printf("\nNo pending orders.\n");
        return;
    }

    printf("\n========== PENDING ORDERS ==========\n");

    for (i = 0; i < count; i++) {
        index = (front + i) % MAX_ORDERS;

        printf("\nToken       : %d\n", orders[index].token);
        printf("Student     : %s\n", orders[index].studentName);
        printf("Food Items  : %s\n", orders[index].foodItems);
        printf("Quantity    : %d\n", orders[index].quantity);
        printf("Total Bill  : Rs. %.2f\n", orders[index].total);
        printf("Status      : Pending\n");
    }
}

void searchOrder() {
    int token, i, index, found = 0;

    printf("\nEnter token number: ");

    if (scanf("%d", &token) != 1) {
        printf("Invalid input.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        index = (front + i) % MAX_ORDERS;

        if (orders[index].token == token) {
            printf("\nOrder Found!\n");
            printf("Token      : %d\n", orders[index].token);
            printf("Student    : %s\n", orders[index].studentName);
            printf("Food Items : %s\n", orders[index].foodItems);
            printf("Quantity   : %d\n", orders[index].quantity);
            printf("Total Bill : Rs. %.2f\n", orders[index].total);
            printf("Status     : Pending\n");

            found = 1;
            break;
        }
    }

    if (found == 0) {
        printf("Pending order not found.\n");
    }
}

void processOrder() {
    if (count == 0) {
        printf("\nNo pending orders to process.\n");
        return;
    }

    printf("\n========== ORDER COMPLETED ==========\n");
    printf("Token      : %d\n", orders[front].token);
    printf("Student    : %s\n", orders[front].studentName);
    printf("Food Items : %s\n", orders[front].foodItems);
    printf("Total Bill : Rs. %.2f\n", orders[front].total);
    printf("Status     : Completed\n");

    front = (front + 1) % MAX_ORDERS;
    count--;

    printf("Order processed in FIFO order.\n");
}

int main() {
    int choice;

    printf("====================================\n");
    printf(" CANTEEN ORDER MANAGEMENT SYSTEM\n");
    printf("      DATA STRUCTURES USING C\n");
    printf("====================================\n");

    do {
        printf("\n1. Display Food Menu\n");
        printf("2. Place Order and Calculate Bill\n");
        printf("3. Display Pending Orders\n");
        printf("4. Search Order by Token\n");
        printf("5. Process Next Order\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 0;
        }

        switch (choice) {
            case 1:
                displayMenu();
                break;

            case 2:
                placeOrder();
                break;

            case 3:
                displayOrders();
                break;

            case 4:
                searchOrder();
                break;

            case 5:
                processOrder();
                break;

            case 6:
                printf("\nThank you for using the Canteen System!\n");
                break;

            default:
                printf("\nInvalid choice. Try again.\n");
        }

    } while (choice != 6);

    return 0;
}
