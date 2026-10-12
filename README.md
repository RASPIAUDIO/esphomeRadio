# Raspiaudio Radio Full — experimental unified firmware

`radio.yaml` is a standalone ESPHome configuration for one Radio running
Home Assistant Voice Assistant and Music Assistant Sendspin. It does not include
either original YAML at build time. Local display images are in `images/`.

The firmware detects active Music Assistant playback automatically; no HA mode
switch is needed. The physical Assist button and the matching IR command start
a temporary voice interaction. During MA playback, Assist pauses the entire
Sendspin group, waits for the Radio's I²S speaker to release the shared bus,
uses the microphone, and resumes the group after the spoken response. The
physical Mute button and IR mute command also toggle the whole MA group's
mute state while MA is playing; outside MA playback they retain the local
voice-player behavior. The Radio displays Sendspin metadata while playing.
The volume knob and IR volume commands adjust the Sendspin group during music
and the local voice-response volume during Assist.

The MA screen follows RadioMusic's dark-blue panels and gold borders. Radio
station names use a 28-pixel font (22 pixels for longer names), with the song
title and artist underneath. The bottom panel permanently shows group volume,
a volume bar, the battery level and its percentage; `USB` indicates external
power. Battery color changes below 50% and 20%. Missing metadata is cleared,
and long text is shortened on whole UTF-8 characters. Updates remain driven by
metadata, volume and diagnostic events rather than a new scrolling timer.

The wake word is available when music is stopped. It is disabled during MA
playback: the microphone and speaker share an I²S bus and cannot run together
with this ESPHome configuration. The physical Assist button is therefore
required during music. A brief Sendspin source interruption during a station
change does not switch to the wake-word screen; the Radio waits for both MA
and the I²S speaker to stop. The Assist button accepts a natural 50 ms to 2 s
press.

This is a development build, **partially validated on hardware**. User tests
passed for idle wake word, idle physical Assist, Assist during solo MA playback,
Assist during grouped Radio + Luxe playback, and group mute/unmute. The four
station-preset buttons received a positive RadioFull user test on 28 September.
On the `rc4` build, the user confirmed that changing stations no longer flashes
the wake-word screen or causes an audible dropout, and that Assist responds
more naturally. The user also accepted the MA screen and the repeated grouped
Radio + Luxe Assist scenario. The current `rc5` YAML is loaded on the Radio;
it closes a Volume/Mute routing edge case during station changes. The user
reported satisfactory normal-use testing on 29 September. One private radio
station replayed its connection advertisement after Assist during an earlier
test, but this did not recur during a monitored repeat or a direct MA
Pause → Play test. The upstream reconnection condition remains unknown.
The final Volume/Mute behavior and the detailed checks in `VALIDATION.md` have
not all been individually confirmed.
A short press plays the saved station; hold for about two seconds **then release**
to save the station currently playing in Music Assistant. The four
stored URIs are also editable in the HA device configuration, and a short
screen message confirms a save or reports a missing station.
In the ESPHome integration options in HA, enable **Allow the device to perform
Home Assistant actions** for these station buttons. No HA automation is needed.
Allow at least five seconds after a save before disconnecting power so that
ESPHome can flush the preferences to flash.


## Complete standalone AEC version

`radio_full_aec.yaml` is the complete standalone AEC firmware. The current
`2026.9.5-aec4` candidate extends the AEC3 firmware accepted on 5 October 2026
with a selectable microphone input. On 8 October the user confirmed that both
microphones are received; extended acoustic comparisons remain useful.
It requires **no packages,
other local YAML files or sibling Radio repository**. It includes the full
screens, battery/volume display, four station memories, rotary/IR controls,
group mute, Assist pause/resume, separate voice/music volume management and
the three persisted wake-word sensitivity choices. Unlike `radio_full.yaml`,
its duplex/AEC backend permits wake-word detection during music playback.

Keep the supplied `images/` directory beside the YAML. First compilation needs
network access for fonts, wake-word models, the announcement sound and the
audio backend, pinned to commit `31b576975cae32379df8a025826e03b58bdd1069`.
The unused legacy ES8388 component dependency has been removed; Radio codec
routing remains in the configuration.

The commissioned device name `raspiaudio-radio-aec-test` and existing entity
names are intentionally unchanged. Edit the two
`substitutions` entries only if commissioning a differently named device.
Wi-Fi provisioning remains available through Improv Serial and the fallback
AP `Raspiaudio-radio-full` (password `12345678`). ESPHome 2026.9.0 is the build
version used for verification:

