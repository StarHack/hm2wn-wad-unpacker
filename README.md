![Header Image](http://i.imgur.com/zm2bjlH.jpg)

# *.WAD Ripper

This program dumps assets from *.wad files (used by Hotline Miami 2: Wrong Number) to disk.  

# Usage

If you're on Mac OS X and have the GOG version of the game, it will automatically use **hlm2_data_desktop.wad** from the mounted .dmg file. Alternatively you might as well pass the location of your *.wad file to the program:

```sh
$ ./ripper <hlm2_data_desktop.wad>
```

The program will extract all files into working directory.

# Compiling

I built this tool on Mac OS X but you may compile and use it on any platform of your choice. Compiling with gcc:

```sh
$ gcc ripper.c -o ripper
```

# OGG Files (Music)

For some reason, VLC might not be capable of playing the *.ogg (vorbis) files of this game. However, it's possible to convert them to e.g. *.mp3 with a converter such as ffmpeg.

Mac OS X:
```sh
$ brew install ffmpeg
$ ffmpeg -f ogg -i Detection.ogg -acodec libmp3lame Detection.mp3
```

or batch convert all *.ogg files to *.mp3:

```sh
$  find . -name '*.ogg' -exec sh -c 'ffmpeg -f ogg -i "$0" -acodec libmp3lame "${0%%.ogg}.mp3"' {} \;
```

**Note:** The `-f ogg` flag forces ffmpeg to read a vorbis stream. This is necessary because automatic detection doesn't work for all music files.

# File Format Specification

A *.wad file has the following file header specification:

```sh
4 bytes ... file header - only contains a 32-bit Integer for the file count
```

Afterwards, each file has an individual header of dynamic size:

```sh
4 bytes ... file name length - 32-bit Integer for the file header length
dynamic ... file name - length is equal to the value read above
8 bytes ... file length - 64-bit Integer for the file length
8 bytes ... file offset - 64-bit Integer showing at which offset the file begins in the binary
```

Enjoy!


