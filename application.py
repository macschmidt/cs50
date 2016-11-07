from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Home page stuff."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
    # remember which user has logged in
        id = session["user_id"]
        identification = session["user_id"]
    
        # CALCULATING THE DIFFERENT NUMBER OF STOCKS THE USER HAS
        retabled = db.execute("SELECT symbol, name, SUM(quantity), pricepershare, total FROM transactions GROUP BY name")
        numStocks = len(retabled[0])
        
        # render the template "index" with the embedded necessary values
        return render_template("index.html", retabled = retabled)
    
    else:
        return render_template("index.html")

@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    """Add cash to your account."""
    if request.method == "POST":
        id = session["user_id"]
        
        identification = session["user_id"]
        
        # ensure the user put in a valid amount of cash
        cashtoadd = request.form.get("cashtoadd")
        cashtoadd = float(cashtoadd)
        if cashtoadd < 0:
            return apology("You can't add a negative amount of cash to your account.")
        
        # add to transactions table
        db.execute("INSERT INTO transactions (total, userid) VALUES(:total, :identification)", total = cashtoadd, identification = identification)
        
        # update the user table
        cash = db.execute("SELECT cash FROM users WHERE id = :identification", identification = identification)
        usercash = cash[0]['cash']
        db.execute("UPDATE users SET cash = :cash WHERE id = :identification", cash = usercash + cashtoadd, identification = identification)
        
        # once done, return to the index page
        return redirect(url_for("index"))
    else:
        return render_template("addcash.html")
        
        
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # remember which user has logged in
        id = session["user_id"]
        
        # ensure the user input a stock symbol
        symbol = request.form.get("stocksymbol")
        stockinfo = lookup(symbol)
        if not stockinfo or not symbol:
            return apology("Must provide valid stock symbol.")
            
        # ensure the user input a valid quantity for the stock
        quantity = request.form.get("quantity")
        quantity = int(quantity)
        if quantity < 1:
            return apology("Must enter a positive integer value.")
        
        # then assign the various contents to different variables
        stockname = stockinfo.get('name')
        stockprice = stockinfo.get('price')
        stocksymbol = stockinfo.get('symbol')
            
        # query database for username
        # rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        identification = session["user_id"]
        
        # now, the user has requested a valid stock and a valid number of this stock
        # now we need to check to see if the user CAN buy it before they actually buy it
        cash = db.execute("SELECT cash FROM users WHERE id = :identification", identification = identification)
        usercash = cash[0]['cash']
        
        transactionprice = quantity * stockprice
        
        # if the user can't afford the stock, then return an apology
        if transactionprice > usercash:
            return apology("You can't afford this stock.")
        
        # if the user can afford the stock, then add the stock to transactions
        db.execute("INSERT INTO transactions (symbol, name, quantity, pricepershare, total, userid) VALUES(:symbol, :name, :quantity, :pricepershare, :total, :userid)", symbol = stocksymbol, name = stockname, quantity = quantity, pricepershare = stockprice, total = transactionprice, userid = identification)
        
        # now update the other table that contains how much cash the user has
        db.execute("UPDATE users SET cash = :cash WHERE id = :identification", cash = (usercash - transactionprice), identification = identification)
        
        # once done, return to the index page
        return redirect(url_for("index"))
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    if request.method == "POST":
        
    # remember which user has logged in
        id = session["user_id"]
        identification = session["user_id"]
    
        # CALCULATING THE DIFFERENT NUMBER OF STOCKS THE USER HAS
        retabled = db.execute("SELECT symbol, name, SUM(quantity), pricepershare, total FROM transactions GROUP BY name")
        numStocks = len(retabled[0])
        
        # render the template "index" with the embedded necessary values
        return render_template("index.html", retabled = retabled)
    
    else:
        return render_template("history.html")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # remember which user has logged in
        id = session["user_id"]
        
        # ensure the user input a stock symbol
        stocksymbol = request.form.get("quote")
        stockinfo = lookup(stocksymbol)
        if not stocksymbol or not stockinfo:
            return apology("Must provide a valid stock symbol")
        
        # then assign the various contents to different variables
        stockname = stockinfo.get('name')
        stockprice = stockinfo.get('price')
        stocksymbol = stockinfo.get('symbol')
        
        # now render the template "quoted" with the embedded necessary values
        return render_template("quoted.html", stockname=stockname, stockprice=stockprice, stocksymbol=stocksymbol)
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure the user input a username
        username = request.form.get("username")
        if not username:
            return apology("must provide username")
            
        # query database for username to see if it already exists
        rows = db.execute("SELECT * FROM users WHERE username = :username", username = username)
        
        # if the username already exists in the table, return apology
        if len(rows) > 0:
            return apology("username already exists")

        # ensure both password spaces contain at least something
        passwordone = request.form.get("passwordone")
        passwordtwo = request.form.get("passwordtwo")
        
        if not passwordone or not passwordtwo:
            return apology("must fill both password spaces")
            
        # ensure that the two passwords match
        elif passwordone != passwordtwo:
            return apology("passwords must match")
            
        # encrypt the password
        hashedpassword = pwd_context.encrypt(passwordone)
            
        # insert the new user into users, storing a hash of the user's password
        db.execute("INSERT INTO users (username, hash) VALUES(:username, :hashedpw)", username = username, hashedpw = hashedpassword)
        
        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # remember which user has logged in
        id = session["user_id"]
        
        # ensure the user input a stock symbol
        symbol = request.form.get("stocksymbol")
        stockinfo = lookup(symbol)
        if not stockinfo or not symbol:
            return apology("Must provide valid stock symbol.")
                
        # then assign the various contents to different variables
        stockname = stockinfo.get('name')
        stockprice = stockinfo.get('price')
        stocksymbol = stockinfo.get('symbol')
        
        identification = session["user_id"]
        
        # check transactions to see if they have that stock
        row = db.execute("SELECT quantity FROM transactions WHERE symbol = :symbol AND userid = :userid", symbol = stocksymbol, userid = identification)
        if not row:
            return apology("You do not own this stock.")
            
        quantity = row[0]['quantity']
            
        # ensure the user input a valid quantity for the stock
        quantityrequested = request.form.get("quantity")
        quantityrequested = int(quantityrequested)
        if quantityrequested > quantity:
            return apology("You can't sell more than you own.")

        transactionprice = quantityrequested * stockprice
        
        # add the transaction to the transaction table
        db.execute("INSERT INTO transactions (symbol, name, quantity, pricepershare, total, userid) VALUES(:symbol, :name, :quantity, :pricepershare, :total, :userid)", symbol = stocksymbol, name = stockname, quantity = (-1 * quantityrequested), pricepershare = stockprice, total = transactionprice, userid = identification)

        # update the user table as well with his new found profits
        cash = db.execute("SELECT cash FROM users WHERE id = :identification", identification = identification)
        usercash = cash[0]['cash']
        db.execute("UPDATE users SET cash = :cash WHERE id = :identification", cash = (usercash + transactionprice), identification = identification)
        
        # once done, return to the index page
        return redirect(url_for("index"))
        
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html")