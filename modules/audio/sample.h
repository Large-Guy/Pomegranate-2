#ifndef POMEGRANATEENGINE_SAMPLE_H
#define POMEGRANATEENGINE_SAMPLE_H
#include <cstddef>
#include <vector>

class AudioSample {
private:
    std::vector<float> _data;
    size_t _channels;
    size_t _sampleRate;
    size_t _bitDepth;
public:
    AudioSample(std::vector<float> data, size_t channels, size_t sampleRate);
    float* getData();
    size_t getSize();
    size_t getChannels();
    size_t getSampleRate();
    size_t getBitDepth();

    void setData(std::vector<float> data);
    void setChannels(size_t channels);
    void setSampleRate(size_t sampleRate);
    void setBitDepth(size_t bitDepth);
};


#endif //POMEGRANATEENGINE_SAMPLE_H
