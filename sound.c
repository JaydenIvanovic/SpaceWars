#include <stdlib.h>
#include <stdio.h>
#include "sound.h"



/*Keep alut calls separate to main application which
 *can access sounds through calls such as the following.
 *Plays a rocket launching sound.*/
void playRocketSound()
{
    alSourcePlay(globalMusic.rocketLaunchSource);
}




/*Plays a crash sound for collision.*/
void playCrashSound()
{
    alSourcePlay(globalMusic.crashSource);
}




/*Setup the buffer to read in the music to a source.
 *Create a thread which loops the song.*/
void setupBackgroundMusic()
{
    alutInit(NULL, NULL);

    ALuint bgMusicBuffer;

    bgMusicBuffer = alutCreateBufferFromFile("./Music/EightBitSpaceBattle.wav");
    
    ALenum error = alutGetError();
    if(error != ALUT_ERROR_NO_ERROR)
        printf("Returned:%s\n",alutGetErrorString(error));
    
    alGenSources(1, &globalMusic.bgMusicSource);

    alSourcef(globalMusic.bgMusicSource, AL_GAIN, 0.6f);
    alSourcei(globalMusic.bgMusicSource, AL_BUFFER, bgMusicBuffer);
    alSourcei(globalMusic.bgMusicSource, AL_LOOPING, AL_TRUE);
}




/*Setup the sounds for a missile launch.*/
void setupRocketSound()
{
    ALuint rocketLaunchBuffer;

    rocketLaunchBuffer = alutCreateBufferFromFile("./Music/LaserBlasts.wav");
    
    ALenum error = alutGetError();
    if(error != ALUT_ERROR_NO_ERROR)
        printf("Returned:%s\n",alutGetErrorString(error));
    
    alGenSources(1, &globalMusic.rocketLaunchSource);

    alSourcef(globalMusic.rocketLaunchSource, AL_GAIN, 0.5f);
    alSourcei(globalMusic.rocketLaunchSource, AL_BUFFER, rocketLaunchBuffer);
}




/*Setup the crash sounds.*/
void setupCrashSound()
{
    ALuint crashBuffer;

    crashBuffer = alutCreateBufferFromFile("./Music/8BitExplosion.wav");
    
    ALenum error = alutGetError();
    if(error != ALUT_ERROR_NO_ERROR)
        printf("Returned:%s\n",alutGetErrorString(error));
    
    alGenSources(1, &globalMusic.crashSource);

    alSourcef(globalMusic.crashSource, AL_GAIN, 0.4f);
    alSourcei(globalMusic.crashSource, AL_BUFFER, crashBuffer);
}




/*Helper to call all setup functions.*/
void setupAllMusic()
{
    setupBackgroundMusic();
    setupRocketSound();
    setupCrashSound();

    alSourcePlay(globalMusic.bgMusicSource);
}




/*Hide away alut call in a function so if other operations
 *have to be performed before it in the future it can
 *be done here.*/
void stopAllMusic()
{
    alutExit();
}
