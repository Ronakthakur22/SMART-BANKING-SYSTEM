// Smart Banking System 
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


//  One bank account


struct Account {
    int accountNumber;
    string name;
    int age;
    string address;
    int pin;
    double balance;
    bool locked;
    int wrongTries;
};


//  Global data


vector<Account> accounts;
int nextAccountNumber = 100001;


//  Save all accounts to file


void saveAll() {
    ofstream f("bank.txt");
    f << nextAccountNumber << "\n";
    for (int i = 0; i < accounts.size(); i++) {
        Account &a = accounts[i];
        f << a.accountNumber << "\n";
        f << a.name << "\n";
        f << a.age << "\n";
        f << a.address << "\n";
        f << a.pin << "\n";
        f << a.balance << "\n";
        f << a.locked << "\n";
    }
}


//  Load all accounts from file


void loadAll() {
    ifstream f("bank.txt");
    if (!f) return;  // No file yet, skip

    f >> nextAccountNumber;
    f.ignore();

    Account a;
    while (f >> a.accountNumber) {
        f.ignore();
        getline(f, a.name);
        f >> a.age;
        f.ignore();
        getline(f, a.address);
        f >> a.pin >> a.balance >> a.locked;
        a.wrongTries = 0;
        accounts.push_back(a);
    }
}


//  Find account index by number
//  Returns -1 if not found


int findAccount(int num) {
    for (int i = 0; i < accounts.size(); i++)
        if (accounts[i].accountNumber == num)
            return i;
    return -1;
}

//  Create a new account


void createAccount() {
    Account a;
    a.accountNumber = nextAccountNumber++;
    a.locked = false;
    a.wrongTries = 0;

    cout << "Name: ";
    cin.ignore();
    getline(cin, a.name);

    cout << "Age: ";
    cin >> a.age;

    cout << "Address: ";
    cin.ignore();
    getline(cin, a.address);

    cout << "Set PIN: ";
    cin >> a.pin;

    cout << "Initial Deposit: ";
    cin >> a.balance;

    accounts.push_back(a);
    saveAll();

    cout << "\nAccount Created! Your Account Number: " << a.accountNumber << "\n";
    cout << "Please remember this number for login.\n";
}


//  Login - returns index or -1


int login() {
    int num, pin;
    cout << "Account Number: ";
    cin >> num;
    cout << "PIN: ";
    cin >> pin;

    int i = findAccount(num);

    if (i == -1) {
        cout << "Account not found.\n";
        return -1;
    }

    if (accounts[i].locked) {
        cout << "Account is locked. Contact admin.\n";
        return -1;
    }

    if (accounts[i].pin == pin) {
        accounts[i].wrongTries = 0;
        cout << "Welcome, " << accounts[i].name << "!\n";
        return i;
    }

    // Wrong PIN
    accounts[i].wrongTries++;
    if (accounts[i].wrongTries >= 3) {
        accounts[i].locked = true;
        cout << "Too many wrong tries. Account locked!\n";
    } else {
        cout << "Wrong PIN. Tries left: " << (3 - accounts[i].wrongTries) << "\n";
    }
    saveAll();
    return -1;
}


//  Account menu after login


void accountMenu(int i) {
    int choice;
    do {
        cout << "\n--- Account Menu ---\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Transfer\n";
        cout << "4. Check Balance\n";
        cout << "5. Account Details\n";
        cout << "6. FD Calculator\n";
        cout << "7. Loan Check\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            double amt;
            cout << "Deposit Amount: ";
            cin >> amt;
            if (amt <= 0) { cout << "Invalid amount.\n"; }
            else {
                accounts[i].balance += amt;
                cout << "Deposited! New Balance: " << accounts[i].balance << "\n";
            }
        }

        else if (choice == 2) {
            double amt;
            cout << "Withdraw Amount: ";
            cin >> amt;
            if (amt <= 0) { cout << "Invalid amount.\n"; }
            else if (amt > accounts[i].balance) { cout << "Not enough balance.\n"; }
            else {
                accounts[i].balance -= amt;
                cout << "Withdrawn! New Balance: " << accounts[i].balance << "\n";
            }
        }

        else if (choice == 3) {
            int toNum;
            double amt;
            cout << "Receiver Account Number: ";
            cin >> toNum;
            cout << "Transfer Amount: ";
            cin >> amt;

            int j = findAccount(toNum);
            if (j == -1) { cout << "Receiver not found.\n"; }
            else if (amt <= 0) { cout << "Invalid amount.\n"; }
            else if (amt > accounts[i].balance) { cout << "Not enough balance.\n"; }
            else {
                accounts[i].balance -= amt;
                accounts[j].balance += amt;
                cout << "Transfer done! Your Balance: " << accounts[i].balance << "\n";
            }
        }

        else if (choice == 4) {
            cout << "Balance: Rs. " << accounts[i].balance << "\n";
        }

        else if (choice == 5) {
            cout << "\nAccount Number : " << accounts[i].accountNumber << "\n";
            cout << "Name           : " << accounts[i].name << "\n";
            cout << "Age            : " << accounts[i].age << "\n";
            cout << "Address        : " << accounts[i].address << "\n";
            cout << "Balance        : Rs. " << accounts[i].balance << "\n";
        }

        else if (choice == 6) {
            // Fixed Deposit Calculator
            double amount, rate;
            int years;
            cout << "FD Amount: "; cin >> amount;
            cout << "Rate (%): "; cin >> rate;
            cout << "Years: "; cin >> years;

            double total = amount;
            for (int y = 0; y < years; y++)
                total += total * rate / 100;

            cout << "Maturity Amount: Rs. " << total << "\n";
        }

        else if (choice == 7) {
            // Loan eligibility
            double income;
            int score;
            cout << "Monthly Income: "; cin >> income;
            cout << "Credit Score: "; cin >> score;

            if (income >= 30000 && score >= 700)
                cout << "Loan Approved!\n";
            else
                cout << "Loan Rejected. (Need income >= 30000 and score >= 700)\n";
        }

        else if (choice == 0) {
            cout << "Logged out.\n";
        }

        else {
            cout << "Invalid choice.\n";
        }

        saveAll();

    } while (choice != 0);
}


