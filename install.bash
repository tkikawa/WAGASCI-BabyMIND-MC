#!/bin/bash

# install.bash
# Copyright (C) 2018 by Pintaudi Giorgio <giorgio-pintaudi-kx@ynu.jp>
# Released under the GPLv3 license
#
#     Pintaudi Giorgio (PhD Student)
#     Yokohama National University
#     giorgio-pintaudi-kx@ynu.jp
#
# This is a bash script that installs the WAGASCI-BabyMIND-MC software
# along with all its dependencies for the Ubuntu and Scientific Linux OS.

ROOTREP="n"
GEANTREP="n"
CONTINUE="n"
UBUNTU="n"
SL6="n"
ROOTVERS="6-14-06"
GEANTVERS="v10.5.0"
CMAKEVERS="3.13.4"
GCCVERS_UBUNTU="7.3.0"
GCCVERS="6.3.1"
GCCVERS_SL6="4.4.7"
GCCREP="1"
PYTHONVERS="2.7.13"
CLHEPVERS="2.4.1.0"

# Check the Ubuntu and Scientific Linux releases

if [ ! -f "/usr/bin/lsb_release" ] && [ ! -f "/etc/redhat-release" ];
then
    echo ""
    echo "This installer is for Ubuntu 18.04 and Scientific Linux 6.10 only!"
    echo "You can get this script to run also on other versions of Ubuntu"
    echo "by simply replacing the 18.04 string on line 34 with your Ubuntu"
    echo "version but be warned that other modifications may be needed."
    echo ""
    exit 1
fi

if [ -f "/usr/bin/lsb_release" ] && [ "`lsb_release -rs`" == "18.04" ];
then
    UBUNTU="y"
    CMAKE=cmake
elif [ -f "/etc/redhat-release" ] && [ "`cat /etc/redhat-release`" == "Scientific Linux release 6.10 (Carbon)" ];
then
    SL6="y"
    CMAKE=cmake
	# Stop if there is some error
	set -e
else
    echo "There is something wrong about OS detection."
    echo "UBUNTU = $UBUNTU"
    echo "SL6 = $SL6"
    echo ""
    exit 1
fi

#check if sudo has been used

if [ "`whoami`" == "root" ];
then
    echo ""
    echo "This installer is not intended be run as root or with sudo"
    echo "You will need to insert the user password AFTER the script has started."
    echo ""
    exit 1
fi

# Check for ROOT
if [ -z "${ROOTSYS}" ] ;
then
    echo ""
    echo "ROOT is a mandatory dependency but it seems that it is not installed"
    echo "(looking for a non null ROOTSYS variable)."
	echo "Maybe you have just forgotten to set up the root enviroment with the"
	echo "script thisroot.sh."
    echo "In that case please run that script and then restart the installation."
    echo ""
    echo "If ROOT is not installed in your system, this script can take care of"
    echo "the ROOT installation."
    echo ""
    echo -n "Do you want this installer to install ROOT? (y|n) : "
    read ROOTREP
    if [ "${ROOTREP}" == "n" ];
    then
		echo -n "Do you want this installer to continue anyway? (y|n) : "
		read CONTINUE
		if [ "${CONTINUE}" == "n" ];
		then
			exit 1
		else
			CONTINUE = "" 
		fi
    elif [ "${ROOTREP}" == "y" ];
    then
		echo -n "Set to install it (ROOTREP=\"y\")"
    else
		echo "I didn't understand your answer. Sorry, try again."
		exit 1
    fi
fi

# Check for Geant4
if [ ! -d "/usr/local/geant4" ] && [ ! -d "${HOME}/geant4-${GEANTVERS}" ] ;
then
    echo ""
    echo "Geant4 is a mandatory dependency but it seems that it is not installed"
    echo "(looking for the /usr/local/geant4 or ${HOME}/geant4-${GEANTVERS} folders)."
    echo ""
    echo -n "Do you want this installer to install Geant4? (y|n) : "
    read GEANTREP
    if [ "${GEANTREP}" == "n" ];
    then
		echo -n "Do you want this installer to continue anyway? (y|n) : "
		read CONTINUE
		if [ "${CONTINUE}" == "n" ];
		then
			exit 1
		else
			CONTINUE = "" 
		fi
    elif [ "${GEANTREP}" == "y" ];
    then
		echo -n "Set to install it (GEANTREP=\"y\")"
    else
		echo "I didn't understand your answer. Sorry, try again."
		exit 1
    fi
