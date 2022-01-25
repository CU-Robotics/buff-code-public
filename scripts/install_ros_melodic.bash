#! /bin/bash

ROS_PKG=$2
ROS_DISTRO=$1 

echo -e "\n\tSetting up ROS ${ROS_DISTRO}\n"

# ROS installation
# add repositories
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'

#
# setup ROS keys
#
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -

sudo apt update

#
# Install ROS
#
sudo apt install -y ros-${ROS_DISTRO}-${ROS_PKG}

#
# Install ROS dependencies
#
python3 -m pip install -r ${PROJECT_ROOT}/config/install/ros_dependencies.txt


echo -e "\n\tFinishing ROS setup...\n"

#
# Init rosdep
#
sudo rosdep init
rosdep update

#
# Install cv2 bridge for python3
#
apt update && \
cd /home/cu-robotics && mkdir opencv_ws && cd opencv_ws && \
git clone https://github.com/ros-perception/vision_opencv.git src/vision_opencv && \
cd src/vision_opencv && git checkout 1.13.0 && cd ../.. && \
bash /opt/ros/melodic/setup.sh && \
catkin init && \
catkin config -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_INCLUDE_DIR=/usr/include/python3.6m -DPYTHON_LIBRARY=/usr/lib/$(uname -m)-linux-gnu/libpython3.6m.so --install --extend /opt/ros/melodic && \
catkin build cv_bridge && \
cp -r install/lib/python3/dist-packages/* /usr/lib/python3/dist-packages/ && \
cd /home/cu-robotics && rm -rf opencv_ws





