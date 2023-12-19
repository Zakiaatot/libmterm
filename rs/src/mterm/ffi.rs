pub use core::ffi::{c_char, c_int, c_uint, c_ushort};

#[link(name = "mterm_static", kind = "static")]
extern "C" {
    pub fn CreateMterm(
        cmd: *const c_char,
        cwd: *const c_char,
        argv: *const *const c_char,
        envp: *mut *mut c_char,
        rows: c_ushort,
        cols: c_ushort,
    ) -> c_int;
    pub fn CreateMtermDefault() -> c_int;
    pub fn DestroyMterm(id: c_uint) -> c_int;
    pub fn ReadMterm(id: c_uint, buf: *mut c_char, size: usize) -> c_int;
    pub fn WriteMterm(id: c_uint, buf: *const c_char, size: usize) -> c_int;
    pub fn WaitMterm(id: c_uint) -> c_int;
    pub fn SetReadNonblockMterm(id: c_uint);
    pub fn SetWindowSizeMterm(id: c_uint, rows: c_ushort, cols: c_ushort);
    pub fn CheckRunningMterm(id: c_uint) -> bool;
}
