# TUNED-GUI
a simple raylib app to change tuned profile.

## BUILD
```sh
make build
```

## RUN
```sh
make run
# or
./tuned-gui
```

## CONTROL

| Keyboard | do   |
|----------|------|
| j        | down |
| k        | up   |
| q        | quit |

## MODIFY

all the option can be changed from the `#define` keyword in `src/main.c`.

```c
#define BACKGROUND_COLOR GetColor(0x1e1e2eff)
#define TITLE "Tuned GUI"
#define TITLE_SIZE 28
#define PADDING 25
#define ELEMENT_SIZE 100
#define SELECTED_COLOR GetColor(0x89b4faff)
#define HOVER_COLOR GetColor(0x89dcebff)
#define BUFFER_SIZE 4096
#define ONE_TIME_USE true
#define PROFILE_COUNT 3
#define WINDOW_WIDTH 400
#define WINDOW_HEIGTH 400

#undef WHITE
#define WHITE GetColor(0xcdd6f4ff)

static char *sort[PROFILE_COUNT] = {
    "desktop",
    "balanced",
    "powersave",
};
```

then just recompile the program.
