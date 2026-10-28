-- MCS-SHELL | WEZTERM-PROFIL
-- Wird nur genutzt, wenn die anderen Engines fehlen.

local config = {}

config.font = wezterm.font("FantasqueSansM Nerd Font Mono")
config.font_size = 15.0
config.initial_cols = 120
config.initial_rows = 40
config.window_background_opacity = 0.94
config.hide_tab_bar_if_only_one_tab = true
config.enable_scroll_bar = false
config.scrollback_lines = 10000
config.window_decorations = "NONE"
config.default_cursor_style = "BlinkingBlock"

config.colors = {
  background = "#0b0f14",
  foreground = "#e8e8e8",
  cursor_bg = "#ff9f1c",
  cursor_fg = "#0b0f14",
  selection_bg = "#ff9f1c",
  selection_fg = "#0b0f14",
  ansi = {
    "#0b0f14", "#ff5f5f", "#5fd75f", "#ffd75f",
    "#5fafff", "#d787ff", "#5fd7d7", "#d0d0d0",
  },
  brights = {
    "#4e4e4e", "#ff8787", "#87ff87", "#ffff87",
    "#87d7ff", "#ff87ff", "#87ffff", "#ffffff",
  },
}

return config
