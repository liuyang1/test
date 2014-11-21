import re
import itertools
import string

def solve(puzzle):
    words = re.findall('[A-Z]+', puzzle.upper())
    unique_characters = set(''.join(words))
    assert len(unique_characters) <= 10, "Too many letters"
    first_letters = {word[0] for word in words}
    n = len(first_letters)
    sorted_characters = "".join(first_letters) + \
            "".join(unique_characters - first_letters)
    characters = sorted_characters
    digits = '0123456789'
    zero = digits[0]
    for guess in itertools.permutations(digits, len(characters)):
        if zero not in guess[:n]:
            guess = "".join(guess)
            d = string.maketrans(characters,guess)
            equation = puzzle.translate(d)
            if eval(equation):
                return equation


if __name__ == "__main__":
    import sys
    for puzzle in sys.argv[1:]:
        print puzzle
        solution = solve(puzzle)
        if solution:
            print solution
