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

fn counter(s :&str) -> usize {
    let a = counter_char(s);
    let mut sum = 0;
    for cnt in a {
        if cnt != 0 {
            sum = sum + 1;
        }
    }
    sum
}

fn counter_every(s :&str) -> usize {
    let xs = s.split_whitespace().collect::<Vec<&str>>();
    let a = counter_char(s);
    let ln :u32 = xs.len() as u32;
    let mut sum = 0;
    for cnt in a {
        if cnt == ln {
            sum = sum + 1;
        }
    }
    println!("{:?} {}", xs, sum);
    sum
}

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();
    let xs = contents.split("\n\n").collect::<Vec<&str>>();
    let mut cnt = 0;
    // for s in xs {
    //     cnt = cnt + counter(s);
    // }
    // println!("{}", cnt); // 7027
    cnt = 0;
    for s in xs {
        cnt = cnt + counter_every(s);
    }
    println!("{}", cnt); // 3579
}
