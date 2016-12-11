class Card:
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank
        
        if rank == 'A':
            self.point = 11
        elif rank in ['K', 'Q', 'J']:
            self.point = 10
        else:
            self.point = int(rank)
        
        self.hidden = False
    
    def __str__(self):
        if self.hidden:
            return "[_]"
        else:
            return "[" + self.rank + " " + self.suit + "]"
    
    def reveal_card(self):
        self.hidden = False
        
    def hide_card(self):
        self.hidden = True
    
    def is_ace(self):
        return self.rank == 'A'