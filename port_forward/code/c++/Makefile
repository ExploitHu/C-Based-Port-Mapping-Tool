
obeject=rsJson.o error.o config.o log.o obDb.o table.o tbUserInfo.o toolFun.o uiFunc.o uiFuncSys.o main.o 
obejectFile=obeject
libFolder=/port_forward/code/node/lib/


#g++ -std=c++11 -g -o main  *.cpp `mysql_config --cflags --libs` -llog4cpp -lpthread 
main : $(obeject)
	mkdir -p $(obejectFile)
	g++ -std=c++11 -g -o main  $(obejectFile)/*.o  `mysql_config --cflags --libs` -llog4cpp -lpthread -ljansson -g

error.o :
	g++ -std=c++11 -g -fPIC  -c error.cpp  -o $(obejectFile)/error.o 

config.o :
	g++ -std=c++11 -g -fPIC  -c config.cpp -o $(obejectFile)/config.o

log.o :
	g++ -std=c++11 -g -fPIC -c log.cpp  -o $(obejectFile)/log.o

obDb.o :
	g++ -std=c++11 -g -fPIC -c obDb.cpp  -o $(obejectFile)/obDb.o

table.o :
	g++ -std=c++11 -g -fPIC -c table.cpp -o $(obejectFile)/table.o

tbUserInfo.o :
	g++ -std=c++11 -g -fPIC -c tbUserInfo.cpp -o $(obejectFile)/tbUserInfo.o

toolFun.o :
	g++ -std=c++11 -g -fPIC -c toolFun.cpp -o $(obejectFile)/toolFun.o

main.o : 
	g++ -std=c++11 -g -fPIC -c main.cpp  -o $(obejectFile)/main.o

uiFunc.o :
	g++ -std=c++11 -g -fPIC -c uiFunc.cpp  -o $(obejectFile)/uiFunc.o 

uiFuncSys.o :
	g++ -std=c++11 -g -fPIC -c uiFuncSys.cpp  -o $(obejectFile)/uiFuncSys.o

rsJson.o :
	g++ -std=c++11 -g -fPIC -c rsJson.cpp  -o $(obejectFile)/rsJson.o

libui.so : $(obeject)
	g++ -std=c++11 -g $(obejectFile)/*.o `mysql_config --cflags --libs` -llog4cpp -lpthread -ljansson -fPIC -shared  -o $(obejectFile)/libui.so 
	cp $(obejectFile)/libui.so  $(libFolder)

clean :
	rm $(obejectFile)/*.o $(obejectFile)/*.so main