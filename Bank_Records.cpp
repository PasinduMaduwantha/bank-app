/*
* DATE       : 28-09-2021
* COURSE     : PROGRAMMING QUEST (CO2210)
* TITLE      : QUEST 03- BOOK KEEPING
* AUTHOR     : MADHUWANTHA A.P.
* INDEX NO   : 19/ENG/121
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <time.h> //check the validity of the date
using namespace std;

/*
Function    : split_data
Iputs       : file name
Output      : vector with splited data of the file
*/
vector<string> split_data(string str)
{   //split file within "," and then push all data into one vector
    //eg-inside a vector : [20210101,102340,150000.00,20210101,102341,305430.98,20210101...] 
    fstream inFile;
    inFile.open(str);
    if(!inFile.is_open())
    {    
        cout << "Unnable to find the file!, "<<str; //if file not opend correctly, stop the execute
        exit(0);
    }

    string dat, elements;
    vector<string> account;
    while(inFile>>dat)  //take a line
    {
        istringstream str(dat);
        while(getline(str, elements,  ',')) //line separated by ","
            account.push_back(elements);    //all the srings put into one vector
        //account vectro : [date, accNo, amount,date, accNo, amount,date, accNo, amount,... ] order
        //transction vectro:[date,accNo,type,amount,date,accNo,type,amount,date,accNo,type,amount,... ] order
    }
    inFile.close(); //close the file
    return account; //return the vector
}

/*
Function    : create_accounts
Iputs       : acount vector
Output      : N/A
*/
void create_accounts(vector<string> account)
{
    float toatalBalance;
    ofstream bankAcc;
    for (int accNum = 1; accNum < account.size(); accNum += 3)
    {
        string filename = account[accNum] + ".txt";
        remove(filename.c_str());   //remove the file when strat the program
        ofstream accountFile(account[accNum] + ".txt"); //create the account
        accountFile << account[accNum - 1]<<",0,"<<account[accNum + 1]<<","<<account[accNum + 1]<<endl;
        //initially update the account
        toatalBalance += stof(account[accNum + 1]);
        accountFile.close();
    }
    remove("BankAccount.txt");  //remove the file when strat the program
    bankAcc.open("BankAccount.txt");    //create the BankAccount.txt
    bankAcc << account[0]<<",0,"<<fixed << setprecision(2)<<toatalBalance<<","<<fixed << setprecision(2)<<toatalBalance<<endl;

    bankAcc.close();
}

/*
Function    : interestPaidorCharge
Iputs       : date, account number, balane of the account
Output      : N/A
*/
void interestPaidorCharge(string date, string accNum, float value)
{
    //all the interest and charges are calculated with end balance of the day
    //both bank vault and coustomer account update simultanously
    ofstream charges;
    ofstream bank;
    bank.open("BankAccount.txt", fstream::app);
    vector<string> bankBal = split_data("BankAccount.txt");
    float finalval = stof(bankBal[bankBal.size() - 1]);
    charges.open(accNum+".txt",fstream::app);
    if(!charges.is_open() || !bank.is_open())
    {    
        cout << "Unnable to find the file "<<accNum<<".txt to apply interest!";
        exit(0);
    }
    if(value<1000 && value>=0) //if account balnce is eqaul to 1000 there will no charge appply
    {
        charges << date<<",4,"<<10<<","<< fixed << setprecision(2)<<value-10<<endl;   //account value decerese by 10
        bank << date<<",4,"<<10<<","<< fixed << setprecision(2)<<finalval+10<<endl;   //bank vault increase when charge from account
        //update account with charge from lower balance
    }
    if(value<0) //if account balnce is negative, charge will be 50
    {
        charges << date<<",4,"<<50<<","<< fixed << setprecision(2)<<value-50<<endl;   //account value decerese by 50
        bank << date<<",4,"<<50<<","<< fixed << setprecision(2)<<finalval+50<<endl;   //bank vault increase when charge from account
        //update account with charge  be charged from the customer’s account as overdraft charge at the end of 
    }
    if(value>0)
    {     
        //if value > 0, then interest add to the owner
        charges << date<<",3,"<<value * 0.0002<<","<< fixed << setprecision(2)<<value+value*0.0002<<endl;   
        bank << date<<",3,"<<value*0.0002<<","<< fixed << setprecision(2)<<finalval-value*0.0002<<endl;   //bank vault increase when charge from account
        //update account with interest
    }
    charges.close();
}

