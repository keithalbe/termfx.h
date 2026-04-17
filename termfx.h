// Minimal ANSI terminal effects helpers for POSIX terminals.

#ifndef TERMFX_H
#define TERMFX_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TFX_ESC          "\x1b"
#define TFX_CSI          "\x1b["
#define TFX_OSC          "\x1b]"
#define TFX_ST           "\x1b\\"

// Styles
#define TFX_RESET        TFX_CSI"0m"
#define TFX_BOLD         TFX_CSI"1m"
#define TFX_DIM          TFX_CSI"2m"
#define TFX_ITALIC       TFX_CSI"3m"
#define TFX_UNDERLINE    TFX_CSI"4m"
#define TFX_BLINK        TFX_CSI"5m"
#define TFX_REVERSE      TFX_CSI"7m"
#define TFX_HIDDEN       TFX_CSI"8m"
#define TFX_STRIKE       TFX_CSI"9m"

#define TFX_NORMAL       TFX_CSI"22m"
#define TFX_NO_ITALIC    TFX_CSI"23m"
#define TFX_NO_UNDERLINE TFX_CSI"24m"
#define TFX_NO_BLINK     TFX_CSI"25m"
#define TFX_NO_REVERSE   TFX_CSI"27m"
#define TFX_NO_HIDDEN    TFX_CSI"28m"
#define TFX_NO_STRIKE    TFX_CSI"29m"

// Foreground colors
#define TFX_FG_BLACK     TFX_CSI"30m"
#define TFX_FG_RED       TFX_CSI"31m"
#define TFX_FG_GREEN     TFX_CSI"32m"
#define TFX_FG_YELLOW    TFX_CSI"33m"
#define TFX_FG_BLUE      TFX_CSI"34m"
#define TFX_FG_MAGENTA   TFX_CSI"35m"
#define TFX_FG_CYAN      TFX_CSI"36m"
#define TFX_FG_WHITE     TFX_CSI"37m"
#define TFX_FG_DEFAULT   TFX_CSI"39m"

// Background colors
#define TFX_BG_BLACK     TFX_CSI"40m"
#define TFX_BG_RED       TFX_CSI"41m"
#define TFX_BG_GREEN     TFX_CSI"42m"
#define TFX_BG_YELLOW    TFX_CSI"43m"
#define TFX_BG_BLUE      TFX_CSI"44m"
#define TFX_BG_MAGENTA   TFX_CSI"45m"
#define TFX_BG_CYAN      TFX_CSI"46m"
#define TFX_BG_WHITE     TFX_CSI"47m"
#define TFX_BG_DEFAULT   TFX_CSI"49m"

// Bright foreground
#define TFX_FG_BBLACK    TFX_CSI"90m"
#define TFX_FG_BRED      TFX_CSI"91m"
#define TFX_FG_BGREEN    TFX_CSI"92m"
#define TFX_FG_BYELLOW   TFX_CSI"93m"
#define TFX_FG_BBLUE     TFX_CSI"94m"
#define TFX_FG_BMAGENTA  TFX_CSI"95m"
#define TFX_FG_BCYAN     TFX_CSI"96m"
#define TFX_FG_BWHITE    TFX_CSI"97m"

// Bright background
#define TFX_BG_BBLACK    TFX_CSI"100m"
#define TFX_BG_BRED      TFX_CSI"101m"
#define TFX_BG_BGREEN    TFX_CSI"102m"
#define TFX_BG_BYELLOW   TFX_CSI"103m"
#define TFX_BG_BBLUE     TFX_CSI"104m"
#define TFX_BG_BMAGENTA  TFX_CSI"105m"
#define TFX_BG_BCYAN     TFX_CSI"106m"
#define TFX_BG_BWHITE    TFX_CSI"107m"

// Screen / cursor
#define TFX_CLEAR        TFX_CSI"2J"
#define TFX_CLEAR_LINE   TFX_CSI"2K"
#define TFX_CURSOR_HOME  TFX_CSI"H"
#define TFX_CURSOR_HIDE  TFX_CSI"?25l"
#define TFX_CURSOR_SHOW  TFX_CSI"?25h"
#define TFX_ALT_ENTER    TFX_CSI"?1049h"
#define TFX_ALT_EXIT     TFX_CSI"?1049l"
#define TFX_SCROLL_RESET TFX_CSI"r"

