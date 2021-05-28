use std::fs;

fn main() {
    let contents = fs::read_to_string("input").unwrap();
    let mut xs = contents.lines()
        .map(|s| s.parse().unwrap()).collect::<Vec<u32>>();
    xs.sort();
    // println!("{:?}", xs);

    let c = 2020;
    let mut b = 0;
    let mut e = xs.len() - 1;
    while b < e {
        let s = xs[b] + xs[e];
        // println!("{}:{} {}:{} += {}\n", b, xs[b], e, xs[e], s);
        if s < c {
            b = b + 1;
        } else if s > c {
            e = e - 1;
        } else {
            println!("{} {}", xs[b], xs[e]);
            println!("{}", xs[b] * xs[e]);
            return;
        }
    }
    println!("cannot find pair which sum is {}", c);
}
