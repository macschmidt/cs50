from hand import Hand
from player import Player

class Dealer(Player):
    def __init__(self, hand, name):
        dealer_hand = Hand()
        self.hand = hand
        self.name = name
        self.isBust = False
        
    def get_move(self):
        while dealer_hand.get_value() < 17:
            return "h"