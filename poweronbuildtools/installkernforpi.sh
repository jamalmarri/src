# This is a series of shell scripts/commands used to simplify the process of building a new kernel for a raspberry pi

mv /netbsd /netbsd.old

# Save the old kern for recovery and install the new one, will overwrite any currently saved "netbsd.old"
