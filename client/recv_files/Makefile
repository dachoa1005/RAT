all: 
	mkdir -p build
	g++ source/*.cpp ../file_manager/file_manager.cpp -o build/server

run: 
	mkdir -p build
	g++ source/*.cpp ../file_manager/file_manager.cpp -o build/server
	./build/server

clean:
	rm -rf build
	rm -rf recv_files/*
	rm -rf *.txt
