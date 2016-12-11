from hand import Hand
from deck import Deck

class Player():
    def __init__(self, name, hand):
        self.hand = hand
        self.name = name
        
    # good for broad player class - dealer and human will both have to do this
    def show_hand(self):
        for card in self.hand:
            print (card.__str__()),
        print

    def get_move(self):
        print("ERROR. This should never appear. This should be overwritten.")
        
    # good for broad player class - dealer, ai, and human will both have to do this
    def stand(self):
        print("{} gets {}. Done.\n".format(self.name, self.hand.get_value()))
    
    # good for broad player class - dealer and human will both have to do this
    def check_bust(self):
        if self.hand.get_value() > 21:
            self.isBust = True
            print("{} busts!".format(self.name))
        else:
            self.stand()
            
        # good for both human and player class
    def report(self, player, dealer):
        if player.isSurrender:
            tag = "Surrenders :/"
        elif player.isBust:
            tag = "Loses :("
        elif (player.hand.__len__) == 2 and player.hand.get_value() == 21:
            tag = "Blackjack!"
        elif dealer.isBust or (player.hand.get_value() > dealer.hand.get_value()):
            tag = "Wins!"
            player.balance += player.bet * 2
        elif player.hand.get_value() == dealer.hand.get_value():
            tag = "Push/Tie"
            player.balance += player.bet
        else:
            tag = "Loses :("
        # this is the output that the user sees after the round ends
        print("{}: {}\n{}'s balance: {}\n".format(player.name, tag, player.name, player.balance))