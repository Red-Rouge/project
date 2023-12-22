
if [ -z "$1" ]; then
    local_files="./bin/test"
else
    local_files="$1"
fi
echo "本地文件：$local_files"
# 判断$s1路径的文件是否不存在
if [ ! -f $local_files ]; then
    echo "文件 $local_files 不存在"
    exit 0
fi




if [ -z "$2" ]; then
    remote_files="/root/project"
else
    remote_files="$2"
fi
echo "远程文件夹：$remote_files"
# 用ssh发送s1的文件到root@192.168.11.56:s2处
sshpass -p "6633" scp ${local_files} root@192.168.11.56:$remote_files




