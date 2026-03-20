# 🏦 Transaction Processing System
### Mini Project – 24UCS271 Lab | CS3251

> An enhanced C-based Bank Account Management System built on a random-access binary file architecture. This project extends the original textbook implementation with significant bug fixes, new features, and UI improvements.

---

## 📋 Table of Contents
- [Overview](#overview)
- [Bug Fixes & Stability](#-bug-fixes--stability)
- [New Features](#-new-features)
- [UI Improvements](#-ui-improvements)
- [How to Run](#-how-to-run)
- [File Structure](#-file-structure)
- [Evaluation Criteria](#-evaluation-criteria)

---

## Overview

This program manages up to **100 bank accounts** stored in a binary random-access file (`credit.dat`). Each account occupies a fixed-size slot, allowing O(1) direct access to any record using `fseek()`.

### Core Data Structure
```c
struct clientData {
    unsigned int acctNum;  // account number (1–100)
    char lastName[15];
    char firstName[10];
    char phone[15];        // ✨ NEW: phone number field
    double balance;
};
```

---

## 🐛 Bug Fixes & Stability

### 1. Input Validation — Anti-Crash System
The original code would freeze or loop infinitely if a user typed a letter where a number was expected. Fixed by adding input-clearing loops after every `scanf` prompt:
```c
while (getchar() != '\n');  // flush leftover input buffer
```

### 2. Auto Database Creation
The original `main()` would crash with an error if `credit.dat` didn't exist. Rewrote the `fopen` logic to **automatically create a fresh database** when no file is found:
```c
// If file doesn't exist, create it fresh
cfPtr = fopen("credit.dat", "wb+");
```

### 3. Compiler Warning Fix
Eliminated the `-Woverflow` compiler warning by explicitly casting the signed seek offset during file pointer rewinds:
```c
fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
```

### 4. `feof()` Loop Bug (Original Logical Error)
The original `textFile()` used `while (!feof(readPtr))` which reads one extra garbage record after the last valid one. Fixed by checking `fread`'s return value directly:
```c
// BEFORE (buggy)
while (!feof(readPtr)) { fread(...); }

// AFTER (fixed)
while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) { ... }
```

---

## ✨ New Features

### Menu Overview
| Option | Function |
|--------|----------|
| 1 | Export all accounts to `accounts.txt` |
| 2 | Update account balance |
| 3 | Add new account |
| 4 | Delete account |
| **5** | **Search by Last Name** *(new)* |
| **6** | **Bank Totals Dashboard** *(new)* |
| **7** | **Wipe / Reset Database** *(new)* |
| 8 | Exit |

---

### Option 5 — Search by Name Engine (`searchByName()`)
Scans the entire binary file and filters results by **Last Name**, printing all matching accounts. Useful for quickly locating a customer without knowing their account number.

```c
void searchByName(FILE *fPtr) {
    char searchName[15];
    printf("Enter last name to search: ");
    scanf("%14s", searchName);
    // scans all 100 slots, prints matches
}
```

---

### Option 6 — Bank Totals Dashboard (`calculateTotals()`)
An administrative reporting tool that scans all active records and reports:
- **Total number of active accounts**
- **Total funds held across the entire bank**

```
=== BANK TOTALS ===
Active Accounts : 7
Total Balance   : $  24530.75
```

---

### Option 7 — Database Wipe Tool (`wipeDatabase()`)
Safely resets a corrupted or garbage-filled database. Requires **user confirmation** before proceeding. Writes 100 clean, zeroed `clientData` structs to completely reinitialize the random-access file.

```c
printf("Are you sure? Type YES to confirm: ");
// only proceeds on exact "YES" confirmation
```

---

### Phone Number Integration
Expanded the `clientData` struct with a `char phone[15]` field. Updated the **create**, **update**, and **accounts.txt export** pipelines to handle the new field:

```
Acct  Last Name       First Name  Phone           Balance
5     Jones           Sarah       9876543210      750.00
```

---

## 🎨 UI Improvements

### Static Console Dashboard
Replaced the scrolling console text with a clean, persistent application interface:
- `system("cls")` clears the screen before redrawing the menu header
- A centered `=== BANK MANAGEMENT SYSTEM ===` banner is shown every time
- `system("pause")` checkpoints let the user read output before the screen refreshes

```
=========================================
      === BANK MANAGEMENT SYSTEM ===
=========================================
1 - Export accounts to accounts.txt
2 - Update an account
3 - Add a new account
4 - Delete an account
5 - Search by last name
6 - View bank totals
7 - Wipe database
8 - Exit
? _
```

---

## 🚀 How to Run

### On repl.it (required for demo)
1. Upload `trans.c` and `credit.dat` to your repl
2. Click **Run** — the program compiles and launches automatically

### Locally (Linux/macOS)
```bash
gcc trans.c -o trans
./trans
```

### Locally (Windows)
```bash
gcc trans.c -o trans.exe
trans.exe
```

> **Note:** `credit.dat` must be in the same directory. If missing, the program will create a fresh one automatically.

---

## 📁 File Structure

```
.
├── trans.c          # Main source code
├── credit.dat       # Binary random-access database (100 fixed slots)
├── accounts.txt     # Generated text export (Option 1)
└── README.md        # This file
```

---

## 📊 Evaluation Criteria

| Dimension | Activity | Points |
|-----------|----------|--------|
| **General (25)** | Self-effort | 5 |
| | Turnaround Time | 10 |
| | Project Demo (repl.it) | 10 |
| **Comprehension (15)** | Domain Knowledge | 5 |
| | Added Functionality Ideas | 5 |
| | Code Comprehension | 5 |
| **Modification (35)** | Code Improvement | 5 |
| | Functional Decomposition | 10 |
| | Refactoring for Memory | 10 |
| | Refactoring for Speed | 10 |
| **Innovation (45)** | New Features | 5 |
| | Requirements → Code | 10 |
| | Added Functionality (Simple) | 10 |
| | Added Functionality (Advanced) | 20 |
| **Total** | | **120 → 20** |

---

## 📌 Rules

- Maximum **2 attempts** at presentation — best score counts
- Discussion between students is allowed
- **Copying code is strictly prohibited** — violators receive zero (0)

---

*Project Date: 31/01/2026 | Lab: 24UCS271*
