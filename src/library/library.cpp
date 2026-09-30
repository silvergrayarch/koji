// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <taglib/fileref.h>

#include "library.h"

using namespace std;
using namespace filesystem;

path xdg_config_home()
{
  path config_path;

  const char *xdg_config = getenv("XDG_CONFIG_HOME");
  if (xdg_config)
    config_path = path(xdg_config);

  const char *home = getenv("HOME");
  if (home)
    config_path = path(home) / ".config";

  return config_path;
}

vector<SongEntry> get_album_songs(const AlbumEntry &album)
{
  vector<SongEntry> songs;
  for (const directory_entry &song : directory_iterator(album.path))
  {
    if (!is_regular_file(song))
      continue;

    TagLib::FileRef song_file(song.path().string().c_str());

    const int track = song_file.tag()->track();
    string title = song_file.tag()->title().to8Bit(true);
    string artist = song_file.tag()->artist().to8Bit(true);

    if (title.empty())
      title = song.path().stem().string();

    float duration;
    if (song_file.audioProperties() != nullptr)
      duration = song_file.audioProperties()->lengthInSeconds();
    else
      continue;

    if (title.empty())
      title = "Unknown";

    if (artist.empty())
      artist = "Unknown";

    SongEntry entry = {song.path(), artist, album.title, title, duration};

    if (track > 0 && track <= songs.size())
      songs.insert(songs.begin() + track - 1, entry);
    else
      songs.push_back(entry);
  }
  return songs;
}

vector<AlbumEntry> get_albums()
{
  vector<AlbumEntry> albums;

  path config_home = xdg_config_home();
  if (config_home.empty())
  {
    cout << "xdg config directory locate failed" << endl;
    return albums;
  }

  path album_directory = config_home / "koji" / "albums";

  for (const directory_entry &artist : directory_iterator(album_directory))
  {
    if (!is_directory(artist))
      continue;

    for (const directory_entry &album : directory_iterator(artist))
    {
      if (!is_directory(album))
        continue;

      const AlbumEntry entry = {album.path(), album.path().filename().string(), artist.path().filename().string()};
      albums.push_back(entry);
    }
  }

  for (AlbumEntry &album : albums)
    album.songs = get_album_songs(album);

  return albums;
}

vector<SongEntry> get_playlist_songs(const PlaylistEntry &playlist)
{
  vector<SongEntry> songs;

  ifstream playlist_file(playlist.path);

  if (!playlist_file)
    return songs;

  path songs_directory = playlist.path.parent_path();

  string song;
  while (getline(playlist_file, song))
  {
    string title;
    string album;
    string artist;
    path song_path;
    if (exists(songs_directory / song))
      song_path = songs_directory / song;
    else
      song_path = song;

    TagLib::FileRef song_file(song_path.c_str());

    title = song_file.tag()->title().to8Bit(true);
    album = song_file.tag()->album().to8Bit(true);
    artist = song_file.tag()->artist().to8Bit(true);

    if (title.empty())
      title = song_path.stem().string();

    float duration;
    if (song_file.audioProperties() != nullptr)
      duration = song_file.audioProperties()->lengthInSeconds();
    else
      continue;

    if (title.empty())
      title = "Unknown";

    if (album.empty())
      album = "Unknown";

    if (artist.empty())
      artist = "Unknown";

    SongEntry entry = {song_path, artist, album, title, duration};
    songs.push_back(entry);
  }
  return songs;
}

vector<PlaylistEntry> get_playlists()
{
  vector<PlaylistEntry> playlists;

  path config_home = xdg_config_home();
  if (config_home.empty())
  {
    cout << "xdg config directory locate failed" << endl;
    return playlists;
  }
  path playlists_directory = config_home / "koji" / "playlists";

  for (const directory_entry &folder : directory_iterator(playlists_directory))
  {
    if (!is_directory(folder))
      continue;

    const path m3u = folder.path() / (folder.path().filename().string() + ".m3u");
    const string title = m3u.stem().string();
    const PlaylistEntry entry = {m3u, title};

    playlists.push_back(entry);
  }

  for (PlaylistEntry &playlist : playlists)
    playlist.songs = get_playlist_songs(playlist);

  return playlists;
}

bool rename_playlist(const PlaylistEntry &entry, const string &name)
{
  const path m3u = entry.path;
  const path new_m3u = m3u.parent_path() / (name + m3u.extension().string());

  const path directory = m3u.parent_path();
  const path new_directory = directory.parent_path() / name;

  if (exists(new_m3u))
    return false;

  if (exists(new_directory))
    return false;

  rename(m3u, new_m3u);
  rename(directory, new_directory);

  return true;
}

void save_playlist(const PlaylistEntry &playlist)
{
  path config_home = xdg_config_home();
  if (config_home.empty())
  {
    cout << "xdg config directory locate failed" << endl;
    return;
  }

  ofstream playlist_file(playlist.path, ios::trunc);

  if (!playlist_file.is_open())
    return;

  path songs_directory = playlist.path.parent_path();

  for (const auto &song : playlist.songs)
  {
    const string song_filename = song.path.filename().string();
    if (exists(songs_directory / song_filename))
      playlist_file << song_filename << endl;
    else
      playlist_file << song.path.string() << endl;
  }

  playlist_file.close();
}

bool duplicate_playlist(const PlaylistEntry &playlist)
{
  const path directory = playlist.path.parent_path();
  const path directory_copy = directory.parent_path() / (playlist.title + " copy");

  const path m3u = directory_copy / playlist.path.filename();
  const path m3u_copy = m3u.parent_path() / (m3u.stem().string() + " copy" + m3u.extension().string());

  if (exists(directory_copy))
    return false;

  if (exists(m3u_copy))
    return false;

  copy(directory, directory_copy);
  rename(m3u, m3u_copy);

  return true;
}
