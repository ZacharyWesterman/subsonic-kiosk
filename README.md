# subsonic-kiosk
This project uses an Arduino Giga and Giga Display Shield to create a kiosk for browsing and playing audio from any Subsonic-compatible server.

It's very much a WIP (there's not even a UI yet!) but by the end, any audio should be able to be browsed and played.

[The documentation](https://zacharywesterman.github.io/subsonic-kiosk) is automatically generated with Doxygen. Check it out for a breakdown of the internals.

# Road Map
- File System
  - [x] Allow disconnecting/reconnecting at run-time
  - [x] Simple file system traversal/modification
    - [x] Read files/dirs
    - [x] Write files
    - [x] Create dirs
    - [x] Delete files/dirs (recursively delete dirs)
  - [x] Stream file contents (don't load whole file at once)
- Network
  - [x] Non-blocking poll for connection
  - [x] Simple HTTP(S) requests
    - [x] Handle redirections (http -> https, etc) or at least indicate them, so they can be handled later.
    - [x] Check progress of downloads
  - [x] Stream downloads into file
  - [x] Async file downloads
  - [x] File download queue
    - [x] Iterate over downloads
  - [ ] Make Subsonic requests
    - [ ] Request transcoding of audio if possible
    - [ ] Browse audio based on folder
- Audio
  - [ ] Play audio
    - [x] Play WAV files
    - [ ] Play FLAC files
    - [ ] Play OGG files
    - [ ] Play MP3 files
    - [ ] Play AAC files...? see if needed.
    - (other audio formats may be added as needed)
  - [ ] Seek position in audio (seconds)
- Misc
    - [x] JSON parsing
    - [ ] Mic input? (OPTIONAL)
      - [ ] Keyword recognition? (for quick controls)
      - [ ] Speech to text? (for searching audio)
- UI
    - [ ] LVGL
    - [ ] Run on separate thread from backend?
    - **(Future plans will be decided when non-UI is in a more complete state)**
