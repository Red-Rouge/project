# 检测是否存在cmake指令，如果存在就输出存在如果不存在就安装

# 执行cmake --version，并将显示出的第一行赋值给变量cmake_version
cmake_version=$(cmake --version | head -n 1)
if [ $? -eq 0 ];then
    echo "cmake已安装"
else
    echo "正在安装cmake"
    sudo apt-get install cmake
    cmake_version=$(cmake --version | head -n 1)
fi
# 去除$cmake_version的钱6个字符并将小写变大写
cmake_version=${cmake_version:6}
cmake_version=${cmake_version^^}

# 删除CMakeLists.txt的第一行，并将cmake_version添加到第一行


# 删除CMakeLists.txt的第一行
sed -i "1d" CMakeLists.txt
sed -i "1s/^/cmake_minimum_required(${cmake_version})\n/" CMakeLists.txt


# 删除build下的所有文件
rm -r build
# 新建build文件夹
mkdir build
# 进入到build里
cd build
# 执行cmake ..
cmake ..

