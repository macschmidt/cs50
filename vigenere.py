import cs50
import sys

NUM_LETTERS = 26

def main():

    # ensure proper usage
    if len(sys.argv) != 2:
        print("Usage: python vigenere.py keyword")
        return 1
    
    keyword = sys.argv[1]
    keywordLength = len(keyword)
    
    # ensure keyword is valid
    if keyword.isalpha() == False:
        print("Keyword must only contain letters A-Z and a-z.")
        return 1
    
    # get plaintext from user
    message = cs50.get_string()
    messageLength = len(message)
    
    outputMessage = ""
    
    # encrypt the plaintext in place, tracking letters seen to skip spaces
    num_letters_seen = 0
    for i in range(messageLength):
        
        # only encrpyt letters, not other characters
        if message[i].isalpha():
            key = 0
            
            # check to see if the current character is upper case 
            if keyword[num_letters_seen % keywordLength].isupper():
                # if it is, set the key to that character in lower case
                key = (ord(keyword[num_letters_seen % keywordLength])) - ord('A')
            else:
                key = (ord(keyword[num_letters_seen % keywordLength])) - ord('a')
            
            # encrypt the current message letter with Caesar cipher
            if message[i].isupper():
                outputMessage += chr(((ord(message[i]) - ord('A') + key) % NUM_LETTERS) + ord('A'))
            
            else:
                outputMessage += chr(((ord(message[i]) - ord('a') + key) % NUM_LETTERS) + ord('a'))
                
            num_letters_seen += 1
    
    # output changed message
    print(outputMessage)
    
    return 0
    
if __name__ == "__main__":
    main()