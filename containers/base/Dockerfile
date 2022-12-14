#
# this dockerfile roughly follows the 'Ubuntu install of ROS Melodic' from:
#   http://wiki.ros.org/melodic/Installation/Ubuntu
#

# change this to select ARM or x86_64
# nvcr.io/nvidia/l4t-base:r32.6.1 # ubuntu:18.04
FROM nvcr.io/nvidia/l4t-base:r32.6.1

ENV ROS_PKG=ros-base
ENV ROS_DISTRO=melodic
ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_ROOT=/opt/ros/${ROS_DISTRO}
ENV PROJECT_ROOT=/home/cu-robotics/buff-code
ENV PATH=/home/cu-robotics/.local/bin:${PATH}
ENV PYTHONPATH=/home/cu-robotics/.local/lib/python3.6/dist-packages:${PYTHONPATH}


#
# Create a new user
#
RUN useradd -ms /bin/bash cu-robotics


#
# Install Dependencies
#

RUN apt update && \
	apt upgrade -y && \
	apt install -y --no-install-recommends \
		htop=2.1.0-3 \
		tmux=2.6-3ubuntu0.2 \
		wget=1.19.4-1ubuntu2.2 \ 
		gnupg2=2.2.4-1ubuntu1.4 \
		curl=7.58.0-2ubuntu3.18 \
		git=1:2.17.1-1ubuntu0.11 \
		python3-dev=3.6.7-1~18.04 \
		vim=2:8.0.1453-1ubuntu1.8 \
		build-essential=12.4ubuntu1 \
		lsb-release=9.20170808ubuntu1 \ 
		iputils-ping=3:20161105-1ubuntu3 \
		ca-certificates=20210119~18.04.2 \ 
		openssh-client=1:7.6p1-4ubuntu0.7 \
		openssh-server=1:7.6p1-4ubuntu0.7


#
#	Clean apt
#

RUN apt update && \
	apt clean && \
	apt autoremove -y


# 
# Install ROS 
#   see ros install script for more info
#	Also installs our python reqs


 RUN sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list' && \
	curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add - && \
	apt update && \
	apt install -y --no-install-recommends ros-${ROS_DISTRO}-${ROS_PKG} python-rosdep \
	ros-${ROS_DISTRO}-rqt ros-${ROS_DISTRO}-rqt-common-plugins  ros-${ROS_DISTRO}-rqt-robot-plugins && \
	curl https://bootstrap.pypa.io/pip/3.6/get-pip.py -o get-pip.py && \
	python3 get-pip.py && \
	rm get-pip.py

RUN pip3 install \
		pyyaml==6.0 \
		torch==1.9.0 \
		tqdm==4.62.3 \
		numpy==1.19.5 \
		pandas==1.1.5 \
		pillow==8.4.0 \
		pyserial==3.5 \
		pydrive==1.3.1 \
		seaborn==0.11.2 \
		cython==0.29.26 \
		protobuf==3.12.4 \
		requests==2.27.1 \
		depthai==2.15.2.0 \
		platformio==6.0.1 \
		torchvision==0.10.0 \
		opencv-python==4.5.5.64 \
		rospkg==1.4.0 \
		wstool==0.1.17 \
		rosinstall==0.7.8 \
		defusedxml==0.7.1 \
		catkin-tools==0.8.2 \
		rosinstall-generator==0.1.22


RUN cd ${ROS_ROOT} && \
	rosdep init && \
	rosdep update


#
#	Clean apt
#

RUN apt update && \
	apt clean && \
	apt autoremove -y


#
# 	Setup catkin and cv_bridge
# so blocky eww

RUN cd /home/cu-robotics && mkdir opencv_ws && cd opencv_ws && \
	git clone -b noetic https://github.com/ros-perception/vision_opencv.git src/vision_opencv && \
	cd src/vision_opencv && sed -i 's/python37/python3/g' cv_bridge/CMakeLists.txt && cd ../.. && \
	bash /opt/ros/melodic/setup.sh && catkin init && \
	catkin config -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_INCLUDE_DIR=/usr/include/python3.6m -DPYTHON_LIBRARY=/usr/lib/$(uname -m)-linux-gnu/libpython3.6m.so --install --extend /opt/ros/melodic && \
	catkin build cv_bridge && \
	rm -rf /opt/ros/melodic/lib/python2.7/dist-packages/cv_bridge* && \
	cp -r /home/cu-robotics/opencv_ws/install/lib/python3/dist-packages/* /opt/ros/melodic/lib/python2.7/dist-packages/ && \
	rm -rf /home/cu-robotics/opencv_ws


#
#	Clean apt
#

RUN apt update && \
	apt clean && \
	apt autoremove -y


# 
# setup entrypoint
#

CMD ["bash"]
WORKDIR /home/cu-robotics/buff-code
