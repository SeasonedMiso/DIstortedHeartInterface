#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]
use serialport::SerialPort;
use serialport::SerialPortType::UsbPort;
use std::env;
use std::sync::Mutex;
use std::{thread::sleep, time::Duration};
use tauri::State;

struct ArduinoContext {
    pub port: Mutex<Option<Box<dyn SerialPort>>>,
}
#[derive(Clone, serde::Serialize)]
struct Payload {
    message: String,
}

fn main() {
    let port = find_arduino();
    tauri::Builder::default()
        .manage(ArduinoContext {
            port: Mutex::new(port),
        })
        .invoke_handler(tauri::generate_handler![
            change_preset,
            save_preset,
            arduino_found,
            send_test_msg
        ])
        .run(tauri::generate_context!())
        .expect("failed to run app");
    find_arduino();
}

#[tauri::command]
fn arduino_found(arduino_context: State<ArduinoContext>) -> String {
    let mut mutex_guard = arduino_context.port.lock().unwrap();
    let port_option = mutex_guard.as_mut();
    if let Some(port) = port_option {
        let result = port.write("$".as_bytes());
        println!("here");
        if let Ok(_) = result {
            "1".to_string()
        } else {
            let new_port = find_arduino();
            *mutex_guard = new_port;
            "0".to_string()
        }
    } else {
        let new_port = find_arduino();
        *mutex_guard = new_port;
        "0".to_string()
    }
}

#[tauri::command]
fn save_preset(save_info: String, arduino_context: State<ArduinoContext>) {
    println!("Save preset: {}", save_info);
    let mut mutex_guard = arduino_context.port.lock().unwrap();

    let port_option = mutex_guard.as_mut();
    if let Some(port) = port_option {
        let result = port.write("$".as_bytes());
        if let Ok(_) = result {
            sleep(Duration::new(1, 0));
            let mut output = save_info.as_bytes();
            // output = "2\n".as_bytes();
            println!("Successful port write");
        }
    }
}

#[tauri::command]
fn send_test_msg(test_msg: String, arduino_context: State<ArduinoContext>) {
    println!("Sending message: {}", test_msg);
    let mut mutex_guard = arduino_context.port.lock().unwrap();

    let port_option = mutex_guard.as_mut();
    if let Some(port) = port_option {
        let result = port.write("$".as_bytes());
        if let Ok(_) = result {
            sleep(Duration::new(1, 0));
            let mut output = test_msg.as_bytes();
            // output = "2\n".as_bytes();
            port.write(output).expect("Write failed!");
            println!("Successful port write");
        }
    }
}
#[tauri::command]
fn change_preset(preset_no: String, arduino_context: State<ArduinoContext>) {
    println!("Preset changed to: {}", preset_no);
    let mut mutex_guard = arduino_context.port.lock().unwrap();
    let port_option = mutex_guard.as_mut();
    if let Some(port) = port_option {
        let result = port.write("$".as_bytes());
        if let Ok(_) = result {
            sleep(Duration::new(1, 0));
            let mut temp_string = "p".to_string();
            temp_string.push_str(&preset_no);
            let mut output = temp_string.as_bytes();
            // output = "2\n".as_bytes();
            println!("Successful port write");
        }
    }
}

fn find_arduino() -> Option<Box<dyn SerialPort>> {
    let mut arduino_port_name: String;
    arduino_port_name = "".to_string();
    let ports = serialport::available_ports().expect("No ports found!");
    for p in ports {
        println!("{}", p.port_name);
        if cfg!(macos) {
            if p.port_name.contains("cu") {
                arduino_port_name = match p.port_type {
                    UsbPort(info) => {
                        // println!("{:#06x}", info.vid);
                        // println!("{:#06x}", info.pid);
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
                // println!("{:?}", p.port_type);
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
        // println!("Arduino not found");
        None
    } else {
        // println!("Arduino found!");
        port_result.ok()
    }
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
