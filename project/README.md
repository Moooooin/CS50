# Dici

#### Video Demo:  https://youtu.be/I_V59nP1QG4

## Description:

### Index :
#### Every property in the "propertys" table of the database gets listed alongside with some additional information like the price, size, category and location.
#

### Login & Register :
#### You can register with a unique username and password which gets encrypted. All the data is stored in the table "users" in the "dici.db" database.
#

### View :
#### If clicked on a property you navigate to another page with more information about it such as the whole address, number of rooms and the publisher. The Contact button is just there for show and doesn't work.
#

### Filter :
#### Filter propertys to lookup those that fit your criteria (country, category, price, size, etc.) in the filter page. Be sure to spell countries correctly.
##### Login is required.
#

### Add :
#### Upload a picture and fill out the necessary information to upload your property to the database. The image gets stored as a .jpg file in a seperate folder called "propertys" within the "static" folder with the property id as it's name.
##### Login is required.
#

### My Propertys :
#### A list of the propertys you published. See the views you got or delete the property in the modal that opens with a click on the property-card. You can only get views by other users (that are logged in and didn't view the property before). If you delete your property it also will be deleted from the views and history of everybody.
##### Login is required.
#

### Account :
#### A modal where you are able to edit your account information. Changing your username or password, deleting your account or logging out. Your propertys, history and views will also be deleted if you decide to delete your account.
##### Login is required.
#

### History :
#### A list of the propertys you clicked on. Clear your history by clicking the button on top of the list. The history is seperated from your views. Propertys can be deleted from your history but not from your views. And you also can have your own propertys in your history but not in your views.
##### Login is required.
#

### Database (dici.db):
#### A Table users with id, username and hash (encrypted password)
#### A Table propertys with id, user_id, typ (the category), property_size, plot_size, rent (a boolean for the payment option (rent or sale)), rooms (number of rooms), views (number of views), country, city, address, price, currency, unit (measurement in m² or f²)
#### A Table views with property_id and user_id
#### And a Table history (same schema as views)