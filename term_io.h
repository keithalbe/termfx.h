// Minimal ANSI terminal I/O helpers for POSIX terminals.

#ifndef TERM_IO_H
#define TERM_IO_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIO_ESC          "\x1b"
#define TIO_CSI          "\x1b["
#define TIO_OSC          "\x1b]"
#define TIO_ST           "\x1b\\"

// Styles
#define TIO_RESET        TIO_CSI"0m"
#define TIO_BOLD         TIO_CSI"1m"
#define TIO_DIM          TIO_CSI"2m"
#define TIO_ITALIC       TIO_CSI"3m"
#define TIO_UNDERLINE    TIO_CSI"4m"
#define TIO_BLINK        TIO_CSI"5m"
#define TIO_REVERSE      TIO_CSI"7m"
#define TIO_HIDDEN       TIO_CSI"8m"
#define TIO_STRIKE       TIO_CSI"9m"

#define TIO_NORMAL       TIO_CSI"22m"
#define TIO_NO_ITALIC    TIO_CSI"23m"
#define TIO_NO_UNDERLINE TIO_CSI"24m"
#define TIO_NO_BLINK     TIO_CSI"25m"
#define TIO_NO_REVERSE   TIO_CSI"27m"
#define TIO_NO_HIDDEN    TIO_CSI"28m"
#define TIO_NO_STRIKE    TIO_CSI"29m"

// Foreground colors
#define TIO_FG_BLACK     TIO_CSI"30m"
#define TIO_FG_RED       TIO_CSI"31m"
#define TIO_FG_GREEN     TIO_CSI"32m"
#define TIO_FG_YELLOW    TIO_CSI"33m"
#define TIO_FG_BLUE      TIO_CSI"34m"
#define TIO_FG_MAGENTA   TIO_CSI"35m"
#define TIO_FG_CYAN      TIO_CSI"36m"
#define TIO_FG_WHITE     TIO_CSI"37m"
#define TIO_FG_DEFAULT   TIO_CSI"39m"

// Background colors
#define TIO_BG_BLACK     TIO_CSI"40m"
#define TIO_BG_RED       TIO_CSI"41m"
#define TIO_BG_GREEN     TIO_CSI"42m"
#define TIO_BG_YELLOW    TIO_CSI"43m"
#define TIO_BG_BLUE      TIO_CSI"44m"
#define TIO_BG_MAGENTA   TIO_CSI"45m"
#define TIO_BG_CYAN      TIO_CSI"46m"
#define TIO_BG_WHITE     TIO_CSI"47m"
#define TIO_BG_DEFAULT   TIO_CSI"49m"

// Bright foreground
#define TIO_FG_BBLACK    TIO_CSI"90m"
#define TIO_FG_BRED      TIO_CSI"91m"
#define TIO_FG_BGREEN    TIO_CSI"92m"
#define TIO_FG_BYELLOW   TIO_CSI"93m"
#define TIO_FG_BBLUE     TIO_CSI"94m"
#define TIO_FG_BMAGENTA  TIO_CSI"95m"
#define TIO_FG_BCYAN     TIO_CSI"96m"
#define TIO_FG_BWHITE    TIO_CSI"97m"

// Bright background
#define TIO_BG_BBLACK    TIO_CSI"100m"
#define TIO_BG_BRED      TIO_CSI"101m"
#define TIO_BG_BGREEN    TIO_CSI"102m"
#define TIO_BG_BYELLOW   TIO_CSI"103m"
#define TIO_BG_BBLUE     TIO_CSI"104m"
#define TIO_BG_BMAGENTA  TIO_CSI"105m"
#define TIO_BG_BCYAN     TIO_CSI"106m"
#define TIO_BG_BWHITE    TIO_CSI"107m"

// Screen / cursor
#define TIO_CLEAR        TIO_CSI"2J"
#define TIO_CLEAR_LINE   TIO_CSI"2K"
#define TIO_CURSOR_HOME  TIO_CSI"H"
#define TIO_CURSOR_HIDE  TIO_CSI"?25l"
#define TIO_CURSOR_SHOW  TIO_CSI"?25h"
#define TIO_ALT_ENTER    TIO_CSI"?1049h"
#define TIO_ALT_EXIT     TIO_CSI"?1049l"
#define TIO_SCROLL_RESET TIO_CSI"r"

