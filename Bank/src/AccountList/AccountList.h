#pragma once

#ifndef ACCOUNT_LIST_H
#define ACCOUNT_LIST_H

#include "Account/Account.h"
#include "Accounts/CurrentAccount.h"	
#include "Accounts/SavingsAccount.h"
#include "Accounts/PrivilegeAccount.h"
#include <vector>

class AccountList
{
public:
	AccountList() = default;
	AccountList(const AccountList& other);
	~AccountList();
	AccountList& operator=(const AccountList& other);

	void AddAccount(Account* account);
	void DeleteAccount(const std::string& IBAN);
	void DeleteAllCustomersAccounts(const std::string& customerID);
	void Clear();

	void WithdrawFromAccount(const std::string& IBAN, double withdrawAmmount);
	void DepositToAccount(const std::string& IBAN, double depotisAmmount);
	
	void PrintAccounts() const;
	void PrintCustomerAccounts(const std::string& ownerID) const;
	bool NoOpenedAccounts() const;

private:
	void ClearAccounts();
	void CopyAccounts(const std::vector<Account*>& otherAccounts);

	std::vector<Account*>::const_iterator GetAccountPosition(const std::string& accountIBAN) const;
	Account* GetAccount(const std::string& IBAN);

private:
	std::vector<Account*> m_accounts;
};

#endif