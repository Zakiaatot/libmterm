use std::{ffi::CString, ptr::null};

use super::ffi::*;

// Wrapper function for CreateMterm
pub fn create_mterm(
    cmd: &str,
    cwd: &str,
    argv: &[&str],
    envp: &mut [*mut u8],
    rows: u16,
    cols: u16,
) -> i32 {
    let cmd_ptr = CString::new(cmd).unwrap().into_raw() as *const c_char;
    let cwd_ptr = CString::new(cwd).unwrap().into_raw() as *const c_char;
    let mut argv_ptr: Vec<*const c_char> = argv
        .iter()
        .map(|arg| CString::new(*arg).unwrap().into_raw() as *const c_char)
        .collect();
    argv_ptr.push(null());
    let argv_ptr_ptr = argv_ptr.as_ptr();
    let envp_ptr = envp.as_mut_ptr() as *mut *mut c_char;

    unsafe { CreateMterm(cmd_ptr, cwd_ptr, argv_ptr_ptr, envp_ptr, rows, cols) }
}

// Wrapper function for CreateMtermDefault
pub fn create_mterm_default() -> i32 {
    unsafe { CreateMtermDefault() }
}

// Wrapper function for DestroyMterm
pub fn destroy_mterm(id: u32) -> i32 {
    unsafe { DestroyMterm(id) }
}

// Wrapper function for ReadMterm
pub fn read_mterm(id: u32, buf: &mut [u8]) -> i32 {
    let buf_ptr = buf.as_mut_ptr() as *mut c_char;
    let size = buf.len();
    unsafe { ReadMterm(id, buf_ptr, size) }
}

// Wrapper function for WriteMterm
pub fn write_mterm(id: u32, buf: &[u8]) -> i32 {
    let buf_ptr = buf.as_ptr() as *const c_char;
    let size = buf.len();
    unsafe { WriteMterm(id, buf_ptr, size) }
}

// Wrapper function for WaitMterm
pub fn wait_mterm(id: u32) -> i32 {
    unsafe { WaitMterm(id) }
}

// Wrapper function for SetReadNonblockMterm
pub fn set_read_nonblock_mterm(id: u32) {
    unsafe { SetReadNonblockMterm(id) }
}

// Wrapper function for SetWindowSizeMterm
pub fn set_window_size_mterm(id: u32, rows: u16, cols: u16) {
    unsafe { SetWindowSizeMterm(id, rows, cols) }
}

// Wrapper function for CheckRunningMterm
pub fn check_running_mterm(id: u32) -> bool {
    unsafe { CheckRunningMterm(id) }
}
