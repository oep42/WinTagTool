# WinTagTool

WinTagTool is a command-line utility for reading and modifying metadata in audio files on Windows 10 (64 bit) and Windows 11. It is intended for scripting and automation.

WinTagTool uses TagLib, so the audio formats supported by WinTagTool depend on the version and build of TagLib used to build WinTagTool. The formats tested with WinTagTool include AIFF/AIF, APE, FLAC, M4A, MP3, OGG, OPUS, WMA, and WV.

The design goal is simplicity. Each command performs exactly one operation and is therefore easy to use from scripting environments such as PowerShell, batch files, Python and other scripting languages.

## Features

WinTagTool supports the following operations:

### Metadata tags

- Set a tag value
- Append a value to a multi-value tag
- Read one or more values of a tag
- Remove a tag
- Count values of a tag
- Count all tags
- Count all tag values
- Check whether a tag exists
- List all tag names
- List all tag names with their values

### Embedded pictures

- Import picture
- Append picture
- Export picture
- Remove all pictures
- Count embedded pictures
- Check whether an embedded picture exists
- Read the picture type of an embedded picture
- Read the picture description of an embedded picture
- Read the MIME type of an embedded picture
- Read the size of an embedded picture
- Read the width of an embedded picture
- Read the height of an embedded picture

### Audio properties

- Read audio length
- Read audio bitrate
- Read audio sample rate
- Count audio channels

## Installation

Place the following files in the same directory as `wintagtool.exe`.

```text
libgcc_s_seh-1.dll
libstdc++-6.dll
libwinpthread-1.dll
zlib1.dll
libtag_c-2.dll
libtag-2.dll
```

## Quick examples

Set a tag:

```text
wintagtool.exe "--set-tag=TITLE=My Song" "D:\Music\audio.mp3"
```

Read a tag:

```text
wintagtool.exe "--get-tag=ARTIST" "audio.mp3"
```

Append a tag value to a multi-value tag:

```text
wintagtool.exe "--append-tag=ARTIST=The Smith Band" "audio.mp3"
```

List all tags:

```text
wintagtool.exe "--list-tags" "audio.mp3"
```

Import a picture:

```text
wintagtool.exe "--import-picture-from=C:\Images\Cover.jpg=Front Cover=Album Cover" "audio.mp3"
```

Export the first embedded picture:

```text
wintagtool.exe "--export-picture=0=C:\Temp\Cover" "audio.mp3"
```

Read picture information:

```text
wintagtool.exe "--get-picture-type=0" "audio.mp3"
wintagtool.exe "--get-picture-description=0" "audio.mp3"
wintagtool.exe "--get-picture-mime-type=0" "audio.mp3"
wintagtool.exe "--get-picture-size=0" "audio.mp3"
```

Read audio properties:

```text
wintagtool.exe "--get-audio-length" "audio.mp3"
wintagtool.exe "--get-audio-bitrate" "audio.mp3"
wintagtool.exe "--get-audio-samplerate" "audio.mp3"
wintagtool.exe "--count-audio-channels" "audio.mp3"
```

## Documentation

See [`MANUAL.md`](MANUAL.md) for the complete command reference, exit codes, PictureType values, supported audio file formats, and known limitations.

## License

See the [`LICENSE`](LICENSE) file for license information.

## Credits

WinTagTool is built on top of the excellent TagLib library.

Special thanks to the TagLib developers for providing a high-quality, cross-platform metadata library.

## Release

Contents of [release](../../releases):

```text
WinTagTool-1.x.x-Windows-x64.zip
├── wintagtool.exe
├── libgcc_s_seh-1.dll
├── libstdc++-6.dll
├── libwinpthread-1.dll
├── zlib1.dll
├── libtag_c-2.dll
├── libtag-2.dll
└── MANUAL.html
```
