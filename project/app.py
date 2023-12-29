import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from werkzeug.utils import secure_filename

from helpers import apology, login_required, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///dici.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


def allowed_file(filename, allowed_extensions):
    return "." in filename and filename.rsplit(".", 1)[1].lower() in allowed_extensions


@app.route("/", methods=["GET"])
def index():
    """List propertys"""

    # Get propertys
    propertys = db.execute(
        "SELECT propertys.id, propertys.user_id, propertys.typ, propertys.property_size, propertys.plot_size, propertys.rent, propertys.rooms, propertys.views, propertys.country, propertys.city, propertys.address, propertys.price, propertys.unit, propertys.currency, users.username FROM propertys INNER JOIN users ON propertys.user_id = users.id"
    )

    # Get userdata
    try:
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    except:
        user = "Not logged in"

    return render_template("index.html", propertys=propertys, user=user)


@app.route("/history", methods=["GET", "POST"])
@login_required
def map():
    """Show history"""

    # Get userdata
    try:
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    except:
        user = "Not logged in"

    # Get history
    history = db.execute(
        "SELECT * FROM propertys WHERE id IN (SELECT property_id FROM history WHERE user_id = ?)",
        session["user_id"],
    )

    return render_template("history.html", user=user, history=history)


@app.route("/filter", methods=["GET", "POST"])
@login_required
def filter():
    """Apply / Show filter"""

    if request.method == "POST":
        # Get userdata
        try:
            user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        except:
            user = "Not logged in"

        # Get userinput and validate
        country = request.form.get("country")
        category = request.form["category"]
        if category not in [
            "Any",
            "House",
            "Apartment",
            "Duplex",
            "Commercial",
            "Forced Sale",
            "Garage / Parking Plot",
            "Nursing Home",
            "Hotel",
            "Office",
            "Construction Project",
            "Plot",
            "Storeroom",
            "Condominium",
            "Production Shed",
        ]:
            return apology("Don't modify the HTML!", 400)

        minprice = request.form.get("minprice")
        maxprice = request.form.get("maxprice")

        currency = request.form["currency"]
        if currency not in [
            "Any",
            "EUR",
            "USD",
            "CAD",
            "RUB",
            "JPY",
            "AUD",
            "GBP",
            "CHF",
            "CNY",
            "INR",
            "BRY",
        ]:
            return apology("Don't modify the HTML!", 400)

        payment = request.form["payment"]
        if payment not in ["Any", "r", "s"]:
            return apology("Don't modify the HTML!", 400)

        minrooms = request.form.get("rooms")
        minsize = request.form.get("minsize")

        unit = request.form["unit"]
        if unit not in ["Any", "m", "f"]:
            return apology("Don't modify the HTML!", 400)

        try:
            if minprice:
                int(minprice)

            if maxprice:
                int(maxprice)

            if minsize:
                int(minsize)

            if minrooms:
                int(minrooms)

        except ValueError:
            return apology(
                "Enter whole numbers as Min Price, Max Price, Min Rooms, Min Size"
            )

        # Construct query
        query = "SELECT * FROM propertys WHERE id > 0"

        if country:
            query += f" AND country = '{country}'"

        if category != "Any":
            query += f" AND typ = '{category}'"

        if minprice:
            query += f" AND price >= {minprice}"

        if maxprice:
            query += f" AND price <= {maxprice}"

        if currency != "Any":
            query += f" AND currency = '{currency}'"

        if payment != "Any":
            if payment == "r":
                query += f" AND rent = True"
            elif payment == "s":
                query += f" AND rent = False"

        if minrooms:
            query += f" AND rooms >= {minrooms}"

        if minsize:
            query += f" AND property_size >= {minsize}"

        if unit != "Any":
            query += f" AND unit = '{unit}'"

        # Get propertys
        propertys = db.execute(query)

        # Show filteredpropertys
        return render_template("index.html", user=user, propertys=propertys)

    else:
        # Get userdata
        try:
            user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        except:
            user = "Not logged in"

        return render_template("filter.html", user=user)


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    """Show add page and add property"""

    if request.method == "POST":
        # Get user input and validate it
        typ = request.form["category"]
        if typ is None or typ == "Select a category":
            return apology("must select a category", 400)

        if typ not in [
            "House",
            "Apartment",
            "Duplex",
            "Commercial",
            "Forced Sale",
            "Garage / Parking Plot",
            "Nursing Home",
            "Hotel",
            "Office",
            "Construction Project",
            "Plot",
            "Storeroom",
            "Condominium",
            "Production Shed",
        ]:
            return apology("must select a valid category", 400)

        currency = request.form["currency"]
        if currency is None or currency not in [
            "EUR",
            "USD",
            "CAD",
            "RUB",
            "JPY",
            "AUD",
            "GBP",
            "CHF",
            "CNY",
            "INR",
            "BRY",
        ]:
            return apology("must select a provided currency", 400)

        prSize = request.form.get("propertySize")
        plSize = request.form.get("plotSize")
        if prSize is None or plSize is None:
            return apology("please provide the size of the property and plot", 400)

        else:
            try:
                prSize = int(prSize)
                plSize = int(plSize)
            except ValueError:
                return apology("Please provide a whole number as the area", 400)

        m1 = request.form["measurement1"]
        m2 = request.form["measurement2"]
        if m1 is None or m2 is None or m1 not in ["m", "f"] or m2 not in ["m", "f"]:
            return apology("please providevalid measurement units", 400)

        elif m1 != m2:
            return apology("Measurement units don't match", 400)

        rooms = request.form.get("rooms")
        if rooms is None:
            return apology("Please provide a number of rooms", 400)
        else:
            try:
                rooms = int(rooms)
            except ValueError:
                return apology("Please provide a whole number of rooms", 400)

        price = request.form.get("price")
        if price is None:
            return apology("Please provide a price", 400)
        else:
            try:
                price = round(float(price), 2)
            except ValueError:
                return apology("Please provide a valid price", 400)

        payment = request.form["payment"]
        rent = True
        if payment is None or payment not in ["r", "s"]:
            return apology("Must select a valid payment option", 400)

        if payment == "s":
            rent = False
        elif payment == "r":
            rent = True
        else:
            return apology("Must select a valid payment option", 400)

        country = request.form.get("country")
        city = request.form.get("city")
        address = request.form.get("address")
        if (
            country is None
            or country.strip() == ""
            or city is None
            or city.strip() == ""
            or address is None
            or address.strip() == ""
        ):
            return apology("Please provide a valid location", 400)

        if "image" not in request.files:
            return apology("must provide an Image", 400)

        image = request.files["image"]

        # Check if the file has a valid filename
        if image.filename == "":
            return apology("must provide a valid Image", 400)

        allowed_extensions = {"jpg", "jpeg", "png", "gif"}
        if not allowed_file(image.filename, allowed_extensions):
            return apology(
                "Invalid file format. Allowed formats are jpg, jpeg, png, and gif.", 400
            )

        # Add property to database
        property_id = db.execute(
            "INSERT INTO propertys (user_id, typ, property_size, plot_size, rent, rooms, country, city, address, price, unit, currency) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
            session["user_id"],
            typ,
            prSize,
            plSize,
            rent,
            rooms,
            country,
            city,
            address,
            price,
            m1,
            currency,
        )

        # Save property image with the name of property ID and the correct file extension
        image = request.files["image"]
        image.save(f"static/propertys/{property_id}.jpg")

        # Redirect to my Propertys
        return redirect("/myPropertys")

    else:
        # Get userdata
        try:
            user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        except:
            user = "Not logged in"

        # Render page
        return render_template("add.html", user=user)


