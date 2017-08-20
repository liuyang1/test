secret = rand(100)

while true
    puts "input the guess number:"
    s = gets
    n = s.to_i
    if n > secret
        cond = "greater than"
    else if n < secret
        cond = "less than"
    else
        cond = "equal to"
    end
    end
    puts "#{n} is #{cond} secret"
    if n == secret
        puts "game over"
        break
    end
end
