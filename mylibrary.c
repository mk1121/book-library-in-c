// password protection login system
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOK_NAME 50
#define MAX_AUTHOR_NAME 50
#define MAX_STUDENT_NAME 50
#define MAX_CLASS_NAME 50
#define USERNAME "admin"
#define PASSWORD "admin"
typedef struct {
  int id;
  char bookName[MAX_BOOK_NAME];
  char authorName[MAX_AUTHOR_NAME];
  char date[12];
  int isIssued;
} Book;

int16_t login() {
  char username[30];
  char password[30];
  int16_t attempts = 3;
  printf("Enter your username: ");
  scanf("%s", username);
  printf("Enter your password: ");
  scanf("%s", password);
  if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
    return 1;
  } else {
    attempts--;
    printf("Invalid username or password. %d attempts left.\n", attempts);
    if (attempts == 0) {
      printf("You have exceeded the maximum number of attempts. Exiting...\n");
      exit(0);
    }
    login();
  }
  return 0;
}

// Functions such as crud operation for book
// Add book
// Delete book
// Update book
// Search book
// View all books
// Issue book
// View issued books
// Return book
void addBook() {
  Book book;
  FILE *booksFile = fopen("books.txt", "ab+");
  if (booksFile == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  printf("Enter Book ID: ");
  scanf("%d", &book.id);
  printf("Enter Book Name: ");
  scanf("%s", book.bookName);
  printf("Enter Author Name: ");
  scanf("%s", book.authorName);
  printf("Enter Date: ");
  scanf("%s", book.date);
  book.isIssued = 0;
  fwrite(&book, sizeof(Book), 1, booksFile);
  fclose(booksFile);
  printf("Book added successfully!\n");
}

void booksList() {
  Book book;
  FILE *booksFile = fopen("books.txt", "rb");
  if (booksFile == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  printf("Book ID\tBook Name\tAuthor Name\tDate\tIs Issued\n");
  while (fread(&book, sizeof(Book), 1, booksFile)) {
    printf("%d\t%s\t%s\t%s\t%d\n", book.id, book.bookName, book.authorName,
           book.date, book.isIssued);
  }
  fclose(booksFile);
}
void removeBook() {
  int id;
  int found = 0;
  FILE *booksFile = fopen("books.txt", "rb");
  FILE *tempFile = fopen("temp.txt", "wb");
  if (booksFile == NULL || tempFile == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  printf("Enter Book ID to remove: ");
  scanf("%d", &id);
  Book book;
  while (fread(&book, sizeof(Book), 1, booksFile)) {
    if (book.id == id) {
      found = 1;
    } else {
      fwrite(&book, sizeof(Book), 1, tempFile);
    }
  }
  fclose(booksFile);
  fclose(tempFile);
  if (found) {
    remove("books.txt");
    rename("temp.txt", "books.txt");
    printf("Book removed successfully!\n");
  } else {
    printf("Book not found!\n");
    remove("temp.txt");
  }
}
void issueBook() {
  int id;
  int found = 0;
  FILE *booksFile = fopen("books.txt", "rb+");
  if (booksFile == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  printf("Enter Book ID to issue: ");
  scanf("%d", &id);
  Book book;
  while (fread(&book, sizeof(Book), 1, booksFile)) {
    if (book.id == id && book.isIssued == 0) {
      book.isIssued = 1;
      found = 1;
      fseek(booksFile, -sizeof(Book), SEEK_CUR);
      fwrite(&book, sizeof(Book), 1, booksFile);
      break;
    }
  }
  fclose(booksFile);
  if (found) {
    printf("Book issued successfully!\n");
  } else {
    printf("Book not found or already issued!\n");
  }
}
void issueList() {
  Book book;
  FILE *booksFile = fopen("books.txt", "rb");
  if (booksFile == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  printf("Book ID\tBook Name\tAuthor Name\tDate\tIs Issued\n");
  while (fread(&book, sizeof(Book), 1, booksFile)) {
    if (book.isIssued) {
      printf("%d\t%s\t%s\t%s\t%d\n", book.id, book.bookName, book.authorName,
             book.date, book.isIssued);
    }
  }
  fclose(booksFile);
}
void returnBook() {
  int id;
  int found = 0;
  FILE *booksFile = fopen("books.txt", "rb+");
  if (booksFile == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  printf("Enter Book ID to return: ");
  scanf("%d", &id);
  Book book;
  while (fread(&book, sizeof(Book), 1, booksFile)) {
    if (book.id == id && book.isIssued == 1) {
      book.isIssued = 0;
      found = 1;
      fseek(booksFile, -sizeof(Book), SEEK_CUR);
      fwrite(&book, sizeof(Book), 1, booksFile);
      break;
    }
  }
  fclose(booksFile);
  if (found) {
    printf("Book returned successfully!\n");
  } else {
    printf("Book not found or not issued!\n");
  }
}
void searchBook() {
  int id;
  int found = 0;
  FILE *booksFile = fopen("books.txt", "rb");
  if (booksFile == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  printf("Enter Book ID to search: ");
  scanf("%d", &id);
  Book book;
  while (fread(&book, sizeof(Book), 1, booksFile)) {
    if (book.id == id) {
      found = 1;
      printf("Book ID\tBook Name\tAuthor Name\tDate\tIs Issued\n");
      printf("%d\t%s\t%s\t%s\t%d\n", book.id, book.bookName, book.authorName,
             book.date, book.isIssued);
      break;
    }
  }
  fclose(booksFile);
  if (!found) {
    printf("Book not found!\n");
  }
}
int main() {
  if (login() == 1) {
    printf("Login successful!\n");
    int choice;
    FILE *booksFile = fopen("books.txt", "rb+");
    if (booksFile == NULL) {
      booksFile = fopen("books.txt", "wb+");
    }
    FILE *issueFile = fopen("issue.txt", "rb+");
    if (issueFile == NULL) {
      issueFile = fopen("issue.txt", "wb+");
    }
    while (1) {
      system("cls");
      printf("<==Library Management System ==>\n");
      printf("1.Add Book\n");
      printf("2. Books List\n");
      printf("3. Remove Book\n");
      printf("4. Issue Book\n");
      printf("5. Issued Book List\n");
      printf("6. Search Book\n");
      printf("0. Exit\n\n");
      printf("Enter your choice: ");
      scanf("%d", &choice);
      switch (choice) {
      case 0:
        fclose(booksFile);
        fclose(issueFile);
        exit(0);
      case 1:
        addBook();
        break;
      case 2:
        booksList();
        break;
      case 3:
        removeBook();
        break;
      case 4:
        issueBook();
        break;
      case 5:
        issueList();
        break;
      case 6:
        searchBook();
        break;
      default:
        printf("Invalid Choice...\n\n");
      }
      printf("Press Any Key To Continue...");
      getchar();
      getchar();
    }
  }
  return 0;
}
// int main() {
//   if (login() == 1) {
//     printf("Login successful!\n");
//
//

//
//   return 0;
// }
