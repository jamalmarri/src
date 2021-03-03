# This is a series of shell scripts/commands used to simplify the process of building a new kernel for a raspberry pi

cp ../sys/arch/evbarm/conf/GENERIC ../sys/arch/evbarm/conf/testKern
config ../src/sys/evbarm/conf/testKern
cd ../sys/arch/evbarm/compile/testKern
make -j4 depend
cd ../../../../poweronbuildtools/

# Copies GENRIC to testKern
