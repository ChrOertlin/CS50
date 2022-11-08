# TODO
height = 0
while height < 1 or height > 8:
    try:
        height = int(input("Height: "))
    except ValueError:
        print("This is not an integer")

for i in range(1,height + 1):
    print(" " * (height - i) + "#" * i + "  " + "#" * i)
