#include "PrivilegeAccount.h"
#include <exception>

// static member initialization
const double PrivilegeAccount::OVERDRAFT_DEFAULT = 100.0;

PrivilegeAccount::PrivilegeAccount()
	:	Account(),
		overdraft(OVERDRAFT_DEFAULT)
{}

PrivilegeAccount::PrivilegeAccount(const std::string& ownerName)
	:	Account(ownerName),
		overdraft(OVERDRAFT_DEFAULT)
{}

PrivilegeAccount::PrivilegeAccount(const std::string& ownerName, double initialDeposit, double overdraft)
	:	Account(ownerName, initialDeposit),
		overdraft((overdraft >= 0) ? overdraft : OVERDRAFT_DEFAULT)
{}

PrivilegeAccount& PrivilegeAccount::operator=(const PrivilegeAccount& other)
{
	if (this != &other) 
	{
		Account::operator=(static_cast<const Account&>(other));

		overdraft = other.overdraft;
	}

	return *this;
}

const double PrivilegeAccount::GetOverdraft() const
{
	return overdraft;
}

void PrivilegeAccount::IncreaseOverdraft(double overdraftIncrease)
{
	if (overdraftIncrease < 0)
	{
		throw std::exception("error : overdraft increase can\'t be negative value\n");
	}

	overdraft += overdraftIncrease;
}

void PrivilegeAccount::DecreaseOverdraft(double overdraftDecrease)
{
	if (overdraftDecrease < 0)
	{
		throw std::exception("error : overdraft descrease can\'t be negative number\n");
	}

	double decreasedOverdraft = overdraft - overdraftDecrease;

	if (decreasedOverdraft < 0)
	{
		throw std::exception("error : overdraft can\'t become negative number\n");
	}

	overdraft = decreasedOverdraft;
}

void PrivilegeAccount::Deposit(double depositAmmount)
{
	IncreaseBalance(depositAmmount);
}

void PrivilegeAccount::Withdraw(double withdrawAmmount)
{
	// if there is zero balance no withdraw
	if (GetBalance() <= 0)
	{
		throw std::exception("error : negative balance");
	}

	// if balance alone is enough for the withdraw, then just directly decrease from it
	else if (GetBalance() >= withdrawAmmount)
	{
		DecreaseBalance(withdrawAmmount);
	}

	// if balance is not enough for the withdraw and we can also use the overdraft
	else if (GetBalance() + overdraft >= withdrawAmmount) 
	{
		double moneyLackFromBalance = withdrawAmmount - GetBalance();

		DecreaseOverdraft(moneyLackFromBalance);
		DecreaseBalance(withdrawAmmount - moneyLackFromBalance);
	}

	// if balance plus overdraft is not enough to cover the withdraw
	else 
	{
		throw std::exception("error : not enough balace to finish withdraw");
	}
}

void PrivilegeAccount::DisplayAccount(std::ostream& outStream) const
{
	outStream << *this << '\n';
}

int PrivilegeAccount::GetAccountType() const
{
	return static_cast<int>(AccountType::PrivileAccount);
}

Account* PrivilegeAccount::CloneAccount() const
{
	return new PrivilegeAccount(*this);
}

std::ostream& operator<<(std::ostream& outStream, const PrivilegeAccount& privilegedAccount)
{
	outStream << "account type : Privileged Account" << "\n"
		<< "overdraft : " << privilegedAccount.overdraft << "\n";

	outStream << static_cast<const Account&>(privilegedAccount);

	return outStream;
}