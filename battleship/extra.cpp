// Arturo Pedraza González
// Santiago Vargas Rugeles A01647287
// Bruno Fernando Posada Lomelí
// Andrés Ramírez Ruiz A01647072

#include <iostream>
#include <string>

using namespace std;

class Account {
private:
  // Atributos Class Account
  string account_number;
  string account_holder;
  float balance;

public:
  // Constructor
  Account(string acc_number, string acc_holder, float initial_balance)
      : account_number(acc_number), account_holder(acc_holder),
        balance(initial_balance) {}

  // Metodo Deposito
  void deposit(float amount) {
    balance += amount;
    cout << "Depositado $" << amount << " a la cuenta " << account_number
         << endl;
  }

  // Metodo Retiro
  void withdraw(float amount) {
    if (amount <= balance) {
      balance -= amount;
      cout << "Retiro de $" << amount << " de la cuenta " << account_number
           << endl;
    } else {
      cout << "Balance Insuficiente!" << endl;
    }
  }

  // Metodo Info de cuenta
  void display_account_info() {
    cout << "Cuenta: " << account_number << "\nTitular: " << account_holder
         << "\nBalance: " << balance << "\n"
         << endl;
  }

  // Getter retorno balance
  float get_balance() { return balance; }

  // Modificar el balance
  void set_balance(float new_balance) { balance = new_balance; }

  // Getter para el numero de cuenta
  string get_account_number() { return account_number; }
};

class Transaction {
private:
  // Atributos Class Transaction
  Account *sender_account;
  Account *receiver_account;
  float amount;

public:
  // Constructor
  Transaction(Account *sender, Account *receiver, float amt)
      : // Constructor
        sender_account(sender), receiver_account(receiver), amount(amt) {}

  void process_transaction() {
    if (amount <= sender_account->get_balance()) {
      sender_account->set_balance(sender_account->get_balance() - amount);
      receiver_account->set_balance(receiver_account->get_balance() + amount);
      cout << "Se transfirio $" << amount << " de la cuenta "
           << sender_account->get_account_number() << " a la cuenta "
           << receiver_account->get_account_number() << endl;
    } else {
      cout << "Transaccion fallida: fondos insuficientes en la cuenta "
           << sender_account->get_account_number() << endl;
    }
  }

  // Metodo para mostrar informacion de la transaccion
  void display_transaction_info() {
    cout << "Transaccion:\n"
         << "Remitente: " << sender_account->get_account_number() << "\n"
         << "Destinatario: " << receiver_account->get_account_number() << "\n"
         << "Monto: $" << amount << "\n"
         << endl;
  }
};

int main() {
  // Crear cuentas
  Account acc1("12345", "Arturo Pedraza", 800.0);
  Account acc2("67890", "Santiago Vargas", 600.0);

  // Informacion Cuentas
  cout << "Informacion inicial de las cuentas:" << endl;
  acc1.display_account_info();
  acc2.display_account_info();

  // Transaccion
  Transaction t1(&acc1, &acc2, 200.0);
  t1.process_transaction();
  t1.display_transaction_info();

  // Informacion Actualizada
  cout << "Informacion final de las cuentas:" << endl;
  acc1.display_account_info();
  acc2.display_account_info();

  return 0;
}
