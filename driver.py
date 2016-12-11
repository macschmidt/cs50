import player
import deck
import dealer
import human_player
import hand
import computer_player

SUITS = ['Hearts', 'Diamonds', 'Clubs', 'Spades']
RANKS = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']

def make_bets(num_players):
    for player in players:
        print("{}:".format(player.name))
        if player.name == "Hal9000":
            player_bet = int(player.balance / 10)
            print("Hal9000 has placed a bet of {} chips.".format(player_bet))
            player.balance -= player_bet
            player.bet = player_bet
            print("Hal9000's balance is now {}.".format(player.balance))
        else:
            while True:
                player_bet = int(input("Please make a bet between 1 and your balance ({}): ".format(player.balance)))
                if player_bet <= player.balance and player_bet > 0:
                    break
                print("Try again.")
            
            player.balance -= player_bet
            player.bet = player_bet
            print("Your balance is now {}.\n".format(player.balance))

def deal_cards(num_players):
    for i in range(2):
        for player in (players + [dan_the_dealer]):
            player.hand.add_card(my_deck.deal_card())

def execute_move(player, choice):
    while choice == "h":
        hit(player)
        player.show_hand()
        if player.hand.get_value() > 21:
            player.isBust = True
            print("{} busts.".format(player.name))
            break
        while True:
            choice = input("Hit (h) or Stand (s): ")
            if choice == "h" or choice == "s":
                break
            print("Try again.")
    
    if choice == "s":
        player.stand()
        
    if choice == "sur":
        player.isSurrender = True
        player.balance += (player.bet - (player.bet / 2))
        print("Your balance is now {}\n".format(player.balance))

    if choice == "d":
        player.balance -= player.bet
        player.bet *= 2
        print("Your balance is now {}\n".format(player.balance))
        hit(player)
        player.show_hand()
        player.check_bust()
    
# pass in player as the argument 
def hit(player):
    print("Hitting...")
    player.hand.add_card(my_deck.deal_card())
    
def get_first_move(player):
    choice = ""
    print(player.name + ":"),
    player.show_hand()
    
    dealer_first_card = dan_the_dealer.hand.__getitem__(0)
    first_card_value = dealer_first_card.point
    
    if player.name == "Dan the Dealer":
        while player.hand.get_value() < 17:
            hit(player)
            player.show_hand()
            print
        player.check_bust()
        
    elif player.name == "Hal9000":
        # call method that figures out Hal's move and return the letter he wants
        player.calculate_move(first_card_value)
    else:
        if player.balance > player.bet:
            while True:
                choice = input("Hit (h), Stand (s), Double Down (d), or Surrender (sur): ")
                if choice == "h" or choice == "s" or choice == "d" or choice == "sur":
                    break
                print("Try again.")
        else:
            while True:
                choice = input("Hit (h), Stand (s), or Surrender (sur): ")
                if choice == "h" or choice == "s" or choice == "sur":
                    break
                print("Try again.")
    return choice
                
if __name__ == '__main__':
    while True:
        num_players = int(input("Welcome to Blackjack! How many human players are there (choose a number between 1-4): "))
        if num_players > 0 and num_players < 5:
            break
        print("Try again.")

    print
    
    while True:
        cpu_yes_no = input("Would you like to play against a computer ('y' for Yes, 'n' for No): ")
        if cpu_yes_no == 'y' or cpu_yes_no == 'n':
            break
        print("Try again.")
    
    while True:
        starting_balance = int(input("How many chips would you like the players to start with? (Max of 1000): "))
        if starting_balance > 0 and starting_balance < 1001:
            break
        print("Try again.")

    # boolean that controls whether the next round happens or not
    next_round = True
    
    # empty list of the players for the game – will fill up with player objects
    players = []
    
    # creating the deck for the game and shuffling it
    my_deck = deck.Deck()
    
    # creating the dealer for the game    
    dealer_hand = hand.Hand()
    dan_the_dealer = dealer.Dealer(dealer_hand, "Dan the Dealer")
    
    # for however many players you start with, add each new player to an array of players 
    for i in range(num_players):
        player_name = input("\nWhat is Player {}'s name?\n".format(i+1))
        new_hand = hand.Hand()
        new_player = human_player.HumanPlayer(player_name, starting_balance, 0, new_hand)
        players.append(new_player)
        
    if cpu_yes_no == 'y':
        cpu_name = "Hal9000"
        cpu_hand = hand.Hand()
        cpu_player = computer_player.ComputerPlayer(cpu_name, starting_balance, 0, cpu_hand)
        players.append(cpu_player)
        print("\nA CPU named Hal9000 has been added to the game.")
            
    # check after every round if the user wants to keep playing
    while next_round:
    
        print("\nRound beginning!\n")
        
        # shuffle the deck
        my_deck.shuffle()
        
        # place initial bets for the hand
        make_bets(num_players)
        
        # deal the cards out for the beginning of the hand
        deal_cards(num_players)
        
        # don't want the second card of the dealer revealed immediately
        dan_the_dealer.hand[1].hide_card()
        print("\nDealer:")
        dan_the_dealer.show_hand()
        print
        dan_the_dealer.hand[1].reveal_card()
        
        # make the first round of moves based on the one card the dealer has played
        for player in players + [dan_the_dealer]:
            first_choice = get_first_move(player)
            execute_move(player, first_choice)
        
        print("\nFinal Result:")
        
        for player in players:
            player.report(player, dan_the_dealer)
        
        for player in players:
            if player.balance == 0:
                print("You're out of chips, {}. Game over!\n".format(player.name))
                players.remove(player)

        # clear the hands so they can play again
        for player in players + [dan_the_dealer]:
            player.hand.clear_hand()
            
        # note: this is the same code as above, but for some reason it doesn't remove multiple users if they lose simultaneously
        #       this fixes that problem for some reason
        for player in players:
            if player.balance == 0:
                print("You're out of chips, {}. Game over!\n".format(player.name))
                players.remove(player)
                
        if len(players) == 0:
            print("No more players in the game. Goodbye!")
            next_round = False
        
        while True:
            play_again = input("Do you want to play another round? Type 'y' for yes, 'n' for no: ")
            if play_again == "y" or play_again == "n":
                break
            print("Try again.")
            
        if play_again == 'n':
            print("\nThank you for trying Mac's CS50 project! Goodbye!")
            next_round = False