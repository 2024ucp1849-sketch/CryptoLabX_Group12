import logging

from database import (
    get_account,
    update_balance,
    add_beneficiary,
    get_beneficiaries,
    record_transaction
)


# Intentionally insecure logging configuration for the assignment.
logging.basicConfig(
    filename="../outputs/banking.log",
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)

logger = logging.getLogger(__name__)


def check_balance(user_id, requested_account_id):
    """
    Intentionally vulnerable to Broken Access Control.

    The function retrieves the requested account but does not verify
    that the account belongs to the currently logged-in user.
    """

    account = get_account(requested_account_id)

    if account is None:
        print("Account not found.")
        return

    account_id, owner_id, balance = account

    # Missing authorization/ownership check.
    print(f"Account ID: {account_id}")
    print(f"Balance: ₹{balance:.2f}")

    # Intentionally sensitive information in logs.
    logger.info(
        "Balance inquiry: user_id=%s, account_id=%s, owner_id=%s, balance=%.2f",
        user_id,
        account_id,
        owner_id,
        balance
    )


def transfer_funds(user_id, receiver_account, amount):
    """Transfer money from the logged-in user's account."""

    if amount <= 0:
        print("Amount must be greater than zero.")
        return False

    sender_account = get_account(user_id)

    if sender_account is None:
        print("Sender account not found.")
        return False

    account_id, owner_id, balance = sender_account

    if amount > balance:
        print("Insufficient balance.")
        return False

    new_balance = balance - amount
    update_balance(account_id, new_balance)

    record_transaction(
        user_id,
        receiver_account,
        amount
    )

    # Intentionally exposes sensitive transaction information.
    logger.info(
        "Transfer completed: sender_user=%s, receiver_account=%s, amount=%.2f, remaining_balance=%.2f",
        user_id,
        receiver_account,
        amount,
        new_balance
    )

    print("Transfer successful.")
    print(f"Transferred: ₹{amount:.2f}")
    print(f"Remaining balance: ₹{new_balance:.2f}")

    return True


def add_new_beneficiary(user_id, name, account_number):
    """Add a beneficiary for the logged-in user."""

    if not name or not account_number:
        print("Beneficiary information cannot be empty.")
        return False

    add_beneficiary(
        user_id,
        name,
        account_number
    )

    logger.info(
        "Beneficiary added: user_id=%s, name=%s, account_number=%s",
        user_id,
        name,
        account_number
    )

    print("Beneficiary added successfully.")
    return True


def view_beneficiaries(user_id):
    """Display beneficiaries belonging to the logged-in user."""

    beneficiaries = get_beneficiaries(user_id)

    if not beneficiaries:
        print("No beneficiaries found.")
        return

    print("\nYour Beneficiaries:")

    for number, beneficiary in enumerate(beneficiaries, start=1):
        name, account_number = beneficiary

        print(
            f"{number}. {name} - Account: {account_number}"
        )


def logout():
    """Logout message."""

    print("\nYou have been logged out.")
