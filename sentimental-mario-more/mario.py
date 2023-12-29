import cs50


def main():
    height = 0

    while height < 1 or height > 8:
        height = cs50.get_int("Height: ")

    draw(height)


def draw(n):
    for i in range(1, n + 1):
        print(" " * (n - i), end="")
        print("#" * i, end="")
        print("  ", end="")
        print("#" * i)


if __name__ == "__main__":
    main()
