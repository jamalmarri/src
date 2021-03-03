# This is a series of shell scripts/commands used to simplify the process of building a new kernel for a raspberry pi

#.././build.sh -U -u -j4 -m evbarm -a earmv7hf -O ~/obj release kernel=testKern

cd ../sys/arch/evbarm/compile/testKern
make -j4
cd ../../../../poweronbuildtools

# Raspbery pis work a bit different than traditional systems
# We need to use the build script becasuse we need netbsd.img instead of the netbsd build by the makefile
