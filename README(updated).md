# Transaction Processing System — Change Log
24UCS271 Mini Project

---

## Bug Fixes

**Input Validation**
Added input buffer clearing after every scanf call. Prevents the program from crashing or looping infinitely when non-numeric input is entered.

**Auto Database Creation**
The program now creates a fresh credit.dat file automatically if one does not exist, instead of exiting with an error.

**feof() Loop Fix**
Replaced the faulty `while(!feof(...))` loop in textFile() with `while(fread(...) == 1)`. The original code was reading one extra garbage record at the end of the file.

**Compiler Warning Fix**
Resolved -Woverflow warning by casting the fseek offset to `-(long)sizeof(struct clientData)`.

---

## New Features

**Search by Last Name**
Added function searchByName() as Option 5. Scans all records and displays every account that matches the entered last name.

**Bank Totals Report**
Added function calculateTotals() as Option 6. Displays the total number of active accounts and the sum of all account balances.

**Database Reset Tool**
Added function wipeDatabase() as Option 7. Overwrites all 100 record slots with blank data to reset a corrupted database. Requires the user to type YES to confirm before proceeding.

**Phone Number Field**
Added a phone[15] field to the clientData struct. The add, update, and accounts.txt export functions have been updated to include this field.

---

## UI Changes

- Screen is cleared before the menu is displayed on each loop using system("cls")
- A fixed header "BANK MANAGEMENT SYSTEM" is displayed at the top of every screen
- system("pause") is called after every operation so the output remains visible before the screen refreshes
