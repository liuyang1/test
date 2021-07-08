// #[macro_use] extern crate text_io;
use std::fs;

/*
use std::str::FromStr;
use std::num::ParseIntError;
struct Trans {
    bot: u32,
    top: u32,
    c: char,
    pwd: &'static str,
}

impl FromStr for Trans {
    type Err = ParseIntError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let idx0 = s.find('-').unwrap();
        let idx1 = s.find(' ').unwrap();
        let idx2 = s.find(':').unwrap();
        println!("{} {} {}", idx0, idx1, idx2);
        let s0 = &s[..idx0];
        let s1 = &s[(idx0+1)..idx1];
        let s2 = &s[(idx1+1)..(idx1+2)];
        let s3 = &s[(idx2+2)..];
        println!("{} {} {} {}", s0, s1, s2, s3);
        let bot = s0.parse::<u32>().unwrap();
        let top = s1.parse::<u32>().unwrap();
        let c = s2.chars().nth(0).unwrap();
        println!("{} {} {} {}", bot, top, c, s3);
        // let i_ = s.find('-');
        // let i = s.parse::<u32>().unwrap();
        //println!("{}", i);
        Ok(Trans {bot: bot, top: top, c: c, pwd: s3})
    }
}
*/

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

fn char_count(s: &str, c: char) -> u32 {
    s.chars().map(|i| if i == c {1} else {0}).sum()
}

fn main() {
    // let ts = Trans::from_str("12-24 a: adefbc").unwrap();
    // println!("{} {} {} {}", ts.bot, ts.top, ts.c, ts.pwd);

    let contents = fs::read_to_string("input.0").unwrap();
    let xs = contents.lines().collect::<Vec<&str>>();
    // println!("{:?}", xs);
    for i in xs {
        let (bot, top, c, pwd) = from_str_2(i);
        let cnt = char_count(pwd, c);
        let res = if bot <= cnt && cnt <= top { "valid  " } else{ "invalid"};
        println!("{} {} {} {} {}", res, bot, top, c, pwd);
    }
}
