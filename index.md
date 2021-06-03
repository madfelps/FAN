

## Welcome to GitHub Pages

It’s important to highlight that all the processes described by this document must run on Linux, since some libraries used in the embedded software are exclusively for this operating system. Also, Ubuntu 16.04 is the reference for the commands related to the operating system.

### Compilation Process 

The embedded software doesn’t run on x86 processor architecture, since the BeagleBone Black device contains an ARM A8 processor. So, you must compile the source code using the following tutorial.

On shell, type: 

<code>sudo apt-get install gcc-arm-none-eabi binutils-arm-none-eabi gdb-arm-none-eabi openocd</code>

Great. Now, you are able to compile source codes on a x86 architecture into ARM executable files. 

For compiling this project, go to the project directory and type:

 <code>make</code>

As you can see, the generated executable is called “main_bbb”. 

### CAN Configuration

First, we will instal CAN-utils, a set of utilities that enable Linux to communicate with the CAN network. 

<pre><code>
  $ git clone https://github.com/linux-can/can-utils.git
  $ cd can-utils
  $ ./autogen.sh
  $ ./configure
  $ sudo make
  $ sudo make install 
</code></pre>

All the process below is for enabling CAN1 (pins P9.24 and P9.26). However, if you want to enable CAN0, just enable pins P9.19 and P9.20.

<pre><code>
$ sudo config-pin p9.24 can
$ sudo config-pin p9.26 can
</code></pre>

Setting CAN bitrate: 

<pre><code>
 sudo ip link set can1 up type can bitrate 250000
</code></pre>
  
### Setting torque limit parameter


In the beginning of the main code, set the define directive value for the desired value. 


<pre><code>
  #define TORQUE_LIMIT_PARAMETER 800
</code></pre>
  
  
### Running the software

The executable generated from make is called main_bbb. Just run this and the console will show messages if it’s all right. 

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/madfelps/FAN/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.
