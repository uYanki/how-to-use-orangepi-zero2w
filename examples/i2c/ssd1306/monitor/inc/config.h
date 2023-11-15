#ifndef __CONFIG_H__
#define __CONFIG_H__

/* 无线网卡文件名 */
#define WLAN_IF        "wlan0"
/* 有线网卡文件名 */
#define ETH_IF         "eth0"
/* i2c设备文件名 */
#define LINUX_IIC_FILE "/dev/i2c-2"
/* 刷新时间(s) */
#define REFRESH_TIME   1

#define WIRELESS       "/sys/class/net/wlan0/operstate"
#define ETHERNET       "/sys/class/net/eth0/operstate"

#endif