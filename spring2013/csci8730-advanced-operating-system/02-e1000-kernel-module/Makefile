obj-m += netdev.o

install: all
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules_install

all:
	make E1000_DEBUG=TXERR,TX -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
