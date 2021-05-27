use std::fs;
use std::io::prelude::*;
use std::net::TcpStream;
use std::net::TcpListener;

fn main() {
    println!("Hello, world!");
    // bind return Result<T, E>
    // unwrap, stop the program if errors happen
    let listener = TcpListener::bind("127.0.0.1:10000").unwrap();

    for stream in listener.incoming() {
        let stream = stream.unwrap();

        println!("Connection established!");
        handle(stream);
    }
}

fn show_content(code: &str, contents: String) -> String {
    let response = format!("{}\r\nContent-Length: {}\r\n\r\n{}", code, contents.len(), contents);
    response
}

fn handle(mut stream: TcpStream) {
    let mut buffer = [0; 1024]; // 1K size
    stream.read(&mut buffer).unwrap();

    // from_utf8_lossy: convert utf8 string to char string, use specical char when invalid
    println!("Request:\n{}", String::from_utf8_lossy(&buffer[..]));

    let get_index = b"GET / HTTP/1.1\r\n";
    let (status_line, filename) = if buffer.starts_with(get_index) {
        ("HTTP/1.1 200 OK", "hello.html")
    } else {
        ("HTTP/1.1 404 NOT FOUND", "404.html")
    };

    let contents = fs::read_to_string(filename).unwrap();
    let response = show_content(status_line, contents);
    stream.write(response.as_bytes()).unwrap();
    stream.flush().unwrap();
}
