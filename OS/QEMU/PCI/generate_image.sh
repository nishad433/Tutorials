#!/bin/bash
script_name=$(echo $0 | awk -F"/" '{print $NF}')
CMD=$1
if [ "$2" != "" ]; then
	OUTDIR=$2
else
	OUTDIR="/tmp/ncc"
fi

if [ ! -d "$OUTDIR" ]; then
	mkdir -p $OUTDIR
fi

ARCH="arm64"
LINUX_VER="4.19.287"
BUSYBOX_VER="1.35.0"
CROSS_NG_VER="1.26.0"

CROSS_NG_DIR="$OUTDIR/crosstool-ng-${CROSS_NG_VER}"
LINUX_DIR="$OUTDIR/linux-${LINUX_VER}"
BUSYBOX_DIR="$OUTDIR/busybox-${BUSYBOX_VER}"

TOOLCHAIN_PATH="${OUTDIR}/toolchain"
CROSS_COMPILER_PATH="${TOOLCHAIN_PATH}/aarch64-unknown-linux-gnu"
CROSS_COMPILER_BINPATH="${CROSS_COMPILER_PATH}/bin"
CROSS_COMPILER_PREFIX="${CROSS_COMPILER_BINPATH}/aarch64-unknown-linux-gnu-"
#CROSS_COMPILER_PREFIX="${OUTDIR}/aarch64-unknown-linux-gnu/bin/aarch64-unknown-linux-gnu-"

ROOTFS_DIR="$OUTDIR/rootfs"
ROOTFS_IMG_FILE="$OUTDIR/rootfs.img"
ROOTFS_MNT_DIR="$OUTDIR/mnt"

create_rootfs_file() {
	file=$1
	outfile="$ROOTFS_DIR/etc/$file"

	if [ "$file" = "passwd" ]; then
		cat <<EOT >"$outfile"
root::0:0:root:/root:/bin/sh
EOT
	elif [ "$file" = "fstab" ]; then
		cat <<EOT >"$outfile"
# <file system>	<mount pt>	<type>	<options>	<dump>	<pass>
/dev/root	/		ext2	rw,noauto	0	1
proc		/proc		proc	defaults	0	0
tmpfs		/dev/shm	tmpfs	mode=1777	0	0
tmpfs		/tmp		tmpfs	mode=1777	0	0
tmpfs		/run		tmpfs	mode=0755,nosuid,nodev	0	0
sysfs		/sys		sysfs	defaults	0	0
EOT

	elif
		[ "$file" = "inittab" ]
	then
		cat <<EOT >"$outfile"
# /etc/inittab
#
# Copyright (C) 2001 Erik Andersen <andersen@codepoet.org>
#
# Note: BusyBox init doesn't support runlevels.  The runlevels field is
# completely ignored by BusyBox init. If you want runlevels, use
# sysvinit.
#
# Format for each entry: <id>:<runlevels>:<action>:<process>
#
# id        == tty to run on, or empty for /dev/console
# runlevels == ignored
# action    == one of sysinit, respawn, askfirst, wait, and once
# process   == program to run

# Startup the system
::sysinit:/bin/mount -t proc proc /proc
::sysinit:/bin/mount -o remount,rw /
::sysinit:/bin/mkdir -p /dev/pts /dev/shm
::sysinit:/bin/mount -a
::sysinit:/bin/mkdir -p /run/lock/subsys
::sysinit:/sbin/swapon -a
null::sysinit:/bin/ln -sf /proc/self/fd /dev/fd
null::sysinit:/bin/ln -sf /proc/self/fd/0 /dev/stdin
null::sysinit:/bin/ln -sf /proc/self/fd/1 /dev/stdout
null::sysinit:/bin/ln -sf /proc/self/fd/2 /dev/stderr
::sysinit:/bin/hostname -F /etc/hostname
# now run any rc scripts
::sysinit:/etc/init.d/rcS

# Put a getty on the serial port
console::respawn:/sbin/getty -L  console 0 vt100 # GENERIC_SERIAL

# Stuff to do for the 3-finger salute
#::ctrlaltdel:/sbin/reboot

# Stuff to do before rebooting
::shutdown:/etc/init.d/rcK
::shutdown:/sbin/swapoff -a
::shutdown:/bin/umount -a -r
EOT
	fi

}

copy_needed_shared_libraries() {
	file=$1
	libraries_needed=$(readelf -d $file | grep "NEEDED" | awk -F'[][]' '{print $2}' | xargs)

	ret=0
	for lib in $libraries_needed; do
		res=$(find $ROOTFS_DIR -name $lib)
		if [ "$res" != "" ]; then
			continue
		fi
		res=$(find $CROSS_COMPILER_PATH -name $lib)
		if [ "$res" != "" ]; then
			cp $res $ROOTFS_DIR/lib/
			cp $res $ROOTFS_DIR/lib64/
		fi
		copy_needed_shared_libraries $res
	done
	return $ret
}

print_usage() {
	printf "Usage: %s <toolchain|kernel|busybox|rootfs|all> [outdir]\n" "$script_name"
}

