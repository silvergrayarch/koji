// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray
#include <format>
#include <gtkmm.h>
#include <iostream>
#include <string>
#include <vector>

#include "components/footer.h"
#include "components/tabs/albums_tab.h"
#include "components/tabs/playlists_tab.h"
#include "components/tabs/queue_tab.h"
#include "player/player.h"

class Window : public Gtk::Window
{
public:
  Window();
  virtual ~Window();

  bool update();

  Player player;

  Gtk::Box main_window{Gtk::Orientation::VERTICAL};

  Gtk::Notebook tabbar;
  QueueTab queue_tab;
  AlbumsTab albums_tab;
  PlaylistsTab playlists_tab;

  Footer footer;

private:
  bool onWindowKeyPressed(guint keyval, guint keycode, Gdk::ModifierType state);
};

bool Window::update()
{
  player.update();
  queue_tab.update();
  footer.update(player);
  return true;
}

Window::Window()
{
  set_title("Koji");
  set_default_size(1920, 1080);
  set_child(main_window);

  tabbar.set_vexpand(true);
  main_window.append(tabbar);
  main_window.append(footer);

  if (!player.init())
    return;

  queue_tab.setPlayer(&player);
  albums_tab.setPlayer(&player);
  playlists_tab.setPlayer(&player);

  albums_tab.update();
  playlists_tab.update();

  tabbar.append_page(queue_tab.box, "Queue");
  tabbar.append_page(albums_tab.box, "Albums");
  tabbar.append_page(playlists_tab.box, "Playlists");

  auto controller = Gtk::EventControllerKey::create();
  controller->signal_key_pressed().connect(sigc::mem_fun(*this, &Window::onWindowKeyPressed), false);
  add_controller(controller);

  Glib::signal_timeout().connect(sigc::mem_fun(*this, &Window::update), 250);
}

bool Window::onWindowKeyPressed(guint keyval, guint, Gdk::ModifierType state)
{
  if (keyval == GDK_KEY_Escape) // - `Esc`: Quits the program
  {
    close();
    return true;
  }
  else if (keyval == GDK_KEY_s) // - `S`: Toggle shuffle
  {
    player.toggleShuffle();
    return true;
  }
  else if (keyval == GDK_KEY_r) // - `R`: Toggle repeat mode
  {
    player.toggleRepeat();
    return true;
  }
  else if (keyval == GDK_KEY_x) // - `X`: Stop music
  {
    player.stopPlayback();
    return true;
  }
  else if (keyval == GDK_KEY_space) // - `Space Bar`: Toggle pause
  {
    player.togglePause();
    return true;
  }
  else if (keyval == GDK_KEY_equal && player.volume + 5 <= 100) // - `+`: Increase volume by 5%
  {
    player.volume += 5;
    player.updateVolume();
    return true;
  }
  else if (keyval == GDK_KEY_minus && player.volume - 5 >= 0) // - `-`: Decrease volume by 5%
  {
    player.volume -= 5;
    player.updateVolume();
    return true;
  }
  else if (keyval == GDK_KEY_Tab) // - `Tab`: Cycle tabs
  {
    const int current_page = tabbar.get_current_page();
    const int pages = tabbar.get_n_pages();

    if (current_page + 1 >= pages)
      tabbar.set_current_page(0);
    else
      tabbar.set_current_page(current_page + 1);

    return true;
  }

  return false;
}

Window::~Window()
{
  player.cleanup();
  albums_tab.cleanup();
  playlists_tab.cleanup();
}

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create("cc.silverfiles.test");
  Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme() = true;
  return app->make_window_and_run<Window>(argc, argv);
}
