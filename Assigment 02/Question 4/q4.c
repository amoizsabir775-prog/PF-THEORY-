#include <stdio.h>


int getValidDays() {
    int days = 0;
    while (days < 1 || days > 7) {
        printf("Enter number of festival days (1-7): ");
        scanf("%d", &days);
        if (days < 1 || days > 7) {
            printf("Invalid! Please enter a number between 1 and 7.\n");
        }
    }
    return days;
}


float calculateDailySales(int dayNumber) {
    int orders, i;
    float total_sales = 0.0, avg_order = 0.0, highest_order = 0.0;
    
    printf("\nDay %d - Number of orders: ", dayNumber);
    scanf("%d", &orders);

    printf("Order amounts: ");
    for (i = 0; i < orders; i++) {
        float amount;
        scanf("%f", &amount);

        
        while (amount <= 0) {
            printf("Invalid amount! Enter amount greater than 0 for order %d: ", i + 1);
            scanf("%f", &amount);
        }

        total_sales += amount;

        
        if (i == 0 || amount > highest_order) {
            highest_order = amount;
        }
    }

    avg_order = total_sales / orders;

    
    printf("Total sales: Rs. %.2f\n", total_sales);
    printf("Average order: Rs. %.2f\n", avg_order);
    printf("Highest order: Rs. %.2f\n", highest_order);

    return total_sales;
}


int findHighestSalesDay(float dailySales[], int numberOfDays) {
    int i;
    int best_day = 0;
    float max_sales = dailySales[0];

    for (i = 1; i < numberOfDays; i++) {
        if (dailySales[i] > max_sales) {
            max_sales = dailySales[i];
            best_day = i;
        }
    }
    return best_day + 1; // Return day number starting from 1
}

void displayBarChart(float dailySales[], int numberOfDays) {
    int i, j;
    printf("\n===== DAILY SALES BAR CHART =====\n");
    for (i = 0; i < numberOfDays; i++) {
        int stars = (int)(dailySales[i] / 100);
        printf("Day %d: ", i + 1);
        for (j = 0; j < stars; j++) {
            printf("*");
        }
        printf(" (Rs. %.2f)\n", dailySales[i]);
    }
}

int main() {
    int days, i;
    float dailySales[7];
    float total_festival_sales = 0.0;

    // Get valid festival days
    days = getValidDays();

    // Process each day and store totals in array
    for (i = 0; i < days; i++) {
        dailySales[i] = calculateDailySales(i + 1);
        total_festival_sales += dailySales[i];
    }

    // Festival Summary Report
    int top_day = findHighestSalesDay(dailySales, days);
    float highest_daily_sales = dailySales[top_day - 1];

    printf("\n===== FESTIVAL SUMMARY =====\n");
    printf("Total festival sales: Rs. %.2f\n", total_festival_sales);
    printf("Day with highest sales: Day %d\n", top_day);
    printf("Highest daily sales: Rs. %.2f\n", highest_daily_sales);

    // Bar Chart Report
    displayBarChart(dailySales, days);

    return 0;
}