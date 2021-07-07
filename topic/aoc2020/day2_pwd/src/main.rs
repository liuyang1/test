// #[macro_use] extern crate text_io;
use std::str::FromStr;
use std::num::ParseIntError;
use std::fs;

struct Trans {
    bot: u32,
    top: u32,
    c: char,
    pwd: &'static str,
}

impl FromStr for Trans {
    type Err = ParseIntError;

    fn from_str(s: &'static str) -> Result<Self, Self::Err> {
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

fn main() {
    let ts = Trans::from_str("12-24 a: adefbc").unwrap();
    println!("{} {} {} {}", ts.bot, ts.top, ts.c, ts.pwd);
    /*
    let contents = fs::read_to_string("input.0").unwrap();
    let xs = contents.lines()
        .collect::<Vec<&str>>();
    println!("{:?}", xs);
    */
}
