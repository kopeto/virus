# virus spread naive simulation

![GitHub Logo](/image.jpg)

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
You can configure virus propagation intial values in a file:
```bash
./virus initial-values
```
Or simulate with default values:
```bash
./virus 
```
