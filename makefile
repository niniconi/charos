header=charos.h cat/cat.h
source=winmgr.c util.c cat/cat.c cmd-win.c cmd-charos.c charos.c event.c listenMouse.c listenKey.c
obj=winmgr.o util.o cat.o cmd-win.o cmd-charos.o charos.o event.o listenMouse.o listenKey.o
clean:
	rm $(obj) -f
	rm *.out -f
	rm ./charos -f
obj:$(source) $(header)
	gcc event.c -c -o event.o
	gcc listenKey.c -c -o listenKey.o
	gcc listenMouse.c -c -o listenMouse.o
	gcc winmgr.c -c -o winmgr.o
	gcc cat/cat.c -c -o cat.o
	gcc charos.c -c -o charos.o
	gcc util.c -c -o util.o
	gcc cmd-win.c -c -o cmd-win.o
	gcc cmd-charos.c -c -o cmd-charos.o
build:obj
	gcc $(obj) -o charos -lm
run:
	$(MAKE) clean
	$(MAKE) build
	./charos
