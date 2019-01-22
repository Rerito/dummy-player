# dummy-player

[![Build Status](https://travis-ci.org/Rerito/dummy-player.svg?branch=master)](https://travis-ci.org/Rerito/dummy-player)

Just a little project to resfresh some skillz

## About

This is a pseudo music player. It only uses the STL so there is no actual music
playback. However, it emulates the action of a playlist-based music player.

In that sense, the user can perform commands to manage a playlist and the pseudo
music playback.

The application is multithreaded and playback is simulated.

## How to build

You will need a C++17 compliant compiler and STL and CMake with version >= 3.9.
If this is ok, just run cmake appropriately and you are done!
Of course, adapt the generators depending on what you have available...


    mkdir build && cd build && cmake .. && make


## How to use

Now, you can launch the app:

    app/dummy-player/dummy-player

And let the console guide you (try typing help or any gibberish)
The "supported" music files are simply made out of `key=value` lines.
Take a look at [an example track](data/megadeth/01_HolyWars.music) to get a
better idea.

Durations are expressed in seconds.