fi

#############################################################################
#                                                                           #
#                                 GCC COMPILER                              #
#                                                                           #
#############################################################################

# Check for GCC
if [ $SL6 == "y" ]; then
	GCCVERS_DETECTED=`gcc -dumpversion`
	if [ ${GCCVERS_DETECTED} == ${GCCVERS_UBUNTU} ] ; then
		echo ""
		echo "The compiler version is ${GCCVERS_DETECTED}, the same as the Ubuntu 18.04 one."
		echo "So far so good."
		export CC=${HOME}/gcc-${GCCVERS_DETECTED}/bin/gcc
		export CXX=${HOME}/gcc-${GCCVERS_DETECTED}/bin/g++
		export GCCVERS=${GCCVERS_DETECTED}
		export LD_LIBRARY_PATH=${HOME}/gcc-${GCCVERS}/lib:$LD_LIBRARY_PATH
		export LD_LIBRARY_PATH=${HOME}/gcc-${GCCVERS}/lib64:$LD_LIBRARY_PATH
	elif [ ${GCCVERS_DETECTED} == ${GCCVERS} ] ; then
		echo ""
		echo "The compiler version is ${GCCVERS_DETECTED}, enough for our purposes."
		echo "So far so good."
		export CC=/opt/rh/devtoolset-6/root/usr/bin/gcc
		export CXX=/opt/rh/devtoolset-6/root/usr/bin/g++
		export GCCVERS=${GCCVERS_DETECTED}
		export LD_LIBRARY_PATH=/opt/rh/devtoolset-6/root/usr/lib/gcc/x86_64-redhat-linux/${GCCVERS}:$LD_LIBRARY_PATH
		export LD_LIBRARY_PATH=/opt/rh/devtoolset-6/root/usr/lib64:$LD_LIBRARY_PATH
	elif [ ${GCCVERS_DETECTED} == ${GCCVERS_SL6} ] ; then
		echo ""
		echo "The compiler version ${GCCVERS_DETECTED} is ancient, not enough to compile"
		echo "ROOT6 or Geant4 v10.5.0: it must be updated. Choose 1 if you want to just"
		echo "use the precompiled version ${GCCVERS} or choose 2 to compile the same"
		echo "version as Ubuntu 18.04 ${GCCVERS_UBUNTU}. The two version are both fine"
		echo "but to compile the latter one it takes about 3 hours."
		echo ""
		echo -n "[1] ${GCCVERS} (Recommended) - [2] ${GCCVERS_UBUNTU} "
		read GCCREP
		if [ "${GCCREP}" == "1" ];
		then
			export PATH=/opt/rh/devtoolset-6/root/usr/bin:$PATH
			export LD_LIBRARY_PATH=/opt/rh/devtoolset-6/root/usr/lib/gcc/x86_64-redhat-linux/${GCCVERS}:$LD_LIBRARY_PATH
			export LD_LIBRARY_PATH=/opt/rh/devtoolset-6/root/usr/lib64:$LD_LIBRARY_PATH
			export CC=/opt/rh/devtoolset-6/root/usr/bin/gcc
			export CXX=/opt/rh/devtoolset-6/root/usr/bin/g++
			cat >> "${HOME}/.bash_profile" <<EOF

# set PATH to include a more recent gcc version
if [ -d "/opt/rh/devtoolset-6/root/usr/bin" ] ; then
   export PATH=/opt/rh/devtoolset-6/root/usr/bin:\$PATH
   export LD_LIBRARY_PATH=/opt/rh/devtoolset-6/root/usr/lib/gcc/x86_64-redhat-linux/${GCCVERS}:\$LD_LIBRARY_PATH
   export LD_LIBRARY_PATH=/opt/rh/devtoolset-6/root/usr/lib64:\$LD_LIBRARY_PATH
