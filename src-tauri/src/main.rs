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
    pub is_locked: Mutex<bool>,
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
            is_locked: Mutex::new(false),
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
    let mut mutex_is_locked = arduino_context.is_locked.lock().unwrap();
    if *mutex_is_locked == false {
        *mutex_is_locked = true;
    } else {
        return "0".to_string();
    }
    let mut mutex_guard = arduino_context.port.lock().unwrap();
    let port_option = mutex_guard.as_mut();
    if let Some(port) = port_option {
        let result = port.write("$".as_bytes());
        if let Ok(_) = result {
            *mutex_is_locked = false;
            "1".to_string()
        } else {
            let new_port = find_arduino();
            *mutex_guard = new_port;
            *mutex_is_locked = false;
            "0".to_string()
        }
    } else {
        let new_port = find_arduino();
        *mutex_guard = new_port;
        *mutex_is_locked = false;
        "0".to_string()
    }
}

#[tauri::command]
fn save_preset(save_info: String, arduino_context: State<ArduinoContext>) {
    println!("Save preset: {}", save_info);
    let mut mutex_guard = arduino_context.port.lock().unwrap();
    let mut mutex_is_locked = arduino_context.is_locked.lock().unwrap();
    if *mutex_is_locked == false {
        *mutex_is_locked = true;
    } else {
        return;
    }
    let port_option = mutex_guard.as_mut();
    if let Some(port) = port_option {
        let result = port.write("$".as_bytes());
        if let Ok(_) = result {
            sleep(Duration::new(0, 1000));
            let output = save_info.as_bytes();
            port.write(output).expect("Write failed!");
            if await_arduino(port) == "1".to_string() {
                *mutex_is_locked = false;
                println!("Successful port write");
            } else {
                *mutex_is_locked = false;
                println!("---Write operation failed");
                //throw error here
            }
        }
    }
}

#[tauri::command]
fn send_test_msg(test_msg: String, arduino_context: State<ArduinoContext>) {
    if test_msg.len() > 20 {
        println!("Write operation failed");
        return;
    }
    let mut mutex_is_locked = arduino_context.is_locked.lock().unwrap();
    if *mutex_is_locked == false {
        *mutex_is_locked = true;
    } else {
        return;
    }
    println!("Sending message: {}", test_msg);
    let mut mutex_guard = arduino_context.port.lock().unwrap();

    let port_option = mutex_guard.as_mut();
    if let Some(port) = port_option {
        let result = port.write("$".as_bytes());
        if let Ok(_) = result {
            sleep(Duration::new(0, 1000));
            let output = test_msg.as_bytes();
            port.write(output).expect("Write failed!");
            if await_arduino(port) == "1".to_string() {
                println!("Successful port write");
                *mutex_is_locked = false;
            } else {
                println!("Write operation failed");
                *mutex_is_locked = false;
                //throw error here
            }
        }
        *mutex_is_locked = false;
    }
}
#[tauri::command]
fn change_preset(preset_no: String, arduino_context: State<ArduinoContext>) -> String {
    println!("Preset changed to: {}", preset_no);
    let mut mutex_guard = arduino_context.port.lock().unwrap();
    let port_option = mutex_guard.as_mut();
    let mut mutex_is_locked = arduino_context.is_locked.lock().unwrap();
    if *mutex_is_locked == false {
        *mutex_is_locked = true;
    } else {
        return "0".to_string();
    }
    if let Some(port) = port_option {
        let result = port.write("$".as_bytes());
        if let Ok(_) = result {
            sleep(Duration::new(0, 1000));
            let mut temp_string = "p".to_string();
            temp_string.push_str(&preset_no);
            let output = temp_string.as_bytes();
            port.write(output).expect("Write failed!");
            let result = await_arduino(port);
            if result != "0".to_string() {
                println!("Successful port write");
                //send back the pot positions to the front end
                println!("{:?}", result);
                *mutex_is_locked = false;
                return result;
            } else {
                println!("Write operation failed");
                *mutex_is_locked = false;
                //throw error here
                return "0".to_string();
            }
        }
    }
    return "0".to_string();
}
fn await_arduino(port: &mut Box<dyn SerialPort>) -> String {
    // println!("Awaiting acknowledgement from arduino...");
    // port.flush().unwrap();
    let mut serial_buf: Vec<u8> = vec![0; 32];
    let mut fail_index = 0;
    let mut result = "".to_string();
    loop {
        // .expect("Found no data!");
        sleep(Duration::new(1, 0));
        let read_result = port.read(serial_buf.as_mut_slice());
        let read_string = String::from_utf8_lossy(&serial_buf).to_string();
        let formatted_string = read_string.replace("$", "").replace("\n", "");
        sleep(Duration::new(0, 1000));
        if let Ok(size) = read_result {
            let last_char = read_string.chars().nth(size - 3).unwrap();
            //is this always correct???
            let str_len = size;
            if last_char == '@' {
                result = "1".to_string();
                if formatted_string.contains('q') {
                    let start_bytes = formatted_string.find("q").unwrap_or(0);
                    let end_bytes = formatted_string.find("/").unwrap_or(formatted_string.len());
                    result = formatted_string[start_bytes..end_bytes].to_string();
                }
                return result;
            } else {
                println!("failed due to not reading @ succesfully");
                println!("result:{:?}", read_result);
                println!("Received {}", read_string);
                println!("Formatted to {}", formatted_string);
                result = "0".to_string();
            }
        } else {
            fail_index += 1;
            println!("result:{:?}", read_result);
            println!("timeout time:{:?}", port.timeout());
            println!("Received {}", read_string);
            println!("Formatted to {}", formatted_string);
        }
        if fail_index > 10 {
            result = "0".to_string();
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
                //maybe these needs to just be COM??
                if let UsbPort(info) = p.port_type {
                    if let Some(product) = info.product {
                        if product.contains("Arduino") {
                            arduino_port_name = p.port_name
                        }
                    }
                }
            }
        }
    }
    let port_result = serialport::new(arduino_port_name, 9600)
        .timeout(Duration::from_millis(100))
        .open();
    if let Err(_err) = port_result {
        None
    } else {
        port_result.ok()
    }
}