//  Admin panel

void adminPanel() {
    string user, pass;
    cout << "Admin Username: "; cin >> user;
    cout << "Admin Password: "; cin >> pass;

    if (user != "admin" || pass != "admin123") {
        cout << "Wrong credentials.\n";
        return;
    }

    int choice;
    do {
        cout << "\n--- Admin Panel ---\n";
        cout << "1. View All Accounts\n";
        cout << "2. Search Account\n";
        cout << "3. Delete Account\n";
        cout << "4. Total Bank Balance\n";
        cout << "5. Total Accounts\n";
        cout << "6. Richest Customer\n";
        cout << "7. Locked Accounts\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            for (int i = 0; i < accounts.size(); i++) {
                cout << "\nAcc No: " << accounts[i].accountNumber;
                cout << " | Name: " << accounts[i].name;
                cout << " | Balance: Rs. " << accounts[i].balance << "\n";
            }
        }

        else if (choice == 2) {
            int num; cout << "Account Number: "; cin >> num;
            int i = findAccount(num);
            if (i == -1) { cout << "Not found.\n"; }
            else {
                cout << "Name: " << accounts[i].name << "\n";
                cout << "Balance: Rs. " << accounts[i].balance << "\n";
                cout << "Locked: " << (accounts[i].locked ? "Yes" : "No") << "\n";
            }
        }

        else if (choice == 3) {
            int num; cout << "Account Number to delete: "; cin >> num;
            int i = findAccount(num);
            if (i == -1) { cout << "Not found.\n"; }
            else {
                accounts.erase(accounts.begin() + i);
                saveAll();
                cout << "Account deleted.\n";
            }
        }

        else if (choice == 4) {
            double total = 0;
            for (int i = 0; i < accounts.size(); i++)
                total += accounts[i].balance;
            cout << "Total Bank Balance: Rs. " << total << "\n";
        }

        else if (choice == 5) {
            cout << "Total Accounts: " << accounts.size() << "\n";
        }

        else if (choice == 6) {
            if (accounts.empty()) { cout << "No accounts.\n"; }
            else {
                int rich = 0;
                for (int i = 1; i < accounts.size(); i++)
                    if (accounts[i].balance > accounts[rich].balance)
                        rich = i;
                cout << "Richest: " << accounts[rich].name;
                cout << " | Balance: Rs. " << accounts[rich].balance << "\n";
            }
        }

        else if (choice == 7) {
            bool found = false;
            for (int i = 0; i < accounts.size(); i++) {
                if (accounts[i].locked) {
                    cout << "Acc No: " << accounts[i].accountNumber;
                    cout << " | Name: " << accounts[i].name << "\n";
                    found = true;
                }
            }
            if (!found) cout << "No locked accounts.\n";
        }

    } while (choice != 0);
}

//  Main - program starts here

int main() {
    loadAll();  // Load saved data

    int choice;
    do {
        cout << "\n===== SMART BANKING SYSTEM =====\n";
        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. Admin Panel\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) createAccount();
        else if (choice == 2) {
            int i = login();
            if (i != -1) accountMenu(i);
        }
        else if (choice == 3) adminPanel();
        else if (choice == 0) cout << "Goodbye!\n";
        else cout << "Invalid choice.\n";

    } while (choice != 0);

    return 0;
}
