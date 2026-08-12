from database import initialize_database, vulnerable_login
from banking import (
    check_balance,
    transfer_funds,
    add_new_beneficiary,
    view_beneficiaries,
    logout
)


def login():
    print("\n========== LOGIN ==========")

    username = input("Username: ")
    password = input("Password: ")

    user = vulnerable_login(username, password)

    if user is None:
        print("Invalid username or password.")
        return None

    user_id, logged_username = user

    print(f"\nLogin successful. Welcome, {logged_username}!")
    return user_id


def banking_menu(user_id):
    while True:
        print("\n========== ONLINE BANKING ==========")
        print("1. Check Balance")
        print("2. Transfer Funds")
        print("3. Add Beneficiary")
        print("4. View Beneficiaries")
        print("5. Logout")

        choice = input("Enter your choice: ")

        if choice == "1":
            account_id = input("Enter account ID: ")

            try:
                account_id = int(account_id)
                check_balance(user_id, account_id)
            except ValueError:
                print("Invalid account ID.")

        elif choice == "2":
            receiver_account = input("Receiver account number: ")
            amount_input = input("Amount: ")

            try:
                amount = float(amount_input)
                transfer_funds(
                    user_id,
                    receiver_account,
                    amount
                )
            except ValueError:
                print("Invalid amount.")

        elif choice == "3":
            name = input("Beneficiary name: ")
            account_number = input("Beneficiary account number: ")

            add_new_beneficiary(
                user_id,
                name,
                account_number
            )

        elif choice == "4":
            view_beneficiaries(user_id)

        elif choice == "5":
            logout()
            break

        else:
            print("Invalid choice.")


def main():
    initialize_database()

    while True:
        print("\n================================")
        print("       ONLINE BANKING SYSTEM")
        print("================================")
        print("1. Login")
        print("2. Exit")

        choice = input("Enter your choice: ")

        if choice == "1":
            user_id = login()

            if user_id is not None:
                banking_menu(user_id)

        elif choice == "2":
            print("Thank you for using Online Banking.")
            break

        else:
            print("Invalid choice.")


if __name__ == "__main__":
    main()
