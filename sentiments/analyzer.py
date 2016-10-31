import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives="positive-words.txt", negatives="negative-words.txt"):
        """Initialize Analyzer."""
        
        # TODO
        positiveWordFile = open(positives, "r")
        negativeWordFile = open(negatives, "r")
        
        # create an empty dictionary file that will hold the words and the score
        self.words = {}
        
        for line in positiveWordFile:
            # if the current line does not start with a semi-colon, it's an actual word, so add it to words
            if not line.startswith(";") and not line.startswith("\n"):
                # remove line break
                line = line.rstrip().lower()
                # update dictionary
                self.words.update({line: 1})
        
        for line in negativeWordFile:
            # if the current line does not start with a semi-colon, it's an actual word, so add it to words
            if not line.startswith(";") and not line.startswith("\n"):
                # remove line break
                line = line.rstrip().lower()
                # update dictionary
                self.words.update({line: -1})
                

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        score = 0
        
        tokenizer = nltk.tokenize.TweetTokenizer()
        tweetText = tokenizer.tokenize(text)
        # I know have a list containing the tweet in separated strings
        
        # iterate through every word in the list
        for currentWord in tweetText:
            currentWord = currentWord.lower()
            # if the word is in the dictionary
            if currentWord in self.words:
                # return the value, 1 or -1, for that word
                score += self.words[currentWord]
        
        return score