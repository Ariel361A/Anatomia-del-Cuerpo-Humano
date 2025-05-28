#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <map>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool loadWAV(const std::string& name, const std::string& filepath);
    void play(const std::string& name);
    void pause(const std::string& name);
    void stop(const std::string& name);

    void setLooping(const std::string& name, bool loop);
    void setGain(const std::string& name, float gain);

private:
    ALCdevice* device;
    ALCcontext* context;

    std::map<std::string, ALuint> buffers;
    std::map<std::string, ALuint> sources;

    bool loadWAVFile(const std::string& filepath, ALuint& buffer);
};

#endif // AUDIOMANAGER_H
