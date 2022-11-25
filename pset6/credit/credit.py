from cs50 import get_string


ccno = get_string("Number: ")
if len(ccno) in [13, 15, 16]:

    odd = ""
    even = 0

    for i in range(len(ccno)):
        if len(ccno) % 2 == 0:
            if i % 2 == 0:
                odd = odd + str(int(ccno[i]) * 2)
            else:
                even = even + int(ccno[i])
        else:
            if i % 2 == 1:
                odd = odd + str(int(ccno[i]) * 2)
            else:
                even = even + int(ccno[i])

    for i in range(len(odd)):
        even = even + int(odd[i])

    if even % 10 == 0:
        if ccno[0:2] in ["34", "37"]:
            print("AMEX")
        elif ccno[0:2] in ["51", "52", "53", "54", "55"]:
            print("MASTERCARD")
        elif ccno[0] == "4":
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")
else:
    print("INVALID")
