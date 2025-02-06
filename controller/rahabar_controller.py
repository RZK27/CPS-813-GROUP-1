import asyncio
import websockets
import cv2
import numpy as np
import pygame
import sys
import json
import struct
import os

# WebSocket server details
WS_SERVER_URL = "ws://192.48.56.2:80"  # Replace with your Arduino's WebSocket server address
FRAME_WIDTH = 600
FRAME_HEIGHT = 800

# Initialize Pygame for controller input
pygame.init()
clock = pygame.time.Clock()
pygame.joystick.init()
if pygame.joystick.get_count() > 0:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
else:
    joystick = None

# Create an OpenCV window for video feed
# cv2.namedWindow("Arduino Video Feed")

async def send_controller_data(websocket):
    """Capture joystick or keyboard input and send to WebSocket."""
    while True:
        pygame.event.pump()  # Update input states
        data = {}

        # Capture joystick data if available
        if joystick:
            data['ls_x'] = round(joystick.get_axis(0) * 10)  # Left joystick X
            data['ls_y'] = round(joystick.get_axis(1) * 10)  # Left joystick Y
            data['rs_x'] = -round(joystick.get_axis(2) * 90)  # Right joystick Y
            data['rs_y'] = -round(joystick.get_axis(3) * 15)  # Right joystick Y
        else:
            # Use keyboard as a fallback
            keys = pygame.key.get_pressed()
            data['left'] = keys[pygame.K_LEFT]
            data['right'] = keys[pygame.K_RIGHT]
            data['up'] = keys[pygame.K_UP]
            data['down'] = keys[pygame.K_DOWN]

        # Send controller input data
        json_data = json.dumps(data)  # Convert dict to JSON string
        # print(json_data)
        await websocket.send(json_data)
        await asyncio.sleep(0.1)  # Avoid sending data too frequently

async def receive_data(websocket):
    """Receive video frames from the WebSocket and display them using OpenCV."""
    try:
        while True:
            # Receive a message from the WebSocket
            message = await websocket.recv()

            # Assume the message ultrasonic string
            distance = struct.unpack('<i', message)[0]
            if distance < 20:
                square = '🟥'
            else:
                square = '🟩'

            toPrint = f"{square * min(40, distance)}\n{distance} cm"

            os.system('cls')
            print(toPrint)


    except Exception as e:
        print(f"Error receiving ultrasonic feed: {e}")

async def main():
    async with websockets.connect(WS_SERVER_URL) as websocket:
        print("Connected to Arduino WebSocket!")
        # Run send and receive tasks concurrently
        await asyncio.gather(
            send_controller_data(websocket),
            receive_data(websocket)
        )

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("Exiting...")
        pygame.quit()
        cv2.destroyAllWindows()
        sys.exit()
