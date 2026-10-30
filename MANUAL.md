# WinTagTool Manual

## Introduction

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

Place the following DLL files in the same directory as `wintagtool.exe`.

```text
libgcc_s_seh-1.dll
libstdc++-6.dll
libwinpthread-1.dll
zlib1.dll
libtag_c-2.dll
libtag-2.dll
```

# Command Reference

## METADATA TAGS

### Set a tag

```text
--set-tag=<tag name>=<tag value>
```

Example:

```text
wintagtool.exe "--set-tag=TITLE=My Song" "D:\Music\audio.mp3"
```

Replaces the value of the specified tag, or adds this tag with this value.

### Append a tag value to a multi-value tag

```text
--append-tag=<tag name>=<tag value>
```

Example:

```text
wintagtool.exe "--append-tag=ARTIST=The Smith Band" "audio.mp3"
```

Appends a new value to a multi-value tag.

This is mainly useful for formats supporting multi-value tags.

### Get tag value(s)

```text
--get-tag=<tag name>
```

Example:

```text
wintagtool.exe "--get-tag=ARTIST" "audio.mp3"
```

For single-value formats: outputs the value associated with the specified tag.

For multi-value formats: outputs all values associated with the specified tag, one value per line.

### Remove a tag

```text
--remove-tag=<tag name>
```

Example:

```text
wintagtool.exe "--remove-tag=COMMENT" "audio.mp3"
```

Removes the specified tag.

### Count values of a tag

```text
--count-values-of-tag=<tag name>
```

Example:

```text
wintagtool.exe "--count-values-of-tag=ARTIST" "audio.mp3"
```

Outputs the number of values associated with the specified tag.

This is mainly useful for formats supporting multi-value tags.

If the specified tag does not exist, the output of this command is `0`.

This command outputs a number if the operation is supported for the specified audio file.

This command does not support WMA files and returns exit code 20 for that format.

For example, if the file contains:

```text
TITLE=My Song
ARTIST=John Smith
ARTIST=The Smith Band
GENRE=Jazz
```

the command `--count-values-of-tag=ARTIST` outputs:

```text
2
```

because ARTIST has two values.

**Note:** Use `--has-tag` to check whether a tag exists.

### Count all tags

```text
--count-all-tags
```

Example:

```text
wintagtool.exe "--count-all-tags" "audio.mp3"
```

Outputs the total number of distinct tag names.

This command does not support WMA files and returns exit code 20 for that format.

The number of tag values is not counted. For example, if the file contains:

```text
TITLE=My Song
ARTIST=John Smith
ARTIST=The Smith Band
GENRE=Jazz
```

the command outputs:

```text
3
```

because the file contains three distinct tag names: TITLE, ARTIST, and GENRE.

### Count all tag values

```text
--count-all-tag-values
```

Example:

```text
wintagtool.exe "--count-all-tag-values" "audio.mp3"
```

Outputs the total number of tag values.

This command does not support WMA files and returns exit code 20 for that format.

For example, if the file contains:

```text
TITLE=My Song
ARTIST=John Smith
ARTIST=The Smith Band
GENRE=Jazz
```

the command outputs:

```text
4
```

because there are four tag values in total.

Multi-value tags are counted once for each value.

### Has tag

```text
--has-tag=<tag name>
```

Example:

```text
wintagtool.exe "--has-tag=ALBUM" "audio.mp3"
```

Output:

```text
true
```

or:

```text
false
```

Checks whether a tag exists. Outputs `true` if a tag with the specified tag name exists, or `false` otherwise.

An invalid tag name does not produce an error; it produces `false`.

### List all tag names

```text
--list-tags
```

Example:

```text
wintagtool.exe "--list-tags" "audio.mp3"
```

Outputs the name of all tags. Outputs one tag name per line.

Example output:

```text
ARTIST
TITLE
GENRE
```

### List all tags and values

```text
--list-tags-and-values
```

Example:

```text
wintagtool.exe "--list-tags-and-values" "audio.mp3"
```

Outputs each tag name together with its value, one tag value per line.

Example output (with a multi-value tag):

```text
ARTIST=John Smith
ARTIST=The Smith Band
TITLE=My Song
GENRE=Jazz
```

## EMBEDDED PICTURES

### Import picture

```text
--import-picture-from=<image file>
```

or:

```text
--import-picture-from=<image file>=<picture type>
```

or:

```text
--import-picture-from=<image file>=<picture type>=<picture description>
```

Examples:

```text
wintagtool.exe "--import-picture-from=C:\Images\Cover.jpg" "audio.mp3"
wintagtool.exe "--import-picture-from=C:\Images\Cover.jpg=Front Cover" "audio.mp3"
wintagtool.exe "--import-picture-from=C:\Images\Cover.jpg=Front Cover=Album Cover" "audio.mp3"
```

Removes any existing embedded pictures and imports the specified image.

