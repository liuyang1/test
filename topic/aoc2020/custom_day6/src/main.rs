use std::fs;

fn counter_char(s :&str) -> [u32; 26] {
    let mut a:[u32; 26] = [0; 26];
    for c in s.bytes() {
        if c == b'\n' {
            continue;
        }
        let i: usize = (c - b'a').into();
        a[i] = a[i] + 1;
    }
    a
}

fn counter_pred(a :[u32;26], pred: impl Fn(u32) -> bool) -> usize {
    a.iter().map(|i| if pred(*i) {1} else {0}).sum()
}

fn counter(s :&str) -> usize {
    counter_pred(counter_char(s), |i| i != 0)
}

fn counter_every(s :&str) -> usize {
    let xs = s.split_whitespace().collect::<Vec<&str>>();
    let ln :u32 = xs.len() as u32;
    counter_pred(counter_char(s), |i| i == ln)
}

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();
    let xs = contents.split("\n\n").collect::<Vec<&str>>();
    let cnt0:usize = xs.iter().map(|s| counter(s)).sum();
    let cnt1:usize = xs.iter().map(|s| counter_every(s)).sum();
    println!("{} {}", cnt0, cnt1); // 7027 3579
}
