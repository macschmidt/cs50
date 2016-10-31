import cs50

CENTS_PER_DOLLAR = 100

coinValues = [25, 10, 5, 1]

while True:
    print("How much change is owed?")
    dollars = cs50.get_float();
    if dollars > 0:
        break

cents = round(dollars * CENTS_PER_DOLLAR)

coins = 0

# continously check to remove a coin as long as there are some cents left
for currentCoin in coinValues:
    # remove this coin value as long as the current cents is greater than the coin
    while cents >= currentCoin:
        # remove the coin value from the cents total
        cents -= currentCoin
        # add one to the number of coins to return
        coins += 1

print(coins)