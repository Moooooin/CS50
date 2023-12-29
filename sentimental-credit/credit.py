import cs50


def main():
    n = getNumber()
    firstInts = int(str(n)[:2])
    length = len(str(n))

    checkSum = getCheckSum(n)

    result = checkDN(length, firstInts, checkSum)
    print(result)


def getCheckSum(n):
    c = str(n)
    mDigits = c[-2::-2]
    nDigits = c[-1::-2]
    checkSum = 0

    for digit in mDigits:
        digit = int(digit)
        if digit * 2 < 9:
            checkSum += digit * 2
        else:
            checkSum += digit * 2 % 10 + 1

    for digit in nDigits:
        checkSum += int(digit)

    return checkSum


def getNumber():
    number = 0
    while number < 1:
        number = cs50.get_int("Credit card number: ")

    return number


def checkDN(l, f, checkSum):
    if checkSum % 10 == 0:
        # Check digits and start-numbers
        if l == 15 and (f == 34 or f == 37):
            return "AMEX"
        elif l == 16 and f <= 55 and f >= 51:
            return "MASTERCARD"
        elif (l == 13 or l == 16) and f <= 49 and f >= 40:
            return "VISA"
        else:
            return "INVALID"
    else:
        return "INVALID"


if __name__ == "__main__":
    main()
