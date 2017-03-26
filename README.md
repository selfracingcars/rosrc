apt-get install arduino-core arduino-mk ros-kinetic-rosserial-arduino


export WS=~/catkin_ws

cd $WS
. ~/catkin_ws/devel/setup.bash

cd $WS/src
git clone https://github.com/ros-drivers/rosserial.git
cd $WS
catkin_make
cd $WS/rosrc
rosrun rosserial_arduino make_libraries.py .

sudo cp udev/99-rc.rules /etc/udev/rules.d

rosrun rosserial_python serial_node.py /dev/rc0
