wget https://cmake.org/files/v3.8/cmake-3.8.0.tar.gz
tar xzfv cmake-3.8.0.tar.gz 
cd cmake-3.8.0
./configure
gmake -j `nproc`
cd ..
git submodule add https://github.com/aws/aws-sdk-cpp.git
cd aws-sdk-cpp/
../cmake-3.8.0/bin/cmake -DBUILD_ONLY="monitoring" -DBUILD_SHARED_LIBS=false .
