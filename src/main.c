#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <ctype.h>

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

int handle_select(size_t current, int mode) {
    if (current == 0 && mode < 0 ) return PROFILE_COUNT - 1;
    if (current >= PROFILE_COUNT - 1 && mode > 0) return 0;
    return current += mode;
}

bool exec_process(char *buffer, const char *command, bool ignore_err) {
    FILE *fp;
    fp = popen(command, "r");

    if (fp == NULL) {
        if (!ignore_err) {
            fprintf(stderr, "ERROR: Failed to open the program!\n");
        }
        return false;
    }
    char temp[128];
    while (fgets(temp, sizeof(temp), fp) != NULL) {
        strncat(buffer, temp, BUFFER_SIZE - strlen(buffer) - 1);
    }

    if (pclose(fp) == -1) {
        if (!ignore_err) {
            fprintf(stderr, "ERROR: Failed to close the program!\n");
        }
        return false;
    }
    if (strncmp(buffer, "", BUFFER_SIZE) == 0) {
        if (!ignore_err) {
            fprintf(stderr, "ERROR: Command not found!\n");
        }
        return false;
    }
    return true;
}

void get_substring_after(const char *str, char ch, char *result) {
    char *pos = strchr(str, ch);

    if (pos != NULL) {
        size_t index = pos - str;
        strcpy(result, &str[index + 1]);
    } else {
        result[0] = '\0';
    }
}

void trim_whitespace(const char *src, char *result) {
    while (*src != '\0') {
        if (!isspace((unsigned char)*src)) {
            *result = *src;
            result++;
        }
        src++;
    }
    *result = '\0';
}

void cleanup_output(const char *input_str, char *res) {
    char buff_sec[BUFFER_SIZE];
    get_substring_after(input_str, ':', buff_sec);
    trim_whitespace(buff_sec, res);
} 

int main() {
    char buffer[BUFFER_SIZE];
    const char *command = "tuned-adm active";
    if (!exec_process(buffer, command, false)) return 120;
    char current_mode[BUFFER_SIZE];
    cleanup_output(buffer, current_mode);
    size_t selected_kb = 0;
    int selected_m = -1;

    for (size_t i = 0; i < PROFILE_COUNT; i++) {
        if (strncmp(current_mode, sort[i], strlen(sort[i])) == 0) {
            selected_kb = i;
            break;
        }
    }

    const int w = WINDOW_WIDTH;
    const int h = WINDOW_HEIGTH;
    Vector2 mouse_pos;
    InitWindow(w, h, TITLE);
    SetTargetFPS(60);
    SetExitKey(KEY_Q);

    while (!WindowShouldClose()) {
        mouse_pos = GetMousePosition();
        selected_m = -1;

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        DrawText(TITLE, (GetScreenWidth()/2.0) - (MeasureText(TITLE, TITLE_SIZE) / 2.0), PADDING/2.0, TITLE_SIZE, WHITE);
        DrawLineV((Vector2) {0 + PADDING, TITLE_SIZE + PADDING}, (Vector2) {(GetScreenWidth() - PADDING), TITLE_SIZE + PADDING}, WHITE);

        for (size_t i = 0; i < PROFILE_COUNT; i++) {
            Rectangle new_rec = (Rectangle) {
                .x = PADDING,
                .y = (float) TITLE_SIZE + (PADDING * 2) + (i * ELEMENT_SIZE),
                .width = (float) GetScreenWidth() - (PADDING * 2),
                .height = ELEMENT_SIZE - 10
            };
            float thickness = 3.0f;
            Color c = WHITE;
            Color txtc = WHITE;

            if (CheckCollisionPointRec(mouse_pos, new_rec)) {
                selected_m = i;
                c = HOVER_COLOR;
                txtc = HOVER_COLOR;
            }

            if (selected_kb == i) {
                DrawRectangleRec(new_rec, SELECTED_COLOR);
                txtc = BACKGROUND_COLOR;
            }
            DrawRectangleLinesEx(new_rec, thickness, c);
            DrawText(sort[i], PADDING * 2, new_rec.y + (new_rec.height/2.0) - (24 / 2.0), 24, txtc);
        }

        EndDrawing();

        if (IsKeyPressed(KEY_J)) {
            selected_kb = handle_select(selected_kb, 1);
        }

        if (IsKeyPressed(KEY_K)) {
            selected_kb = handle_select(selected_kb, -1);
        }

        if (IsKeyPressed(KEY_ENTER)) {
            char buffer[BUFFER_SIZE];
            char command[BUFFER_SIZE] = "tuned-adm profile ";
            strncat(command, sort[selected_kb], strlen(sort[selected_kb]));
            printf("output : %s\n", command);
            exec_process(buffer, command, true);
            if (strncmp(buffer, "", 1) != 0) {
                fprintf(stderr, "ERROR: Failed to change profile!\n");
                return 1;
            }
            if (ONE_TIME_USE == true) {
                CloseWindow();
                return 0;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && selected_m >= 0) {
            char buffer[BUFFER_SIZE];
            char command[BUFFER_SIZE] = "tuned-adm profile ";
            strncat(command, sort[selected_m], strlen(sort[selected_m]));
            printf("output : %s\n", command);
            exec_process(buffer, command, true);
            if (strncmp(buffer, "", 1) != 0) {
                fprintf(stderr, "ERROR: Failed to change profile!\n");
                return 1;
            }
            selected_kb = selected_m;
            if (ONE_TIME_USE == true) {
                CloseWindow();
                return 0;
            }
        }
    }
    CloseWindow();
    return 0;
}
