import sqlite3

DB_NAME = "bank.db"


def get_connection():
    return sqlite3.connect(DB_NAME)


def initialize_database():
    conn = get_connection()
    cursor = conn.cursor()

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL
        )
    """)

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS accounts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            balance REAL NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(id)
        )
    """)

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS beneficiaries (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            account_number TEXT NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(id)
        )
    """)

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            sender_id INTEGER NOT NULL,
            receiver_account TEXT NOT NULL,
            amount REAL NOT NULL
        )
    """)

    cursor.execute("SELECT COUNT(*) FROM users")
    user_count = cursor.fetchone()[0]

    if user_count == 0:
        cursor.execute(
            "INSERT INTO users (username, password) VALUES (?, ?)",
            ("alice", "alice123")
        )

        cursor.execute(
            "INSERT INTO users (username, password) VALUES (?, ?)",
            ("bob", "bob123")
        )

        cursor.execute(
            "INSERT INTO accounts (user_id, balance) VALUES (?, ?)",
            (1, 50000)
        )

        cursor.execute(
            "INSERT INTO accounts (user_id, balance) VALUES (?, ?)",
            (2, 25000)
        )

    conn.commit()
    conn.close()


def vulnerable_login(username, password):
    """
    Intentionally vulnerable SQL query for Assignment 3/4.
    DO NOT FIX before the initial SonarQube scan.
    """

    conn = get_connection()
    cursor = conn.cursor()

    query = (
        "SELECT id, username FROM users "
        "WHERE username = '" + username +
        "' AND password = '" + password + "'"
    )

    cursor.execute(query)
    user = cursor.fetchone()

    conn.close()
    return user


def get_account(account_id):
    conn = get_connection()
    cursor = conn.cursor()

    cursor.execute(
        "SELECT id, user_id, balance FROM accounts WHERE id = ?",
        (account_id,)
    )

    account = cursor.fetchone()

    conn.close()
    return account


def update_balance(account_id, new_balance):
    conn = get_connection()
    cursor = conn.cursor()

    cursor.execute(
        "UPDATE accounts SET balance = ? WHERE id = ?",
        (new_balance, account_id)
    )

    conn.commit()
    conn.close()


def add_beneficiary(user_id, name, account_number):
    conn = get_connection()
    cursor = conn.cursor()

    cursor.execute(
        """
        INSERT INTO beneficiaries (user_id, name, account_number)
        VALUES (?, ?, ?)
        """,
        (user_id, name, account_number)
    )

    conn.commit()
    conn.close()


def get_beneficiaries(user_id):
    conn = get_connection()
    cursor = conn.cursor()

    cursor.execute(
        """
        SELECT name, account_number
        FROM beneficiaries
        WHERE user_id = ?
        """,
        (user_id,)
    )

    beneficiaries = cursor.fetchall()

    conn.close()
    return beneficiaries


def record_transaction(sender_id, receiver_account, amount):
    conn = get_connection()
    cursor = conn.cursor()

    cursor.execute(
        """
        INSERT INTO transactions
        (sender_id, receiver_account, amount)
        VALUES (?, ?, ?)
        """,
        (sender_id, receiver_account, amount)
    )

    conn.commit()
    conn.close()
