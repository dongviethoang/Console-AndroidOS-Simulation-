import argparse
import asyncio
import socket
import websockets
import pyaudio
import os
import time
from zeroconf import ServiceInfo, Zeroconf

async def handle_incoming_call(websocket, path=None):
    """Handle incoming voice call connection"""
    print("Incoming call received...")
    
    # Initialize PyAudio for playback
    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paInt16, channels=1, rate=44100, output=True, frames_per_buffer=1024)
    
    try:
        # Log the incoming call
        caller_ip = websocket.remote_address[0]
        with open("call_log.txt", "a") as log_file:
            log_file.write(f"Received call from: {caller_ip} at {time.ctime()}\n")
        
        print(f"Call connected from {caller_ip}")
        print("Listening for audio data. Press Ctrl+C to end call...")
        
        while True:
            try:
                # Receive audio data from caller
                data = await websocket.recv()
                if data:
                    # Play the received audio
                    stream.write(data)
            except websockets.exceptions.ConnectionClosed:
                print("Caller has ended the call.")
                break
    except KeyboardInterrupt:
        print("\nEnding call...")
    finally:
        stream.stop_stream()
        stream.close()
        p.terminate()
        print("Call ended.")

async def start_receiver(host="0.0.0.0", port=5000):
    """Start WebSocket server to receive voice calls"""
    print(f"Voice receiver starting on {host}:{port}...")
    
    try:
        async with websockets.serve(handle_incoming_call, host, port):
            print(f"Voice receiver listening on ws://{host}:{port}")
            print("Waiting for incoming calls...")
            await asyncio.Future()  # run forever
    except OSError as e:
        print(f"Failed to bind receiver on {host}:{port}: {e}")
        print("Make sure the host is a local address on this machine.")
        print("Use 0.0.0.0 to listen on all interfaces, or use the actual local IP.")
        raise

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Voice receiver server")
    parser.add_argument("--host", default="0.0.0.0", help="Host/interface to bind on (default: 0.0.0.0)")
    parser.add_argument("--port", type=int, default=5000, help="Port to listen on (default: 5000)")
    args = parser.parse_args()

    try:
        asyncio.run(start_receiver(args.host, args.port))
    except KeyboardInterrupt:
        print("Voice receiver stopped.")
    except OSError:
        local_ip = socket.gethostbyname(socket.gethostname())
        print(f"Your machine's detected local IP is {local_ip}.")
        print("If you're trying to bind to 192.168.1.1, use an IP actually assigned to this machine.")
