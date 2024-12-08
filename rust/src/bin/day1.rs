use std::env;
use std::iter::zip;
use std::path::Path;

use aoc2024::read_txt;

fn get_pairs_distance_and_similarity_score(pairs: &[Vec<i32>] ) -> (i32, i32) {
    let mut left: Vec<i32> = pairs.iter().map(|x| x[0]).collect();
    let mut right: Vec<i32> = pairs.iter().map(|x| x[1]).collect();
    left.sort();
    right.sort();
    let distance = zip(&left, &right).map(|(x, y)| (x - y).abs()).sum();
    let score = left.iter().map(|val| val * (right.iter().filter(|&n| n == val).count()) as i32).sum();
    (distance, score)
}

pub fn main() {
    let args : Vec<String> = env::args().collect();

    match args.len() {
        2 => {
            let arg = &args.get(1).unwrap();
            let filename = Path::new(arg);
            let pairs = read_txt::<i32>(filename).unwrap();
            let (distance, score) = get_pairs_distance_and_similarity_score(&pairs[..]);
            println!("{distance}");
            println!("{score}");
        },
        _ => {
            println!("Usage: ./day1 [input]!");
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test() {
        let input = [
            vec![3, 4],
            vec![4, 3],
            vec![2, 5],
            vec![1, 3],
            vec![3, 9],
            vec![3, 3],
        ];
        let (result, score) = get_pairs_distance_and_similarity_score(&input);
        assert_eq!(result, 11);
        assert_eq!(score, 31);
    }
}