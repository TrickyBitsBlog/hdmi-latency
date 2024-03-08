# hdmi-latency
HDMI latency tester using a Raspberry PI 3, from TrickyBits blog post https://trickybits.blog/2024/03/07/hdmi-latency-tester.html. 

Please do with this code whatever you wish, its as public as it gets.


# Building

To build this project you need the `aarch64-none-elf` tools, these can be obtained from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

Depending on if your GNU build tools are pathed or not, you'll have to change the first line of the Makefile:

```make
ARMGNU ?= /Applications/ArmGNUToolchain/13.2.Rel1/aarch64-none-elf/bin/aarch64-none-elf
```

Thats the path I use to build on OSX. You're machine will probably be different.

Once the project builds you'll get kernel8.img in the root folder which is a runnable kernel on the pi. There is a prebuilt kernel in the repo for convenience, 
copy this kernel your SD card to boot along with the following config file:


```
#uncomment this section to output hdmi
hdmi_force_hotplug=1
hdmi_group=1    #1=CEA, 2=DMT 
hdmi_mode=16 

# uncomment this section for composite output: 0=ntsc, 2=pal
#sdtv_mode=2

# Run in 64-bit mode
arm_64bit=1

enable_uart=1
```

