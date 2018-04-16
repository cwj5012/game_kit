### use_benchmark

C/C++ -> 附加包含目录 $(SolutionDir)include;

链接器 -> 常规 -> 附加库目录 $(SolutionDir)lib;

链接器 -> 常规 -> 输入 -> 附加依赖项 shlwapi.lib;benchmark.lib;

### use_mysql

![mysql connector c++ 官网](https://dev.mysql.com/downloads/connector/cpp/)

下载源码，用于编译

Source Code

Windows (Architecture Independent), ZIP Archive	1.1.9	0.7M	
Download
(mysql-connector-c++-1.1.9.zip)	MD5: 9fed4494fce96a9129c6130e52fe2768 | Signature

下载预编译文件，包含 include 头文件

Microsoft Windows

Windows (x86, 64-bit), ZIP Archive	1.1.9	10.4M	
Download
(mysql-connector-c++-noinstall-1.1.9-winx64.zip)	MD5: fef0361a95f5e87a298eb7e810c87bed | Signature

编译

$ cd C:\Users\cwj\Desktop\github\mysql-connector-c++-1.1.9\out
$ "C:\Users\cwj\AppData\Local\cmake-3.11.0-win64-x64\bin\cmake"  -G "Visual Studio 15 Win64" .. -DBOOST_ROOT:STRING="C:\\boost_1_66_0"

### use_glog

C/C++ -> 附加包含目录 $(SolutionDir)include;

链接器 -> 常规 -> 附加库目录 $(SolutionDir)lib;

链接器 -> 常规 -> 输入 -> 附加依赖项 glogd.lib;

C/C++ -> 预处理器 -> 预处理器定义 GOOGLE_GLOG_DLL_DECL=

### use_libuv

C/C++ -> 附加包含目录 $(SolutionDir)include;

链接器 -> 常规 -> 附加库目录 $(SolutionDir)lib;

链接器 -> 常规 -> 输入 -> 附加依赖项 ws2_32.lib;IPHLPAPI.lib;Psapi.lib;Userenv.lib;libuv.lib;

链接器 -> 常规 -> 输入 -> 忽略特定默认库 LIBCMTD.lib

### use_protobuf

编译源代码，版本 3.5.1

$ protoc --version
libprotoc 3.5.1

C/C++ -> 附加包含目录 $(SolutionDir)include;

C/C++ -> 常规 -> SDL检查  否(/sdl-)

链接器 -> 常规 -> 附加库目录 $(SolutionDir)lib;

链接器 -> 常规 -> 输入 -> 附加依赖项 libprotobufd.lib;


### use_mongodb

cmake  -G "Visual Studio 15 Win64" -DCMAKE_BUILD_TYPE=Debug .. 
	-DLIBBSON_DIR=C:\Users\cwj\Desktop\github\libbson\out\Debug


cmake -G "Visual Studio 15 Win64" -DCMAKE_BUILD_TYPE=Debug ..
    -DCMAKE_INSTALL_PREFIX=C:\mongo-cxx-driver
    -DCMAKE_PREFIX_PATH=c:\mongo-c-driver
    -DBOOST_ROOT=c:\boost_1_67_0

cmake -G "Visual Studio 15 2017 Win64" 
  "-DENABLE_SSL=WINDOWS" 
  "-DENABLE_SASL=SSPI" 
  "-DCMAKE_INSTALL_PREFIX=C:\mongo-c-driver" 
  "-DCMAKE_PREFIX_PATH=C:\mongo-c-driver" 
  "-DCMAKE_BUILD_TYPE=Debug"
  "-DBOOST_ROOT=c:\boost_1_67_0"
  "-Dlibmongoc-1.0_DIR=c:\boost_1_67_0"

#### libmongoc

cmake -G "Visual Studio 15 2017 Win64" .. "-DBOOST_ROOT=c:\boost_1_67_0" "-DCMAKE_INSTALL_PREFIX=C:\mongo-c-driver"

cmake -G "Visual Studio 15 2017 Win64" .. 
  "-DCMAKE_INSTALL_PREFIX=C:\mongo-c-driver" 
  "-DCMAKE_PREFIX_PATH=C:\mongo-c-driver" 
  "-DCMAKE_BUILD_TYPE=Debug"
  "-DBOOST_ROOT=c:\boost_1_67_0"