import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd, initHistory

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    initHistory()

    cashTotal = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])[0]["cash"]

    rows = db.execute("SELECT stockPurchased, SUM(shares) AS shares FROM history WHERE user_id = :user_id GROUP BY stockPurchased HAVING SUM(shares) > 0",
                        user_id=session["user_id"])

    # append to stockInfo all the table data about portfolio
    stockInfo = []
    cashBalance = 0
    for row in rows:
        row["price"] = lookup(row["stockPurchased"])["price"]
        row["valueStock"] = usd(row["price"] * row["shares"])
        cashBalance += row["price"] * row["shares"]
        stockInfo.append(row)

    cashBalance += cashTotal

    return render_template("index.html", stockInfo = stockInfo, cashTotal=usd(cashTotal), cashBalance=usd(cashBalance))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # validate symbol
        if not request.form.get("symbol"):
            return apology("Must provide a symbol")

        stock = lookup(request.form.get("symbol"))

        if stock == None:
            return apology("Sorry, could not find.")

        # validate shares
        if not request.form.get("shares"):
            return apology("Must provide a positive integer of shares.")

        shares = int(request.form.get("shares"))

        if type(shares) != int or shares <= 0:
            return apology("Must provide a positive integer of shares.")

        # check if enough cash
        cashNeeded = stock["price"] * shares
        cashAvailable = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])[0]["cash"]

        if cashAvailable > cashNeeded:
            # create history table
            initHistory()

            # record purchase in history and update cash in users table
            purchase = db.execute("INSERT into history (user_id, cashSpent, cashTotal, stockPurchased, shares, datetime) VALUES (:user_id, :cashSpent, :cashTotal, :stockPurchased, :shares, :datetime)",
                                    user_id=session["user_id"], cashSpent=cashNeeded, cashTotal=cashAvailable-cashNeeded, stockPurchased=stock["symbol"], shares=shares, datetime=datetime.datetime.now())
            update = db.execute("UPDATE users SET cash = :cashAvailable WHERE id = :id", cashAvailable=cashAvailable-cashNeeded, id=session["user_id"])
            return redirect("/")

        else:
            return apology("Sorry, you dont have enough cash for this.")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute("SELECT stockPurchased, shares, cashSpent, round(cashTotal, 2), datetime FROM history WHERE user_id=:user_id",
                        user_id = session["user_id"])

    return render_template("history.html", rows=rows)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    # as POST
    if request.method == "POST":

        # must provide symbol
        if not request.form.get("symbol"):
            return apology("Must provide symbol")

        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Sorry, could not find.")

        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/changePassword", methods=["GET", "POST"])
@login_required
def changePassword():
    """Change passord"""
    # submitted as POST
    if request.method == "POST":
        # require old password and new password, confirmation
        if not request.form.get("oldPassword") or not request.form.get("newPassword") or not request.form.get("confirmation"):
            return apology("Provide old password, new password and confirm it.", 403)

        # query for password-hash
        rows = db.execute("SELECT * FROM users WHERE id = :id",
                          id=session["user_id"])

        # check if old password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("oldPassword")):
            return apology("Sorry, username is not correct.", 403)

        # check if new password and confirmation is the same
        if request.form.get("newPassword") != request.form.get("confirmation"):
            return apology("New password and confirmation are not the same.", 403)

        # update the hash
        user = db.execute("UPDATE users SET hash=:newHash WHERE id=:id",
                            newHash = generate_password_hash(request.form.get("newPassword")),
                            id = session["user_id"])

        # Redirect user to login page
        return redirect("/login")

    else:
        return render_template("changePassword.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # submitted as POST
    if request.method == "POST":
        # require username: not blank, not existing yet
        if not request.form.get("username"):
            return apology("invalid username", 403)

        # query for username: if taken apology
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if len(rows) != 0:
            return apology("username already taken, choose different")

        # require password, same as confirmation
        if not request.form.get("password") or request.form.get("password") != request.form.get("confirmation"):
            return apology("Provide a password and confirm it!")

        # insert the new user into users, hash the password
        user = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                            username = request.form.get("username"),
                            hash = generate_password_hash(request.form.get("password")))

        # Redirect user to login page
        return redirect("/login")

    # user reached route via GET:
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # validate symbol
        if not request.form.get("symbol"):
            return apology("Provide a symbol")
        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if stock == None:
            return apology("Sorry, could not found.")

        # validate shares
        if not request.form.get("shares"):
            return apology("Provide how many shares you sell.")

        shares = int(request.form.get("shares"))

        # check if enough shares
        sharesAvailable = db.execute("SELECT sum(shares) AS shares FROM history WHERE user_id=:user_id AND stockPurchased=:symbol GROUP BY stockPurchased",
        user_id=session["user_id"], symbol=symbol)[0]["shares"]

        if sharesAvailable >= shares:
            price = stock["price"]
            cashSpent = price * shares
            cashAvailable = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])[0]["cash"]

            sell = db.execute("INSERT into history (user_id, cashSpent, cashTotal, stockPurchased, shares, datetime) VALUES (:user_id, :cashSpent, :cashTotal, :stockPurchased, :shares, :datetime)",
                                    user_id=session["user_id"], cashSpent=-cashSpent, cashTotal=cashAvailable+cashSpent, stockPurchased=stock["symbol"], shares=-shares, datetime=datetime.datetime.now())
            update = db.execute("UPDATE users SET cash = :cashAvailable WHERE id = :id",
                                cashAvailable=cashAvailable+cashSpent, id=session["user_id"])

            return redirect("/")

        else:
            return apology("Sorry, you dont have enough stocks.")

    else:
        # query for non-zero sum stocks
        symbols = db.execute("SELECT stockPurchased, SUM(shares) FROM history WHERE user_id = :user_id GROUP BY stockPurchased HAVING sum(shares) > 0",
                            user_id=session["user_id"])

        return render_template("sell.html", symbols=symbols)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)