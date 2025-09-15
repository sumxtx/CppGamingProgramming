## TODO Makefile

## Library -> https://www.sfml-dev.org/download/sfml/2.6.0/

# 
# if the extracted Library is one directory above the src files
#


## libFLAC.s0.12 required by SFML play sounds, otherwise won't compile ->https://ftp.osuosl.org/pub/xiph/releases/flac/flac-1.4.0.tar.xz
## Compile it but doesn't `sudo make install`,
## instead copy the libFLAC.so.12.0.0 as ../../SFML-2.6.0/libFLAC.so.12

# 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../SFML-2.6.0/lib
#
g++ -c\
   ./src/Factory.cpp\
   ./src/Graphics.cpp\
   ./src/InputReceiver.cpp\
   ./src/PlayerGraphics.cpp\
   ./src/GameObject.cpp\
   ./src/InputDispatcher.cpp\
   ./src/LevelUpdate.cpp\
   ./src/PlayerUpdate.cpp\
   ./src/SoundEngine.cpp\
   ./src/Update.cpp\
   ./src/CameraGraphics.cpp\
   ./src/CameraUpdate.cpp\
   ./src/run.cpp\
    -I../../SFML-2.6.0/include -I./include
mv *.o ./obj/
g++ \
   ./obj/Factory.o\
   ./obj/Graphics.o\
   ./obj/InputReceiver.o\
   ./obj/PlayerGraphics.o\
   ./obj/GameObject.o\
   ./obj/InputDispatcher.o\
   ./obj/LevelUpdate.o\
   ./obj/PlayerUpdate.o\
   ./obj/SoundEngine.o\
   ./obj/Update.o\
   ./obj/CameraGraphics.o\
   ./obj/CameraUpdate.o\
   ./obj/run.o\
     -o EndlessRun-app -L../../SFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio ../../SFML-2.6.0/lib/libFLAC.so.12



