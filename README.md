# subsonic-kiosk
This project uses an Arduino Giga and Giga Display Shield to create a kiosk for browsing and playing audio from any Subsonic-compatible server.

It's very much a WIP (there's not even a UI yet!) but by the end, any audio should be able to be browsed and played.

[The documentation](https://zacharywesterman.github.io/subsonic-kiosk) is automatically generated with Doxygen. Check it out for a breakdown of the internals.

# Build Requirements

- The "Arduino Community Edition" VS Code extension, if uploading with VS Code.
- The following Arduino libraries:
  - `Arduino_USBHostMbed5` - Managing files on a connected USB device.
  - `ArduinoJson` - Simple, efficient parsing of JSON data (usually from network requests).
  - `Arduino_AdvancedAnalog` - Playing music through the audio jack.
  - `WiFi` - Network requests.

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
    - [ ] Request transcoding of audio to a supported format (fail gracefully if no supported format)
    - [ ] Browse audio based on folder
- Audio
  - [ ] Play audio
    - [x] Play WAV files
    - [ ] Transcode audio (on-the-fly, ideally)
      - [ ] MP3 to WAV using lame
    - Not planned
      - [-] Play FLAC files
      - [-] Play OGG files
      - [-] Play AAC files...? see if needed.
      - (other audio formats may be added as needed) 
  - [ ] Seek position in audio (seconds)
- Misc
    - [x] JSON parsing
    - [ ] Mic input? (OPTIONAL)
      - [ ] Keyword recognition? (for quick controls)
      - [ ] Speech to text? (for searching audio)
- UI
    - [ ] Gigawidgets
    - [ ] Run on separate thread from backend?
    - **(Future plans will be decided when non-UI is in a more complete state)**


## Configs

Required configs on the top-level directory of the bulk storage (flash drive, etc):

### `network.json`
```json
{
  "ssid": "YOUR NETWORK SSID",
  "password": "PLAINTEXT PASSWORD"
}
```

### `subsonic.json`
```json
{
  "host": "CONNECTION URL, E.G. http://www.example.com:1234/subsonic",
  "username": "SUBSONIC_USER",
  "md5sum": "MD5 SALTED AND HASHED PASSWORD",
  "salt": "SALT STRING"
}
```
Here's a small sample Python script that can be used to generate the md5sum and salt.
```py
import random, hashlib

salt = ('%32x' % random.randrange(16**32)).strip()
md5sum = hashlib.md5((password + salt).encode('utf-8')).hexdigest()

print(salt, md5sum)
```


## Emulating

If emulating, this project requires libcurl development files. On Ubuntu, you can install this with
```sh
sudo apt install libcurl4-gnutls-dev
# or
sudo apt install libcurl4-openssl-dev
```

Also, a `./usb/` directory will be used as the USB flash drive storage, and will be created if it doesn't exist.