fi
EOF
		elif [ "${GCCREP}" == "2" ];
		then
			cd
			wget https://ftp.gnu.org/gnu/gcc/gcc-${GCCVERS}/gcc-${GCCVERS}.tar.gz
			tar xzf gcc-${GCCVERS}.tar.gz
			mv -f gcc-${GCCVERS} gcc-${GCCVERS}-sources
			cd gcc-${GCCVERS}-sources
			wget https://gmplib.org/download/gmp/gmp-6.1.0.tar.bz2
			wget https://www.mpfr.org/mpfr-3.1.4/mpfr-3.1.4.tar.bz2
			wget https://ftp.gnu.org/gnu/mpc/mpc-1.0.3.tar.gz
			wget http://isl.gforge.inria.fr/isl-0.16.1.tar.bz2
			./contrib/download_prerequisites
			cd ..
			mkdir -p objdir
			mkdir -p gcc-${GCCVERS}
			cd objdir
			$PWD/../gcc-${GCCVERS}-sources/configure --prefix=$HOME/gcc-${GCCVERS} --enable-languages=c,c++,fortran,go
			make -j56
			make install
			export PATH=${HOME}/gcc-${GCCVERS}/bin:$PATH
			export LD_LIBRARY_PATH=${HOME}/gcc-${GCCVERS}/lib:$LD_LIBRARY_PATH
			export LD_LIBRARY_PATH=${HOME}/gcc-${GCCVERS}/lib64:$LD_LIBRARY_PATH
			export CC=${HOME}/gcc-${GCCVERS_DETECTED}/bin/gcc
			export CXX=${HOME}/gcc-${GCCVERS_DETECTED}/bin/g++
			hash -r
			cat >> "${HOME}/.bash_profile" <<EOF

# set PATH to include a more recent gcc version
if [ -d "${HOME}/gcc-${GCCVERS}/bin" ] ; then
   export PATH=${HOME}/gcc-${GCCVERS}/bin:\$PATH
   export LD_LIBRARY_PATH=${HOME}/gcc-${GCCVERS}/lib:\$LD_LIBRARY_PATH
   export LD_LIBRARY_PATH=${HOME}/gcc-${GCCVERS}/lib64:\$LD_LIBRARY_PATH
fi
EOF
			cd && rm -rf gcc-${GCCVERS}-sources objdir
		else
			echo "I didn't understand your answer. Sorry, try again."
			exit 1
		fi
	else
		echo "The detected GCC version ${GCCVERS_DETECTED} not recognized."
		echo "If this version is C++11 compatible (>= 4.8.1) it is probably fine."
		echo "Otherwise you need to upgrade to a C++11 compatible version, but it"
		echo "is difficult to say without testing. If you want to upgrade GCC with"
		echo "this script just set the GCCVERS_SL6=${GCCVERS_DETECTED} variable at"
		echo "the beginning of the script and re-run it."
		echo ""
		echo -n "Do you want this installer to continue anyway? (y|n) : "
		read CONTINUE
		if [ "${CONTINUE}" == "n" ];
		then
			exit 1
		else
			CONTINUE = "" 
		fi
	fi
fi

echo ""
echo ""

#install Geant4 if necessary
if [ "${GEANTREP}" == "y" ];
then
	echo ""
	echo "-------------------"
	echo "Geant4 INSTALLATION"
	echo "-------------------"

	if [ $UBUNTU == "y" ];
	then

		#############################################################################
		#                                                                           #
		#                                 GEANT 4 (UBUNTU)                          #
		#                                                                           #
		#############################################################################
		
		cd
		sudo apt-get update
		sudo apt-get upgrade
		sudo apt-get install -y git build-essential libexpat1-dev libxmu-dev cmake cmake-curses-gui qt5-default
		git clone https://github.com/Geant4/geant4.git geant4-sources
		cd geant4-sources
		git checkout ${GEANTVERS}
		cd ..
		mkdir -p geant4-${GEANTVERS}-build
		cd build
		${CMAKE} -DCMAKE_INSTALL_PREFIX=/usr/local/geant4 \
				 -DGEANT4_INSTALL_DATA=ON \
				 -DGEANT4_BUILD_MULTITHREADED=ON \
				 -DGEANT4_USE_OPENGL_X11=ON \
				 -DGEANT4_USE_QT=ON \
				 -DGEANT4_USE_SYSTEM_EXPAT=ON \
				 ../geant4-sources
		make -j8
		sudo make install
		source /usr/local/geant4/share/Geant4-10.5.0/geant4make/geant4make.sh
		cat >> "${HOME}/.profile" <<EOF

