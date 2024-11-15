#ifndef POMEGRANATEENGINE_SAMPLE_H
#define POMEGRANATEENGINE_SAMPLE_H
#include <cstddef>
#include <vector>
#include <core/core.h>

class AudioSample : Asset {
private:

    struct WAVHeader {
        char riff[4];
        uint32_t chunkSize;
        char wave[4];
        char fmt[4];
        uint32_t waveSize;
        uint16_t audioFormat;
        uint16_t channels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        char data[4];
        uint32_t dataSize;
    };

    void loadWAV(File& path);

    std::vector<char> _data;
    size_t _channels;
    size_t _sampleRate;
    size_t _bitDepth;
public:
    AudioSample(std::vector<char> data, size_t channels, size_t sampleRate);
    AudioSample(std::string path, std::string name);
    char* getData();
    size_t getSize();
    size_t getChannels();
    size_t getSampleRate();
    size_t getBitDepth();

    void setData(std::vector<char> data);
    void setChannels(size_t channels);
    void setSampleRate(size_t sampleRate);
    void setBitDepth(size_t bitDepth);

    template<typename T>
    T sample(size_t index, size_t channel) {
        if(index * _channels + channel >= _data.size() / sizeof(T)) {
            return 0;
        }

        return *(T*)(_data.data() + (index * _channels + channel) * sizeof(T));
    }
};


#endif //POMEGRANATEENGINE_SAMPLE_H
