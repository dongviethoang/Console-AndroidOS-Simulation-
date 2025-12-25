#include <al.h>
#include <alc.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "sleepdelay.h"

// WAV file header structure
struct WAVHeader {
    char riff[4];           // "RIFF"
    uint32_t fileSize;      // File size - 8
    char wave[4];           // "WAVE"
    char fmt[4];            // "fmt "
    uint32_t fmtSize;       // Format chunk size
    uint16_t audioFormat;   // Audio format (1 = PCM)
    uint16_t channels;      // Number of channels
    uint32_t sampleRate;    // Sample rate
    uint32_t byteRate;      // Byte rate
    uint16_t blockAlign;    // Block align
    uint16_t bitsPerSample; // Bits per sample
    char data[4];           // "data"
    uint32_t dataSize;      // Data size
};

bool loadWAV(const char* filename, ALuint& buffer) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open WAV file: " << filename << std::endl;
        return false;
    }

    // Read WAV header
    WAVHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));

    // Verify it's a valid WAV file
    if (std::string(header.riff, 4) != "RIFF" || 
        std::string(header.wave, 4) != "WAVE" ||
        std::string(header.fmt, 4) != "fmt " ||
        std::string(header.data, 4) != "data") {
        std::cerr << "Error: Invalid WAV file format" << std::endl;
        file.close();
        return false;
    }

    // Only support PCM format
    if (header.audioFormat != 1) {
        std::cerr << "Error: Only PCM WAV files are supported" << std::endl;
        file.close();
        return false;
    }

    // Read audio data
    std::vector<char> audioData(header.dataSize);
    file.read(audioData.data(), header.dataSize);
    file.close();

    // Determine OpenAL format
    ALenum format;
    if (header.channels == 1) {
        format = (header.bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    } else if (header.channels == 2) {
        format = (header.bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Error: Unsupported number of channels: " << header.channels << std::endl;
        return false;
    }

    // Create OpenAL buffer and load data
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, audioData.data(), header.dataSize, header.sampleRate);

    // Check for OpenAL errors
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error loading WAV data into OpenAL buffer: " << error << std::endl;
        alDeleteBuffers(1, &buffer);
        return false;
    }

    std::cout << "WAV file loaded successfully:" << std::endl;
    std::cout << "  Channels: " << header.channels << std::endl;
    std::cout << "  Sample Rate: " << header.sampleRate << " Hz" << std::endl;
    std::cout << "  Bits per Sample: " << header.bitsPerSample << std::endl;
    std::cout << "  Duration: " << (float)header.dataSize / header.byteRate << " seconds" << std::endl;

    return true;
}

bool initializeOpenAL(ALCdevice*& device, ALCcontext*& context) {
    // Open default audio device
    device = alcOpenDevice(NULL);
    if (!device) {
        std::cerr << "Error: Cannot open OpenAL device" << std::endl;
        return false;
    }

    // Create audio context
    context = alcCreateContext(device, NULL);
    if (!context) {
        std::cerr << "Error: Cannot create OpenAL context" << std::endl;
        alcCloseDevice(device);
        return false;
    }

    // Make context current
    alcMakeContextCurrent(context);

    std::cout << "OpenAL initialized successfully" << std::endl;
    return true;
}

void cleanupOpenAL(ALCdevice* device, ALCcontext* context) {
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    std::cout << "OpenAL cleaned up" << std::endl;
}

bool playStartupSound() {
    ALCdevice* device;
    ALCcontext* context;
    
    // Initialize OpenAL
    if (!initializeOpenAL(device, context)) {
        return false;
    }

    ALuint buffer, source;
    
    // Load the Windows 3.1 startup sound
    if (!loadWAV("windows-31-startup-sound-classic-retro-computer-booting-audio-332124", buffer)) {
        cleanupOpenAL(device, context);
        return false;
    }

    // Create audio source
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_LOOPING, AL_FALSE);

    // Play the sound
    std::cout << "Playing Windows 3.1 startup sound..." << std::endl;
    alSourcePlay(source);

    // Wait for sound to finish playing
    ALint sourceState;
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
        delay(10); // Small delay to prevent busy waiting
    } while (sourceState == AL_PLAYING);

    std::cout << "Startup sound finished" << std::endl;

    // Cleanup
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    cleanupOpenAL(device, context);

    return true;
}