use std::fs;

fn valid_passport_0(s: &str) -> bool {
    let keys = ["byr:", "iyr:", "eyr:", "hgt:", "hcl:", "ecl:", "pid:"];
    keys.iter().all(|i| s.contains(i))
}

fn is_digits(s:  &str) -> bool {
    s.chars().all(char::is_numeric)
}

fn in_range(s: &str, bot: usize, top: usize) -> bool {
    if !is_digits(s) {
        return false;
    }
    let x = s.parse::<usize>().unwrap();
    bot <= x && x <= top
}

fn is_hexdigit(c: char) -> bool {
    ('a' <= c && c <= 'f') || ('0' <= c && c <= '9')
}

fn is_color(s: &str) -> bool {
    s.len() == 7 && s.chars().nth(0).unwrap() == '#' && s.chars().skip(1).all(is_hexdigit)
}

fn is_hair(s: &str) -> bool {
    let hair = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"];
    hair.iter().any(|i| s == *i)
}

fn is_height(s: &str) -> bool {
    let idx = s.len() - 2;
    let (hgt, unit) = s.split_at(idx);
    if hgt.len() == 0 {
        return false;
    }
    let hgt = hgt.parse::<usize>().unwrap();
    match unit {
        "cm" => 150 <= hgt && hgt <= 193,
        "in" => 59 <= hgt && hgt <= 76,
        _ => return false,
    }
}

fn valid_passport(s: &str) -> bool {
    if !valid_passport_0(s) {
        return false;
    }
    for sub in s.split_whitespace() {
        let k = &sub[0..4];
        let v = &sub[4..];
        match k {
            "byr:" => if !in_range(v, 1920, 2002) { return false },
            "iyr:" => if !in_range(v, 2010, 2020) {  return false },
            "eyr:" => if !in_range(v, 2020, 2030) {  return false },
            "hgt:" => if !is_height(v) { return false},
            "hcl:" => if !is_color(v) {  return false },
            "ecl:" => if !is_hair(v) { return false},
            "pid:" => if !(v.len() == 9 && is_digits(v)) { return false},
            "cid:" => (),
            _ => {println!("unknown {}: {}", k, v); return false },
        }
    }
    true
}

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();
    let xs = contents.split("\n\n").collect::<Vec<&str>>();
    // println!("{:?}", xs);
    let mut sum = 0;
    for x in xs.iter() {
        if valid_passport(x) {
            sum = sum + 1;
        }
    }
    println!("{}/{}", sum, xs.len());
}
