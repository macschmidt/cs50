import random
from card import Card

SUITS = ['Clubs', 'Spades', 'Hearts', 'Diamonds']
RANKS = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']

class Deck:
    def __init__(self):
        self.cards = [Card(suit, rank) for suit in SUITS for rank in RANKS]
        self.shuffle()
        
    def __str__(self):
        cards_in_deck = ''
        for card in self.cards:
            cards_in_deck = cards_in_deck + str(card) + ' '
        return cards_in_deck
        
    def shuffle(self):
        random.shuffle(self.cards)
        
    def deal_card(self):
        card = self.cards.pop(0)
        return card