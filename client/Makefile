all: 
	mkdir -p build
	g++ source/*.cpp ../file_manager/file_manager.cpp -o build/client

run: 
	mkdir -p build
	g++ source/*.cpp ../file_manager/file_manager.cpp -o build/client
	./build/client
	
clean:
	rm -rf build
	rm -rf recv_files/*
	rm -rf *.txt

