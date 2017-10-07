Write a reverseWords function that accepts a string a parameter, and reverses each word in the string. Every space should stay, so you cannot use words from Prelude.

Example:

    reverseWords "An example!"    -- "nA !elpmaxe"
    reverseWords "double  spaces" -- "elbuod  secaps"

>>> space function, only need check one char ' ', other chars like "\t\n\r" still consider it as normal char
