all : asd

asd : asd.cc
	g++ -D _SUNOS asd.cc /home/common/dyd/aisdi/liniowe/timer.cc /home/common/dyd/aisdi/liniowe/main.cc /home/common/dyd/aisdi/tests/ltest_liniowe.so -o asd 
	
del :
	rm asd
debug : asd.cc
	g++ -g -D _SUNOS asd.cc /home/common/dyd/aisdi/liniowe/timer.cc /home/common/dyd/aisdi/liniowe/main.cc /home/common/dyd/aisdi/tests/ltest_liniowe.so -o asd_debug
	gdb asd_debug 
	

view:
	lynx /home/common/dyd/aisdi/liniowe/info/index.html
