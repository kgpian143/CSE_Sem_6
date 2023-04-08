largest = None
smallest = None
while True:
    num = input("Enter a number: ")
    if num == "done":
        break
    
    try:
        int(num)
    except:
        print("Invalid input")
        continue

    if largest is None:
        largest = int(num)
    if smallest is None:
        smallest = int(num)
        
    if ( largest < int(num) ):
        largest = int(num)
    if ( smallest > int(num) ):
        smallest = int(num)


print("Maximum", largest)
print("Minimum", smallest)