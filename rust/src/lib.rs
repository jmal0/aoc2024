use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;
use std::str::FromStr;

pub fn read_txt<T>(file_path: &Path) -> io::Result<Vec<Vec<T>>>
where
    T: FromStr,
    <T as FromStr>::Err: std::fmt::Debug,
{
    let path = Path::new(file_path);
    let file = File::open(&path)?;
    let reader = io::BufReader::new(file);

    let mut result = Vec::new();

    for line in reader.lines() {
        let line = line?; // Read the line
        let tokens: Vec<T> = line
            .split_whitespace()
            .filter_map(|s| s.parse::<T>().ok())
            .collect();
        result.push(tokens);
    }
    Ok(result)
}
