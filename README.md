# virus spread naive simulation

<!-- ![](logo.gif) -->
<p align="center">
  <img src="logo_1.gif" width="400">
  <br/>
  <img src="logo_2.gif" width="400">
</p>

## Description
Naive simulation of a virus spreading.

## Installation
Install last build tools:
```bash
sudo apt-get update
sudo apt-get install build-essential
```

Install Boost-random:
```bash
sudo apt-get install libboost-random-dev
```

Install SDL2:
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

Install FFmpeg:
```bash
sudo apt-get install ffmpeg
```

Build:
```bash
make
```

## Run
You can configure virus propagation parameters and pass the filepath as argument to the executable:
```bash
./virus initial-values
```
Or simulate with default values:
```bash
./virus
```

After running the simulation a log file will be created in the current directory with the relevant data.

If the simulation terminated successfully, a mp4 video file will be created in video folder.
