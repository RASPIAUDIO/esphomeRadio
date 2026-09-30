# RASPIAUDIO Radio — Home Assistant + Music Assistant

Unified ESPHome firmware for the ESP32-S3 RASPIAUDIO Radio, version **2026.9.5**.
The source is [radio.yaml](radio.yaml); keep [images/](images/) beside it when
building.





### Features

One device acts as a Home Assistant Assist voice satellite and a Music
Assistant Sendspin player. Select music in MA; the Radio changes its screen and
controls automatically, with no HA/MA mode switch. The MA screen shows station
or track information, volume, an estimated battery percentage and a USB-power
indicator. Assist has listening and response screens.

When music is stopped, the on-device wake words **“Okay Nabu”**, **“Hey Jarvis”**
and **“Hey Mycroft”** can start Assist. During MA playback, use the physical
**Assist** button or its IR equivalent: this firmware cannot run wake-word
detection and music at the same time. Assist pauses the *whole current MA
Sendspin group*, listens and speaks, then resumes the group. Other grouped
players are therefore quiet while you ask a question.

The four station buttons recall internet radio stations without creating an
HA automation. During music, the volume control and Mute button act on the
MA group; otherwise they control local voice-response volume and mute.
Supported IR commands also provide Assist, Mute and volume up/down.

### Installation and first setup

1. Use a RASPIAUDIO Radio with ESP32-S3, 8 MB flash and 8 MB PSRAM. For a
   source build, install **ESPHome 2026.9.0 or newer** and keep `radio.yaml`
   and `images/` together. Compilation downloads the pinned ES8388 component
   and other referenced assets, so it needs internet access.
2. Set up Home Assistant with a working Assist pipeline (speech recognition
   and speech output) and a running Music Assistant server. Add the [MA
   integration to HA](https://www.music-assistant.io/integration/installation/)
   so that MA players are visible to HA. Keep the Radio and MA on the same
   local network for Sendspin discovery.
3. If the release includes a **full 8 MB flash image** captured before Wi-Fi
   provisioning, flash it over USB with
   [esptool](https://docs.espressif.com/projects/esptool/en/latest/esp32s3/esptool/basic-commands.html)
   on an ESP32-S3 Radio:

   ```bash
   python -m esptool --chip esp32s3 --port /dev/ttyACM0 write-flash 0x0 radio.bin
   ```

   Replace the port and filename with yours. The file must be exactly
   8,388,608 bytes. A full-flash image starts at `0x0` and overwrites the
   entire flash, **including saved Wi-Fi settings and station presets**.
   It is not an ESPHome OTA update file; do not use it for an OTA update or
   to preserve an existing installation. The published image must be made
   before storing any personal credentials or other private data.
4. Alternatively, build from source and first flash over USB. From this
   directory, for example:

   ```bash
   esphome config radio.yaml
   esphome run radio.yaml --device /dev/ttyACM0
   ```

   Replace `/dev/ttyACM0` with your Radio's serial port. Later source-built
   updates can use [ESPHome OTA](https://esphome.io/components/ota/esphome/).
   A **separate OTA `.bin`** may be published with a future version for
   compatible installed Radios; it is not the 8 MB full-flash image. No
   precompiled OTA file is included yet, and no precompiled binary is required
   if you compile `radio.yaml`.
5. The YAML and an unconfigured full-flash image do **not** contain your home
   Wi-Fi credentials. Set them over USB with
   [Improv Serial](https://esphome.io/components/improv_serial/), or
   join the fallback AP `Raspiaudio-radio-full` (password `12345678`) and
   open `http://192.168.4.1/`. The AP is only for provisioning; consider
   changing its password in your own build. See the [ESPHome captive portal
   guide](https://esphome.io/components/captive_portal/).
6. Add the discovered ESPHome device to HA and select an Assist pipeline for
   it. In its ESPHome integration options, enable **Allow the device to perform
   Home Assistant actions**. This permission is essential for the station
   presets. [ESPHome instructions](https://esphome.io/components/api/#homeassistant-action-action).
7. In MA, find the Radio's Sendspin player, named **Radio Music Assistant**,
   and play music or a station on it. Sendspin is built into MA and normally
   discovers players automatically; there is no extra player provider to
   install. [MA Sendspin guide](https://www.music-assistant.io/player-support/sendspin/).

### Controls

| Control | During MA playback | When music is stopped |
| --- | --- | --- |
| Assist button | Pauses the MA group, starts Assist, then resumes the group after the answer | Starts Assist |
| Wake word | Inactive; use Assist | Starts Assist unless the microphone is muted |
| Turn volume control | Changes current MA group volume | Changes local voice-response volume |
| Mute button | Mutes/unmutes the MA group | Mutes/unmutes local voice output |
| Short press a station button | Plays its stored station | Plays its stored station |
| Hold a station button about 2 seconds, then release | Stores the station currently playing in MA | Requires a station currently playing in MA |

For radio streams, the display prioritizes the station name, then available
track/artist details. Its footer shows MA volume or `MUTED`, the estimated
battery percentage, and `USB` when external power is detected. `USB` does not
confirm that the battery is charging; the percentage is estimated from voltage.

### Four station presets

1. Play the desired internet station in MA on this Radio or its current group.
2. Hold the chosen station button for about two seconds, then **release** it.
   `MEMO 1 OK` through `MEMO 4 OK` confirms a successful save.
3. Wait at least five seconds before removing power so the value is written
   to flash. A short press of the same button recalls the station.

The four stored MA media URIs are also editable in the Radio's HA device
configuration as **Station button 1–4**. A URI such as `library://radio/109`
is more reliable than guessing the displayed station name. On the first save,
the firmware seeks an MA player with “radio” in its name; if several players
have similar names, verify that the preset addresses this Radio. `NO RADIO`
means no usable station could be found or saved; `PLAY FAILED` means the HA/MA
play action failed. Presets need HA, its MA integration and the action
permission above, but no HA automation.

HA also exposes **Microphone Mute** and **Wake word sensitivity** (Slightly,
Moderately or Very sensitive), plus battery, USB-power and Wi-Fi diagnostics.
Microphone Mute disables wake-word detection and voice capture.

### Limits and troubleshooting

- Wake words are intentionally unavailable during MA playback; the separate
  AEC experiment is **not** included in this release. Use Assist instead.
- If HA is offline, Assist displays `HA OFFLINE`. MA playback may continue,
  but voice requests and preset actions require HA.
- If MA does not find the Radio, check Wi-Fi and local-network discovery. MA's
  [Sendspin settings](https://www.music-assistant.io/player-support/sendspin/)
  allow manual discovery by IP address.
- The documented test setup used ESPHome 2026.9.0, HA 2026.9.3 and MA 2.10.4.
  Other versions may work but are not specifically validated here.


