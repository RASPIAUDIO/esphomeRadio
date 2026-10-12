# User-selected Radio cues / Sons choisis pour la Radio

## Français

Ces fichiers remplacent les sons synthétiques de sound1/sound2 dans
`radio_full_aec_ble_ui_sounds.yaml` (sound3) :

- `start.wav` : démarrage après connexion Wi-Fi et Assist, 2,730667 s.
- `blup.wav` : début réel de Listening, 0,977583 s.

Ils sont intégrés au firmware : aucun serveur de fichiers ni changement de
configuration Home Assistant n'est requis. Pour compiler, conserver ces deux
WAV dans ce dossier. Ne pas les remplacer directement par un WAV arbitraire :
le lecteur attend un en-tête canonique de 44 octets, PCM signé 16 bits, mono,
48 kHz, et au maximum 3 s au démarrage / 2 s pour Listening.

Sources fournies par l'utilisateur le 11 octobre 2026 : `/home/ros/Desktop/start.wav`
et `/home/ros/Desktop/blup.mp3`, laissées intactes. La licence et l'attribution des
sources n'ont pas été fournies : vérifier les droits de redistribution avant
de publier les sons ou un firmware qui les contient.

La préparation conserve la hauteur et la vitesse. Le MP3 est décodé puis
rééchantillonné ; ses extrémités sous −70 dBFS sont retirées en gardant 20 ms
de marge. Le démarrage garde sa durée entière. Chaque fichier est seulement
atténué si nécessaire pour une crête maximale de −6 dBFS ; de petites rampes
de 1 ms assurent des extrémités nulles. Le volume utilisateur reste indépendant.

## English

These prepared recordings replace the synthesized sound1/sound2 cues. They are
embedded in firmware at build time and require no HA configuration or runtime
download. Keep both WAV files beside the YAML in this `sounds/` directory.
The parser requires canonical 44-byte-header PCM16/mono/48 kHz WAVs, at most
3 seconds for startup and 2 seconds for Listening. Invalid replacements are skipped.

The original user-supplied WAV/MP3 files were not changed. Preparation preserves
pitch/speed, resamples the MP3, trims its near-silent edges with 20 ms padding,
leaves at least 6 dB peak headroom without boosting either recording, and adds
1 ms edge ramps. Startup duration is preserved. Source licensing/attribution was
not supplied; confirm redistribution rights before publishing these assets or
firmware containing them.

## Rebuild assets (optional; not needed to compile firmware)

Requires Python, NumPy, SciPy and SoundFile with MP3-capable libsndfile.

```sh
python3 scripts/prepare_ui_sounds.py /path/to/start.wav /path/to/blup.mp3
```

Source SHA-256:

```text
start.wav  62c7b866d978b1e3f551e5a8bc44b6d30166d9abd07de2b1176426b7171e655d
blup.mp3   20dc18215e0460c22c8cc48e43048f333d1d44331f6e65426876a6650c2b9d84
```

Prepared SHA-256:

```text
start.wav  c825c8c66dd46c464cf90d0118f1c73f2001f8d6ace57fde4b3ba14794dcbef4
blup.wav   e9f07313b3b6902d51c5e6305df258ef1c66527393d200276a34e392bd2ef0c2
```
