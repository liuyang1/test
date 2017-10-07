Define a function

    lastDigit :: Integer -> Integer -> Integer

that takes in two numbers a and b and returns the last decimal digit of a^b. Note that a and b may be very large!

For example, the last decimal digit of 9^7 is 9, since 9^7 = 4782969. The last decimal digit of (2^200)^(2^300), which has over 10^92 decimal digits, is 6.

The inputs to your function will always be non-negative integers.

Examples

    lastDigit 4 1             `shouldBe` 4
    lastDigit 4 2             `shouldBe` 6
    lastDigit 9 7             `shouldBe` 9
    lastDigit 10 (10^10)      `shouldBe` 0
    lastDigit (2^200) (2^300) `shouldBe` 6
