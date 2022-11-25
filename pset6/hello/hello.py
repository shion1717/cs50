# imports the get_string function from CS50 library
from cs50 import get_string

# prompts user for the name
name = get_string("What is your name? \n")

# prints out hello to the user
print("hello, " + name)