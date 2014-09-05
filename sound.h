#include <AL/alut.h>
#include <AL/al.h>
#include <AL/alc.h>

struct{
    ALuint bgMusicSource;
    ALuint rocketLaunchSource;
    ALuint crashSource;
}globalMusic;

void playRocketSound();
void playCrashSound();
void setupBackgroundMusic();
void setupRocketSound();
void setupCrashSound();
void setupAllMusic();
void stopAllMusic();
