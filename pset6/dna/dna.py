import csv
import sys


def main():
    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    database = []

    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            database.append(row)

    with open(sys.argv[2], "r") as file:
        sequence = file.readline()

    sequence_dict = {}

    strs = list(database[1].keys())[1:]
    for i in strs:
        sequence_dict[i] = str(chk_max_repeat(i, sequence))

    for i in range(len(database)):
        if sequence_dict.items() < database[i].items():
            return(database[i]['name'])
    return("No match")


# Function takes in two arguments, the string to compare, and the DNA sequence
# Returns the maximum STR
def chk_max_repeat(string, sequence):
    list_string = [u for x in sequence.split(string) for u in (x, string) if u]

    counter = 0
    counter_list = []

    for i in list_string:
        if i == string:
            counter += 1
        else:
            counter_list.append(counter)
            counter = 0

    counter_list.append(counter)

    return(max(counter_list))


print(main())