If `<picture type>` is not specified, the default value `Front Cover` is used.

If `<picture description>` is not specified, an empty description is used.

**Note:** `<picture type>` must be specified using one of the standardized TagLib PictureType strings. See [PictureType values](#picturetype-values) below.

For `<picture description>`, any text string can be used.

This command has limited support for APE, M4A, WMA, and WV files. (See support matrix below for details about this limited support.)

Regarding `<image file>`: This command does not support image paths or filenames containing the `=` character. Such paths/filenames return exit code 7.

### Append picture

```text
--append-picture-from=<image file>
```

or:

```text
--append-picture-from=<image file>=<picture type>
```

or:

```text
--append-picture-from=<image file>=<picture type>=<picture description>
```

Examples:

```text
wintagtool.exe "--append-picture-from=C:\Images\BackCover.png" "audio.mp3"
wintagtool.exe "--append-picture-from=C:\Images\BackCover.png=Back Cover" "audio.mp3"
wintagtool.exe "--append-picture-from=C:\Images\BackCover.png=Back Cover=Band Photo" "audio.mp3"
```

Appends the specified image after any existing embedded pictures.

If `<picture type>` is not specified, the default value `Other` is used.

If `<picture description>` is not specified, an empty description is used.

**Note:** `<picture type>` must be specified using one of the standardized TagLib PictureType strings. See [PictureType values](#picturetype-values) below.

For `<picture description>`, any text string can be used.

This command does not support APE or WV files. It has limited support for M4A and WMA files. (See support matrix below for details about this limited support.)

Regarding `<image file>`: This command does not support image paths or filenames containing the `=` character. Such paths/filenames return exit code 15.

### Export a picture

```text
--export-picture=<picture index>=<picture base filename>
```

Example:

```text
wintagtool.exe "--export-picture=0=C:\Temp\Cover" "audio.mp3"
```

Exports an embedded picture (specified with a zero-based index) to the specified path and base filename. With this, the correct extension is added to the filename.

In addition, the output filename is written to the standard output.

**Note:** The `<picture base filename>` is usually a path with a filename without an extension.

This command does not support APE or WV files and returns exit code 20 for those formats.

### Remove all pictures

```text
--remove-pictures
```

Example:

```text
wintagtool.exe "--remove-pictures" "audio.mp3"
```

Removes every embedded picture stored in the audio file.

This command has limited support for APE and WV files. (See support matrix below for details about this limited support.)

### Count pictures

```text
--count-pictures
```

Example:

```text
wintagtool.exe "--count-pictures" "audio.mp3"
```

Outputs the number of embedded pictures.

This command does not support APE or WV files and returns exit code 20 for those formats.

### Has picture

```text
--has-picture=<picture index>
```

Example:

```text
wintagtool.exe "--has-picture=0" "audio.mp3"
```

Output:

```text
true
```

or:

```text
false
```

Checks whether a picture exists. Outputs `true` if an embedded picture exists at the specified zero-based index, or `false` otherwise.

An invalid picture index does not produce an error; it produces `false`.

This command does not support APE or WV files and returns exit code 20 for those formats.

### Get picture type

```text
--get-picture-type=<picture index>
```

Example:

```text
wintagtool.exe "--get-picture-type=0" "audio.mp3"
```

Outputs the picture type of the embedded picture at the specified zero-based index.

This command returns an empty output if the requested picture or picture type is not available.

If no embedded pictures are present, the command returns exit code 12.

If the specified picture index is out of range, the command returns exit code 13.

This command does not support APE, M4A, or WV files and returns exit code 20 for those formats.

### Get picture description

```text
--get-picture-description=<picture index>
```

Example:

```text
wintagtool.exe "--get-picture-description=0" "audio.mp3"
```

Outputs the description of the embedded picture at the specified zero-based index.

This command returns an empty output if the requested picture or picture description is not available.

If no embedded pictures are present, the command returns exit code 12.

If the specified picture index is out of range, the command returns exit code 13.

This command does not support APE, M4A, or WV files and returns exit code 20 for those formats.

### Get picture MIME type

```text
--get-picture-mime-type=<picture index>
```

Example:

```text
wintagtool.exe "--get-picture-mime-type=0" "audio.mp3"
```

Outputs the MIME type of the embedded picture at the specified zero-based index.

This command returns an empty output if the requested picture or MIME type is not available.

If no embedded pictures are present, the command returns exit code 12.

If the specified picture index is out of range, the command returns exit code 13.

This command does not support APE, M4A, or WV files and returns exit code 20 for those formats.

### Get picture size

```text
--get-picture-size=<picture index>
```

Example:

```text
wintagtool.exe "--get-picture-size=0" "audio.mp3"
```

Outputs the size in bytes of the embedded picture at the specified zero-based index.

This command returns an empty output if the requested picture is not available.

If no embedded pictures are present, the command returns exit code 12.

If the specified picture index is out of range, the command returns exit code 13.

This command does not support APE or WV files and returns exit code 20 for those formats.

### Get picture width

```text
--get-picture-width=<picture index>
```

Example:

```text
wintagtool.exe "--get-picture-width=0" "audio.mp3"
```

Outputs the width in pixels of the embedded picture at the specified zero-based index.

This command returns an empty output if the requested picture is not available or its dimensions cannot be determined.

If no embedded pictures are present, the command returns exit code 12.

If the specified picture index is out of range, the command returns exit code 13.

Supported picture types: JPG, JPEG, JFIF, PNG, and WEBP.

This command does not support APE, M4A, WMA, or WV files and returns exit code 20 for those formats.

### Get picture height

```text
--get-picture-height=<picture index>
```

Example:

```text
wintagtool.exe "--get-picture-height=0" "audio.mp3"
```

Outputs the height in pixels of the embedded picture at the specified zero-based index.

This command returns an empty output if the requested picture is not available or its dimensions cannot be determined.

If no embedded pictures are present, the command returns exit code 12.

If the specified picture index is out of range, the command returns exit code 13.

Supported picture types: JPG, JPEG, JFIF, PNG, and WEBP.

This command does not support APE, M4A, WMA, or WV files and returns exit code 20 for those formats.

## AUDIO PROPERTIES

### Get audio length

```text
--get-audio-length
```

Example:

```text
wintagtool.exe "--get-audio-length" "audio.mp3"
```

Outputs the audio length in seconds.

### Get audio bitrate

```text
--get-audio-bitrate
```

Example:

```text
wintagtool.exe "--get-audio-bitrate" "audio.mp3"
```

Outputs the audio bitrate in kbps.

### Get audio sample rate

```text
--get-audio-samplerate
```

Example:

```text
wintagtool.exe "--get-audio-samplerate" "audio.mp3"
```

Outputs the audio sample rate in Hz.

### Count audio channels

```text
--count-audio-channels
```

Example:

```text
wintagtool.exe "--count-audio-channels" "audio.mp3"
```

Outputs the number of audio channels.

## PictureType values

`<picture type>` must be specified using one of the following TagLib PictureType strings:

| #   | PictureType Name   | TagLib PictureType string | Description                                          | As shown by Mp3tag |
| ---:| ------------------ | ------------------------- | ---------------------------------------------------- | ------------------ |
| 0   | Other              | `Other`                   | A type not enumerated below                          | Other              |
| 1   | FileIcon           | `File Icon`               | 32x32 PNG image that should be used as the file icon | `<empty>`          |
| 2   | OtherFileIcon      | `Other File Icon`         | File icon of a different size or format              | Icon               |
| 3   | FrontCover         | `Front Cover`             | Front cover image of the album                       | Front Cover        |
| 4   | BackCover          | `Back Cover`              | Back cover image of the album                        | Back Cover         |
| 5   | LeafletPage        | `Leaflet Page`            | Inside leaflet page of the album                     | Leaflet            |
| 6   | Media              | `Media`                   | Image from the album itself                          | Media              |
| 7   | LeadArtist         | `Lead Artist`             | Picture of the lead artist or soloist                | Lead Artist        |
| 8   | Artist             | `Artist`                  | Picture of the artist or performer                   | Artist             |
| 9   | Conductor          | `Conductor`               | Picture of the conductor                             | Conductor          |
| 10  | Band               | `Band`                    | Picture of the band or orchestra                     | Band               |
| 11  | Composer           | `Composer`                | Picture of the composer                              | Composer           |
| 12  | Lyricist           | `Lyricist`                | Picture of the lyricist or text writer               | Lyricist           |
| 13  | RecordingLocation  | `Recording Location`      | Picture of the recording location or studio          | Recording Location |
| 14  | DuringRecording    | `During Recording`        | Picture of the artists during recording              | During Recording   |
| 15  | DuringPerformance  | `During Performance`      | Picture of the artists during performance            | During Performance |
| 16  | MovieScreenCapture | `Movie Screen Capture`    | Picture from a movie or video related to the track   | Video Capture      |
| 17  | ColouredFish       | `Coloured Fish`           | Picture of a large, coloured fish                    | `<empty>`          |
| 18  | Illustration       | `Illustration`            | Illustration related to the track                    | Illustration       |
| 19  | BandLogo           | `Band Logo`               | Logo of the band or performer                        | Band Logotype      |
| 20  | PublisherLogo      | `Publisher Logo`          | Logo of the publisher (record company)               | Publisher Logotype |

### Notes

- When specifying a picture type, you must use the exact text string specified in the TagLib PictureType string column. This string is case-sensitive.
- If a text string other than one of those specified in the TagLib PictureType string column is used, the picture type is automatically set to `Other`.
- In Mp3tag, FileIcon, OtherFileIcon, LeafletPage, MovieScreenCapture, ColouredFish, BandLogo, and PublisherLogo are displayed differently or are not displayed.

## Multi-value Tags

Some audio formats support storing multiple values for the same tag. (Whether multiple values are supported, depends on the underlying audio format and TagLib.)

For example (ARTIST is a multi-value tag):

```text
TITLE=My Song
ARTIST=John Smith
ARTIST=The Smith Band
GENRE=Jazz
TRACK=12
```

`--get-tag=ARTIST` outputs this:

```text
John Smith
The Smith Band
```

`--append-tag` appends a new tag value without replacing existing tag values. For the example above, `--append-tag=ARTIST=Denise Shaffer` results in this:

```text
TITLE=My Song
ARTIST=John Smith
ARTIST=The Smith Band
ARTIST=Denise Shaffer
GENRE=Jazz
TRACK=12
```

`--count-all-tags` counts the number of distinct tag names, not tag values. In the last version of the example above, it returns:

```text
4
```

`--count-all-tag-values` counts the number of individual tag values. In the last version of the example above, it returns:

```text
6
```

`--count-values-of-tag=ARTIST` counts the number of values of the tag ARTIST. In the last version of the example above, it returns:

```text
3
```

The commands `--count-all-tags`, `--count-all-tag-values`, and `--count-values-of-tag` are not supported for WMA files and return exit code 20 for that format.

WinTagTool intentionally does not provide commands for removing or replacing an individual value of a multi-value tag. More advanced multi-value tag manipulation can be implemented by reading all values from a multi-value tag, rebuilding the desired list of values, removing that multi-value tag, and recreating that multi-value tag with the desired list of values.

Example:

```text
--count-values-of-tag=ARTIST
--get-tag=ARTIST
(Rebuild the desired list of values)
--remove-tag=ARTIST [optional]
--set-tag=ARTIST=Denise Shaffer
--append-tag=ARTIST=John Smith
--append-tag=ARTIST=The Shaffer Band
```

## Counting tags and tag values

Even when there appear to be no multi-value tags, there may be small differences between the number of reported tag names and the number of reported tag values. For example, for the two tag names `ENCODING` and `ENCODING PARAMS`, there may be just one tag value for `ENCODING`.

There may be differences between the (number of) tags reported by WinTagTool and the (number of) tags reported by Mp3tag. The two tags `DISCNUMBER=1/1` and `TRACKNUMBER=5/6` as reported by WinTagTool, may be reported by Mp3tag as the four tags `TRACK=5`, `TOTALTRACKS=6`, `DISCNUMBER=1`, `TOTALDISKS=1`. (There may also be differences in tag naming. For example, the tag `DATE` may be the same as the tag `YEAR`.)

## Embedded Pictures

`--append-picture-from` always appends the new picture after all existing pictures.

`--import-picture-from` removes all existing pictures before importing the new one.

WinTagTool intentionally does not provide commands for removing or replacing individual pictures. More advanced picture manipulation can be implemented by exporting all pictures from the audio file, rebuilding the desired picture list, removing all pictures from the audio file, and importing the desired picture list to the audio file.

Example:

```text
--count-pictures
--export-picture=0
--export-picture=1
--export-picture=2
(Rebuild the desired picture list)
--remove-pictures [optional]
--import-picture-from=C:\Temp\Image00.jpg
--append-picture-from=C:\Temp\Image01.jpg
--append-picture-from=C:\Temp\Image02.jpg
```

If the picture type or picture description of one or more existing pictures needs to be changed, the pictures must be exported and then re-imported with the desired picture type and picture description. Pictures that do not need to be changed should be re-imported with their original picture type and picture description.

This approach can be used to change the picture type and/or picture description of a single picture, multiple pictures, or all pictures.

## Audio file format limitations

The following limitations were established through testing of the current WinTagTool build. Unsupported commands result in exit code 20.

### APE

Not supported:

```text
--append-picture-from
--export-picture
--count-pictures
--has-picture
--get-picture-type
--get-picture-description
--get-picture-mime-type
--get-picture-size
--get-picture-width
--get-picture-height
```

Limited support (see support matrix below for details):

```text
--import-picture-from
--remove-pictures
```

### M4A

Not supported:

```text
--get-picture-type
--get-picture-description
--get-picture-mime-type
--get-picture-width
--get-picture-height
```

Limited support (see support matrix below for details):

```text
--import-picture-from
--append-picture-from
```

Pictures can be added with `--import-picture-from` and `--append-picture-from`, but setting PictureType and PictureDescription is not supported. The first picture is always displayed in Mp3tag with PictureType "Front Cover", while subsequent pictures are always displayed in Mp3tag with PictureType "Other". Picture descriptions are not preserved.

### WMA

Not supported:

```text
--count-values-of-tag
--count-all-tags
--count-all-tag-values
--get-picture-width
--get-picture-height
```

Limited support (see support matrix below for details):

```text
--import-picture-from
--append-picture-from
```

### WV

Not supported:

```text
--append-picture-from
--export-picture
--count-pictures
--has-picture
--get-picture-type
--get-picture-description
--get-picture-mime-type
--get-picture-size
--get-picture-width
--get-picture-height
```

Limited support (see support matrix below for details):

```text
--import-picture-from
--remove-pictures
```

## Support matrix

Key: ✓ = supported, ✗ = not supported, ✗/✓ = limited support.

### Metadata tags

| Command                  | AIFF/AIF | APE | FLAC | M4A | MP3 | OGG | OPUS | WMA | WV  |
| ------------------------ |:--------:|:---:|:----:|:---:|:---:|:---:|:----:|:---:|:---:|
| `--set-tag`              | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--append-tag`           | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--get-tag`              | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--remove-tag`           | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--count-values-of-tag`  | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✗   | ✓   |
| `--count-all-tags`       | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✗   | ✓   |
| `--count-all-tag-values` | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✗   | ✓   |
| `--has-tag`              | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--list-tags`            | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--list-tags-and-values` | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |

### Embedded pictures

| Command                     | AIFF/AIF | APE  | FLAC | M4A  | MP3 | OGG | OPUS | WMA  | WV   |
| --------------------------- |:--------:|:----:|:----:|:----:|:---:|:---:|:----:|:----:|:----:|
| `--import-picture-from`     | ✓        | ✗/✓¹ | ✓    | ✗/✓² | ✓   | ✓   | ✓    | ✗/✓¹ | ✗/✓¹ |
| `--append-picture-from`     | ✓        | ✗    | ✓    | ✗/✓² | ✓   | ✓   | ✓    | ✗/✓¹ | ✗    |
| `--export-picture`          | ✓        | ✗    | ✓    | ✓    | ✓   | ✓   | ✓    | ✓    | ✗    |
| `--remove-pictures`         | ✓        | ✗/✓¹ | ✓    | ✓    | ✓   | ✓   | ✓    | ✓    | ✗/✓¹ |
| `--count-pictures`          | ✓        | ✗    | ✓    | ✓    | ✓   | ✓   | ✓    | ✓    | ✗    |
| `--has-picture`             | ✓        | ✗    | ✓    | ✓    | ✓   | ✓   | ✓    | ✓    | ✗    |
| `--get-picture-type`        | ✓        | ✗    | ✓    | ✗    | ✓   | ✓   | ✓    | ✓    | ✗    |
| `--get-picture-description` | ✓        | ✗    | ✓    | ✗    | ✓   | ✓   | ✓    | ✓    | ✗    |
| `--get-picture-mime-type`   | ✓        | ✗    | ✓    | ✗    | ✓   | ✓   | ✓    | ✓    | ✗    |
| `--get-picture-size`        | ✓        | ✗    | ✓    | ✓    | ✓   | ✓   | ✓    | ✓    | ✗    |
| `--get-picture-width`       | ✓        | ✗    | ✓    | ✗    | ✓   | ✓   | ✓    | ✗    | ✗    |
| `--get-picture-height`      | ✓        | ✗    | ✓    | ✗    | ✓   | ✓   | ✓    | ✗    | ✗    |

### Audio properties

| Command                  | AIFF/AIF | APE | FLAC | M4A | MP3 | OGG | OPUS | WMA | WV  |
| ------------------------ |:--------:|:---:|:----:|:---:|:---:|:---:|:----:|:---:|:---:|
| `--get-audio-length`     | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--get-audio-bitrate`    | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--get-audio-samplerate` | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |
| `--count-audio-channels` | ✓        | ✓   | ✓    | ✓   | ✓   | ✓   | ✓    | ✓   | ✓   |

¹ Only supported when one or no pictures are already embedded.

² PictureType and PictureDescription are not supported. Supported: `--import-picture-from=ImageFile` and `--append-picture-from=ImageFile`. Not supported: `--import-picture-from=ImageFile=PictureType`, `--import-picture-from=ImageFile=PictureType=PictureDescription`, `--append-picture-from=ImageFile=PictureType`, and `--append-picture-from=ImageFile=PictureType=PictureDescription`. Note that these four commands with PictureType and PictureDescription can be used, but will always result in the value `Null` for PictureType and PictureDescription.

## Tested audio file formats

The following audio file formats were tested: AIFF/AIF, APE, FLAC, M4A, MP3, OGG, OPUS, WMA, and WV.

Other than the limitations listed above, the commands appear to work as expected for the tested audio file formats. However, individual commands may still have format-specific behavior that was not identified during testing.

## Limitations regarding the `=` character

An image path or filename containing `=` cannot be used with these commands:

- `--import-picture-from`
- `--append-picture-from`

When using an image path or filename containing `=` with `--import-picture-from`, exit code 7 will be returned. When doing this with `--append-picture-from`, exit code 15 will be returned.

For example, the following path with file name cannot be used with `--import-picture-from`:

```text
C:\Images\Cover=Front.jpg
```

Adding additional quotation marks or apostrophes does not remove this limitation.

## Diagnostic messages

WMA files may cause diagnostic messages from TagLib to appear on the console. These messages do not prevent the commands from working as expected.

## Exit Codes

| Code | Meaning                                                                                                                                                      |
| ----:| ------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 0    | Operation completed successfully.                                                                                                                            |
| 1    | Invalid number of command-line arguments.                                                                                                                    |
| 2    | Audio file could not be opened or is not supported.                                                                                                          |
| 3    | Invalid `--set-tag` syntax.                                                                                                                                  |
| 4    | Unknown command-line option.                                                                                                                                 |
| 5    | Failed to save changes.                                                                                                                                      |
| 6    | Failed to remove all embedded pictures.                                                                                                                      |
| 7    | Failed to open image file for `--import-picture-from`.                                                                                                       |
| 8    | Image file is empty or has an invalid size.                                                                                                                  |
| 9    | Failed to read image file or insufficient memory.                                                                                                            |
| 10   | Failed to import picture.                                                                                                                                    |
| 11   | Invalid `--export-picture` syntax.                                                                                                                           |
| 12   | No embedded pictures found.                                                                                                                                  |
| 13   | Picture index out of range.                                                                                                                                  |
| 14   | Failed to write exported picture.                                                                                                                            |
| 15   | Failed to open image file for `--append-picture-from`.                                                                                                       |
| 16   | Image file is empty or has an invalid size.                                                                                                                  |
| 17   | Failed to read image file or insufficient memory.                                                                                                            |
| 18   | Failed to append picture.                                                                                                                                    |
| 19   | Invalid `--append-tag` syntax.                                                                                                                               |
| 20   | Tag counting, tag-value counting, picture counting, picture-property reading, or picture dimension reading, is not supported for the specified audio format. |

An exit code of 0 indicates success.

Any non-zero exit code indicates that the requested operation failed.

## Overview of WinTagTool commands with description

| #   | Command                                                                   | Description                                                                                                          |
| ---:| ------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
|     | **METADATA TAGS**                                                         |                                                                                                                      |
| 1   | `--set-tag=<tag name>=<tag value>`                                        | Replaces the value of the specified tag, or adds this tag with this value.                                           |
| 2   | `--append-tag=<tag name>=<tag value>`                                     | Appends a new value to a multi-value tag.                                                                            |
| 3   | `--get-tag=<tag name>`                                                    | Outputs the value or values associated with the specified tag.                                                       |
| 4   | `--remove-tag=<tag name>`                                                 | Removes the specified tag.                                                                                           |
| 5   | `--count-values-of-tag=<tag name>`                                        | Outputs the number of values associated with the specified tag.                                                      |
| 6   | `--count-all-tags`                                                        | Outputs the total number of tags.                                                                                    |
| 7   | `--count-all-tag-values`                                                  | Outputs the total number of tag values.                                                                              |
| 8   | `--has-tag=<tag name>`                                                    | Tests whether the specified tag exists.                                                                              |
| 9   | `--list-tags`                                                             | Outputs the name of all tags.                                                                                        |
| 10  | `--list-tags-and-values`                                                  | Outputs each tag name together with its value, one tag value per line.                                               |
|     | **EMBEDDED PICTURES**                                                     |                                                                                                                      |
| 11  | `--import-picture-from=<image file>`                                      | Imports a picture, which replaces all existing embedded pictures.                                                    |
| 12  | `--import-picture-from=<image file>=<picture type>`                       | Imports a picture with the specified picture type.                                                                   |
| 13  | `--import-picture-from=<image file>=<picture type>=<picture description>` | Imports a picture with the specified picture type and the specified picture description.                             |
| 14  | `--append-picture-from=<image file>`                                      | Appends a picture after the existing embedded pictures.                                                              |
| 15  | `--append-picture-from=<image file>=<picture type>`                       | Appends a picture after the existing pictures with the specified picture type.                                       |
| 16  | `--append-picture-from=<image file>=<picture type>=<picture description>` | Appends a picture after the existing pictures with the specified picture type and the specified picture description. |
| 17  | `--export-picture=<picture index>=<picture base filename>`                | Exports a picture with the specified zero-based index and the specified picture base filename.                       |
| 18  | `--remove-pictures`                                                       | Removes all embedded pictures.                                                                                       |
| 19  | `--count-pictures`                                                        | Outputs the number of embedded pictures.                                                                             |
| 20  | `--has-picture=<picture index>`                                           | Tests whether an embedded picture at the specified zero-based index exists.                                          |
| 21  | `--get-picture-type=<picture index>`                                      | Outputs the picture type of the embedded picture at the specified zero-based index.                                  |
| 22  | `--get-picture-description=<picture index>`                               | Outputs the picture description of the embedded picture at the specified zero-based index.                           |
| 23  | `--get-picture-mime-type=<picture index>`                                 | Outputs the MIME type of the embedded picture at the specified zero-based index.                                     |
| 24  | `--get-picture-size=<picture index>`                                      | Outputs the size in bytes of the embedded picture at the specified zero-based index.                                 |
| 25  | `--get-picture-width=<picture index>`                                     | Outputs the width in pixels of the embedded picture at the specified zero-based index.                               |
| 26  | `--get-picture-height=<picture index>`                                    | Outputs the height in pixels of the embedded picture at the specified zero-based index.                              |
|     | **AUDIO PROPERTIES**                                                      |                                                                                                                      |
| 27  | `--get-audio-length`                                                      | Outputs the audio length in seconds.                                                                                 |
| 28  | `--get-audio-bitrate`                                                     | Outputs the audio bitrate in kbps.                                                                                   |
| 29  | `--get-audio-samplerate`                                                  | Outputs the audio sample rate in Hz.                                                                                 |
| 30  | `--count-audio-channels`                                                  | Outputs the number of audio channels.                                                                                |

## Overview of WinTagTool examples

| #   | Example                                                                                            |
| ---:| -------------------------------------------------------------------------------------------------- |
|     | **METADATA TAGS**                                                                                  |
| 1   | `wintagtool.exe "--set-tag=TITLE=My Song" "audio.mp3"`                                             |
| 2   | `wintagtool.exe "--append-tag=ARTIST=The Smith Band" "audio.mp3"`                                  |
| 3   | `wintagtool.exe "--get-tag=ARTIST" "audio.mp3"`                                                    |
| 4   | `wintagtool.exe "--remove-tag=COMMENT" "audio.mp3"`                                                |
| 5   | `wintagtool.exe "--count-values-of-tag=ARTIST" "audio.mp3"`                                        |
| 6   | `wintagtool.exe "--count-all-tags" "audio.mp3"`                                                    |
| 7   | `wintagtool.exe "--count-all-tag-values" "audio.mp3"`                                              |
| 8   | `wintagtool.exe "--has-tag=ALBUM" "audio.mp3"`                                                     |
| 9   | `wintagtool.exe "--list-tags" "audio.mp3"`                                                         |
| 10  | `wintagtool.exe "--list-tags-and-values" "audio.mp3"`                                              |
|     | **EMBEDDED PICTURES**                                                                              |
| 11  | `wintagtool.exe "--import-picture-from=C:\Images\Cover.jpg" "audio.mp3"`                           |
| 12  | `wintagtool.exe "--import-picture-from=C:\Images\Cover.jpg=Front Cover" "audio.mp3"`               |
| 13  | `wintagtool.exe "--import-picture-from=C:\Images\Cover.jpg=Front Cover=Album Cover" "audio.mp3"`   |
| 14  | `wintagtool.exe "--append-picture-from=C:\Images\BackCover.png" "audio.mp3"`                       |
| 15  | `wintagtool.exe "--append-picture-from=C:\Images\BackCover.png=Back Cover" "audio.mp3"`            |
| 16  | `wintagtool.exe "--append-picture-from=C:\Images\BackCover.png=Back Cover=Band Photo" "audio.mp3"` |
| 17  | `wintagtool.exe "--export-picture=0=C:\Temp\Cover" "audio.mp3"`                                    |
| 18  | `wintagtool.exe "--remove-pictures" "audio.mp3"`                                                   |
| 19  | `wintagtool.exe "--count-pictures" "audio.mp3"`                                                    |
| 20  | `wintagtool.exe "--has-picture=0" "audio.mp3"`                                                     |
| 21  | `wintagtool.exe "--get-picture-type=0" "audio.mp3"`                                                |
| 22  | `wintagtool.exe "--get-picture-description=0" "audio.mp3"`                                         |
| 23  | `wintagtool.exe "--get-picture-mime-type=0" "audio.mp3"`                                           |
| 24  | `wintagtool.exe "--get-picture-size=0" "audio.mp3"`                                                |
| 25  | `wintagtool.exe "--get-picture-width=0" "audio.mp3"`                                               |
| 26  | `wintagtool.exe "--get-picture-height=0" "audio.mp3"`                                              |
|     | **AUDIO PROPERTIES**                                                                               |
| 27  | `wintagtool.exe "--get-audio-length" "audio.mp3"`                                                  |
| 28  | `wintagtool.exe "--get-audio-bitrate" "audio.mp3"`                                                 |
| 29  | `wintagtool.exe "--get-audio-samplerate" "audio.mp3"`                                              |
| 30  | `wintagtool.exe "--count-audio-channels" "audio.mp3"`                                              |

## Overview of WinTagTool PowerShell examples

| #   | PowerShell Example                                                                                   |
| ---:| ---------------------------------------------------------------------------------------------------- |
|     | **METADATA TAGS**                                                                                    |
| 1   | `& $WinTagToolExe "--set-tag=$TagName=$TagValue" $AudioFile`                                         |
| 2   | `& $WinTagToolExe "--append-tag=$TagName=$TagValue" $AudioFile`                                      |
| 3   | `$TagValues = & $WinTagToolExe "--get-tag=$TagName" $AudioFile`                                      |
| 4   | `& $WinTagToolExe "--remove-tag=$TagName" $AudioFile`                                                |
| 5   | `$NumberOfExistingValuesOfTag = & $WinTagToolExe "--count-values-of-tag=$TagName" $AudioFile`        |
| 6   | `$NumberOfExistingTags = & $WinTagToolExe "--count-all-tags" $AudioFile`                             |
| 7   | `$NumberOfExistingTagValues = & $WinTagToolExe "--count-all-tag-values" $AudioFile`                  |
| 8   | `$TagExists = & $WinTagToolExe "--has-tag=$TagName" $AudioFile`                                      |
| 9   | `$ListOfNamesOfTags = & $WinTagToolExe "--list-tags" $AudioFile`                                     |
| 10  | `$ListOfNamesOfTagsWithValue = & $WinTagToolExe "--list-tags-and-values" $AudioFile`                 |
|     | **EMBEDDED PICTURES**                                                                                |
| 11  | `& $WinTagToolExe "--import-picture-from=$ImageFile" $AudioFile`                                     |
| 12  | `& $WinTagToolExe "--import-picture-from=$ImageFile=$PictureType" $AudioFile`                        |
| 13  | `& $WinTagToolExe "--import-picture-from=$ImageFile=$PictureType=$PictureDescription" $AudioFile`    |
| 14  | `& $WinTagToolExe "--append-picture-from=$ImageFile" $AudioFile`                                     |
| 15  | `& $WinTagToolExe "--append-picture-from=$ImageFile=$PictureType" $AudioFile`                        |
| 16  | `& $WinTagToolExe "--append-picture-from=$ImageFile=$PictureType=$PictureDescription" $AudioFile`    |
| 17  | `$ExportedPicture = & $WinTagToolExe "--export-picture=$PictureIndex=$ImageBaseFileName" $AudioFile` |
| 18  | `& $WinTagToolExe "--remove-pictures" $AudioFile`                                                    |
| 19  | `$NumberOfExistingPictures = & $WinTagToolExe "--count-pictures" $AudioFile`                         |
| 20  | `$PictureExists = & $WinTagToolExe "--has-picture=$PictureIndex" $AudioFile`                         |
| 21  | `$PictureType = & $WinTagToolExe "--get-picture-type=$PictureIndex" $AudioFile`                      |
| 22  | `$PictureDescription = & $WinTagToolExe "--get-picture-description=$PictureIndex" $AudioFile`        |
| 23  | `$PictureMimeType = & $WinTagToolExe "--get-picture-mime-type=$PictureIndex" $AudioFile`             |
| 24  | `$PictureSize = & $WinTagToolExe "--get-picture-size=$PictureIndex" $AudioFile`                      |
| 25  | `$PictureWidth = & $WinTagToolExe "--get-picture-width=$PictureIndex" $AudioFile`                    |
| 26  | `$PictureHeight = & $WinTagToolExe "--get-picture-height=$PictureIndex" $AudioFile`                  |
|     | **AUDIO PROPERTIES**                                                                                 |
| 27  | `$AudioLength = & $WinTagToolExe "--get-audio-length" $AudioFile`                                    |
| 28  | `$AudioBitrate = & $WinTagToolExe "--get-audio-bitrate" $AudioFile`                                  |
| 29  | `$AudioSamplerate = & $WinTagToolExe "--get-audio-samplerate" $AudioFile`                            |
| 30  | `$AudioChannels = & $WinTagToolExe "--count-audio-channels" $AudioFile`                              |

## Supported Audio File Formats

WinTagTool relies on the TagLib library for audio-file handling.

The audio file formats tested with the current WinTagTool build include:

- AIFF/AIF
- APE
- FLAC
- M4A
- MP3
- OGG
- OPUS
- WMA
- WV

Support for specific metadata fields and operations depends on both the audio file format and the TagLib implementation.

## Building

WinTagTool is written in C and has been developed and tested using:

- WinLibs GCC 16.1.0 (UCRT)
- MinGW-w64 14.0.0
- TagLib 2.2.1 (C API)

## Disclaimer

WinTagTool modifies audio metadata.

Always keep backup copies of important audio files before modifying them.

Although the software has been tested on a variety of file formats, no warranty is provided. The author is not responsible for data loss, file corruption, or any other damage resulting from the use of this software.

## Credits

WinTagTool is based on the TagLib C API.

Additional runtime libraries include components from:

- GCC Runtime
- libstdc++
- winpthreads
- zlib

Please refer to the licenses of those projects for their respective terms.

The audio file format support provided by WinTagTool is determined by TagLib.
