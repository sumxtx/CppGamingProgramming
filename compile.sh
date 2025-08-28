## TODO Makefile

## Library -> https://www.sfml-dev.org/download/sfml/2.6.0/



# 
# if the extracted Library is one directory above the src files
#
g++ -c Timber.cpp -I../SFML-2.6.0/include
g++ Timber.o -o Timber-app -L../SFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system