/*
Function    : updateAccounts
Iputs       : account number index of the transaction vector, transaction vector, state : deposite or withdraw
Output      : N/A
*/
void updateAccounts(int index, vector<string> transactions, int state)
{   //structure of the account : Date,Type of the transaction,Amount,Balance
    vector<string> accData;
    accData=split_data(transactions[index]+".txt");
    vector<string> bankData;
    bankData=split_data("BankAccount.txt");
    int accountBalance = accData.size() - 1;    //the balance always, last element of the vectro
    int bankIdx = bankData.size() - 1;
    float updatedValue;
    float updateBank;
    // if end of the day
    if(accData.size()>3 && accData[accountBalance-3]!=transactions[index-1] )   
    //compare new trnsaction date and last transaction date of the relavant account
    {   //if the transaction is the last one of the day
        interestPaidorCharge(accData[accountBalance-3], transactions[index], stof(accData[accountBalance]));
        //pass the current balance, index of the account and splited account vector
    }

    //update next date
    accData = split_data(transactions[index] + ".txt");
    accountBalance = accData.size() - 1;

    ofstream updateAccountFile;
    updateAccountFile.open(transactions[index] + ".txt", fstream::app); //we can update(append) the file using fstream::app
    ofstream bank;
    bank.open("BankAccount.txt", fstream::app); //we can update(append) the file using fstream::app
    if(!updateAccountFile.is_open() || !bank.is_open())
    {
        cout << "Error in account updating";
        exit(0);
    }
    if(state==1)
    {
        //if satet 1, then is a deposite then, add values from both customeraccount and bank acount
        updatedValue = stof(accData[accountBalance]) + stof(transactions[index + 2]);
        updateBank = stof(bankData[bankIdx]) + stof(transactions[index + 2]);
        updateAccountFile << transactions[index-1]<<",1,"<<transactions[index + 2]<<","<<fixed<<setprecision(2)<<updatedValue<<endl;
        bank << transactions[index-1]<<",1,"<<transactions[index + 2]<<","<<fixed<<setprecision(2)<<updateBank<<endl;    
    }
    if(state==2)
    {
        //if satet 1, then it is a withdraw then, subtract values from both customeraccount and bank acount
        updatedValue = stof(accData[accountBalance]) - stof(transactions[index + 2]);
        updateBank = stof(bankData[bankIdx]) - stof(transactions[index + 2]);
        updateAccountFile << transactions[index-1]<<",2,"<<transactions[index + 2]<<","<<fixed<<setprecision(2)<<updatedValue<<endl;
        bank << transactions[index-1]<<",2,"<<transactions[index + 2]<<","<<fixed<<setprecision(2)<<updateBank<<endl;    
    }
    updateAccountFile.close();
    bank.close();
}

/*
Function    : isValiDate
Iputs       : splited account vector, date
Output      : true/false
*/
bool isValiDate(vector<string> transDates, string date)
{   //search date is exixt in record
    for (int i = 0; i < transDates.size(); i +=4)
    {
        if(transDates[i]==date)
            return true;
    }
    return false;
}

