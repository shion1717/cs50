import os
import requests
import json

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import date

from helpers import apology, login_required

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

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///expense_tracker.db")

@app.route("/")
@login_required
def index():
    """Display profiles created / have access to"""
    profiles = db.execute(
        "SELECT tables.tableid, tablename, type FROM access LEFT JOIN tables ON access.tableid = tables.tableid WHERE userid = ?", session["user_id"])

    return render_template("index.html", profiles=profiles)

@app.route("/create", methods=["GET", "POST"])
@login_required
def create():

    #Route if user submitted a form to create profile
    if request.method == "POST":
        profilename = request.form.get("profilename").strip()
        profiletype = "OWNER"

        #Checks for valid profile name
        if not profilename:
            return apology("Please enter a profile name")

        #Check if profile name has 20 characters or less
        if(len(profilename)>21):
            return apology("Please enter a shorter username")

        #Check if the user already has the same profile name
        existingprofiles = []
        profiles = db.execute("SELECT tablename FROM access LEFT JOIN tables WHERE userid = ?", session["user_id"])
        for dict in profiles:
            existingprofiles.append(dict['tablename'])

        if profilename in existingprofiles:
            return apology("Profile already exists")

        #If all ok, create profile
        rowid = db.execute("INSERT INTO tables (tablename) VALUES(?)", profilename)
        db.execute("INSERT INTO access (userid, tableid, type) VALUES(?,?,?)", session["user_id"],rowid,profiletype)

        #Redirect back to index page
        return redirect("/")

    else:
        return render_template("create.html")

@app.route("/record", methods=["GET", "POST"])
@login_required
def record():
    if request.method == "POST":

        #Route if user submitted a transaction
        txndate = request.form.get("txndate")
        txncat = request.form.get("category")
        shortdesc = request.form.get("shortdesc")
        txnamount = request.form.get("amount")
        tableid = request.form.get("tableid")

        #Check if user submitted a category that is already in a predefined list
        categorylist = []
        dbcategories = db.execute("SELECT categories FROM expensecategories")
        for cat in dbcategories:
            categorylist.append(cat['categories'])
        if not txncat in categorylist:
            return apology("Please select a category from the dropdown")

        #Check user access
        if not checktaccess(tableid):
            return apology("Unauthorised access")

        #If all ok, record transaction into database
        db.execute("INSERT INTO transactions (tableid,date_time,amount,category,description) VALUES(?,?,?,?,?)", tableid,txndate,txnamount,txncat,shortdesc)
        profile = db.execute("SELECT tablename FROM tables WHERE tableid = ?", tableid)[0]['tablename']

        #Redirect to transaction history page
        return redirect(url_for('history', tableid=tableid, profile=profile))

    else:
        #Route for user to submit a transaction
        tableid=request.args.get("tableid")
        profilename=request.args.get("profile")

        #Check user access
        if not checktaccess(tableid):
            return apology("Unauthorised access")

        #Provide dropdown list and set default date
        categories = db.execute("SELECT categories FROM expensecategories")
        today = date.today()
        return render_template("record.html", today=today, categories=categories, tableid=tableid, profilename=profilename)

@app.route("/edit", methods=["GET", "POST"])
@login_required
def edit():
    if request.method == "POST":

    #Route if user submits the form
        txnid = request.form.get("txnid")
        txndate = request.form.get("txndate")
        txncat = request.form.get("category")
        shortdesc = request.form.get("shortdesc")
        txnamount = request.form.get("amount")
        tableid = request.form.get("tableid")

        #Check user access
        if not checktaccess(tableid):
            return apology("Unauthorised access")

        #Check if user submitted a category that is already in a predefined list
        categorylist = []
        dbcategories = db.execute("SELECT categories FROM expensecategories")
        for cat in dbcategories:
            categorylist.append(cat['categories'])
        if not txncat in categorylist:
            return apology("Please select a category from the dropdown")

        #If all ok, update the edited transaction and return to transaction history
        db.execute("UPDATE transactions SET date_time=?,amount=?,category=?,description=? WHERE id = ?", txndate,txnamount,txncat,shortdesc,txnid)
        profile = db.execute("SELECT tablename FROM tables WHERE tableid = ?", tableid)[0]['tablename']

        return redirect(url_for('history', tableid=tableid, profile=profile))

    else:

        #Route for user to edit a transaction
        txnid=request.args.get("txnid")
        tableid = db.execute("SELECT tableid FROM transactions WHERE id = ?", txnid)[0]['tableid']

        #Check user access
        if not checktaccess(tableid):
            return apology("Unauthorised access")

        txndata = db.execute("SELECT * FROM transactions WHERE id =?",txnid)[0]
        categories = db.execute("SELECT categories FROM expensecategories")
        return render_template("edit.html", txndata=txndata, categories=categories)


