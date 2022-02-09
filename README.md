# bank-app
simple banking app using c++

Book keeping
A urban development bank has used paper based records so far and decided to use an
electronic record keeping system for convenience. The software developer is provided a
sample balance sheet (balance.txt) where it represent the cash balance of all the customers and
cash in the bank’s vault. In addition, a transaction log sheet (transaction.txt) is provided, which
keep all the transactions occurred in the accounts. However, monthly interest calculation and
account keeping charges are not indicated in the transaction logs. Develop a C++ console programme according to the following guidelines for the urban
development bank:
First, read the balance.txt and create text file for each account under account
number with opening balance as indicated in the balance.txt. Second,perform the transactions on the accounts as indicated in the transaction.txt. E. g. : If a customer deposit money, it should be indicated in both customer and bank vault
account as well. Thirdly, implement the following two operations on the accounts:
 If the balance is less than Rs 1000.00, a Rs10.00 charge will be added to the customer’s
account and add that charge to the bank vault at the end of each day.  If the balance is negative, Rs50.00 will be charged from the customer’s account as
overdraft charge at the end of the day.  A daily interest of 0.02% will be added to all the customers’ accounts at the beginning of
the day if the balance is positive. Round the interest value up to 2


When the program starts, delete all the customer account files and rewrite them freshly. The UI should give options to view bank vault balance and current balance of all the customer
accounts. User can enter the date and see the starting balance (before applying interest) and end of the
day balance of any account. Text file description:
Balance.txt
Each line contains the account number and balance of the account by 1-1-2021. The account number is an integer and the balance is indicated up to two decimal places. The account number and the balance is separated by a comma (,). Transaction.txt
Each line contains the date(in yyyymmdd format), customer’s account number, transaction
type, and transaction amount (up to two decimal places), and they are separated by a comma
(,). It contains transactions taken place from 1
st to 11th January 2021. 

Transaction types
1-deposit
2-withdraw

Account.txt <created by the programme for each account>
These accounts should indicate these details
  
Transaction type
0-opening balance
1-deposit
2-withdraw
3-interest added
4-charges for lower/negative balance
  
Transaction amount
Balance
BankAccount.txt
Following details should be included. Total amount of money in the bank at the beginning of the year. Indicate all the transactions with following details:

Transaction type
0-opening balance
1-deposit
2-withdraw
3-interest paid
4-charges for lower/negative balance
Transaction amount
Balance
No need to add a file header for account.txt files. Columns are separated by a comma (,)
