# tuya-zigbee-sdk开发指南
# **1. Clone**
克隆仓库中的zigbee master分支到本地的文件夹下;
```
git clone https://github.com/TuyaInc/tuya_zigbee_sdk.git
```
# **2. 创建新工程**
进入silicon_labs_zigbee/app，进入对应的产品品类下的project路径，复制一份demo工程， 命名为项目对应的文件名称；比如需要创建一个light的工程：
```
#cd silicon_labs_zigbee/app/light/project/
#cp -r demo/ color_light/
#cd color_light/
```
# **3. 编译**
## **3.1 修改所有文件权限**
silicon_labs_zigbee主目录下运行：
```
chmod -R 777 *
```
## **3.2 编译**
在对应的工程文件夹下运行如下命令即可;编译生成的可执行文件在对应的工程目录的build路径下；
```
#./run.sh clean   //make clean
#./run.sh build   //make all with release
#./run.sh build 0 //make all with release
#./run.sh build 1 //make all with debug
```
## **4. 工程应用框架介绍**
工程应用框架如下所示， SDK开发文档参考doc下的《涂鸦ZigBee SDK开发入门文档.docx》
```
app
├── light
│   ├── common
│   │   ├── include
│   │   └── src
│   └── project
│       ├── demo
├── sensor
│   ├── common
│   │   ├── include
│   │   └── src
│   └── project
│       ├── demo
├── smart_plug
│   ├── common
│   │   ├── include
│   │   └── src
│   └── project
│       ├── demo
└── switch
    ├── common
    │   ├── include
    │   └── src
    └── project
        ├── demo
      
```