@app.route("/history", methods=["GET"])
@login_required
def history():

    #Get tableid and check access
    tableid=request.args.get("tableid")
    profilename=request.args.get("profile")
    if not checktaccess(tableid):
        return apology("Unauthorised access")

    #Retrieve transactions for the tableid
    transactions = db.execute("SELECT id,date_time,amount,category,description FROM transactions WHERE tableid = ? ORDER BY date_time DESC, category", tableid)

    #To return a list of categories for chart visualisation
    categorylist = []
    dbcategories = db.execute("SELECT categories from expensecategories")
    for cat in dbcategories:
        categorylist.append(cat['categories'])

    data = [0]*len(categorylist)
    for i in range(len(categorylist)):
        for dict in transactions:
            if dict['category'] == categorylist[i]:
                data[i] = data[i] + float(dict['amount'])

    #Clean up categories with 0 balance
    nonzeroindexes = []
    for i in range(len(data)):
        if not data[i] == 0:
            nonzeroindexes.append(i)
    nonzerocat = []
    nonzerodata = []
    for i in nonzeroindexes:
        nonzerocat.append(categorylist[i])
        nonzerodata.append(data[i])


    categorylist = "['"+"', '".join(categorylist)+"']"
    for dict in transactions:
        dict['amount'] = usd(dict['amount'])
    return render_template("history.html", transactions=transactions,profilename=profilename,data=nonzerodata,categorylist=nonzerocat)

@app.route("/delete", methods=["GET", "POST"])
@login_required
def delete():
    if request.method == "POST":

        #Check user access to the tableid
        tableid=request.form.get("tableid")
        if not checktaccess(tableid):
            return apology("Unauthorised access")

        #Delete all records pertaining to the tableid
        db.execute("DELETE FROM tables WHERE tableid = ?", tableid)
        db.execute("DELETE FROM transactions WHERE tableid = ?", tableid)
        db.execute("DELETE FROM access WHERE tableid = ?", tableid)

        return redirect("/")


    if request.method == "GET":
        #Render a landing page to make the user confirm the deletion
        tableid=request.args.get("tableid")
        profilename=request.args.get("profile")
        if not checktaccess(tableid):
            return apology("Unauthorised access")

        return render_template("delete.html", tableid=tableid, profile=profilename)


@app.route("/register", methods=["GET", "POST"])
def register():
    #Register user

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        userlist = []
        userdict = db.execute("SELECT username FROM users")
        for dict in userdict:
            userlist.append(dict['username'])

        #Check valid username
        if not username:
            return apology("Please enter a username")
        if not password:
            return apology("Please enter a password")
        if not confirmation:
            return apology("Please confirm your password")
        if username in userlist:
            return apology("Username already exists")
        if not password == confirmation:
            return apology("Passwords do not match")

        pwhash = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, pwhash)

        return redirect("/")

    else:
        return render_template("register.html")

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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


def checktaccess(tableid):
    #Function to check that the current user logged in has access to the table s/he wants to modify
    tableaccess = []
    temp = db.execute("SELECT tableid FROM access WHERE userid = ?", session["user_id"])
    for id in temp:
        tableaccess.append(id['tableid'])
    return tableid in tableaccess

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"