from player import Player
from dealer import Dealer

class HumanPlayer(Player):
    def __init__(self, name, balance, bet, hand):
        super(Player, self).__init__()
        self.name = name
        self.balance = balance
        self.bet = bet
        self.hand = hand
        self.isBust = False
        self.isSurrender = False

    # returns the user's name (not necessary, left in for reminder)
    def get_name(self):
        return self.name
    
    # returns the user's balance (not necessary, left in for reminder)
    def get_balance(self):
        return self.balance
    
    # returns the player's bet (not necessary, left in for reminder)
    def get_bet(self):
        return self.bet
        
    # gets the move from the human user - specific to the human user in that cpu doesn't need to type it in
    def get_move(self):
        if self.balance > self.bet:
            while True:
                choice = input("\nHit (h), Stand (s), Double Down (d), or Surrender (su): ")
                if choice == "h" or choice == "s" or choice == "d" or choice == "su":
                    break
                print("Try again.")
        else:
            while True:
                choice = input("\nHit (h), Stand (s), or Surrender (su): ")
                if choice == "h" or choice == "s" or choice == "su":
                    break
                print("Try again.")
        return choice