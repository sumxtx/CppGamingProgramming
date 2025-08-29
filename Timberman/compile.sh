## TODO Makefile

## Library -> https://www.sfml-dev.org/download/sfml/2.6.0/

# 
# if the extracted Library is one directory above the src files
#


## libFLAC.s0.12 required by SFML play sounds, otherwise won't compile ->https://ftp.osuosl.org/pub/xiph/releases/flac/flac-1.4.0.tar.xz
## Compile it but doesn't `sudo make install`,
## instead copy the libFLAC.so.12.0.0 as ../../SFML-2.6.0/libFLAC.so.12

g++ -c Timber.cpp -I../../SFML-2.6.0/include
g++ Timber.o -o Timber-app -L../../SFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio ../../SFML-2.6.0/lib/libFLAC.so.12
