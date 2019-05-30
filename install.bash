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

UBUNTU="n"
SL6="n"
ROOTVERS="6-16-00"
GEANTVERS="10.5.1"
CMAKEVERS="3.14.4"

# Define a function that checks if a package is installed

function isinstalled {
    if [ $SL6 == "y" ];
    then
		if yum list installed "$@" >/dev/null 2>&1; then
			true
		else
			false
		fi
    elif [ $UBUNTU == "y" ];
    then
		dpkg -s $1 &> /dev/null
		if [ $? -eq 0 ]; then
			true
		else
			false
		fi
    fi
}

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

#############################################################################
#                                                                           #
#                                 GCC COMPILER (SL6)                        #
#                                                                           #
#############################################################################

# Check for GCC
if [ $SL6 == "y" ]; then
	echo ""
	echo ""
	echo "Loading GCC 8.2.0 environment"
	module load gcc/820
	if grep -Fxq "module load gcc/820" "${HOME}/.bashrc"
	then
		echo "gcc/820 module already present in the .bashrc file"
	else
		cat >> "${HOME}/.bashrc" <<EOF

# load GCC 8.2.0 environment
module load gcc/820
EOF
	fi

	#############################################################################
	#                                                                           #
	#                                 PYTHON (SL6)                              #
	#                                                                           #
	#############################################################################

	echo ""
	echo "Loading Python 3.5.1 environment"
	module load python/3.5
	if grep -Fxq "module load python/3.5" "${HOME}/.bashrc"
	then
		echo "python/3.5 module already present in the .bashrc file"
	else
		cat >> "${HOME}/.bashrc" <<EOF

# load Python 3.5.1 environment
module load python/3.5
EOF
	fi

	echo ""
	echo "Loading Python 2.7.11 environment"
	module load python/2.7
	if grep -Fxq "module load python/2.7" "${HOME}/.bashrc"
	then
		echo "python/2.7 module already present in the .bashrc file"
	else
		cat >> "${HOME}/.bashrc" <<EOF

# load Python 2.7.11 environment
module load python/2.7
EOF
	fi
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
		if grep -Fxq "# set g77 environment" "${HOME}/.bashrc"
		then
			echo "g77 environment settings already present in the .bashrc file"
		else
			tee -a $HOME/.bashrc << END

# set g77 environment
if [ -d "/usr/lib/x86_64-linux-gnu" ] ; then
   export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:\$LD_LIBRARY_PATH"
fi
END
		fi
	fi
fi

#install Geant4 if necessary
if [ -z "${G4INSTALL}" ] ;
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
		cd geant4-${GEANTVERS}-build
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
		if grep -Fxq "# set PATH to include Geant4" "${HOME}/.bashrc"
		then
			echo "Geant 4 source command already present in the .bashrc file"
		else
			cat >> "${HOME}/.bashrc" <<EOF

# set PATH to include Geant4
if [ -f "/usr/local/geant4/share/Geant4-10.5.0/geant4make/geant4make.sh" ] ; then
   source /usr/local/geant4/share/Geant4-10.5.0/geant4make/geant4make.sh
fi
EOF
		fi
		cd && rm -rf geant4-${GEANTVERS}-build
	elif [ $SL6 == "y" ];
	then


		#############################################################################
		#                                                                           #
		#                                 CMAKE 3 (SL6)                             #
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
			if grep -Fxq "# set CMAKE environment" "${HOME}/.bashrc"
			then
				echo "Cmake PATH export already present in the .bashrc file"
			else
				cat >> "${HOME}/.bashrc" <<EOF

# set CMAKE environment
if [ -d "${HOME}/cmake-${CMAKEVERS}/bin" ] ; then
   export PATH=${HOME}/cmake-${CMAKEVERS}/bin:\$PATH
fi
EOF
			fi
			cd && rm -rf cmake-${CMAKEVERS}-sources cmake-${CMAKEVERS}.tar.gz
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
		${CMAKE} -DCMAKE_C_COMPILER=$(which gcc) \
				 -DCMAKE_CXX_COMPILER=$(which g++) \
				 -DCMAKE_INSTALL_PREFIX=../geant4-${GEANTVERS} \
				 -DGEANT4_INSTALL_DATA=ON \
				 -DGEANT4_BUILD_MULTITHREADED=ON \
				 -DGEANT4_USE_OPENGL_X11=ON \
				 -DGEANT4_USE_QT=ON \
				 -DGEANT4_USE_SYSTEM_EXPAT=ON \
				 ../geant4-sources
		make -j56
		make install
		source ${HOME}/geant4-${GEANTVERS}/share/Geant4-${GEANTVERS}/geant4make/geant4make.sh
		export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/geant4-${GEANTVERS}/lib64
		if grep -Fxq "# set Geant4 environment" "${HOME}/.bashrc"
		then
			echo "Geant4 environment settings already present in the .bashrc file"
		else
			cat >> "${HOME}/.bashrc" <<EOF

# set Geant4 environment
if [ -f "${HOME}/geant4-${GEANTVERS}/share/Geant4-10.5.0/geant4make/geant4make.sh" ] ; then

		source ${HOME}/geant4-${GEANTVERS}/share/Geant4-10.5.0/geant4make/geant4make.sh
		export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:${HOME}/geant4-${GEANTVERS}/lib64
