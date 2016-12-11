from player import Player
from dealer import Dealer

class ComputerPlayer(Player):
    def __init__(self, name, balance, bet, hand):
        super(Player, self).__init__()
        self.name = name
        self.balance = balance
        self.bet = bet
        self.hand = hand
        self.isBust = False
        self.isSurrender = False
        
    # credit to this website for helping with this code: http://www.x121.com/gamblingstrategy/classic-blackjack-gold.gif
    # calculate which move to make next
    def calculate_move(self, first_card_value):
        choice = ""
        cpu_hand_value = self.hand.get_value()
        # rows 17-18, columns 2-A
        if cpu_hand_value >= 17:
            choice = "s"
        # rows 5-8, columns 2-A
        elif cpu_hand_value >= 4 and cpu_hand_value <= 8:
            choice = "h"
        # row 12, columns 2-3
        elif cpu_hand_value == 12 and (first_card_value == 2 or first_card_value == 3):
            choice = "h"
        # rows 12-18, columns 2-6
        elif cpu_hand_value >= 12 and first_card_value <= 6:
            choice = "s"
        # rows 9-11, columns 2-6
        elif cpu_hand_value >= 9 and cpu_hand_value <=11 and first_card_value >= 2 and first_card_value <= 6:
            choice = "d"
        # rows 10-11, columns 7-9
        elif cpu_hand_value == 10 or cpu_hand_value == 11 and first_card_value <= 9:
            choice = "d"
        # rows 12-16, columns 7-A
        elif cpu_hand_value >= 12 and cpu_hand_value <= 16 and first_card_value >= 7 and first_card_value <= 11:
            choice = "h"
        # row 9, columns 7-A
        elif cpu_hand_value == 9 and first_card_value >= 7 and first_card_value <= 11:
            choice = "h"
        # rows 10-11, columns 10-A
        else:
            choice = "h"
        return choice