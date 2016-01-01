!/bin/bash
if [ ! -e cmake-3.4.1 ];then
    wget --no-check-certificate https://cmake.org/files/v3.4/cmake-3.4.1.tar.gz&&tar xf cmake-3.4.1.tar.gz
    cd cmake-3.4.1
    cmake .
    make -j 4
    cd ..
fi;
cd cmake-3.4.1
sudo make install
export CMAKE_ROOT=/usr/local/share/cmake-3.4/
cd ..

if [ ! -e SFML ];then
    git clone https://github.com/SFML/SFML
    cd SFML
    cmake .
    make -j 4
    cd ..
fi
cd SFML
if [! -e Makefile ]; then
    cmake .
fi
sudo make install
cd ..
