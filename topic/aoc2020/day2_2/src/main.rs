// #[macro_use] extern crate text_io;
use std::fs;
use std::convert::TryInto;

fn from_str_2(s: &str) -> (u32, u32, char, &str) {
    let idx0 = s.find('-').unwrap();
    let idx1 = s.find(' ').unwrap();
    let idx2 = s.find(':').unwrap();
    let s0 = &s[..idx0];
    let s1 = &s[(idx0+1)..idx1];
    let s2 = &s[(idx1+1)..(idx1+2)];
    let s3 = &s[(idx2+2)..];
    let bot = s0.parse::<u32>().unwrap();
    let top = s1.parse::<u32>().unwrap();
    let c = s2.chars().nth(0).unwrap();
    (bot, top, c, s3)
}

fn from_bool(b: bool) -> u32 {
    if b {1} else {0}
}

fn xor(b0: bool, b1: bool) -> bool {
    (b0 && !b1) || (!b0 && b1)
}

fn main() {

    let contents = fs::read_to_string("input.txt").unwrap();
    let xs = contents.lines().collect::<Vec<&str>>();
    let mut sum = 0;
    let mut num = 0;
    for s in xs {
        num = num + 1;
        let (i, j, c, pwd) = from_str_2(s);
        /** must create two chars, as it's mutable */
        let pred0 = pwd.chars().nth((i - 1).try_into().unwrap()).unwrap() == c;
        let pred1 = pwd.chars().nth((j - 1).try_into().unwrap()).unwrap() == c;
        let pred = xor(pred0, pred1);
        sum = sum + from_bool(pred);
        let res = if pred { "valid  " } else{ "invalid"};
        println!("{} {} {} {} {}", res, i, j, c, pwd);
    }
    println!("valid password: {}/{}", sum, num);
}
