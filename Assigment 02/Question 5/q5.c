#include <stdio.h>
#include <string.h>

void displayMenu() {
    printf("\n===== UNIVERSITY LIBRARY SYSTEM =====\n");
    printf("1. Register Member\n");
    printf("2. Display Available Books\n");
    printf("3. Borrow a Book\n");
    printf("4. Return a Book\n");
    printf("5. Display Member Record\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

void registerMember(char name[], int *studentID, int *memberType) {
    printf("Enter member name: ");
    scanf("%s", name);
    printf("Enter student/faculty ID: ");
    scanf("%d", studentID);
    printf("Enter membership type (1 for Student, 2 for Faculty): ");
    scanf("%d", memberType);
    while (*memberType != 1 && *memberType != 2) {
        printf("Invalid! Enter 1 for Student, 2 for Faculty: ");
        scanf("%d", memberType);
    }
    printf("Registration successful!\n");
}

void displayBooks(int bookIDs[], char bookTitles[][50], int availableCopies[], int totalBooks) {
    int i;
    printf("\n-------------------------------------------------\n");
    printf("ID\tTitle\t\t\t\tCopies Available\n");
    printf("-------------------------------------------------\n");
    for (i = 0; i < totalBooks; i++) {
        printf("%d\t%-30s\t%d\n", bookIDs[i], bookTitles[i], availableCopies[i]);
    }
    printf("-------------------------------------------------\n");
}

void borrowBook(int bookIDs[], char bookTitles[][50], int availableCopies[], int totalBooks, int borrowedBookIDs[], int *borrowedCount, int memberType) {
    int id, i, foundIndex, days, j, alreadyBorrowed;
    foundIndex = -1;
    alreadyBorrowed = 0;

    if (*borrowedCount >= 3) {
        printf("Error: Maximum limit of 3 borrowed books reached!\n");
        return;
    }

    printf("Enter Book ID to borrow: ");
    scanf("%d", &id);

    for (i = 0; i < totalBooks; i++) {
        if (bookIDs[i] == id) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Error: Invalid Book ID!\n");
        return;
    }

    if (availableCopies[foundIndex] <= 0) {
        printf("Error: Book is out of stock!\n");
        return;
    }

    for (j = 0; j < *borrowedCount; j++) {
        if (borrowedBookIDs[j] == id) {
            alreadyBorrowed = 1;
            break;
        }
    }

    if (alreadyBorrowed) {
        printf("Error: You already borrowed this book!\n");
        return;
    }

    printf("Enter number of days to keep: ");
    scanf("%d", &days);
    while (days <= 0) {
        printf("Enter positive days: ");
        scanf("%d", &days);
    }

    borrowedBookIDs[*borrowedCount] = id;
    (*borrowedCount)++;
    availableCopies[foundIndex]--;
    printf("Book borrowed successfully!\n");
}

float returnBook(int bookIDs[], int availableCopies[], int totalBooks, int borrowedBookIDs[], int *borrowedCount, int memberType, float *totalFine) {
    int id, i, foundIndex, bookPos, actualDays, allowedDays, extraDays, fineRate;
    float currentFine;
    char code[20];
    
    foundIndex = -1;
    bookPos = -1;
    currentFine = 0.0;

    if (*borrowedCount == 0) {
        printf("You have no borrowed books to return!\n");
        return 0.0;
    }

    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    for (i = 0; i < *borrowedCount; i++) {
        if (borrowedBookIDs[i] == id) {
            bookPos = i;
            break;
        }
    }

    if (bookPos == -1) {
        printf("Error: You did not borrow this book!\n");
        return 0.0;
    }

    for (i = 0; i < totalBooks; i++) {
        if (bookIDs[i] == id) {
            foundIndex = i;
            break;
        }
    }

    printf("Enter actual days kept: ");
    scanf("%d", &actualDays);
    while (actualDays <= 0) {
        printf("Enter positive days: ");
        scanf("%d", &actualDays);
    }

    if (memberType == 1) {
        allowedDays = 14;
    } else {
        allowedDays = 30;
    }

    if (actualDays > allowedDays) {
        extraDays = actualDays - allowedDays;
        if (memberType == 1) {
            fineRate = 20;
        } else {
            fineRate = 10;
        }
        currentFine = extraDays * fineRate;
    }

    printf("Enter membership code for waiver (or type NONE): ");
    scanf("%s", code);

    if (strcmp(code, "LIB2026") == 0) {
        currentFine = currentFine / 2.0;
        printf("50%% fine waiver applied!\n");
    }

    *totalFine += currentFine;

    for (i = bookPos; i < *borrowedCount - 1; i++) {
        borrowedBookIDs[i] = borrowedBookIDs[i + 1];
    }
    (*borrowedCount)--;

    if (foundIndex != -1) {
        availableCopies[foundIndex]++;
    }

    printf("Book returned successfully! Fine: Rs. %.2f\n", currentFine);
    return currentFine;
}

void displayMemberRecord(char name[], int studentID, int memberType, int borrowedBookIDs[], int borrowedCount, float totalFine, float fineAfterWaiver) {
    int i;
    printf("\n===== MEMBER RECORD =====\n");
    printf("Name: %s\n", name);
    printf("ID: %d\n", studentID);
    if (memberType == 1) {
        printf("Type: Student\n");
    } else {
        printf("Type: Faculty\n");
    }
    printf("Borrowed Book IDs: ");
    if (borrowedCount == 0) {
        printf("None");
    } else {
        for (i = 0; i < borrowedCount; i++) {
            printf("%d ", borrowedBookIDs[i]);
        }
    }
    printf("\nTotal Fine Charged: Rs. %.2f\n", totalFine);
    printf("=========================\n");
}

int main() {
    int choice;
    char name[50];
    int studentID;
    int memberType;

    int bookIDs[4];
    char bookTitles[4][50];
    int availableCopies[4];
    int totalBooks;

    int borrowedBookIDs[3];
    int borrowedCount;
    float totalFine;
    float fineAfterWaiver;

    strcpy(name, "Not Registered");
    studentID = 0;
    memberType = 1;

    bookIDs[0] = 101; bookIDs[1] = 102; bookIDs[2] = 103; bookIDs[3] = 104;
    strcpy(bookTitles[0], "Programming in C");
    strcpy(bookTitles[1], "Data Structures");
    strcpy(bookTitles[2], "Database Systems");
    strcpy(bookTitles[3], "Computer Networks");
    availableCopies[0] = 5; availableCopies[1] = 3; availableCopies[2] = 0; availableCopies[3] = 4;
    totalBooks = 4;

    borrowedCount = 0;
    totalFine = 0.0;
    fineAfterWaiver = 0.0;

    do {
        displayMenu();
        scanf("%d", &choice);

        if (choice == 1) {
            registerMember(name, &studentID, &memberType);
        }
        else if (choice == 2) {
            displayBooks(bookIDs, bookTitles, availableCopies, totalBooks);
        }
        else if (choice == 3) {
            borrowBook(bookIDs, bookTitles, availableCopies, totalBooks, borrowedBookIDs, &borrowedCount, memberType);
        }
        else if (choice == 4) {
            returnBook(bookIDs, availableCopies, totalBooks, borrowedBookIDs, &borrowedCount, memberType, &totalFine);
        }
        else if (choice == 5) {
            displayMemberRecord(name, studentID, memberType, borrowedBookIDs, borrowedCount, totalFine, fineAfterWaiver);
        }
        else if (choice == 6) {
            printf("Exiting program. Thank you!\n");
        }
        else {
            printf("Invalid choice! Choose between 1 and 6.\n");
        }

    } while (choice != 6);

    return 0;
}