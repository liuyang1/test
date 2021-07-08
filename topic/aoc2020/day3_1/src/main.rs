use std::fs;

fn tree(grid: Vec<&str>, dx: usize, dy: usize) -> usize {
    let mut x = 0;
    let mut y = 0;
    let mut sum = 0;
    let width = grid[0].chars().count();
    while y < grid.len() {
        let c = grid[y].chars().nth(x % width).unwrap();
        if c == '#' {
            sum = sum + 1;
        }
        x = x + dx;
        y = y + dy;
    }
    sum
}

fn main() {
    let contents = fs::read_to_string("input.txt").unwrap();
    let xs = contents.lines().collect::<Vec<&str>>();

    println!("part one: {}", tree(xs.clone(), 3, 1));

    let slopes = [ (1, 1), (3, 1), (5, 1), (7, 1), (1, 2) ];
    let mut prod = 1;
    for (dx, dy) in slopes.iter() {
        let cnt = tree(xs.clone(), *dx, *dy);
        println!("{}", cnt);
        prod = prod * cnt;
    }
    println!("part two: {}", prod);
}
