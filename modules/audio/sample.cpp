#include "sample.h"

void AudioSample::loadWAV(File& path) {
    WAVHeader header;
    path.open();
    List<char> buffer = path.readBuffer();
    size_t size = buffer.size();
    const char* data = buffer.data();

    // Copy the WAV header
    memcpy(&header, data, sizeof(WAVHeader));

    // Store WAV file metadata
    _channels = header.channels;
    _sampleRate = header.sampleRate;
    _bitDepth = header.bitsPerSample;

    // Search for the "data" chunk
    size_t i = sizeof(WAVHeader); // Start after the initial header
    uint32_t dataSize = 0;

    while (i < size) {  // Ensure we don't go out of bounds
        // Read the chunk header
        if (memcmp(&data[i], "data", 4) == 0) {
            // Found "data" chunk
            i++;
            memcpy(&dataSize, &data[i], sizeof(uint32_t));  // Get data size
            i += sizeof(uint32_t);  // Move to the start of the audio data
            break;
        } else {
            // Not "data" chunk, move past this chunk
            uint32_t chunkSize;
            memcpy(&chunkSize, &data[i + 4], sizeof(uint32_t));
            i++;
        }
    }

    if (dataSize == 0) {
        std::cerr << "Could not find 'data' chunk in WAV file." << std::endl;
        path.close();
        return;
    }

    // Copy the audio data
    _data = std::vector<char>(data + i, data + i + header.chunkSize);

    path.close();
}


AudioSample::AudioSample(std::vector<char> data, size_t channels, size_t sampleRate) : Asset() {
    _data = data;
    _channels = channels;
    _sampleRate = sampleRate;
}

AudioSample::AudioSample(std::string path, std::string name) : Asset(path, name) {
    // Load audio file
    File file(path);
    if(file.exists()) {
        loadWAV(file);
    }
    else {
        // Error
    }
}

char *AudioSample::getData() {
    return _data.data();
}

size_t AudioSample::getSize() {
    return _data.size();
}

size_t AudioSample::getChannels() {
    return _channels;
}

size_t AudioSample::getSampleRate() {
    return _sampleRate;
}

size_t AudioSample::getBitDepth() {
    return _bitDepth;
}

void AudioSample::setData(std::vector<char> data) {
    _data = data;
}

void AudioSample::setChannels(size_t channels) {
    _channels = channels;
}

void AudioSample::setSampleRate(size_t sampleRate) {
    _sampleRate = sampleRate;
}

void AudioSample::setBitDepth(size_t bitDepth) {
    _bitDepth = bitDepth;
}