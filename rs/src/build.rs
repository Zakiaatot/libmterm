extern crate dunce;
use std::{env, path::PathBuf};

fn main() {
    let library_name = "mterm_static";
    let root = PathBuf::from(env::var_os("CARGO_MANIFEST_DIR").unwrap());
    let target = env::var("TARGET").unwrap();

    let lib_dir;
    if target.contains("x86_64-unknown-linux-gnu") {
        lib_dir = "lib/linux";
    } else if target.contains("armv7-linux-androideabi") {
        lib_dir = "lib/android/armeabi-v7a";
    } else if target.contains("aarch64-linux-android") {
        lib_dir = "lib/android/arm64-v8a";
    } else if target.contains("i686-linux-android") {
        lib_dir = "lib/android/x86";
    } else if target.contains("x86_64-linux-android") {
        lib_dir = "lib/android/x86_64";
    } else {
        panic!("Unsupported target: {}", target);
    }

    let library_dir = dunce::canonicalize(root.join(lib_dir)).unwrap();
    println!("cargo:rustc-link-lib=stdc++");
    println!("cargo:rustc-link-lib=static={}", library_name);
    println!(
        "cargo:rustc-link-search=native={}",
        env::join_paths(&[library_dir]).unwrap().to_str().unwrap()
    );
}
