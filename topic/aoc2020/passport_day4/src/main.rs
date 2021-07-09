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
    ('0' <= c && c <= '9') || ('a' <= c && c <= 'f')
}

fn is_color(s: &str) -> bool {
    let (h, t) = s.split_at(1);
    h == "#" && t.chars().all(is_hexdigit)
}

fn is_hair(s: &str) -> bool {
    let hair = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"];
    hair.iter().any(|i| s == *i)
}

fn is_height(s: &str) -> bool {
    let (hgt, unit) = s.split_at(s.len() - 2);
    match unit {
        "cm" => in_range(hgt, 150, 193),
        "in" => in_range(hgt, 59, 76),
        _ => return false,
    }
}

fn valid_item(s: &str) -> bool{
    let (k, v) = s.split_at(4);
    match k {
        "byr:" => in_range(v, 1920, 2002),
        "iyr:" => in_range(v, 2010, 2020),
        "eyr:" => in_range(v, 2020, 2030),
        "hgt:" => is_height(v),
        "hcl:" => is_color(v),
        "ecl:" => is_hair(v),
        "pid:" => v.len() == 9 && is_digits(v),
        "cid:" => true,
        _ => {println!("unknown {}: {}", k, v); false},
    }
}

fn valid_passport(s: &str) -> bool {
    valid_passport_0(s) && s.split_whitespace().all(|item| valid_item(item))
}

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();
    let xs = contents.split("\n\n").collect::<Vec<&str>>();
    // println!("{:?}", xs);
    let cnt:usize = xs.iter().map(|x| valid_passport(x) as usize).sum();
    println!("{}/{}", cnt, xs.len());
}