@app.route("/myPropertys", methods=["GET"])
@login_required
def myPropertys():
    """List my propertys"""

    # Get propertys
    propertys = db.execute(
        "SELECT propertys.id, propertys.user_id, propertys.typ, propertys.property_size, propertys.plot_size, propertys.rent, propertys.rooms, propertys.views, propertys.country, propertys.city, propertys.address, propertys.price, propertys.unit, propertys.currency, users.username FROM propertys INNER JOIN users ON propertys.user_id = users.id  WHERE users.id = ?",
        session["user_id"],
    )

    # Get userdata
    try:
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    except:
        user = "Not logged in"

    return render_template("myPropertys.html", user=user, propertys=propertys)


@app.route("/view", methods=["POST"])
def viewProperty():
    """View Property"""

    if request.method == "POST":
        # Get userdata
        try:
            user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        except KeyError:
            user = "Not logged in"

        # Add view if valid
        immobilie = db.execute(
            "SELECT propertys.id, propertys.user_id, propertys.typ, propertys.property_size, propertys.plot_size, propertys.rent, propertys.rooms, propertys.views, propertys.country, propertys.city, propertys.address, propertys.price, propertys.unit, propertys.currency, users.username FROM propertys INNER JOIN users ON propertys.user_id = users.id  WHERE propertys.id = ?",
            request.form.get("id"),
        )

        if len(immobilie) != 1:
            return apology("Property not found")

        try:
            if session["user_id"]:
                views = db.execute(
                    "SELECT * FROM views WHERE user_id = ? AND property_id = ?",
                    session["user_id"],
                    immobilie[0]["id"],
                )
                history = db.execute(
                    "SELECT * FROM history WHERE user_id = ? AND property_id = ?",
                    session["user_id"],
                    immobilie[0]["id"],
                )

                if (
                    session["user_id"] is not immobilie[0]["user_id"]
                    and len(views) == 0
                    and len(history) == 0
                ):
                    db.execute(
                        "INSERT INTO views (user_id, property_id) VALUES (?, ?)",
                        session["user_id"],
                        immobilie[0]["id"],
                    )
                    db.execute(
                        "UPDATE propertys SET views = views + 1 WHERE id = ?",
                        immobilie[0]["id"],
                    )

                if len(history) == 0:
                    db.execute(
                        "INSERT INTO history (user_id, property_id) VALUES (?, ?)",
                        session["user_id"],
                        immobilie[0]["id"],
                    )

                return render_template("view.html", user=user, property=immobilie[0])

        except KeyError:
            return render_template("view.html", user=user, property=immobilie[0])


