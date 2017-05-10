# install gcc 6 and other related dev tools
sudo yum install centos-release-scl -y
sudo yum install devtoolset-6-gcc* -y
# enable for this user
echo 'source scl_source enable devtoolset-6' >> ~/.bashrc
source ~/.bashrc

# install wget
sudo yum install wget -y

# install cmake
cd ~/
wget https://cmake.org/files/v3.7/cmake-3.7.2-Linux-x86_64.tar.gz
tar xf cmake-3.7.2-Linux-x86_64.tar.gz
cd cmake-3.7.2-Linux-x86_64/
sudo cp -r bin/* /usr/local/bin/
sudo cp -r share/* /usr/local/share/

# assuming asl-manzano on ~/

# install green_manzano
cd ~/asl-manzano/code/apps/green_manzano/
mkdir build
cd build
cmake ..
make
sudo make install

# install mce (manzano configuration editor)
cd ~/asl-manzano/code/apps/mce/
mkdir build
cd build
cmake ..
make
sudo make install

echo "-----------------------------------------------------------"
echo " green_manzano and mce installation end."
echo " Use an existing config file or create one with mce"
echo "-----------------------------------------------------------"
