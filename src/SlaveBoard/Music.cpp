#include "Music.h"

/**
   \brief Object instancing the SdFat library.

   principal object for handling all SdCard functions.
*/
static SdFat sd;

/**
   \brief Object instancing the SFEMP3Shield library.

   principal object for handling all the attributes, members and functions for the library.
*/
SFEMP3Shield MP3player;

static uint32_t trackPosition[NBR_PAUSABLE_TRACKS];
static int currentTrack;

void setupMusic()
{

  // Below is only needed if not interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) \
    && ( (USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer) \
    ||   (USE_MP3_REFILL_MEANS == USE_MP3_Polled)      )

  MP3player.available();
#endif

  if (!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if (!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  uint8_t result;
  result = MP3player.begin();

#if defined(__BIOFEEDBACK_MEGA__) // or other reasons, of your choosing.
  // Typically not used by most shields, hence commented out.
  if (MP3player.ADMixerLoad("admxster.053") == 0) {
    MP3player.ADMixerVol(-3);
  }
#endif

  resetTrackPositions();
}

int getTrackPauseIndex(int currentTrack)
{
  int index = -1;

  if (index <= SUPER_STAR && index >= UNDERGROUND)
  {
    index = currentTrack - UNDERGROUND; //The underground track will be associated with index 0 and super star with index 1
  }

  return index;
}

void stopMusic()
{
  int index = getTrackPauseIndex(currentTrack);
  if (index != -1)
  {
    trackPosition[index] = MP3player.currentPosition(); //-1 to the track as track ids start at 1 but array index starts at 0
  }
  MP3player.stopTrack();
}

void playMusic(int track)
{
  //a storage place for track names
  char trackName[] = "track001.mp3";
  uint8_t trackNumber = 1;
  uint8_t result;

  //tack the number onto the rest of the filename
  sprintf(trackName, "track%03d.mp3", track);

  int index = getTrackPauseIndex(track);
  uint32_t pos = 0;
  if (index != -1)
  {
    pos = trackPosition[index];
  }
  result = MP3player.playMP3(trackName, pos);

  //check result, see readme for error codes.
  if (result == 1)
  {
    stopMusic();
  }

  result = MP3player.playMP3(trackName, pos);
  currentTrack = track;
}

char* getTrackTitle()
{
  char title[30];
  //we can get track info by using the following functions and arguments
  //the functions will extract the requested information, and put it in the array we pass in
  MP3player.trackTitle((char*)&title);
  return title;
}

void resetTrackPositions()
{
  for (int i = 0; i < NBR_PAUSABLE_TRACKS; i++)
  {
    trackPosition[i] = 0;
  }
}