# set PATH to include Geant4
if [ -f "/usr/local/geant4/bin/geant4.sh" ] ; then
   source /usr/local/geant4/share/Geant4-10.5.0/geant4make/geant4make.sh
fi
EOF
		cd && rm -rf geant4-${GEANTVERS}-build
	elif [ $SL6 == "y" ];
	then


		#############################################################################
		#                                                                           #
		#                                 CMAKE 3                                   #
		#                                                                           #
		#############################################################################
		
		
		if [ `${CMAKE} --version | head -n 1 | grep -Po '\d.*.*' | tail -1` != ${CMAKEVERS} ]; then
			### Install a more recent version of cmake ###
			cd
			wget https://cmake.org/files/LatestRelease/cmake-${CMAKEVERS}.tar.gz
			tar -zxf cmake-${CMAKEVERS}.tar.gz
			mv -f cmake-${CMAKEVERS} cmake-${CMAKEVERS}-sources
			mkdir -p cmake-${CMAKEVERS}
			cd cmake-${CMAKEVERS}-sources
			./bootstrap --prefix=${HOME}/cmake-${CMAKEVERS}
			make -j56
			make install
			export PATH=${HOME}/cmake-${CMAKEVERS}/bin:$PATH
			hash -r
			cat >> "${HOME}/.bash_profile" <<EOF
# set PATH to include a more recent cmake version
if [ -d "${HOME}/cmake-${CMAKEVERS}/bin" ] ; then
   export PATH=${HOME}/cmake-${CMAKEVERS}/bin:\$PATH
fi
EOF
			cd && rm -rf cmake-${CMAKEVERS}-sources
		fi

		#############################################################################
		#                                                                           #
		#                                 GEANT 4 (SL6)                             #
		#                                                                           #
		#############################################################################
		
		### Install Geant4 ###
		cd
		git clone https://github.com/Geant4/geant4.git geant4-sources
		cd geant4-sources
		git checkout ${GEANTVERS}
		cd ..
		mkdir -p geant4-${GEANTVERS}-build
		mkdir -p geant4-${GEANTVERS}
		cd geant4-${GEANTVERS}-build
		CC=$(CC) CXX=$(CXX) ${CMAKE} \
		  -DCMAKE_INSTALL_PREFIX=../geant4-${GEANTVERS} \
		  -DGEANT4_INSTALL_DATA=ON \
		  -DGEANT4_BUILD_MULTITHREADED=ON \
		  -DGEANT4_USE_OPENGL_X11=ON \
		  -DGEANT4_USE_QT=ON \
		  -DGEANT4_USE_SYSTEM_EXPAT=ON \
		../geant4-sources
		make -j56
		make install
		source ${HOME}/geant4-${GEANTVERS}/share/Geant4-10.5.0/geant4make/geant4make.sh
		# remove the /usr/lib64 that geant4make.sh has inserted in the LD_LIBRARY_PATH
		LD_LIBRARY_PATH=$(echo ${LD_LIBRARY_PATH} | awk -v RS=: -v ORS=: '/usr\/lib64/ {next} {print}')
		cat >> "${HOME}/.bash_profile" <<EOF
# set PATH to include Geant4
if [ -f "${HOME}/geant4-${GEANTVERS}/bin/geant4.sh" ] ; then
   source ${HOME}/geant4-${GEANTVERS}/share/Geant4-10.5.0/geant4make/geant4make.sh
   export LD_LIBRARY_PATH=\$(echo \${LD_LIBRARY_PATH} | awk -v RS=: -v ORS=: '/usr\/lib64/ {next} {print}')
