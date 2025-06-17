use std::sync::mpsc;
use std::thread;
use std::time::Duration;

fn main() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let val = String::from("你好");
        println!("send hello"); // WARN: this may be conflict with the "wait..." log
        tx.send(val).unwrap();
    });

    thread::sleep(Duration::from_millis(1000));
    println!("wait...");
    let received = rx.recv().unwrap();
    println! ("收到：{}", received);
}
