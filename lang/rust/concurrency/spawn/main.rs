use std::thread;
use std::time::Duration;

fn main() {
    let h = thread::spawn(|| {
        for i in 1..10 {
            println! ("\t hello, from thread {}", i);
            thread::sleep(Duration::from_millis(200));
        }
    });
    for i in 1..5 {
        println! ("hello, from main {}", i);
        thread::sleep(Duration::from_millis(200));
    }
    h.join().unwrap();
}
