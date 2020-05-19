#include "AccountList/AccountList.h"
#include <exception>


AccountList::AccountList(const AccountList& other)
	: m_accounts()
{
	CopyAccounts(other.m_accounts);
}

AccountList::~AccountList()
{
	ClearAccounts();
}

AccountList& AccountList::operator=(const AccountList& other)
{
	if (this != &other)
	{
		ClearAccounts();
		CopyAccounts(other.m_accounts);
	}

	return *this;
}



void AccountList::AddAccount(const std::string& ownerID, const AccountType accountType)
{
	Account* temp = nullptr;

	switch (accountType)
	{
		case AccountType::CurrentAccount:
			temp = new CurrentAccount(ownerID);
			break;

		case AccountType::SavingsAccount:
			temp = new SavingsAccount(ownerID);
			break;

		case AccountType::PrivileAccount:
			temp = new PrivilegeAccount(ownerID);
			break;

		default:
			throw std::exception("account addition failed : invalid account type\n");
	}

	//m_accounts.emplace_back(temp->CloneAccount());
	//delete temp;

	// or just that
	m_accounts.emplace_back(temp);
}

void AccountList::DeleteAccount(const std::string& IBAN)
{
	try
	{
		auto accountPosition = GetAccountPosition(IBAN);
		auto accountArrayIndex = accountPosition - m_accounts.begin();

		delete m_accounts[accountArrayIndex];

		m_accounts.erase(accountPosition);
	}
	catch (const std::exception&)
	{
		throw;
	}
}

void AccountList::DeleteAllCustomersAccounts(const std::string& customerID)
{
	for (size_t i = 0; i < m_accounts.size(); i++)
	{
		if (m_accounts[i]->AccountOwnedByCustomer(customerID))
		{
			delete m_accounts[i];

			m_accounts.erase(m_accounts.begin() + i);
		}
	}
}

void AccountList::Clear()
{
	ClearAccounts();
}

void AccountList::WithdrawFromAccount(const std::string& IBAN, double withdrawAmmount)
{
	try
	{
		auto account = GetAccount(IBAN);
		account->Withdraw(withdrawAmmount);
	}
	catch (const std::exception& exception)
	{
		throw;
	}
}

void AccountList::DepositToAccount(const std::string& IBAN, double depotisAmmount)
{
	try
	{
		auto account = GetAccount(IBAN);
		account->Deposit(depotisAmmount);
	}
	catch (const std::exception & exception)
	{
		throw;
	}
}

void AccountList::PrintAccounts() const
{
	if (NoOpenedAccounts())
	{
		throw std::exception("bank has no accounts opened\n");
	}

	std::cout << "printing bank accounts : \n";

	for (const Account* account : m_accounts)
	{
		account->DisplayAccount();
	}
}

void AccountList::PrintCustomerAccounts(const std::string& ownerID) const
{
	if (NoOpenedAccounts())
	{
		throw std::exception("bank has no accounts opened\n");
	}

	std::cout << "printing bank accounts : \n";

	for (const Account* account : m_accounts)
	{
		if (account->AccountOwnedByCustomer(ownerID))
		{
			account->DisplayAccount();
		}
	}
}

bool AccountList::NoOpenedAccounts() const
{
	return m_accounts.empty();
}

Account* AccountList::GetAccount(const std::string& IBAN)
{
	if (NoOpenedAccounts())
	{
		throw std::exception("error : no accounts opened");
	}

	for (Account* account : m_accounts)
	{
		if (account->GetAccountIBAN() == IBAN)
		{
			return account;
		}
	}

	std::string msg = "error : account with IBAN";
	msg.append(IBAN);
	msg.append("does not exist\n");

	throw std::exception(msg.c_str());
}

// private helpers
void AccountList::ClearAccounts()
{
	// deleting the data pointed by the pointers 
	// using a pointer reference to be able to do the assignment to nullptr correctly(maybe unnecessary)
	for (Account*& account : m_accounts)
	{
		delete account;
		account = nullptr;
	}

	// clearing the pointers that the vector holds
	m_accounts.clear();
}

void AccountList::CopyAccounts(const std::vector<Account*>& otherAccounts)
{
	for (const Account* account : otherAccounts)
	{
		m_accounts.emplace_back(account->CloneAccount());
	}
}

std::vector<Account*>::const_iterator AccountList::GetAccountPosition(const std::string& IBAN) const
{
	if (NoOpenedAccounts())
	{
		throw std::exception("error : no accounts opened");
	}

	for (auto it = m_accounts.begin(); it != m_accounts.end(); ++it)
	{
		if ((*it)->GetAccountIBAN() == IBAN)
		{
			return it;
		}
	}

	std::string msg = "error : account with IBAN ";
	msg.append(IBAN);
	msg.append(" does not exist\n");

	throw std::exception(msg.c_str());
}