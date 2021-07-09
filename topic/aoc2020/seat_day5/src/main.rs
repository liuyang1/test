use std::fs;

fn to_id(s: &str) -> usize {
    let bs = s.chars().map(|c| if c == 'F' || c == 'L' {'0'} else {'1'})
        .collect::<String>();
    usize::from_str_radix(&bs, 2).unwrap()
}

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();
    let mut xs = contents.lines().map(|x| to_id(x)).collect::<Vec<usize>>();

    println!("max: {:?}", xs.iter().max().unwrap()); // ans: 922

    xs.sort();
    for (x0, x1) in xs.iter().zip(xs[1..].iter()) {
        if x0 + 1 != *x1 {
            println!("{} {}, missing: {}", x0, x1, x0 + 1);
        }
    }
}
