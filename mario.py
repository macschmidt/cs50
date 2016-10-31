import cs50

def main():

    height = 0;

    while True:
        print("Enter a height between 0 and 24: ")
        height = cs50.get_int()
        if (height < 24 and height >= 0):
            break

    # iterate through the rows of the pyramids
    for row in range(1, height + 1):
        spaces = height - row
    
        # print first set of spaces
        for j in range(spaces):
            print(" ", end="")
            
        # print left pyramid
        printPyramid(row)
        
        # print gap between pyramids
        print("  ", end="")
        
        # print right pyramid
        printPyramid(row)
        
        # print new line
        print("")

# print pyramid method d
def printPyramid(row):
    print("#" * row, end="")

if __name__ == "__main__":
    main()