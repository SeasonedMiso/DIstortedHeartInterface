#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

use serde::de::value::Error;
use serialport::SerialPort;
use serialport::SerialPortType::UsbPort;
use std::env;
use std::iter::{Product, Sum};
use std::net::TcpListener;
use std::{thread::sleep, time::Duration};

static mut global_port: Option<Box<dyn SerialPort>> = None;

#[tauri::command]
fn greet(name: &str) -> () {
    format!("Hello, {}! You've been greeted from Rust!", name);
}

fn main() {
    find_arduino();
    tauri::Builder::default()
        // This is where you pass in your commands
        .invoke_handler(tauri::generate_handler![save_preset])
        .run(tauri::generate_context!())
        .expect("failed to run app");
}

#[tauri::command]
fn save_preset(save_info: String) {
    println!("I was invoked from JS, with this message: {}", save_info);
    sleep(Duration::new(1, 0));
    let mut output = save_info.as_bytes();
    output = "2\n".as_bytes();
    unsafe {
        if let Some(port) = &global_port {
            port.write(output).expect("Write failed!");
        }
    }
}
//    invoke('change_preset', { presetNo: this.activePreset })
#[tauri::command]
fn change_preset(preset_no: String) {
    println!("I was invoked from JS, with this message: {}", preset_no);
    sleep(Duration::new(1, 0));
    let mut tempString = "p".to_string();
    tempString.push_str(&preset_no);
    let mut output = tempString.as_bytes();
    output = "2\n".as_bytes();
    unsafe {
        if let Some(port) = &global_port {
            port.write(output).expect("Write failed!");
        }
    }
}

fn find_arduino() {
    let mut arduino_port_name: String;
    arduino_port_name = "".to_string();
    let ports = serialport::available_ports().expect("No ports found!");
    for p in ports {
        println!("{}", p.port_name);
        if cfg!(macos) {
            if p.port_name.contains("cu") {
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
        } else if cfg!(windows) {
            if p.port_name.contains("COM4") {
                println!("{:?}", p.port_type);
                if let UsbPort(info) = p.port_type {
                    if let Some(product) = info.product {
                        if product.contains("Arduino Uno") {
                            arduino_port_name = p.port_name
                        }
                    }
                }
            }
        }
    }
    let port_result = serialport::new(arduino_port_name, 9600)
        .timeout(Duration::from_millis(10))
        .open();
    if let Err(_err) = port_result {
        println!("Arduino not found");
        return;
    } else {
        // let mut port = port_result.unwrap();
        unsafe {
            global_port = Some(port_result.unwrap());
        }
        println!("Arduino found!");
        // sleep(Duration::new(3, 0));
        // let mut output = "This is a test. This is only a test.".as_bytes();
        // output = "2\n".as_bytes();
        // unsafe {
        //     global_port.write(output).expect("Write failed!");
        // }
        //
        //to read data:
        //
        // port.flush().unwrap();
        // let mut serial_buf: Vec<u8> = vec![0; 32];
        // loop {
        //     // .expect("Found no data!");
        //     let read_result = port.read(serial_buf.as_mut_slice());
        //     if let Ok(_) = read_result {
        //         let read_string = String::from_utf8_lossy(&serial_buf).to_string();
        //         println!("{}", read_string);
        //         println!("{:?}", serial_buf);
        //         break
        //     };
        // }
    }
}