```sh
esphome compile radio_full_aec.yaml
```

Build output is isolated in `.esphome/build/raspiaudio-radio-aec-standalone4/`;
`build/firmware.factory.bin` and `build/firmware.ota.bin` are generated there.
Creating this standalone file does not itself update the connected Radio.
Local regression tests compare its validated configuration with AEC3, allowing
only the declared version/build, unused dependency and microphone changes.
On 8 October, the 36 AEC regressions passed. These check configuration and
selected C++ logic on the host, not acoustic performance on the Radio.
The AEC4 build before the Left/Right label rename also compiled successfully
with ESPHome 2026.9.0 / ESP-IDF 5.5.5:
the OTA binary is 3,729,680 bytes, leaving 202,480 bytes in the existing slot.
No device upload is implied by these build results.

### Microphone input (AEC4 candidate)

The new HA configuration selector **Microphone input** is saved across restarts:

- **Both** (default): mix the two ES8388 input paths into the right ADC.
- **Left** (formerly Input 1): capture the left microphone alone.
- **Right** (formerly Input 2): capture the right microphone alone (previous routing).

The AEC processor still receives one mono signal; this is an analog mix, not
two-microphone beamforming. The user confirmed the physical left/right mapping
on 8 October. The option order and stored selection are preserved by the rename.
Compare Left, Right and Both with the same wake word, distance, music volume and sensitivity. Check
speech recognition, false triggers and music/voice volume restoration too.
Codec register readback is logged as `Microphone input: ...`; this confirms the
register setting only, not that each physical microphone supplies a good signal.

Improv BLE is **not included in this AEC-only candidate**. A compile-only feasibility
build with all existing audio codecs exceeded the current OTA partition by
about 138 KiB. Wi-Fi provisioning through Improv Serial and the fallback AP
remains available. No partition layout or saved Wi-Fi settings have been changed.

A separate complete `radio_full_aec_ble.yaml` candidate now implements first-setup
Improv BLE with larger dual-OTA partitions. It requires the accompanying CSV and
a planned initial USB migration, not a routine OTA upload onto the old layout.
It contains the complete application configuration, without another YAML or HA
configuration package. Keep the CSV and `images/` for compilation. Starting with
`2026.9.5-aec4-ble2`, initial Wi-Fi setup from Home Assistant needs no button press.
Starting with `ble3`, the idle screen waits for the Assist connection before
displaying WW; MA playback remains independent of that connection.
See [IMPROV_BLE.md](IMPROV_BLE.md) for the English/French procedure and tests.

The separate `radio_full_aec_ble_ui.yaml` trial adds the light, minimal display
design without changing the accepted BLE YAML: pastel connection/Assist/music
screens, explicit MUSIC/VOICE volumes and 16-pixel multi-line answers. The logo
and sounds are preserved. It uses the same BLE partitions, not a new migration.
UI2 enlarges “I'm listening” to 30 pixels and adds sparse Assist event diagnostics
for an occasional early return to music; timing and audio behavior are unchanged.
See [UI_LIGHT_TEST.md](UI_LIGHT_TEST.md) for the scope, limits and test checklist.

`radio_full_aec_ble_ui_sounds.yaml` is a separate standalone UI2-based sound trial:
a 2.73-second user-selected startup sound after Wi-Fi/HA voice connection and
a user-selected “blup” listening cue (0.978 s),
and persistent Startup sound / Listening sound switches plus a separate Sound
volume setting. The validated UI2 file and its build remain unchanged. It uses
the same BLE partitions. Keep the two prepared WAVs in `sounds/` beside the YAML;
they are embedded at compile time. See [SOUND_CUES_TEST.md](SOUND_CUES_TEST.md) for behavior,
installation, rollback and the physical listening checklist in French/English.

The original package-based `radio_full_aec_integrated_experiment.yaml` and
minimal `radio_full_aec_experiment.yaml` are preserved as development references.
See `SENSITIVITY_TESTS.md` for the dated measurements and user acceptance,
and `AEC_EXPERIMENT.md` for the earlier prototype history.

The automatic HTTP firmware update entity has deliberately been removed: it
pointed to the HA-only firmware and would overwrite this combined application.

**Fixed voice-command development is paused as of 8 October 2026.** The current
AEC-only work does not require its HA package or custom sentence files.

The separate `radio_full_aec_voice_commands.yaml` overlay adds fixed French and
English music commands to the complete AEC candidate, without modifying either
base YAML. It requires the matching Home Assistant package and sentence files.
See `VOICE_COMMANDS.md` for installation, command meanings and test instructions.
