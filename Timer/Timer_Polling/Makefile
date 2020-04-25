HDRS+= jetsonnano.h jetsonnano_gpio.h jetsonnano_timer.h
CSRCS+= jetsonnano_gpio.cpp jetsonnano_timer.cpp timer_polling.cpp

all:	timer_polling

mem:	$(CSRCS)	
	g++ -g -O1 -o timer_polling $(CSRCS) -Wall -std=gnu++17

clean:
	rm -f timer_polling timer_polling.o core

