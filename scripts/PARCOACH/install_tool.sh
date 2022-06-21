# installs parcoach

git clone https://github.com/N00byKing/parcoach.git --branch=llvm13_compat
mkdir parcoach/build
cd parcoach/build
cmake ..
make -j$(nproc)

cd ../..