@app.route("/deleteH", methods=["POST"])
@login_required
def deleteHistory():
    """Delete History"""

    if request.method == "POST":
        # Delete History
        db.execute("DELETE FROM history WHERE user_id = ?", session["user_id"])

        return redirect("/history")


@app.route("/deleteP", methods=["POST"])
@login_required
def deleteProperty():
    """Delete Property"""

    if request.method == "POST":
        # Get property information and validate
        property_id = request.form.get("id")

        if property_id is None:
            return apology("No property selected", 400)

        immobilie = db.execute("SELECT * FROM propertys WHERE id = ?", property_id)

        if immobilie is None:
            return apology("This Property doesn't exist", 400)

        elif len(immobilie) != 1:
            return apology("Multiple or no propertys with this id", 400)

        elif immobilie[0]["user_id"] is not session["user_id"]:
            return apology("This is not your Property", 400)

        # Delete Property
        db.execute("DELETE FROM views WHERE property_id = ?", property_id)
        db.execute("DELETE FROM history WHERE property_id = ?", property_id)
        db.execute(
            "DELETE FROM propertys WHERE id = ? AND user_id = ?",
            property_id,
            session["user_id"],
        )

        # Delete image
        image = property_id + ".jpg"
        file_path = os.path.join("static/propertys", image)

        if os.path.exists(file_path):
            os.remove(file_path)

            return redirect("/myPropertys")
        else:
            return apology("No image found to delete", 400)


@app.route("/changeP", methods=["POST"])
@login_required
def changePassword():
    """Change Password"""

    if request.method == "POST":
        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide old password", 400)

        # Ensure new password was submitted
        elif not request.form.get("newpassword"):
            return apology("must provide new password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("newpasswordc"):
            return apology("must confirm password", 400)

        # Get form submissions
        password = request.form.get("password")
        new = request.form.get("newpassword")
        confirmation = request.form.get("newpasswordc")

        # Check if passwords match
        if new != confirmation:
            return apology("Passwords don't match", 400)

        # Query database for password
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        if len(rows) != 1:
            return apology("Not logged in", 400)

        # Check password
        elif not check_password_hash(rows[0]["hash"], password):
            return apology("Incorrect Password", 400)

        # Change password
        else:
            db.execute(
                "UPDATE users SET hash = ? WHERE id = ? AND hash = ?",
                generate_password_hash(new),
                session["user_id"],
                rows[0]["hash"],
            )

        # Redirect user to home page
        return redirect("/")


@app.route("/changeU", methods=["POST"])
@login_required
def changeUsername():
    """Change Username"""

    if request.method == "POST":
        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure new username was submitted
        elif not request.form.get("newusername"):
            return apology("must provide new username", 400)

        # Get form submissions
        password = request.form.get("password")
        username = request.form.get("newusername")

        # Query database for password
        hashed = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])

        # Query database for usernames
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(hashed) != 1:
            return apology("Not logged in", 400)

        # Check if username is available
        elif len(rows) != 0:
            return apology("Username taken", 400)

        # Check password
        elif not check_password_hash(hashed[0]["hash"], password):
            return apology("Incorrect Password", 400)

        # Change Username
        else:
            db.execute(
                "UPDATE users SET username = ? WHERE hash = ? AND id = ?",
                username,
                hashed[0]["hash"],
                session["user_id"],
            )

        # Redirect user to home page
        return redirect("/")


@app.route("/deleteA", methods=["POST"])
@login_required
def deleteAccount():
    """Delete Account"""

    if request.method == "POST":
        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Check Password
        hashed = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])

        if len(hashed) != 1:
            return apology("Not logged in", 400)

        elif not check_password_hash(hashed[0]["hash"], request.form.get("password")):
            return apology("Incorrect Password", 400)

        # Userdata
        else:
            ids = db.execute(
                "SELECT id FROM propertys WHERE user_id = ?", session["user_id"]
            )

            # Delete Propertys & history & views
            db.execute(
                "DELETE FROM history WHERE property_id IN (SELECT id FROM propertys WHERE user_id = ?)",
                session["user_id"],
            )
            db.execute(
                "DELETE FROM views WHERE property_id IN (SELECT id FROM propertys WHERE user_id = ?)",
                session["user_id"],
            )
            db.execute("DELETE FROM history WHERE user_id = ?", session["user_id"])
            db.execute("DELETE FROM views WHERE user_id = ?", session["user_id"])
            db.execute("DELETE FROM propertys WHERE user_id = ?", session["user_id"])

            # Delete images
            for id in ids:
                image = str(id["id"]) + ".jpg"
                file_path = os.path.join("static/propertys", image)

                if os.path.exists(file_path):
                    os.remove(file_path)

            # Delete Account
            db.execute(
                "DELETE FROM users WHERE id = ? AND hash = ?",
                session["user_id"],
                hashed[0]["hash"],
            )

        session.clear()
        return redirect("/")


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


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


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
            return apology("Username taken", 400)

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
