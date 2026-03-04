# SerialTool

一个基于Qt5开发的跨平台串口通信工具。

## 功能特性

- **串口连接管理** - 打开、关闭串口连接
- **数据收发** - 支持实时数据接收和发送
- **数据显示** - 接收到的数据实时显示在界面上
- **数据清空** - 快速清空接收缓冲区
- **跨平台支持** - 支持Windows和Linux平台

## 系统要求

- **操作系统**: Windows 或 Linux
- **Qt版本**: Qt 5.x 及以上
- **编译器**: 支持C++11的编译器（如MSVC、GCC）
- **CMake**: 3.5 或以上

## 前置依赖

### 必需组件
- Qt5 Widgets 
- Qt5 SerialPort

### 安装依赖（Windows示例）

通过Qt安装程序安装以上组件，或使用包管理器（如vcpkg）：
```bash
vcpkg install qt5:x64-windows
```

## 构建和编译

### 使用CMAKE构建

```bash
# 创建build目录
mkdir build
cd build

# 运行CMake配置
cmake ..

# 编译项目
cmake --build . --config Release
```

### 使用Qt Creator构建

1. 打开Qt Creator
2. File → Open → 选择CMakeLists.txt
3. 选择合适的编译工具链
4. Build → Build Project

## 使用说明

1. **打开程序** - 运行编译后的SerialTool可执行文件
2. **打开串口** - 点击"打开端口"按钮，选择要连接的串口
3. **发送数据** - 在输入框中输入数据，点击"发送数据"按钮
4. **接收数据** - 实时接收的数据会显示在接收区域
5. **关闭串口** - 点击"关闭端口"按钮断开连接
6. **清空数据** - 点击"清空"按钮清除接收显示区域

## 项目结构

```
SerialTool/
├── CMakeLists.txt          # CMake构建配置文件
├── main.cpp                # 程序入口
├── mainwindow.h            # 主窗口头文件
├── mainwindow.cpp          # 主窗口实现
├── mainwindow.ui           # 主窗口UI文件（Qt Designer设计）
└── README.md               # 项目说明文档
```

## 主要类和功能

### MainWindow类
- **setButtonsEnabled()** - 控制UI按钮的启用/禁用状态
- **on_readyReadSerialData()** - 处理串口接收数据事件
- **on_timerOut()** - 定时器超时处理
- **on_timerShowOut()** - 显示定时器超时处理
- **on_pushButtonOpenPort_clicked()** - 打开串口处理
- **on_pushButtonClosePort_clicked()** - 关闭串口处理
- **on_pushButtonSendData_clicked()** - 发送数据处理
- **on_pushButtonAbout_clicked()** - 关于界面处理
- **on_pushButtonClear_clicked()** - 清空数据处理

## 技术细节

- **串口通信**: 使用Qt5提供的QSerialPort类
- **定时器**: 采用QTimer实现数据接收和显示的定时处理
- **缓冲区**: 使用QByteArray管理接收数据
- **UI框架**: 基于QMainWindow和Qt Designer

## 许可证

本项目为个人项目。

## 联系方式

如有问题或建议，欢迎反馈。
