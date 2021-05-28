/** 3-sum */
use std::fs;

fn find_two_sum(xs: &[u32], c: u32) -> Result<(u32, u32), &'static str> {
    let mut b = 0;
    let mut e = xs.len() - 1;
    while b < e {
        let s = xs[b] + xs[e];
        if s < c {
            b = b + 1;
        } else if s > c {
            e = e - 1;
        } else {
            return Ok((xs[b], xs[e]));
        }
    }
    return Err("cannot find pair of sum");
}

fn main() {
    let contents = fs::read_to_string("input").unwrap();
    let mut xs = contents.lines()
        .map(|s| s.parse().unwrap()).collect::<Vec<u32>>();
    xs.sort();
    // println!("{:?}", xs);

    let c = 2020;
    for i in 0..xs.len() {
        let x = xs[i];
        // pass part of xs
        match find_two_sum(&xs[i..], c - x) {
            Ok((y, z)) => return println!("{} {} {} *= {}", x, y, z, x * y * z),
            Err(_e) => continue,
        }
    }
}
