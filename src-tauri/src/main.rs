#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

use serde::de::value::Error;
use serialport::SerialPortType::UsbPort;
use std::time::Duration;

#[tauri::command]
fn greet(name: &str) -> () {
    format!("Hello, {}! You've been greeted from Rust!", name);
}

fn main() {
    find_arduino();
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![greet])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
fn find_arduino() {
    let mut arduino_port_name: String;
    arduino_port_name = "".to_string();
    // Listing available ports:
    let ports = serialport::available_ports().expect("No ports found!");
    for p in ports {
        println!("{}", p.port_name);
        arduino_port_name = match p.port_type {
            UsbPort(info) => {
                println!("{:#06x}", info.vid);
                println!("{:#06x}", info.pid);
                if info.vid == 0x2341 && info.pid == 0x0043 {
                    p.port_name
                } else {
                    "".to_string()
                }
            }
            _ => "".to_string(),
        }
    }
    println!("{}", arduino_port_name);

    let port_result = serialport::new(arduino_port_name, 9600)
        .timeout(Duration::from_millis(10))
        .open();
    if let Err(_err) = port_result {
        println!("Arduino not found");
        // return;
    } else {
        let mut port = port_result.unwrap();
        println!("Arduino found!");
        let mut output = "This is a test. This is only a test.".as_bytes();
        port.write(output).expect("Write failed!");
        output = "2".as_bytes();
        port.write(output).expect("Write failed!");
        let mut serial_buf: Vec<u8> = vec![0; 32];
        // loop {
        //     // .expect("Found no data!");
        //     let read_result = port.read(serial_buf.as_mut_slice());
        //     if let Ok(_) = read_result {
        //         let read_string = String::from_utf8_lossy(&serial_buf).to_string();
        //         println!("{}", read_string);
        //         // println!("{:?}", serial_buf);
        //         break
        //     };
        // }
    }
}
