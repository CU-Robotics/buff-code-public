
if [[ "${PWD}" != */buffbotics ]]; then
	echo -e "running from ${PWD}, is this your project root?"
	exit 0
fi
#		Setup robot params
export PROJECT_ROOT=${PWD}
export USER_IP='192.168.0.9'
export ROBOT_IP='192.168.0.13'
export ROBOT_ADDRESS="cu-robotics@${ROBOT_IP}"
export ROBOT_ROOT="/home/cu-robotics/buffvision"

#		Setup python tools
export PATH="${PROJECT_DIR}/buffpy:${PATH}"
export PYTHONPATH="${PROJECT_DIR}/buffpy/bin:${PYTHONPATH}"
