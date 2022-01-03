#
# this dockerfile roughly follows the 'Ubuntu install of ROS Melodic' from:
#   http://wiki.ros.org/melodic/Installation/Ubuntu
#

#   Select base image here (ARM64 or AMD64)
ARG BASE_IMAGE=mdsdev0/buffbox:x86_64-ubuntu-base
FROM ${BASE_IMAGE}

ENV ROS_PKG=desktop
ENV ROS_DISTRO=melodic
ENV ROS_ROOT=/opt/ros/${ROS_DISTRO}

ENV PROJECT_ROOT=/home/cu-robotics/buff-code

ENV DEBIAN_FRONTEND=noninteractive

#
# Create a new user
#
RUN useradd -ms /bin/bash cu-robotics


#
# Add your code
#
ADD . /home/cu-robotics/buff-code



#
#	Clean APT/Cache
#
RUN apt-get update && \
	apt-get autoremove --purge -y && \
	apt-get clean -y


#
# Set up the environment
# not sure how to utilize the project_root variable
RUN echo "source /home/cu-robotics/buff-code/buff.bash" >> /root/.bashrc
CMD ["bash"]
ENTRYPOINT ["/home/cu-robotics/buff-code/scripts/buff_entrypoint.sh"]
WORKDIR /home/cu-robotics/buff-code
