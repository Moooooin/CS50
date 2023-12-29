import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get users stocks and needed attributes
    stocks = []
    stocks_total = 0

    for stock in db.execute(
        "SELECT symbol, amount, buy_price FROM stocks WHERE user_id = ?",
        session["user_id"],
    ):
        price = lookup(stock["symbol"])["price"]
        shares = int(stock["amount"])
        profit = price * shares - stock["buy_price"]
        pColor = ""

        if profit > 0:
            pColor = "green"
        elif profit < 0:
            pColor = "red"
        else:
            pColor = "grey"

        stocks.append(
            {
                "symbol": stock["symbol"],
                "shares": shares,
                "price": price,
                "total": price * shares,
                "profit": profit,
                "color": pColor,
            }
        )

        # Get total stock portfolio value
        stocks_total += price * shares

    # Get total cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
        "cash"
    ]

    # Render HTML with these values
    return render_template(
        "index.html",
        total=(cash + stocks_total),
        stocks_total=stocks_total,
        cash=cash,
        stocks=stocks,
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Get form submissions
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Validate input
        if not shares:
            return apology("Enter the amount of shares", 400)

        try:
            shares = int(shares)
        except ValueError:
            return apology("Cannot buy " + str(shares) + " shares", 400)

        if symbol == None:
            return apology("Enter a stock symbol", 400)
        elif shares < 1:
            return apology("Enter a positive amount of shares", 400)

        stock = lookup(symbol)

        if stock == None:
            return apology("This Stock does not exist", 400)
        else:
            id = session["user_id"]
            price = stock["price"] * shares
            cash = db.execute("SELECT cash FROM users WHERE id = ?", id)[0]["cash"]

            if price > cash:
                return apology("You're too poor to buy this", 400)

            # Check if user already has stock
            currPos = db.execute(
                "SELECT amount, buy_price FROM stocks WHERE user_id = ? AND symbol = ?",
                id,
                symbol,
            )

            # Insert transaction into history
            db.execute(
                "INSERT INTO history (user_id, share_price, symbol, time, buy, total_price, amount) VALUES (?, ?, ?, datetime('now'), true, ?, ?)",
                id,
                stock["price"],
                symbol,
                price,
                shares,
            )

            # Pay
            db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", price, id)

            if len(currPos) > 0:
                # Update position
                db.execute(
                    "UPDATE stocks SET amount = amount + ?, buy_price = buy_price + ? WHERE user_id = ? AND symbol = ?",
                    shares,
                    price,
                    id,
                    symbol,
                )

            else:
                # Get the stock
                db.execute(
                    "INSERT INTO stocks (user_id, symbol, amount, buy_price) VALUES (?, ?, ?, ?)",
                    id,
                    symbol,
                    shares,
                    price,
                )

            # Redirect user to home page
            return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get history
    history = db.execute(
        "SELECT share_price, total_price, symbol, time, buy, amount FROM history WHERE user_id = ?",
        session["user_id"],
    )
    length = len(history)

    for i in range(length):
        if history[i]["buy"]:
            history[i]["type"] = "buy"
            history[i]["color"] = "green"

        else:
            history[i]["type"] = "sell"
            history[i]["color"] = "red"

    return render_template("history.html", transactions=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Lookup stock and display info in quoted
        stock = lookup(request.form.get("symbol"))

        if stock == None:
            return apology("This Stock does not exist", 400)
        else:
            return render_template(
                "quoted.html", price=stock["price"], name=stock["name"]
            )

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Get form submissions
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check if username is available
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) != 0:
            return apology("username taken", 400)

        # Check if passwords match
        if password != confirmation:
            return apology("Passwords don't match", 400)

        # Add user to database
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            username,
            generate_password_hash(password),
        )

        # Remember which user has logged in
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Get form submissions
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Validate input
        if not shares:
            return apology("Enter the amount of shares", 400)

        try:
            shares = int(shares)
        except ValueError:
            return apology("Cannot sell " + str(shares) + " shares", 400)

        if symbol == None:
            return apology("Select a stock symbol", 400)
        elif shares < 1:
            return apology("Enter a positive amount of shares", 400)

        stock = lookup(symbol)

        if stock == None:
            return apology("This Stock does not exist", 400)
        else:
            id = session["user_id"]
            price = stock["price"] * shares

            # Check if user has stock to sell and enough shares
            currPos = db.execute(
                "SELECT amount, buy_price FROM stocks WHERE user_id = ? AND symbol = ?",
                id,
                symbol,
            )

            if len(currPos) < 1:
                return apology("You don't have that stock")
            elif shares > currPos[0]["amount"]:
                return apology("You don't have that many shares")

            # Get the money and sell
            db.execute(
                "INSERT INTO history (user_id, share_price, symbol, time, buy, total_price, amount) VALUES (?, ?, ?, datetime('now'), false, ?, ?)",
                id,
                stock["price"],
                symbol,
                price,
                shares,
            )
            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", price, id)

            if currPos[0]["amount"] == shares:
                # Delete shares from user if sold everything
                db.execute(
                    "DELETE FROM stocks WHERE user_id = ? AND symbol = ?", id, symbol
                )

            else:
                # Update amount and buy_price if not
                new_amount = currPos[0]["amount"] - shares
                new_price = currPos[0]["buy_price"] - price
                db.execute(
                    "UPDATE stocks SET amount = ?, buy_price = buy_price - ? WHERE user_id = ? AND symbol = ?",
                    new_amount,
                    new_price,
                    id,
                    symbol,
                )

        # Redirect user to home page
        return redirect("/")

    else:
        # Get stocks in portfolio
        stocks = db.execute(
            "SELECT symbol FROM stocks WHERE user_id = ?", session["user_id"]
        )
        symbols = []

        # Get symbol of each stock
        for stock in stocks:
            symbols.append(stock["symbol"])

        return render_template("sell.html", symbols=symbols)


@app.route("/watchlist", methods=["GET", "POST"])
@login_required
def watchlist():
    """See Watchlist"""
    if request.method == "POST":
        # Get stock
        symbol = request.form.get("symbol")

        if request.form.get("action") == "add":
            # Check if stock exists
            if lookup(symbol) == None:
                return apology("This stock does not exist", 400)

            # Check if stock isn't already in watchlist
            if (
                len(
                    db.execute(
                        "SELECT * FROM watchlist WHERE user_id = ? AND symbol = ?",
                        session["user_id"],
                        symbol,
                    )
                )
                != 0
            ):
                return apology("This stock is already in your watchlist")

            # Add to watchlist
            db.execute(
                "INSERT INTO watchlist (user_id, symbol) VALUES (?, ?)",
                session["user_id"],
                symbol,
            )

        elif request.form.get("action") == "delete":
            # Delete from Watchlist
            db.execute(
                "DELETE FROM watchlist WHERE user_id = ? AND symbol = ?",
                session["user_id"],
                symbol,
            )

    # Get watchlist
    watchlist = db.execute(
        "SELECT symbol FROM watchlist WHERE user_id = ?", session["user_id"]
    )

    # Get price of stocks in watchlist
    length = len(watchlist)

    for i in range(length):
        watchlist[i]["price"] = lookup(watchlist[i]["symbol"])["price"]

    # Render HTML
    return render_template("watchlist.html", liste=watchlist)
