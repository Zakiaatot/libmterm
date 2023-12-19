use core::panic;
use libmterm_rs::*;
use std::{
    io::{self, Write},
    time::Duration,
};

fn main() {
    let id = create_default();
    if id < 0 {
        panic!("failed to create terminal");
    }

    // read
    let read_id = id.clone() as u32;
    let read_thread = std::thread::spawn(move || {
        libmterm_rs::set_read_nonblock(read_id);
        let mut res;
        loop {
            res = libmterm_rs::read(read_id);
            if res.len() > 0 {
                print!("{}", res);
                io::stdout().flush().expect("Failed to flush stdout.");
            }
            std::thread::sleep(Duration::from_millis(1));
        }
    });

    // write
    let write_id = id.clone() as u32;
    let write_thread = std::thread::spawn(move || loop {
        let mut buf = String::new();
        std::io::stdin()
            .read_line(&mut buf)
            .expect("Failed to read line");
        libmterm_rs::write(write_id, buf);
    });

    read_thread.join().unwrap();
    write_thread.join().unwrap();
}
