use std::env;
use std::path::PathBuf;
use std::process::Command;

fn main() {
    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap());
    let build_dir = out_dir.join("cmake-build");

    std::fs::create_dir_all(&build_dir).unwrap();

    // Configure
    let status = Command::new("cmake")
        .current_dir(&build_dir)
        .arg(env::var("CARGO_MANIFEST_DIR").unwrap()) // path to CMakeLists.txt
        .arg("-DCMAKE_BUILD_TYPE=Release")
        .status()
        .expect("failed to run cmake");

    if !status.success() {
        panic!("cmake configure failed");
    }

    // Build
    let status = Command::new("cmake")
        .current_dir(&build_dir)
        .arg("--build")
        .arg(".")
        .arg("--config")
        .arg("Release")
        .arg("--parallel")
        .status()
        .expect("failed to build");

    if !status.success() {
        panic!("cmake build failed");
    }
    let lib_path = build_dir.join("lib");
    let lib_res_path = lib_path.join("Release");
    println!("cargo:rerun-if-changed=src/cxx/lib.cpp");
    println!("cargo:rerun-if-changed=src/cxx/UI.cpp");
    println!("cargo:rustc-link-search=native={}", lib_path.display());
    println!("cargo:rustc-link-search=native={}", lib_res_path.display());

    println!("cargo:rustc-link-lib=static=dx11renderbackend");

    println!("cargo:rustc-link-lib=d3d11");
    println!("cargo:rustc-link-lib=dxgi");
    println!("cargo:rustc-link-lib=d3dcompiler");
    println!("cargo:rustc-link-lib=dxguid");

    println!("cargo:rustc-link-lib=user32");
    println!("cargo:rustc-link-lib=gdi32");
    println!("cargo:rustc-link-lib=winmm");

    println!("cargo:rustc-link-lib=ntdll");
    println!("cargo:rustc-link-lib=userenv");
    println!("cargo:rustc-link-lib=ws2_32");
}