static inline int tio_write(const char* s, size_t len) {
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

static inline int tio_reset(void) {
  return tio_write(TIO_RESET, sizeof(TIO_RESET) - 1);
}
static inline int tio_clear(void) {
  return tio_write(TIO_CLEAR, sizeof(TIO_CLEAR) - 1);
}
static inline int tio_clear_line(void) {
  return tio_write(TIO_CLEAR_LINE, sizeof(TIO_CLEAR_LINE) - 1);
}
static inline int tio_clear_home(void) {
  return tio_write(TIO_CLEAR TIO_CURSOR_HOME,
                   sizeof(TIO_CLEAR TIO_CURSOR_HOME) - 1);
}
static inline int tio_cursor_home(void) {
  return tio_write(TIO_CURSOR_HOME, sizeof(TIO_CURSOR_HOME) - 1);
}
static inline int tio_cursor_hide(void) {
  return tio_write(TIO_CURSOR_HIDE, sizeof(TIO_CURSOR_HIDE) - 1);
}
static inline int tio_cursor_show(void) {
  return tio_write(TIO_CURSOR_SHOW, sizeof(TIO_CURSOR_SHOW) - 1);
}
static inline int tio_alt_enter(void) {
  return tio_write(TIO_ALT_ENTER, sizeof(TIO_ALT_ENTER) - 1);
}
static inline int tio_alt_exit(void) {
  return tio_write(TIO_ALT_EXIT, sizeof(TIO_ALT_EXIT) - 1);
}
static inline int tio_scroll_reset(void) {
  return tio_write(TIO_SCROLL_RESET, sizeof(TIO_SCROLL_RESET) - 1);
}

static inline int tio_cursor_move(unsigned short row, unsigned short col) {
  if (row < 1 || col < 1) return -1;

  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[%hu;%huH", row, col);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_cursor_up(unsigned short n) {
  if (n < 1) return -1;

  char buf[16];
  int len = snprintf(buf, sizeof(buf), "\x1b[%huA", n);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_cursor_down(unsigned short n) {
  if (n < 1) return -1;

  char buf[16];
  int len = snprintf(buf, sizeof(buf), "\x1b[%huB", n);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_cursor_forward(unsigned short n) {
  if (n < 1) return -1;

  char buf[16];
  int len = snprintf(buf, sizeof(buf), "\x1b[%huC", n);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_cursor_back(unsigned short n) {
  if (n < 1) return -1;

  char buf[16];
  int len = snprintf(buf, sizeof(buf), "\x1b[%huD", n);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_fg_256(uint8_t color) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[38;5;%um", color);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_bg_256(uint8_t color) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[48;5;%um", color);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_fg_rgb(uint8_t r, uint8_t g, uint8_t b) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[38;2;%u;%u;%um", r, g, b);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_bg_rgb(uint8_t r, uint8_t g, uint8_t b) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[48;2;%u;%u;%um", r, g, b);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

static inline int tio_get_size(unsigned short* rows, unsigned short* cols) {
  if (!rows || !cols) return -1;

  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0) return -1;
  if (ws.ws_row < 1 || ws.ws_col < 1) return -1;

  *rows = ws.ws_row;
  *cols = ws.ws_col;
  return 0;
}

static inline int tio_scroll_region(unsigned short top, unsigned short bottom) {
  unsigned short rows, cols;
  if (tio_get_size(&rows, &cols) == -1) return -1;
  if (top < 1 || top >= bottom || bottom > rows) return -1;

  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\x1b[%hu;%hur", top, bottom);
  if (len < 1 || (size_t)len > sizeof(buf) - 1) return -1;

  return tio_write(buf, (size_t)len);
}

#ifdef __cplusplus
}
#endif

#endif /* TERM_IO_H */