static inline int tfx_write(const char* s, size_t len) {
  if (!s || len < 1) return -1;

  while (len > 0) {
    ssize_t n = write(STDOUT_FILENO, s, len);
    if (n < 0) {
      if (errno == EINTR) continue;
      return -1;
    }
    if (n == 0) return -1;

    s += n;
    len -= (size_t)n;
  }

  return 0;
}

static inline int tfx_reset(void) {
  return tfx_write(TFX_RESET, sizeof(TFX_RESET) - 1);
}
static inline int tfx_clear(void) {
  return tfx_write(TFX_CLEAR, sizeof(TFX_CLEAR) - 1);
}
static inline int tfx_clear_line(void) {
  return tfx_write(TFX_CLEAR_LINE, sizeof(TFX_CLEAR_LINE) - 1);
}
static inline int tfx_clear_home(void) {
  return tfx_write(TFX_CLEAR TFX_CURSOR_HOME,
                   sizeof(TFX_CLEAR TFX_CURSOR_HOME) - 1);
}
static inline int tfx_cursor_home(void) {
  return tfx_write(TFX_CURSOR_HOME, sizeof(TFX_CURSOR_HOME) - 1);
}
static inline int tfx_cursor_hide(void) {
  return tfx_write(TFX_CURSOR_HIDE, sizeof(TFX_CURSOR_HIDE) - 1);
}
static inline int tfx_cursor_show(void) {
  return tfx_write(TFX_CURSOR_SHOW, sizeof(TFX_CURSOR_SHOW) - 1);
}
static inline int tfx_alt_enter(void) {
  return tfx_write(TFX_ALT_ENTER, sizeof(TFX_ALT_ENTER) - 1);
}
static inline int tfx_alt_exit(void) {
  return tfx_write(TFX_ALT_EXIT, sizeof(TFX_ALT_EXIT) - 1);
}
static inline int tfx_scroll_reset(void) {
  return tfx_write(TFX_SCROLL_RESET, sizeof(TFX_SCROLL_RESET) - 1);
}

static inline int tfx_cursor_move(unsigned short row, unsigned short col) {
  if (row < 1 || col < 1) return -1;

  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[%hu;%huH", row, col);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_cursor_up(unsigned short n) {
  if (n < 1) return -1;

  char buf[16];
  int len = snprintf(buf, sizeof(buf), "\x1b[%huA", n);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_cursor_down(unsigned short n) {
  if (n < 1) return -1;

  char buf[16];
  int len = snprintf(buf, sizeof(buf), "\x1b[%huB", n);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_cursor_forward(unsigned short n) {
  if (n < 1) return -1;

  char buf[16];
  int len = snprintf(buf, sizeof(buf), "\x1b[%huC", n);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_cursor_back(unsigned short n) {
  if (n < 1) return -1;

  char buf[16];
  int len = snprintf(buf, sizeof(buf), "\x1b[%huD", n);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_fg_256(uint8_t color) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[38;5;%um", color);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_bg_256(uint8_t color) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[48;5;%um", color);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_fg_rgb(uint8_t r, uint8_t g, uint8_t b) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[38;2;%u;%u;%um", r, g, b);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_bg_rgb(uint8_t r, uint8_t g, uint8_t b) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[48;2;%u;%u;%um", r, g, b);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

static inline int tfx_get_size(unsigned short* rows, unsigned short* cols) {
  if (!rows || !cols) return -1;

  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0) return -1;
  if (ws.ws_row < 1 || ws.ws_col < 1) return -1;

  *rows = ws.ws_row;
  *cols = ws.ws_col;
  return 0;
}

static inline int tfx_scroll_region(unsigned short top, unsigned short bottom) {
  unsigned short rows, cols;
  if (tfx_get_size(&rows, &cols) == -1) return -1;
  if (top < 1 || top >= bottom || bottom > rows) return -1;

  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[%hu;%hur", top, bottom);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tfx_write(buf, (size_t)len);
}

#ifdef __cplusplus
}
#endif

#endif /* TERMFX_H */