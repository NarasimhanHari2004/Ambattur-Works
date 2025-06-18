sudo apt update
sudo apt install -y build-essential libgmp-dev libmpfr-dev libmpc-dev libisl-dev libzstd-dev
wget http://ftp.gnu.org/gnu/gcc/gcc-14.1.0/gcc-14.1.0.tar.gz
tar -xf gcc-14.1.0.tar.gz
mkdir gcc-build-14.1.0
cd gcc-build-14.1.0
../gcc-14.1.0/configure \ --build=x86_64-linux-gnu \ --host=x86_64-linux-gnu \ --target=x86_64-linux-gnu \ --prefix=/usr/local/gcc-14.1.0 \ --enable-checking=release \ --disable-multilib \ --enable-languages=c,c++ \ --program-suffix=-14.1.0
make -j$(nproc)
sudo make install
#DESKTOP
sudo update-alternatives --install /usr/bin/gcc gcc /usr/local/gcc-14.1.0/bin/gcc-14.1.0 140
sudo update-alternatives --config gcc
