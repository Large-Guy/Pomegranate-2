#include "sample.h"

AudioSample::AudioSample(std::vector<float> data, size_t channels, size_t sampleRate) {
    _data = data;
    _channels = channels;
    _sampleRate = sampleRate;
}

float *AudioSample::getData() {
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

void AudioSample::setData(std::vector<float> data) {
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