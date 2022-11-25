from cs50 import get_string
import re

text = get_string("Text: ")

letters = 0
sentences = 0

for i in text:
    if re.search("[a-zA-Z]", i):
        letters += 1

words = len(text.split(" "))

for i in text:
    if i in ["!", "?", "."]:
        sentences += 1

score = 0.0588 * (letters / (words/100)) - 0.296 * (sentences / (words/100)) - 15.8
if score >= 16:
    print("Grade 16+")
elif score < 1:
    print("Before Grade 1")
else:
    score = int(round(score, 0))
    print("Grade " + str(score))
