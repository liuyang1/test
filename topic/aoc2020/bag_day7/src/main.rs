use std::fs;
use std::collections::HashMap;

/* bag: Vec<num, str> */
fn parse_sub(s: &str) -> Option<(usize, &str)> {
    let idx0 = s.find(" ").unwrap();
    let idx1 = s.find(" bag").unwrap();
    // return None when "no other bags"
    let n = s[0..idx0].parse::<usize>().ok()?;
    let bag = &s[(idx0+1)..idx1];
    // println!("{} {}", n, bag);
    Some((n, bag))
}

fn parse_line(s: &str) -> (&str, Vec<(usize, &str)>) {
    let idx0 = s.find(" bags").unwrap();
    let bag = &s[0..idx0];

    let ctn = "contain ";
    let idx1 = s.find(ctn).unwrap() + ctn.len();
    let sub = &s[idx1..];
    let xs = sub.split(", ").collect::<Vec<&str>>();
    let mut ys = Vec::new();
    for x in xs {
        match parse_sub(x) {
            Some(y) => ys.push(y),
            None => (),
        }
    }

    (bag, ys)
}

fn search_color(t: &HashMap<&str, Vec<&str>>, s: &str) -> usize {
    let mut q = Vec::new();
    q.push(s);
    let mut res = Vec::new();
    loop {
        match q.pop() {
            Some(top) => {
                if t.contains_key(top) {
                    // println!("{}: {:?}", top, t[top]);
                    q.extend(t[top].iter().copied());
                    res.extend(t[top].iter().copied());
                }
            },
            None => {break;},
        }
    }
    res.sort_unstable();
    res.dedup();
    res.len()
}

fn count_bags(t: &HashMap<&str, Vec<(usize, &str)>>, s: &str) -> usize {
    /* recursive way */
    let mut sum = 1;
    if t.contains_key(s) {
        let sub = &t[s];
        for (n, x) in sub {
            sum = sum + n * count_bags(t, x);
        }
    }
    sum
}

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();
    let xs = contents.lines().collect::<Vec<&str>>();
    // println!("{:?}", xs);

    // build reverse-lookup table
    let mut t = HashMap::new();
    let mut t0 = HashMap::new();
    for s in xs.iter() {
        let (x, ys) = parse_line(s);
        for (n, y) in ys {
            let v = t.entry(y).or_insert(Vec::new());
            v.push(x);
            let v = t0.entry(x).or_insert(Vec::new());
            v.push((n, y));
        }
    }
    // t.retain(|k, v| {
    //     println!("{}: {:?}", k, v);
    //     true
    // });
    println!("search color bag: {}", search_color(&t, "shiny gold")); // 172
    // t0.retain(|k, v| {
    //     println!("{}: {:?}", k, v);
    //     true
    // });
    println!("count sub bags: {}", count_bags(&t0, "shiny gold") - 1); // 39645

}
