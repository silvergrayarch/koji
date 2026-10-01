// SPDX-License-Identifier: GPL-3.0
// SPDX-FileCopyrightText: 2026 silver_gray

#include "playlists_tab.h"

PlaylistsTab::PlaylistsTab()
{
  window.set_child(tree);
  tree.set_enable_search(false);
  tree.set_rubber_banding(false);

  window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  window.set_expand();

  box.append(window);

  std::vector<Glib::ustring> album_column_headers = {"Playlist"};
  tree_refrence = setupStringTreeView(tree, collumns, album_column_headers);

  auto mouse_click = Gtk::GestureClick::create();
  mouse_click->set_button(GDK_BUTTON_PRIMARY);
  mouse_click->signal_pressed().connect(sigc::mem_fun(*this, &PlaylistsTab::onLeftClick));
  tree.add_controller(mouse_click);

  mouse_click = Gtk::GestureClick::create();
  mouse_click->set_button(GDK_BUTTON_SECONDARY);
  mouse_click->signal_pressed().connect(sigc::mem_fun(*this, &PlaylistsTab::onRightClick));
  tree.add_controller(mouse_click);

  auto menu = Gio::Menu::create();
  menu->append("Append to queue", "playlists.append");

  auto actions = Gio::SimpleActionGroup::create();
  actions->add_action("append", sigc::mem_fun(*this, &PlaylistsTab::appendButtonClick));
  box.insert_action_group("playlists", actions);

  popup.set_menu_model(menu);
  popup.set_parent(box);
  popup.set_has_arrow(false);
}

void PlaylistsTab::update()
{
  if (!(tree_refrence->children().size() == 0))
    tree_refrence->clear();

  for (PlaylistEntry &playlist : player_->playlists)
  {
    auto row = *(tree_refrence->append());
    row[collumns.string_columns[0]] = playlist.title;
  }
}

void PlaylistsTab::cleanup() { popup.unparent(); }

// makes selected_playlist_ -1 on bad value
void PlaylistsTab::getSelectedPlaylist(double x, double y)
{
  double offset_y = y - tree.get_column(0)->get_button()->get_allocation().get_height();

  Gtk::TreeModel::Path path;

  if (!tree.get_path_at_pos(static_cast<int>(x), static_cast<int>(offset_y), path))
  {
    selected_playlist_ = -1;
    return;
  }

  selected_playlist_ = path[0];

  if (selected_playlist_ < 0 || selected_playlist_ >= player_->playlists.size())
    selected_playlist_ = -1;

  return;
}

void PlaylistsTab::onLeftClick(int n_press, double x, double y)
{
  getSelectedPlaylist(x, y);
  if (selected_playlist_ == -1)
    return;

  player_->clearQueue();
  player_->addSongsToQueue(player_->playlists[selected_playlist_].songs);
}

void PlaylistsTab::onRightClick(int n_press, double x, double y)
{
  getSelectedPlaylist(x, y);
  if (selected_playlist_ == -1)
    return;

  const Gdk::Rectangle rect(x, y, 1, 1);
  popup.set_pointing_to(rect);
  popup.popup();
}

void PlaylistsTab::appendButtonClick() { player_->addSongsToQueue(player_->playlists[selected_playlist_].songs); }
