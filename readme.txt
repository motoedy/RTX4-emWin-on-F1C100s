亲，我为你花了些时间，增加了sunxi-fel直接加载到DDR运行image的功能。
你只要基于FELinside-f1c100s项目模板开发编译，接着使板子进入boot模式；
然后就可以双击output目录下的app-exec.bat，通过usb直接写进DDR运行了。
这样在开发过程中就可以避免烧写spiflash，又慢又损伤flash！

而原先的Blinky-f1c100s项目无此功能，我将作为开源版本公开，请悉知。