// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray

#include "player.h"
#include <algorithm>
#include <clocale>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

bool Player::init()
{
  setlocale(LC_NUMERIC, "C");

  mpv_context = mpv_create();

  if (!mpv_context)
  {
    cout << "Error creating mpv context" << endl;
    cleanup();
    return false;
  }

  if (mpv_initialize(mpv_context) != 0)
  {
    cout << "Error initializing mpv context" << endl;
    cleanup();
    return false;
  }

  mpv_set_option_string(mpv_context, "vo", "null");
  mpv_set_option_string(mpv_context, "audio-format", "s16le");
  updateVolume();

  return true;
}

void Player::update()
{
  if (current_song == -1)
    return;

  double time_remaining = 0.0;
  mpv_get_property(mpv_context, "time-pos", MPV_FORMAT_DOUBLE, &time_remaining);
  position = static_cast<float>(time_remaining);

  mpv_event *event = mpv_wait_event(mpv_context, 0);

  if (event->event_id != MPV_EVENT_END_FILE)
    return;

  mpv_event_end_file *end_file = static_cast<mpv_event_end_file *>(event->data);

  if (end_file->reason != MPV_END_FILE_REASON_EOF)
    return;

  if (repeat_mode == RepeatMode::Track)
  {
    updateCurrentSong();
    return;
  }

  int next_song = current_song + 1;

  if (next_song >= queue.size() && repeat_mode == RepeatMode::All)
    current_song = 0;
  else if (next_song >= queue.size())
    stopPlayback();
  else
    current_song = next_song;

  updateCurrentSong();
}

void Player::cleanup() { mpv_destroy(mpv_context); }

void Player::togglePause()
{
  paused = !paused;

  if (paused)
    mpv_set_property_string(mpv_context, "pause", "yes");
  else
    mpv_set_property_string(mpv_context, "pause", "no");
}

void Player::toggleRepeat()
{
  if (repeat_mode == RepeatMode::Off)
    repeat_mode = RepeatMode::All;
  else if (repeat_mode == RepeatMode::All)
    repeat_mode = RepeatMode::Track;
  else if (repeat_mode == RepeatMode::Track)
    repeat_mode = RepeatMode::Off;
}

void Player::toggleShuffle()
{
  shuffle = !shuffle;

  bool has_current_song = current_song >= 0 && current_song < queue.size();
  SongEntry currently_playing;

  if (has_current_song)
    currently_playing = queue[current_song];

  if (shuffle)
  {
    unshuffled_queue = queue;
    random_engine.seed(random_device{}());
    ranges::shuffle(queue, random_engine);
  }
  else
  {
    queue = unshuffled_queue;
    unshuffled_queue.clear();
  }

  if (has_current_song)
  {
    auto iterator = find(queue.begin(), queue.end(), currently_playing);

    if (iterator != queue.end())
      current_song = distance(queue.begin(), iterator);
  }
}

void Player::updateVolume() { mpv_set_property_string(mpv_context, "volume", to_string(volume).c_str()); }

void Player::stopPlayback()
{
  current_song = -1;
  paused = true;
  mpv_command_string(mpv_context, "stop");
  mpv_set_property_string(mpv_context, "pause", "yes");
}

void Player::clearQueue()
{
  queue.clear();
  unshuffled_queue.clear();
}

void Player::updateCurrentSong()
{
  if (current_song < 0 || current_song >= queue.size())
    return;

  const char *play_command[] = {"loadfile", queue[current_song].path.c_str(), "replace", nullptr};
  mpv_command(mpv_context, play_command);
  mpv_set_property_string(mpv_context, "pause", "no");
}

void Player::addSongsToQueue(vector<SongEntry> &songs)
{
  if (shuffle)
  {
    unshuffled_queue.insert(unshuffled_queue.end(), songs.begin(), songs.end());
    ranges::shuffle(songs, random_engine);
  }

  bool empty_queue = queue.empty();

  queue.insert(queue.end(), songs.begin(), songs.end());

  if (empty_queue)
  {
    current_song = 0;
    updateCurrentSong();
  }
}
