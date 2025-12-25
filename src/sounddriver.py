import sys
import os
import time
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'

import sounddevice as sd
import soundfile as sf

def play_error_sound():
    """Play the bundled error sound (best-effort)."""
    err_path = os.path.join(os.path.dirname(__file__), "sounds", "windows-7-critical-stop.wav")
    try:
        if os.path.isfile(err_path):
            data, fs = sf.read(err_path, dtype='float32')
            sd.play(data, fs)
            sd.wait()
    except Exception:
        try:
            sd.stop()
        except Exception:
            pass

def has_ext(filename: str, exts):
    fn = filename.lower()
    return any(fn.endswith(e) for e in exts)

def suspicious_path(filename: str):
    return any(ch in filename for ch in "&;|<>`$")

def main():
    allowed = (".ogg", ".wav", ".mp3", ".flac")
    if len(sys.argv) < 2:
        print("Usage: python sounddriver.py <soundfile>")
        sys.exit(2)

    filename = sys.argv[1].strip()

    # refuse obviously unsafe input early
    if suspicious_path(filename):
        print("Refusing to open suspicious path.")
        play_error_sound()
        sys.exit(3)

    # normalize path and check extension
    filename = os.path.normpath(filename)
    if not has_ext(filename, allowed):
        print("Unsupported file type. Supported: " + ", ".join(allowed))
        play_error_sound()
        sys.exit(4)

    if not os.path.isfile(filename):
        print(f"Error: Sound file not found: {filename}")
        play_error_sound()
        sys.exit(1)

    # Try to play via pygame if available; otherwise fallback to soundfile+sounddevice
    try:
        import pygame
        try:
            pygame.mixer.init()
        except Exception as e:
            print("Failed to initialize pygame mixer:", e)
            raise

        try:
            sound = pygame.mixer.Sound(filename)
        except Exception as e:
            print("Failed to load sound:", e)
            pygame.mixer.quit()
            play_error_sound()
            sys.exit(6)

        channel = sound.play()
        if channel is None:
            print("Failed to play sound via pygame.")
            pygame.mixer.quit()
            play_error_sound()
            sys.exit(8)

        # Wait until playback finishes (with timeout safety)
        timeout = 300  # seconds max
        start = time.time()
        while channel.get_busy() and (time.time() - start) < timeout:
            time.sleep(0.05)

        try:
            pygame.mixer.quit()
        except Exception:
            pass
        sys.exit(0)

    except Exception:
        # fallback: try using soundfile+sounddevice to play the file
        try:
            data, fs = sf.read(filename, dtype='float32')
            sd.play(data, fs)
            sd.wait()
            sys.exit(0)
        except Exception as e:
            print("Failed to play sound (fallback):", e)
            play_error_sound()
            sys.exit(9)

if __name__ == "__main__":
    main()