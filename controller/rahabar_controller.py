import requests
import os
import keyboard

# Replace this with the Arduino's IP address and port
arduino_url = "http://192.168.0.0"  # Example IP address

def send_command_to_arduino(command):
    try:
        # Send a POST request to the Arduino with the command
        response = requests.post(arduino_url, command, timeout=1)
        return(response.text)
    except Exception as e:
        return(f"Error sending command: {e}")

def clear_and_print(message):
    # Clear the terminal
    os.system('cls' if os.name == 'nt' else 'clear')
    # Print the message
    print(message)

# Declare the global speed variable
speed = 0

def on_key_event(e):
    data = [0,0]

    if keyboard.is_pressed('s'):
        speed = 8
    else:
        speed = 4

    if keyboard.is_pressed('left'):
        data[0] -= speed
    if keyboard.is_pressed('right'):
        data[0] += speed
    if keyboard.is_pressed('up'):
        data[1] += speed
    if keyboard.is_pressed('down'):
        data[1] -= speed

    reponse_text = send_command_to_arduino(str(data))
    clear_and_print(reponse_text)
    print('')
    print(data)

# Register key event listener
keyboard.hook(on_key_event)

# Block the program and listen for key events
keyboard.wait()
