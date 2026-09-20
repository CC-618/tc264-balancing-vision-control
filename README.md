# TC264 平衡与视觉控制

基于英飞凌 AURIX TC264D 的两轮平衡车固件，结合双核控制任务、惯性传感、PID 闭环、电机反馈、摄像头循迹和无线 TCP 通信。

## 主要功能

- 两轮平衡与运动控制
- IMU 采样和姿态解算
- 串级 PID 控制与编码器反馈
- 基于摄像头的赛道检测与循迹
- 基于英飞凌 iLLD 的电机驱动和外设控制
- 用于遥测或远程控制的 Wi-Fi/TCP 通信

## 目录结构

- `4.12/Seekfree_TC264_Opensource_Library/code/`：平衡、运动、摄像头、滤波和初始化等应用代码
- `4.12/Seekfree_TC264_Opensource_Library/libraries/`：英飞凌 iLLD 与逐飞科技支持库
- `4.12/Seekfree_TC264_Opensource_Library/Debug/`：调试和构建相关源码
- `.project`、`.cproject`：Eclipse / AURIX Development Studio 工程配置

## 编译与下载

使用 AURIX Development Studio 打开 `4.12/Seekfree_TC264_Opensource_Library/`，选择对应的 TC264D 目标配置，再使用兼容调试器编译并下载程序。

## 配置注意事项

源码包含与原始开发板相关的引脚映射、控制参数和网络配置。在其他网络中使用通信功能前，请检查 Wi-Fi 名称、密码、目标 IP 和端口。

仓库中包含的英飞凌和逐飞科技库仍适用其各自的许可和署名要求。
