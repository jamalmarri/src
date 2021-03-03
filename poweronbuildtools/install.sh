# This is a series of shell scripts/commands used to simplify the process of building a new kernel for a raspberry pi

cd ../sys/arch/evbarm/compile/testKern
mv /boot/kernel7.img /boot/kernel7.img.old
cp ../sys/arch/evbarm/compile/testKern/netbsd.img /boot/kernel7.img
mv /netbsd /netbsd.old
cp ../sys/arch/evbarm/compile/testKern/netbsd /netbsd

# Save the old kern for recovery and install the new one, will overwrite any currently saved "netbsd.old"
