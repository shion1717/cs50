# Expense tracker - Manage your finances!
By: Ian Chua from Singapore

## Video Demo: https://www.youtube.com/watch?v=MVRBA-HLnYE

## Description:

This is an expense tracking app which integrates features from Bootstrap (for the aesthetics) and Chart.js (for charting of the user's expenses in a categorical format).
In it, users are able to create profiles to track expenses, and can separately record down expenses meant for different profiles.

Some features are reused from CS50 Finance, like the login authentication and error pages.

### Static folder:

styles.css remains largely untouched from the CS50 Finance app.

### Templates folder:
apology.html: largely untouched from CS50 Finance app. I initially wanted to change it to the Doge meme format, but realise that the memegen host was quite unstable, and there were a few times that the api could not generate an image. Hence I opted to stick with the api from CS50 Finance app.

create.html: Page to allow the user to create a profile and store it in the database.

delete.html: Landing page to let the user confirm that they want to delete the profile.

edit.html: I actually spent quite a bit of time on this, thinking how to pass data from HTML to Flask, and Flask to HTML again, with the constraints of "GET" and "POST" methods. Quite happy with the end result - when the user clicks on "Edit" in the "history.html" page, they are immediately taken to the "record.html" page with pre-filled data.

history.html: The main bulk of time was spent on here, researching how to create charts on HTML pages with dynamic data. Stumbled upon Chart.js, and thankfully with experience in using Bootstrap, the integration was not that hard to implement!

index.html: Nothing special, just list the profiles that the user has access to.

layout.html: Largely adapted from CS50 Finance.

login.html: Largely adapted from CS50 Finance.

record.html: Similar to "edit.html", but when designing this I was thinking what are the ways the user could attack the database, but I decided that the target audience was not malicious, and usage is for their own profiles only, hence I went with minimal defence, but mostly used validation checks.

register.html Largely adapted from CS50 Finance.

### Database design and application.py:
At the start when I had the idea to do an expense tracker, I immediately wanted to implement it without thinking much in the details, like how the database should be designed. This made me waste a lot of time as I had to restructure the database due to evolving needs.
For example, I wanted to develop a feature which allowed one user to have many expense profiles for tracking purposes, and said profiles would contain many transactions associated with it.

In the future, I would probably spend more time at the start thinking of how the database should be structured before diving into the development itself.

As for application.py, it quicky gets hard to navigate as I am containing quite a lot of html pages into one single .py file. Maybe next time I could chop up the code into different segments and store them as separate files for ease of reference.
Maintenance is quite hard as well since some routes could make reference to another route, and feed data to and fro via HTML.