import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # Add the user's entry into the database
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        if month is not None and day is not None and name is not None:
            # Convert birthday to number
            isNumber = True

            try:
                month = int(month)
                day = int(day)
            except ValueError:
                isNumber = False


            # Add to database if input is valid
            if month <= 12 and month >= 1 and day <= 31 and day >= 1 and isNumber:
                db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)


        return redirect("/")

    else:

        # Display the entries in the database on index.html
        return render_template("index.html", birthdays=db.execute("SELECT name, month, day FROM birthdays;"))
