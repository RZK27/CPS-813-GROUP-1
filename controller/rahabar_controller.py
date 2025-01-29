from pynput import keyboard
import requests
import os

# Replace this with the Arduino's IP address and port
arduino_url = "http://192.168.0.100"  # Example IP address

def send_command_to_arduino(command):
    try:
        # Send a POST request to the Arduino with the command
        response = requests.post(arduino_url, command)
        clear_and_print(response.text)
    except Exception as e:
        clear_and_print(f"Error sending command: {e}")

def on_press(key):
    try:
        if key == keyboard.Key.up:
            send_command_to_arduino("[F]")
        elif key == keyboard.Key.down:
            send_command_to_arduino("[B]")
        elif key == keyboard.Key.left:
            send_command_to_arduino("[L]")
        elif key == keyboard.Key.right:
            send_command_to_arduino("[R]")
    except Exception as e:
        print(f"Error: {e}")

def clear_and_print(message):
    # Clear the terminal
    os.system('cls' if os.name == 'nt' else 'clear')
    print('hi')
    # Print the message
    print(message)

# Set up the listener for key presses
with keyboard.Listener(on_press=on_press) as listener:
    listener.join()

def main():
    clear_and_print(requests.get(arduino_url).text)
    print("Listening for arrow key presses...")
    with keyboard.Listener(on_press=on_press) as listener:
        listener.join()

if __name__ == "__main__":
    print('hi')
    #main()