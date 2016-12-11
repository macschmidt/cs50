from card import Card

class Hand:
    def __init__(self):
        self.hand = []
        
    def __getitem__(self, item):
        return self.hand[item]
        
    def clear_hand(self):
        self.hand = []
        
    def add_card(self, card):
        self.hand.append(card)
        return self.hand
        
    def __len__(self):
        return len(self)
    
    def get_value(self):
        aces = 0
        value = 0
        for card in self.hand:
            if card.is_ace():
                aces += 1
            value += card.point
        while (value > 21) and aces:
            value -= 10
            aces -= 1
        return value