fi
EOF
		cd && rm -rf geant4-${GEANTVERS}-build geant4-sources
	fi
fi

#install root if necessary
if [ "${ROOTREP}" == "y" ];
then
	echo ""
	echo "-------------------"
	echo "ROOT INSTALLATION"
	echo "-------------------"

	#############################################################################
	#                                                                           #
	#                                 ROOT (UBUNTU)                             #
	#                                                                           #
	#############################################################################
	
	if [ $UBUNTU == "y" ];
	then
		ROOTDIR="${HOME}/ROOT"
		sudo apt-get update
		sudo apt-get upgrade
		sudo apt-get install -y build-essential git dpkg-dev cmake xutils-dev \
			 binutils libx11-dev libxpm-dev libxft-dev libxext-dev \
			 libssl-dev libpcre3-dev libglu1-mesa-dev libglew-dev \
			 libmysqlclient-dev libfftw3-dev libcfitsio-dev libgraphviz-dev \
			 libavahi-compat-libdnssd-dev libldap2-dev python-dev libxml2-dev \
			 libkrb5-dev libgsl-dev libqt4-dev libmotif-dev libmotif-common \
			 libblas-dev liblapack-dev xfstt xfsprogs t1-xfree86-nonfree \
			 ttf-xfree86-nonfree ttf-xfree86-nonfree-syriac xfonts-75dpi \
			 xfonts-100dpi libgif-dev libtiff-dev libjpeg-dev liblz4-dev \
			 liblzma-dev libgl2ps-dev libpostgresql-ocaml-dev libsqlite3-dev \
			 libpythia8-dev davix-dev srm-ifce-dev libtbb-dev python-numpy
		cd
		# Download and install ROOT
		mkdir -p ${ROOTDIR}/{sources,${ROOTVERS},${ROOTVERS}-build}
		cd ${ROOTDIR}
		git clone http://github.com/root-project/root.git sources
		cd sources
		git checkout -b v${ROOTVERS} v${ROOTVERS}
		cd ../${ROOTVERS}-build
		${CMAKE} -Dbuiltin_xrootd=ON -DCMAKE_INSTALL_PREFIX=${ROOTDIR}/${ROOTVERS} \
				 ../sources
		${CMAKE} --build . --target install -- -j8
		cd
		source ${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh
		cat >> "${HOME}/.profile" <<EOF

# set PATH to include ROOT
if [ -f "${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh" ] ; then
   source ${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh
fi
EOF
		rm -rf ${ROOTDIR}/${ROOTVERS}-build
	elif [ $SL6 == "y" ];
	then

		#############################################################################
		#                                                                           #
		#                                 PYTHON 2.7                                #
		#                                                                           #
		#############################################################################
		
		if [ "`python --version`" != "Python ${PYTHONVERS}" ] ; then
			### Install Python ###
			export PATH=/opt/rh/python27/root/usr/bin:$PATH
			export PYTHON_LIBRARY=/opt/rh/python27/root/usr/lib64
			export PYTHON_INCLUDE_DIR=/opt/rh/python27/root/usr/include/python2.7
			export PYTHON_EXECUTABLE=/opt/rh/python27/root/usr/bin/python2.7
			export LD_LIBRARY_PATH=$PYTHON_LIBRARY:$LD_LIBRARY_PATH
			cat >> "${HOME}/.bash_profile" <<EOF

# set PATH to include Python
if [ -d "/opt/rh/python27/root/usr/bin" ] ; then
   export PATH=/opt/rh/python27/root/usr/bin:\$PATH
   export PYTHON_LIBRARY=/opt/rh/python27/root/usr/lib64
   export PYTHON_INCLUDE_DIR=/opt/rh/python27/root/usr/include/python2.7
   export PYTHON_EXECUTABLE=/opt/rh/python27/root/usr/bin/python2.7
   export LD_LIBRARY_PATH=$PYTHON_LIBRARY:\$LD_LIBRARY_PATH
fi
EOF
		fi

		#############################################################################
		#                                                                           #
		#                                 ROOT (SL6)                                #
		#                                                                           #
		#############################################################################
		
		# Download and install ROOT
		ROOTDIR="${HOME}/ROOT"
		mkdir -p ${ROOTDIR}/{sources,${ROOTVERS},${ROOTVERS}-build}
		cd ${ROOTDIR}
		git clone http://github.com/root-project/root.git sources
		cd sources
		git checkout -b v${ROOTVERS} v${ROOTVERS}
		cd ../${ROOTVERS}-build
		CC=$(CC) CXX=$(CXX) ${CMAKE} \
		  -Dbuiltin_xrootd=ON \
		  -DCMAKE_INSTALL_PREFIX=${ROOTDIR}/${ROOTVERS} \
		  -DPYTHON_EXECUTABLE=/opt/rh/python27/root/usr/bin/python \
		  ../sources
		CC=$(CC) CXX=$(CXX) ${CMAKE} \
		  --build . --target install -- -j56
		cd ..
		rm -rf ${ROOTVERS}-build
		cd
		source ${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh
		cat >> "${HOME}/.bash_profile" <<EOF

# set PATH to include ROOT
if [ -f "${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh" ] ; then
   source ${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh
fi
EOF
	fi
fi

# ROOT detection
if [ -z ${ROOTSYS} ]; then
	echo "Couldn't detect ROOT installation."
	echo "Perhaps you forgot to source the thisroot.sh script."
fi

# GEANT4 detection
if [ -z ${G4INSTALL} ]; then
	echo "Couldn't detect Geant4 installation."
	echo "Perhaps you forgot to source the geant4make.sh script."
fi

#############################################################################
#                                                                           #
#                               CERNLIB 2006                                #
#                                                                           #
#############################################################################

if [ $SL6 == "y" ] ; then
	if [ -z "${CERN}" ] ; then
		export CERN=/home/t2k/tatsuya1/cern/cernlib
		export CERN_LEVEL=2006
		export CERN_ROOT=${CERN}/${CERN_LEVEL}
		export PATH=${CERN_ROOT}/bin:${PATH}
		export LD_LIBRARY_PATH=${CERN_ROOT}/lib:${LD_LIBRARY_PATH}
		cat >> "${HOME}/.bash_profile" <<EOF

set PATH to include CERNLIB
if [ -d "/home/t2k/tatsuya1/cern/cernlib" ] ; then
	export CERN=/home/t2k/tatsuya1/cern/cernlib
	export CERN_LEVEL=2006
	export CERN_ROOT=\${CERN}/\${CERN_LEVEL}
	export PATH=\${CERN_ROOT}/bin:\${PATH}
	export LD_LIBRARY_PATH=\${CERN_ROOT}/lib:\${LD_LIBRARY_PATH}
fi
EOF
	fi
elif [ $UBUNTU == "y" ] ; then
	# CERNLIB
	cd
	wget http://www-zeuthen.desy.de/linear_collider/cernlib/new/cernlib-20061220+dfsg3.patches.2019.02.03.txt
	sudo apt install -y devscripts
	rm -rf cernlib_debuild
	mkdir cernlib_debuild
	cd cernlib_debuild
	wget http://mirrors.kernel.org/ubuntu/pool/main/libx/libxp/libxp-dev_1.0.2-1ubuntu1_amd64.deb
	sudo apt-get install ./libxp-dev_1.0.2-1ubuntu1_amd64.deb
	sudo sed -Ei 's/^# deb-src /deb-src /' /etc/apt/sources.list
	sudo apt-get update
	sudo apt-get build-dep -y cernlib
	apt-get source cernlib
	cd cernlib-20061220*
	patch -p1 < ../../cernlib-20061220+dfsg3.patches.2019.02.03.txt
	debuild -us -uc # > ../debuild_us_uc.cernlib.out.txt 2>&1
	cd ..
	rm -f cernlib-extras_*.deb pawserv_*.deb zftp_*.deb
	sudo dpkg -i *.deb
	sudo apt-get -y purge cernlib cernlib-core cernlib-core-dev
	# LIBPAW
	sudo apt-get build-dep -y paw
	apt-get source paw
	cd paw-2.14.04*
	debuild -us -uc # > ../debuild_us_uc.paw.out.txt 2>&1
	cd ..
	sudo dpkg -i *.deb
	sudo apt-get -y purge cernlib
	# CERNLIB Monte Carlo
	sudo apt-get build-dep -y cernlib-montecarlo
	apt-get source cernlib-montecarlo
	cd mclibs-20061220*
	debuild -us -uc # > ../debuild_us_uc.mclibs.out.txt 2>&1
	cd ..
	sudo dpkg -i *.deb
	sudo apt-get -y purge cernlib
	sudo apt-get build-dep -y geant321
	# Geant321
	apt-get source geant321
	cd geant321-3.21.14*
	debuild -us -uc # > ../debuild_us_uc.geant321.out.txt 2>&1
	cd ..
	sudo dpkg -i *.deb
	# Do not "update" the installed libraries
	sudo apt-mark hold `ls -1 *.deb | sed -e '{s/_.*\.deb//}'`
	cd .. && rm -rf cernlib_debuild
	export CERN_ROOT=
fi

#############################################################################
#                                                                           #
#                               G77 (UBUNTU)                                #
#                                                                           #
#############################################################################

if [ $UBUNTU == "y" ] ; then
	if [ ! -f "/usr/bin/g77" ] ; then
		sudo tee -a /etc/apt/sources.list << END

# Old hardy repository needed to install g77
# It is recommended to comment or remove following lines after g77 installation
deb [trusted=yes] http://old-releases.ubuntu.com/ubuntu/ hardy universe
deb-src [trusted=yes] http://old-releases.ubuntu.com/ubuntu/ hardy universe
deb [trusted=yes] http://old-releases.ubuntu.com/ubuntu/ hardy-updates universe
deb-src [trusted=yes] http://old-releases.ubuntu.com/ubuntu/ hardy-updates universe
END
		sudo apt-get update && sudo apt-get install -y g77
		sudo ln -s /usr/lib/gcc/x86_64-linux-gnu/7/libgcc_s.so /usr/lib/x86_64-linux-gnu/
		export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH"
		tee -a $HOME/.profile << END

# set LD_LIBRARY_PATH so it includes x86_64-linux-gnu if it exists
if [ -d "/usr/lib/x86_64-linux-gnu" ] ; then
LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:\$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH
fi
END
	fi
fi

echo ""
echo "-------------------"
echo "WAGASCI-BabyMIND MC INSTALLATION"
echo "-------------------"

#############################################################################
#                                                                           #
#                            WAGASCI-BabyMIND MC                            #
#                                                                           #
#############################################################################

if [ $SL6 == "y" ] ; then
	cd
	cd WAGASCI-BabyMIND-MC
	if [ ! -f "./lib/libg2c.so" ] ; then
		ln -s /usr/lib64/libg2c.so.0 ./lib/libg2c.so
	fi
	CC=${CC} CXX=${CXX} LD_LIBRARY_PATH=${LD_LIBRARY_PATH} make
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH} ./B2MC --help

elif [ $UBUNTU == "y" ] ; then
	cd
	sudo apt-get install -y libxi-dev
	cd WAGASCI-BabyMIND-MC
	make
	./B2MC --help
fi

echo ""
echo "Installation successfully completed!"
echo "For any questions about this script please contact:"
echo "Pintaudi Giorgio (PhD Student)"
echo "Yokohama National University"
echo "giorgio-pintaudi-kx@ynu.jp"
echo ""

exit 0

# install.bash
#
# Copyright (C) 2018 by Pintaudi Giorgio <giorgio-pintaudi-kx@ynu.jp>
# Released under the GPLv3 license
#
#     Pintaudi Giorgio (PhD Student)
#     Yokohama National University
#     giorgio-pintaudi-kx@ynu.jp
