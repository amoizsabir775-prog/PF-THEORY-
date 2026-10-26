/*
** Programmer A.MOiz Sabir
Assignment 2 question 1
Date 10/24/2026
Class BDS-1A
*/

#include<stdio.h>
int main(){
    
    int no_of_rows;
    int no_of_seat_per_rows;
    int seat_status;
    printf("Enter the Number of Rows");
    scanf("%d",&no_of_rows);

    printf("Enter the number of seats per row: ");
    scanf("%d", &no_of_seat_per_rows);

    int arr[no_of_rows][no_of_seat_per_rows];

    printf("Enter each seat status (0 = available, 1 = booked):\n");
    for (int i = 0; i < no_of_rows; i++) {
        for (int j = 0; j < no_of_seat_per_rows; j++) {
            scanf("%d", &arr[i][j]);
        }
    }

    printf("\nSeating Arrangement:\n");
    printf("    ");
    for (int j = 0; j < no_of_seat_per_rows; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < no_of_rows; i++) {
        printf("R%d  ", i);
        for (int j = 0; j < no_of_seat_per_rows; j++) {
            if (arr[i][j] == 0) {
                printf("_ ");
            } else {
                printf("X ");
            }
        }
        printf("\n");
    }
    // Task 3: Seat Availability Report
    int total_free = 0;
    printf("\nFree seats per row: ");
    for (int i = 0; i < no_of_rows; i++) {
        int row_free = 0;
        for (int j = 0; j < no_of_seat_per_rows; j++) {
            if (arr[i][j] == 0) {
                row_free++;
            }
        }
        printf("%d ", row_free);
        total_free += row_free;
    }
    printf(" Total free: %d\n", total_free);
    // Task 3: Seat Availability Report
     total_free = 0;
    printf("\nFree seats per row: ");
    for (int i = 0; i < no_of_rows; i++) {
        int row_free = 0;
        for (int j = 0; j < no_of_seat_per_rows; j++) {
            if (arr[i][j] == 0) { // <-- Make sure this check is here
                row_free++;
            }
        }
        printf("%d ", row_free);
        total_free += row_free;
    }
    printf(" Total free: %d\n", total_free);
    // Task 4: Group Booking Input & Validation
    int k;
    printf("\nEnter the number of customers (k) who want to sit together: ");
    scanf("%d", &k);

    if (k <= 0 || k > no_of_seat_per_rows) {
        printf("Invalid group size! Must be between 1 and %d.\n", no_of_seat_per_rows);
    } else {
        // Task 5: Seat Allocation Rules & Search
        int mid = no_of_rows / 2;
        int found = 0;
        int alloc_row = -1, alloc_start = -1;

        // Search outwards from the middle row by distance d
        for (int d = 0; d < no_of_rows && !found; d++) {
            int r1 = mid - d; 
            int r2 = mid + d; 

            int rows_to_check[2];
            int count_rows = 0;

            if (r1 >= 0) {
                rows_to_check[count_rows++] = r1;
            }
            if (r2 < no_of_rows && r2 != r1) {
                rows_to_check[count_rows++] = r2;
            }

            // Check each valid row at distance d
            for (int idx = 0; idx < count_rows && !found; idx++) {
                int i = rows_to_check[idx];
                int consecutive_count = 0;

                // Scan row for k consecutive available seats (0)
                for (int j = 0; j < no_of_seat_per_rows; j++) {
                    if (arr[i][j] == 0) {
                        consecutive_count++;
                        if (consecutive_count == k) {
                            alloc_row = i;
                            alloc_start = j - k + 1; // Starting seat index
                            found = 1;
                            break;
                        }
                    } else {
                        consecutive_count = 0; // Reset if booked seat met
                    }
                }
            }
        }

        // Task 6: Confirm Booking & Display Updated Arrangement
        if (found) {
            printf("\nGroup of %d seated in Row %d, Seats %d to %d\n", k, alloc_row, alloc_start, alloc_start + k - 1);

            // Mark allocated seats as booked (1)
            for (int j = alloc_start; j < alloc_start + k; j++) {
                arr[alloc_row][j] = 1;
            }

            // Display updated seating arrangement
            printf("\nUpdated Seating Arrangement:\n");
            printf("    ");
            for (int j = 0; j < no_of_seat_per_rows; j++) {
                printf("%d ", j);
            }
            printf("\n");

            for (int i = 0; i < no_of_rows; i++) {
                printf("R%d  ", i);
                for (int j = 0; j < no_of_seat_per_rows; j++) {
                    if (arr[i][j] == 0) {
                        printf("_ ");
                    } else {
                        printf("X ");
                    }
                }
                printf("\n");
            }

            // Calculate and display remaining available seats
            int remaining_free = 0;
            for (int i = 0; i < no_of_rows; i++) {
                for (int j = 0; j < no_of_seat_per_rows; j++) {
                    if (arr[i][j] == 0) {
                        remaining_free++;
                    }
                }
            }
            printf("Remaining number of available seats: %d\n", remaining_free);

        } else {
            // Task 7: Handle Unsuccessful Bookings
            printf("\nSorry, no %d consecutive available seats found. Booking unsuccessful and plan left unchanged.\n", k);
        }
    }
    
    return 0;
}