fi
EOF
		fi
		cd && rm -rf geant4-${GEANTVERS}-build geant4-sources
	fi
fi

#install root if necessary
if [ -z "${ROOTSYS}" ] ;
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
		${CMAKE} \
			-Dbuiltin_xrootd=ON \
            -DCMAKE_INSTALL_PREFIX=${ROOTDIR}/${ROOTVERS} \
			../sources
		${CMAKE} --build . --target install -- -j8
		cd
		source ${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh
		if grep -Fxq "# set ROOT environment" "${HOME}/.bashrc"
		then
			echo "ROOT environment settings already present in the .bashrc file"
		else
			cat >> "${HOME}/.bashrc" <<EOF

# set ROOT environment
if [ -f "${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh" ] ; then
   source ${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh
fi
EOF
		fi
		rm -rf ${ROOTDIR}/${ROOTVERS}-build
	elif [ $SL6 == "y" ];
	then


		#############################################################################
		#                                                                           #
		#                                 ROOT (SL6)                                #
		#                                                                           #
		#############################################################################
		
		echo $LD_LIBRARY_PATH
		# Download and install ROOT
		ROOTDIR="${HOME}/ROOT"
		mkdir -p ${ROOTDIR}/{sources,${ROOTVERS},${ROOTVERS}-build}
		cd ${ROOTDIR}
		git clone http://github.com/root-project/root.git sources
		cd sources
		git checkout -b v${ROOTVERS} v${ROOTVERS}
		cd ../${ROOTVERS}-build
		${CMAKE} \
  			-DCMAKE_C_COMPILER=$(which gcc) \
			-DCMAKE_CXX_COMPILER=$(which g++) \
			-Dbuiltin_xrootd=ON -Dsqlite=OFF \
			-DCMAKE_INSTALL_PREFIX=${ROOTDIR}/${ROOTVERS} \
			-DPYTHON_EXECUTABLE=$(which python3) \
			-Dfortran=ON \
			../sources
		${CMAKE} \
			--build . --target install -- -j56
		cd ..
		rm -rf ${ROOTVERS}-build
		cd
		source ${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh
		if grep -Fxq "# set ROOT environment" "${HOME}/.bashrc"
		then
			echo "ROOT environment settings already present in the .bashrc file"
		else
			cat >> "${HOME}/.bashrc" <<EOF

# set ROOT environment
if [ -f "${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh" ] ; then
   source ${ROOTDIR}/${ROOTVERS}/bin/thisroot.sh
fi
EOF
		fi
	fi
fi

#############################################################################
#                                                                           #
#                               CERNLIB 2006                                #
#                                                                           #
#############################################################################

if [ $SL6 == "y" ] ; then
	if [ -z "${CERN}" ] ; then
		export CERN=/home/nu/giorgio/cernlib
		export CERN_LEVEL=2006
		export CERN_ROOT=${CERN}/${CERN_LEVEL}
		export PATH=${CERN_ROOT}/bin:${PATH}
		export LD_LIBRARY_PATH=${CERN_ROOT}/lib:${LD_LIBRARY_PATH}
		if grep -Fxq "# set CERNLIB environment" "${HOME}/.bashrc"
		then
			echo "CERNLIB environment settings already present in the .bashrc file"
		else
			cat >> "${HOME}/.bashrc" <<EOF

# set CERNLIB environment
if [ -d "/home/nu/giorgio/cernlib" ] ; then
	export CERN=/home/nu/giorgio/cernlib
	export CERN_LEVEL=2006
	export CERN_ROOT=\${CERN}/\${CERN_LEVEL}
	export PATH=\${CERN_ROOT}/bin:\${PATH}
	export LD_LIBRARY_PATH=\${CERN_ROOT}/lib:\${LD_LIBRARY_PATH}
fi
EOF
		fi
	fi
elif [ $UBUNTU == "y" ] ; then
	if ! isinstalled cernlib ; then
		# CERNLIB
		cd
		wget http://www-zeuthen.desy.de/linear_collider/cernlib/new/cernlib-20061220+dfsg3.patches.2019.02.03.txt
		sudo apt install -y devscripts
		rm -rf cernlib_debuild
		mkdir cernlib_debuild
		cd cernlib_debuild
		wget http://mirrors.kernel.org/ubuntu/pool/main/libx/libxp/libxp-dev_1.0.2-1ubuntu1_amd64.deb
		sudo apt-get install -y ./libxp-dev_1.0.2-1ubuntu1_amd64.deb
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

# ROOT detection
if [ -z ${ROOTSYS} ]; then
	echo "Couldn't detect ROOT installation."
	echo "Perhaps you forgot to source the thisroot.sh script."
	exit
else
	source ${ROOTSYS}/bin/thisroot.sh
fi

# GEANT4 detection
if [ -z ${G4INSTALL} ]; then
	echo "Couldn't detect Geant4 installation."
	echo "Perhaps you forgot to source the geant4make.sh script."
	exit
fi

if [ $SL6 == "y" ] ; then
	cd
	cd WAGASCI-BabyMIND-MC
	if [ ! -f "./lib/libg2c.so" ] ; then
		ln -s /usr/lib64/libg2c.so.0 ./lib/libg2c.so
	fi
	make
	./B2MC --help

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
