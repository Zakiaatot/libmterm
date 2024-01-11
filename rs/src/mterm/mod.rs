mod ffi;
mod wrapper;

use std::ffi::CString;
use std::ptr::null_mut;

use wrapper::*;

pub fn create(
    cmd: &String,
    cwd: &String,
    argv: &Vec<String>,
    envp: &mut Vec<String>,
    rows: u16,
    cols: u16,
) -> i32 {
    let binding = argv.iter().map(|s| s.as_str()).collect::<Vec<&str>>();
    let argv_str: &[&str] = binding.as_slice();
    let mut binding = envp
        .iter_mut()
        .map(|s| CString::new(s.as_str()).unwrap().into_raw() as *mut u8)
        .collect::<Vec<*mut u8>>();
    binding.push(null_mut());
    let envp_ptr: &mut [*mut u8] = binding.as_mut_slice();
    create_mterm(cmd.as_str(), cwd.as_str(), argv_str, envp_ptr, rows, cols)
}

pub fn create_default() -> i32 {
    create_mterm_default()
}

pub fn destroy(id: u32) -> i32 {
    destroy_mterm(id)
}

pub fn read(id: u32) -> String {
    const BUFFER_SIZE: usize = 4096;
    let mut buffer: Vec<u8> = vec![0; BUFFER_SIZE];
    let bytes_read = read_mterm(id, &mut buffer);
    if bytes_read <= 0 {
        return "".to_string();
    }
    let result = String::from_utf8_lossy(&buffer[..bytes_read as usize]).to_string();
    return result;
}

pub fn write(id: u32, data: String) -> i32 {
    write_mterm(id, data.as_bytes())
}

pub fn wait(id: u32) -> i32 {
    wait_mterm(id)
}

pub fn set_read_nonblock(id: u32) {
    set_read_nonblock_mterm(id);
}

pub fn set_window_size(id: u32, rows: u16, cols: u16) {
    set_window_size_mterm(id, rows, cols);
}

pub fn check_running(id: u32) -> bool {
    check_running_mterm(id)
}
