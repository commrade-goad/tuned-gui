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

Tuned profile can be listed using : `tuned-adm list`

Example : 
```txt
Available profiles:
- accelerator-performance     - Throughput performance based tuning with disabled higher latency STOP states
- aws                         - Optimize for aws ec2 instances
- balanced                    - General non-specialized tuned profile
- balanced-battery            - Balanced profile biased towards power savings changes for battery
- desktop                     - Optimize for the desktop use-case
- hpc-compute                 - Optimize for HPC compute workloads
- intel-sst                   - Configure for Intel Speed Select Base Frequency
- latency-performance         - Optimize for deterministic performance at the cost of increased power consumption
- network-latency             - Optimize for deterministic performance at the cost of increased power consumption, focused on low latency network performance
- network-throughput          - Optimize for streaming network throughput, generally only necessary on older CPUs or 40G+ networks
- optimize-serial-console     - Optimize for serial console use.
- powersave                   - Optimize for low power consumption
- throughput-performance      - Broadly applicable tuning that provides excellent performance across a variety of common server workloads
- virtual-guest               - Optimize for running inside a virtual guest
- virtual-host                - Optimize for running KVM guests`
```

all the option can be changed from this part in `src/main.c`.

```c
#define BACKGROUND_COLOR GetColor(0x1e1e2eff)
#define TITLE "Tuned GUI"
#define TITLE_SIZE 28
#define PADDING 25
#define ELEMENT_SIZE 100
#define ELEMENT_FONT_SIZE 24
#define ELEMENT_THICKNESS 3.0
#define ELEMENT_SPACING 10
#define SELECTED_COLOR GetColor(0x89b4faff)
#define HOVER_COLOR GetColor(0x89dcebff)
#define BUFFER_SIZE 4096
#define ONE_TIME_USE true
#define PROFILE_COUNT 3
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

#undef WHITE
#define WHITE GetColor(0xcdd6f4ff)

static char *sort[PROFILE_COUNT] = {
    "desktop",
    "balanced",
    "powersave",
};
```

then just recompile the program.
