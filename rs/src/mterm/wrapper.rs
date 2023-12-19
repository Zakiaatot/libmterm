use super::ffi::*;

// Wrapper function for CreateMterm
pub fn create_mterm(
    cmd: &str,
    cwd: &str,
    argv: &[&str],
    envp: &mut [*mut i8],
    rows: u16,
    cols: u16,
) -> i32 {
    let cmd_ptr = cmd.as_ptr() as *const i8;
    let cwd_ptr = cwd.as_ptr() as *const i8;
    let argv_ptr: Vec<*const i8> = argv
        .iter()
        .map(|arg| (*arg).as_ptr() as *const i8)
        .collect();
    let argv_ptr_ptr = argv_ptr.as_ptr();
    let envp_ptr = envp.as_mut_ptr();

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
    let buf_ptr = buf.as_mut_ptr() as *mut i8;
    let size = buf.len();
    unsafe { ReadMterm(id, buf_ptr, size) }
}

// Wrapper function for WriteMterm
pub fn write_mterm(id: u32, buf: &[u8]) -> i32 {
    let buf_ptr = buf.as_ptr() as *const i8;
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