/*
Function    : showDetails
Iputs       : account vector, transaction vectro
Output      : true/false
*/
bool showDetails(vector<string> accounts, vector<string> transDates)
{
    string date, accNum;
    int count = 0;
    while(count<3)
    {   //The Bank Give 3 chance to input Correct, If you unnable to input correct account this process will be terminate
        cout << "Please Enter The Account Number : ";
        cin >> accNum;
        fstream seeFile(accNum + ".txt");
        if(!seeFile.is_open())  //check from the file liss if the account is exists
        {
            cout << "We haven't your Account! :-( \n please try again\n";
            continue;
        }
        cout << "Ok, We Have Your Account! :-)\n";
        //then we can see details for relavant date
        int dateCount = 0;
        vector<string> details = split_data(accNum + ".txt");
        while(dateCount<3)
        {   //three chances to enter correct date
            cout << "Enter The Date [yyyymmdd -eg: 20210101] : ";
            cin >> date;
            if(isValiDate(details, date))
            {    
                cout << "ok\n";
                
                for (int Idx = 0; Idx < details.size(); Idx +=4)
                {
                    if(details[Idx]==date)
                    {//balance at the end of the day
                        cout << "..........Details At The Beginning Of The Day..........\n";
                        cout << "Date\t\t"<< "Balance\n";
                        cout << details[Idx-4] << "\t" <<details[Idx-1] << "\n";
                        break;
                    }
                }
                cout << "..........Details Of The Day..........\n";
                cout << "Date\t\t"<<"Type\t"<<"Amount\t\t"<< "Balance\n";
                for (int Idx = 0; Idx < details.size(); Idx +=4)
                {   
                    if(details[Idx]==date)  //print all transactions of the day
                        {cout << details[Idx] << "\t"<< details[Idx+1] << "\t"<< details[Idx+2] << "\t\t" <<details[Idx+3] << "\n";}

                    if(details[Idx]==date && details[Idx+4]!=date)
                    {
                        //balance at the end of the day
                        cout << "..........Details At The End Of The Day..........\n";
                        cout << "Date\t\t"<< "Balance\n";
                        cout << details[Idx] << "\t" <<details[Idx+3] << "\n";
                    }
                }
                return true;
            }
            else 
            {
                cout << "Invalid Date! :-( try again\n";    //give another chance to incorrect date input
                continue;
            }
            dateCount++;
        }        
        return true;    //if data entering ok, return true
        count++;
    }
    cout << "Sorry! :=( You Take all Three Chances.\nseems You can't remember the accont number.\n";
    return false;
}

/*
Function    : main
Iputs       : N/A
Output      : N/A
*/
int main()
{
    vector<string> accountNumbers;
    vector<string> transactions;
    accountNumbers=split_data("balance.txt");   //here assume no errors in balance.txt and transaction.txt
    transactions = split_data("transaction.txt");   //that means no missuese values and field in those files
    // for (int i = 0 ; i< accountNumbers.size() ; i++)
        // cout << accountNumbers[i] << endl;
    create_accounts(accountNumbers);
    // transaction and account vector include whole data in a file
    // we can separate each column fro there order
    // eg: transactions[1] is a account number, again we meet account number at transactions[5]

    for (int index = 1; index < transactions.size(); index +=4)
    {
        //account numbers are apear in index+4 order, because transaction field has 4 columns
        //take each transaction according to account number
        //then update data 
        if(transactions[index+1]=="1")
            updateAccounts(index, transactions, 1);

        if(transactions[index+1]=="2")
            updateAccounts(index, transactions, 2);
    }
    //aply interet at the final day
    for (int index = 1; index < accountNumbers.size();index +=3)
    {   //go through all the accounts and update last date
        string file = accountNumbers[index];
        vector<string> acc = split_data(file + ".txt");
        int lastindex = acc.size() - 1;
        interestPaidorCharge(acc[lastindex - 3], file, stof(acc[lastindex]));
    }

    //user's functionality for see the records
    char input;
    cout << "Do You Want To See Details :-)\n[y/n]";
    cin >> input;
    bool status;
    if(tolower(input)=='y')
        status = showDetails(accountNumbers, transactions); //if inputs are correct then you can see details
    else
        cout << "Ok, Thank You :-)\nHave A Nice Day! :-)\n";

    if (status)
        cout << "Have A Nice Day! :-)\n";
    else
        cout << "Please Try Again Later!\nHave A Nice Day! :-)\n";
    return 0;   //end of the program
}