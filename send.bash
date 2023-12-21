
# 判断$s1是否输入
if [ -z "$1" ]; then
    # 输出用法
    echo "用法：$0 s1 s2"
    exit 0
fi
# 判断$s1路径的文件是否不存在
if [ ! -f "$1" ]; then
    echo "文件 $1 不存在"
    exit 0
fi

if [ -z "$2" ]; then
    echo "未输入目标参数"
    exit 0
fi


# 用ssh发送s1的文件到root@192.168.11.56:s2处
scp $1 root@192.168.11.56:$2
