# subsonic-kiosk
This project uses an Arduino Giga and Giga Display Shield to create a kiosk for browsing and playing audio from any Subsonic-compatible server.

It's very much a WIP (there's not even a UI yet!) but by the end, any audio should be able to be browsed and played.

# Road Map
- File System
  - [x] Allow disconnecting/reconnecting at run-time
  - [x] Simple file system traversal/modification
    - [x] Read files/dirs
    - [x] Write files
    - [x] Create dirs
    - [ ] Delete files/dirs (recursively delete dirs)

  - [x] Stream file contents (don't load whole file at once)
- Network
  - [x] Non-blocking poll for connection
  - [x] Simple HTTP(S) requests
  - [x] Stream downloads into file
  - [ ] Async file downloads
  - [ ] File download queue
  - [ ] Make Subsonic requests
    - [ ] Request transcoding of audio if possible
    - [ ] Browse audio based on folder
- Audio
    - [x] Play WAV files
    - [ ] Play FLAC files
    - [ ] Play OGG files
    - [ ] Play MP3 files
    - (other audio formats may be added as needed)
- Misc
    - [x] JSON parsing
    - [ ] Mic input? (OPTIONAL)
      - [ ] Keyword recognition? (for quick controls)
      - [ ] Speech to text? (for searching audio)
- UI
    - [ ] LVGL
    - **(Future plans will be decided when non-UI is in a more complete state)**
