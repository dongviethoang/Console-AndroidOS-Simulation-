import asyncio
import websockets
import pyaudio
import os
import time
import json
import sys
import argparse
from zeroconf import ServiceInfo, Zeroconf

async def voice_call(sim_file=None, phoneline_config="phoneline_uris.json", phonelines_file="existing_phonelines.txt", uri_override=None):
    # Check for SIM card presence
    if sim_file and os.path.exists(sim_file):
        print(f"SIM card found at {sim_file}. Starting voice call and logging number...")
    time.sleep(1)
    a = input("Enter the phone number to call: ")
    
    # Check if that number exists
    try:
        with open(phonelines_file, "r") as f:
            existing_numbers = f.read().splitlines()
    except FileNotFoundError:
        print(f"Error: {phonelines_file} not found.")
        return
    
    if a not in existing_numbers:
        print("Phone number not found. Please enter a valid number.")
        return
    else:
        print(f"Calling {a}...")

    # Load phoneline URIs mapping from JSON file
    uri = None
    try:
        if os.path.exists(phoneline_config):
            with open(phoneline_config, "r") as f:
                phoneline_map = json.load(f)
                uri = phoneline_map.get(a)
                print(f"DEBUG: Found URI mapping: {uri}")
        else:
            print(f"Warning: {phoneline_config} not found. Checked at: {os.path.abspath(phoneline_config)}")
    except json.JSONDecodeError as e:
        print(f"Error: {phoneline_config} is not valid JSON: {e}")
    except Exception as e:
        print(f"Error reading {phoneline_config}: {e}")
    
    if not uri:
        print(f"No URI configured for phone number {a}. Using default.")
        uri = "ws://127.0.0.1:5000"
        print(f"DEBUG: Using default URI: {uri}")

    if uri_override:
        print(f"Using URI override: {uri_override}")
        uri = uri_override

    # Log the called number
    with open("call_log.txt", "a") as log_file:
        log_file.write(f"Called: {a} at {time.ctime()}\n")
    
    print(f"Attempting connection to {uri}...")
    try:
        async with websockets.connect(uri) as websocket:
            print("Connected to the receiver. Starting voice call...")
            # Initialize PyAudio
            p = pyaudio.PyAudio()
            stream = p.open(format=pyaudio.paInt16, channels=1, rate=44100, input=True, frames_per_buffer=1024)
            
            try:
                while True:
                    data = stream.read(1024)
                    await websocket.send(data)
            except KeyboardInterrupt:
                print("Ending voice call...")
            finally:
                stream.stop_stream()
                stream.close()
                p.terminate()
    except ConnectionRefusedError:
        print(f"Connection refused when trying to reach {uri}.")
        print("Make sure the receiver is running and listening on that address.")
    except OSError as e:
        print(f"OS error connecting to {uri}: {e}")
    except Exception as e:
        print(f"Failed to connect to {uri}: {e}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Voice call system")
    parser.add_argument("sim_file", nargs="?", default=None, help="Path to SIM card file (.casim)")
    parser.add_argument("--config", default="phoneline_uris.json", help="Path to phoneline URIs config JSON (default: phoneline_uris.json)")
    parser.add_argument("--phonelines", default="existing_phonelines.txt", help="Path to existing phonelines file (default: existing_phonelines.txt)")
    parser.add_argument("--uri", dest="uri_override", default=None, help="Override the target WebSocket URI for the call")
    
    args = parser.parse_args()
    
    # Run the voice call
    asyncio.run(voice_call(sim_file=args.sim_file, phoneline_config=args.config, phonelines_file=args.phonelines, uri_override=args.uri_override))