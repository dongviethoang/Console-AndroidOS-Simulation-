import asyncio
import websockets
import pyaudio
import os
import time
import json
import sys
import argparse
from zeroconf import ServiceInfo, Zeroconf

async def voice_call(uri, sim_file=None, phoneline_config="phoneline_uris.json", phonelines_file="existing_phonelines.txt"):
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

    # Log the called number
    with open("call_log.txt", "a") as log_file:
        log_file.write(f"Called: {a} at {time.ctime()}\n")
    
    # Start the voice call
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

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Voice call system")
    parser.add_argument("sim_file", nargs="?", default=None, help="Path to SIM card file (.casim)")
    parser.add_argument("--config", default="phoneline_uris.json", help="Path to phoneline URIs config JSON (default: phoneline_uris.json)")
    parser.add_argument("--phonelines", default="existing_phonelines.txt", help="Path to existing phonelines file (default: existing_phonelines.txt)")
    
    args = parser.parse_args()
    
    # Get script directory and check for config files there first
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    config_file = args.config
    phonelines_file = args.phonelines
    
    if config_file == "phoneline_uris.json" and os.path.exists(os.path.join(script_dir, "phoneline_uris.json")):
        config_file = os.path.join(script_dir, "phoneline_uris.json")
    
    if phonelines_file == "existing_phonelines.txt" and os.path.exists(os.path.join(script_dir, "existing_phonelines.txt")):
        phonelines_file = os.path.join(script_dir, "existing_phonelines.txt")
    
    asyncio.run(voice_call(None, sim_file=args.sim_file, phoneline_config=config_file, phonelines_file=phonelines_file))
