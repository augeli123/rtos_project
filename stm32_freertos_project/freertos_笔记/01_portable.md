1、freertos源码文件夹
   source-{
            include/
	    .c（软件源码）
	    portable/(软硬件桥梁)-{
	                           MemMang/(内存管理)
	    			   gcc/keil(RVDS)
	    }                      }
2、利用emblocks建立工程，并添加以上文件
3、找到FreeRTOSConfig.h文件并添加
4、{
       sys.c位带操作   
       delay.c
       usart.c
   }可以不用添加，后期自己实现
编译通过。

