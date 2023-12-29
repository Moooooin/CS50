import cs50


# Prompt Text
t = cs50.get_string("Text: ")


# Calculate Grade level
avWords = len(t.split()) / 100.0
letters = 0
sentences = 0

for char in t:
    if char.isalpha():
        letters += 1
    elif char == "." or char == "!" or char == "?":
        sentences += 1

grade = round(0.0588 * (letters / avWords) - 0.296 * (sentences / avWords) - 15.8)


# Print grade
if grade < 1:
    print("Before Grade 1")
elif grade > 15:
    print("Grade 16+")
else:
    print("Grade " + str(grade))
