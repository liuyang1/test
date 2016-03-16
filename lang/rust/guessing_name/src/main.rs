extern crate rand; // crate: package of rust code

use std::io;
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    println!("Guess the number!");

    let secret_number = rand::thread_rng().gen_range(1, 101);

    loop {
        println!("Please input your guess.");

        // let statement to var binding
        // mut      mutable (default is immutable)
        let mut guess = String::new();

        // & references, which also default to immutable
        // so need `mut` to declare it mutable
        io::stdin().read_line(&mut guess)
            .expect("failed to read line");
        // `read_line` return `Result`, then handle error result with `expect`
        // have to add this `expect`, else get one comipler warning

        // new u32 guess shadow old string guess
        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        // fast error handling statement

        // use {} as placeholder for output, python also support this style
        println!("You guessed: {}", guess);

        // Pattern Match
        match guess.cmp(&secret_number) {
            Ordering::Less    => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal   => {
                println!("You win!");
                break;
            }
        }
    }
}
