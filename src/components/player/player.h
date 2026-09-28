// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#pragma once

#include <random>
#include <vector>
#include <gtkmm.h>
#include <mpv/client.h>
#include "../../library/entries.h"
#include "../../library/library.h"
#include "../albums/albums.h"
#include "../playlists/playlists.h"
#include "../queue/queue.h"
#include "../utils/utils.h"

enum class RepeatMode
{
    Off,
    All,
    Track
};

class Player
{
  public:
    Player();
    bool init();
    void update();
    void cleanup();

    void togglePause();
    void toggleRepeat();
    void toggleShuffle();
    void updateVolume();
    void stopPlayback();

    void clearQueue();
    void updateCurrentSong();
    void addSongsToQueue(std::vector<SongEntry> &songs);

    int   volume   = 35;
    bool  paused   = false;
    bool  shuffle  = false;
    float position = 0.0f; // in seconds

    Queue     queue;
    Albums    albums;
    Playlists playlists;

    RepeatMode repeat_mode = RepeatMode::All;

    int          current_song = -1;
    std::mt19937 random_engine{std::random_device{}()};
    mpv_handle  *mpv_context = nullptr;
};
