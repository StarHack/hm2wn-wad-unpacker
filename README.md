# *.WAD Ripper

This program dumps assets from *.wad files (used by Hotline Miami 2: Wrong Number) to disk.  

# Usage

If you're on Mac OS X and using the GOG version of the game, it will automatically use **hlm2_data_desktop.wad** from the mounted .dmg file. Otherwise you can still pass the location of your *.wad file to the program:

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

For some reason, VLC wasn't capable of playing the *.ogg (vorbis) files dumped to disk. However, it's possible to convert them to e.g. *.mp3 with a converter such as ffmpeg.

Mac OS X:
```sh
$ brew install ffmpeg
$ ffmpeg -i Detection.ogg -acodec libmp3lame Detection.mp3
```

or batch convert all *.ogg files to *.mp3:

```sh
$  find . -name '*.ogg' -exec sh -c 'ffmpeg -i "$0" -acodec libmp3lame "${0%%.ogg}.mp3"' {} \;
```

Enjoy!


