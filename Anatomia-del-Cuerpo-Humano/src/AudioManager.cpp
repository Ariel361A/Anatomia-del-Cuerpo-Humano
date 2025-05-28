#include "AudioManager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

AudioManager::AudioManager() {
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "No se pudo abrir el dispositivo de audio\n";
        return;
    }

    context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context)) {
        std::cerr << "No se pudo crear el contexto de audio\n";
        return;
    }
}

AudioManager::~AudioManager() {
    for (auto& [_, src] : sources) {
        alDeleteSources(1, &src);
    }
    for (auto& [_, buf] : buffers) {
        alDeleteBuffers(1, &buf);
    }

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool AudioManager::loadWAV(const std::string& name, const std::string& filepath) {
    ALuint buffer;
    if (!loadWAVFile(filepath, buffer)) {
        std::cerr << "Error cargando WAV: " << filepath << "\n";
        return false;
    }

    buffers[name] = buffer;

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    sources[name] = source;

    return true;
}

void AudioManager::play(const std::string& name) {
    if (sources.count(name)) {
        alSourcePlay(sources[name]);
    }
}

void AudioManager::pause(const std::string& name) {
    if (sources.count(name)) {
        alSourcePause(sources[name]);
    }
}

void AudioManager::stop(const std::string& name) {
    if (sources.count(name)) {
        alSourceStop(sources[name]);
    }
}

void AudioManager::setLooping(const std::string& name, bool loop) {
    if (sources.count(name)) {
        alSourcei(sources[name], AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }
}

void AudioManager::setGain(const std::string& name, float gain) {
    if (sources.count(name)) {
        alSourcef(sources[name], AL_GAIN, gain);
    }
}

bool AudioManager::loadWAVFile(const std::string& filepath, ALuint& buffer) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filepath << "\n";
        return false;
    }

    // Variables que necesitamos conservar
    int32_t sampleRate = 0;
    int16_t channels = 0;
    int16_t bitsPerSample = 0;

    // Leer encabezado RIFF
    char chunkId[5] = { 0 };
    file.read(chunkId, 4);
    if (std::string(chunkId) != "RIFF") {
        std::cerr << "Archivo no es formato RIFF\n";
        return false;
    }

    file.ignore(4); // Tamaño del archivo (no lo necesitamos)

    file.read(chunkId, 4);
    if (std::string(chunkId) != "WAVE") {
        std::cerr << "Archivo no es formato WAVE\n";
        return false;
    }

    // Buscar el chunk 'fmt '
    bool fmtFound = false;
    while (!fmtFound && !file.eof()) {
        file.read(chunkId, 4);
        uint32_t chunkSize;
        file.read(reinterpret_cast<char*>(&chunkSize), 4);

        if (std::string(chunkId) == "fmt ") {
            fmtFound = true;

            int16_t audioFormat;
            file.read(reinterpret_cast<char*>(&audioFormat), 2);
            file.read(reinterpret_cast<char*>(&channels), 2);
            file.read(reinterpret_cast<char*>(&sampleRate), 4);
            file.ignore(4); // byteRate (no lo necesitamos)
            file.ignore(2); // blockAlign (no lo necesitamos)
            file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

            // Verificar formato soportado
            if (audioFormat != 1) { // 1 = PCM
                std::cerr << "Formato de audio no soportado (solo PCM)\n";
                return false;
            }

            // Saltar el resto del chunk si es necesario
            if (chunkSize > 16) {
                file.ignore(chunkSize - 16);
            }
        }
        else {
            // Saltar este chunk
            file.ignore(chunkSize);
        }
    }

    if (!fmtFound) {
        std::cerr << "No se encontró el chunk 'fmt '\n";
        return false;
    }

    // Buscar el chunk 'data'
    bool dataFound = false;
    uint32_t dataSize = 0;
    std::vector<char> audioData;

    while (!dataFound && !file.eof()) {
        file.read(chunkId, 4);
        file.read(reinterpret_cast<char*>(&dataSize), 4);

        if (std::string(chunkId) == "data") {
            dataFound = true;

            // Leer los datos de audio
            audioData.resize(dataSize);
            file.read(audioData.data(), dataSize);

            // Aplicar padding si es necesario
            size_t padding = (4 - (dataSize % 4)) % 4;
            if (padding > 0) {
                audioData.insert(audioData.end(), padding, 0); // Rellenar con ceros
                std::cout << "Aplicado padding de " << padding << " bytes al archivo WAV\n";
            }
        }
        else {
            // Saltar este chunk
            file.ignore(dataSize);
        }
    }

    if (!dataFound) {
        std::cerr << "No se encontró el chunk 'data'\n";
        return false;
    }

    file.close();

    // Determinar el formato OpenAL
    ALenum format = 0;
    if (channels == 1) {
        format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    }
    else if (channels == 2) {
        format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }
    else {
        std::cerr << "Número de canales no soportado: " << channels << "\n";
        return false;
    }

    // Crear buffer OpenAL
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, audioData.data(), static_cast<ALsizei>(audioData.size()), sampleRate);

    // Verificar errores
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error al cargar el buffer OpenAL: " << error << "\n";
        alDeleteBuffers(1, &buffer);
        return false;
    }


    return true;
}