build_toolchain() {
	printf "Building toolchain\n"
	(
		cd $OUTDIR &&
			sudo rm -rf ${CROSS_NG_DIR}

		# Download tar (Latest: 1.26.0) and untar it.
		if [ ! -f crosstool-ng-${CROSS_NG_VER}.tar.xz ]; then
			wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-${CROSS_NG_VER}.tar.xz
		fi
		tar -xf crosstool-ng-${CROSS_NG_VER}.tar.xz -C ./

		(
			cd ${CROSS_NG_DIR} &&
				# Build and Install Crosstool-NG
				# Prepare and configure the build environment
				./bootstrap

			# Configure the installation path for Crosstool-NG
			./configure --prefix=${CROSS_NG_DIR}/opt/crosstool-ng #(add --prefix if you want to install it to custom directory)

			# Build and install Crosstool-NG
			make
			sudo make install # only needed if you want to install, otherwise use it (ct-ng ) from the build directory itself

			# Set Up the Crosstool-NG Configuration
			./ct-ng aarch64-unknown-linux-gnu # for glibc
			#./ct-ng  aarch64-unknown-linux-uclibc    # for uclibc

			# Build the Toolchain
			./ct-ng build CT_PREFIX=${TOOLCHAIN_PATH}

			# Install the Toolchain.(Default is ~/x-tools, otherwise, set CT_PREFIX to custom path through menuconfig or export CT_PREFIX as export CT_PREFIX=/custom/path/to/toolchain)
			#./ct-ng install CT_PREFIX=${TOOLCHAIN_PATH}

			# Verify the Toolchain
			#aarch64-unknown-linux-gnu-gcc --version
			# Set the Environment Variables
			#export PATH=$PATH:/path/to/your/toolchain/bin
		)
	)
}

build_kernel() {
	printf "Building kernel\n"
	(
		cd $OUTDIR &&
			# Download Linux kernel and untar it
			wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-${LINUX_VER}.tar.xz
		tar -xf linux-${LINUX_VER}.tar.xz -C ./
		(
			cd ${LINUX_DIR} &&

				# To use a default configuration for AARCH64:
				make ARCH=${ARCH} defconfig

			# Build the Kernel
			make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILER_PREFIX} -j$(nproc)

			# Build the Kernel Modules (Optional)
			make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILER_PREFIX} modules
		)
	)
}

build_busybox() {
	printf "Building busybox\n"
	(
		cd $OUTDIR &&
			# Download busybox and untar it
			wget https://busybox.net/downloads/busybox-${BUSYBOX_VER}.tar.bz2
		tar -xjf busybox-${BUSYBOX_VER}.tar.bz2
		(
			cd ${BUSYBOX_DIR} &&
				# Confiugre busybox
				make ARCH=$(ARCH) CROSS_COMPILE=${CROSS_COMPILER_PREFIX} defconfig

			# build busybox
			make ARCH=$(ARCH) CROSS_COMPILE=${CROSS_COMPILER_PREFIX}
		)
	)
}

create_rootfs() {
	rm -rf $ROOTFS_DIR $ROOTFS_IMG_FILE $ROOTFS_MNT_DIR

	# Create the Root Filesystem Directory
	mkdir -p $ROOTFS_DIR

	(
		cd $ROOTFS_DIR &&

			# Create Basic Directory Structure
			mkdir -p {bin,boot,dev,etc,home,lib,lib64,mnt,opt,proc,root,sbin,sys,tmp,usr,var}

		# Create Essential Device Nodes

		# 	(
		# 		cd dev &&
		 		# Block Device: /dev/vda
		#		sudo mknod -m 666 vda b 254 0

				# Serial Console: /dev/ttyAMA0
		#		sudo mknod -m 666 ttyAMA0 c 5 64

				# Null Device: /dev/null
		#		sudo mknod -m 666 null c 1 3

				# Random Device: /dev/random and /dev/urandom
		#		sudo mknod -m 666 random c 1 8
		#		sudo mknod -m 666 urandom c 1 9

				# Console Device: /dev/console
		#		sudo mknod -m 666 console c 5 1

				# PTY Device: /dev/ptmx
		#		sudo mknod -m 666 ptmx c 5 2

				# Memory Device: /dev/mem
		#		sudo mknod -m 666 mem c 1 1

				# zero Device: /dev/zero
		#		sudo mknod -m 666 zero c 1 5
		#	)

		# Create /etc/fstab
		create_rootfs_file fstab

		# Create /etc/passwd:
		create_rootfs_file passwd

		# Create /etc/inittab (Optional):
		create_rootfs_file inittab

		# Create /etc/hostname
		echo "my-arm-system" >./etc/hostname
	)

	if [ -d $BUSYBOX_DIR ]; then
		(
			cd $BUSYBOX_DIR &&
				# Install Basic Binaries (e.g., BusyBox):
				make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILER_PREFIX} CONFIG_PREFIX=${ROOTFS_DIR} install
			copy_needed_shared_libraries ./busybox
		)
	fi

	# Create an empty disk image (size of your choice):
	dd if=/dev/zero of=$ROOTFS_IMG_FILE bs=1M count=1024

	# Create the ext4 filesystem on the disk image:
	mkfs.ext4 $ROOTFS_IMG_FILE

	# Mount the image to copy the root filesystem into it:
	mkdir -p $ROOTFS_MNT_DIR
	sudo mount $ROOTFS_IMG_FILE $ROOTFS_MNT_DIR

	# Copy the root filesystem into the mounted image:
	sudo cp -a $ROOTFS_DIR/* $ROOTFS_MNT_DIR/

	# make all file as root
	sudo sudo chown -R root:root $ROOTFS_MNT_DIR/

	# Unmount the image once all files have been copied:
	sudo umount $ROOTFS_MNT_DIR

	rm -rf $ROOTFS_MNT_DIR
	rm -rf $ROOTFS_DIR

	printf "rootfs img file created in %s\n" "$ROOTFS_IMG_FILE"
}

################# MAIN STARTS HERE ########################################

case $CMD in
-h | --h | --help | help)
	print_usage
	exit 0
	;;

toolchain)
	build_toolchain
	;;

kernel)
	build_kernel
	;;

busybox)
	build_busybox
	;;

rootfs)
	create_rootfs
	;;

all)
	build_toolchain
	build_kernel
	build_busybox
	create_rootfs
	;;
*)
	print_usage
	exit 0
	;;
esac
