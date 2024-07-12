obj-m += moduls.o

all:
	make -C /lib/modules/6.8.0-36-generic M=$(PWD) modules

clean:
	make -C /lib/modules/6.8.0-36-generic M=$(PWD) clean
