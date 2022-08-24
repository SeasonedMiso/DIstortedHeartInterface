
#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}
use std::time::Duration;

fn main() {
    // Listing available ports:
    let ports = serialport::available_ports().expect("No ports found!");
    for p in ports {
        println!("{}", p.port_name);
    }
    // Opening and configuring a port:
    
    // let port = serialport::new("/dev/ttyUSB0", 115_200)
    // .timeout(Duration::from_millis(10))
    // .open().expect("Failed to open port");

// Writing to a port:

// let output = "This is a test. This is only a test.".as_bytes();
// port.write(output).expect("Write failed!");

// Reading from a port (default is blocking with a 0ms timeout):

// let mut serial_buf: Vec<u8> = vec![0; 32];
// port.read(serial_buf.as_mut_slice()).expect("Found no data!");

// Some platforms expose additional functionality, which is opened using the open_native() method:

// let port = serialport::new("/dev/ttyUSB0", 115_200)
//     .open_native().expect("Failed to open port");


    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![greet])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
