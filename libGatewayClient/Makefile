objects = \
	obj/Release/aes/Rijndael.o \
	obj/Release/gway_api.o \
	obj/Release/gway_errors.o \
	obj/Release/json/jsoncpp.o \
	obj/Release/src/Encryptor.o \
	obj/Release/src/GatewayClient.o \
	obj/Release/src/JsonConverter.o \
	obj/Release/src/MessageBuffer.o

Release : bin/Release/libGatewayClient.a

Debug : bin/Debug/libGatewayClient.a

bin/Release/libGatewayClient.a : $(objects)
	ar -r -s $@ $^

obj/Release/aes/%.o : aes/%.cpp
	g++ -O2 -Wall -Iinclude -c $^ -o $@

obj/Release/%.o : %.cpp
	g++ -O2 -Wall -Iinclude -c $^ -o $@

obj/Release/json/%.o : json/%.cpp
	g++ -O2 -Wall -Iinclude -c $^ -o $@

obj/Release/src/%.o : src/%.cpp
	g++ -O2 -Wall -Iinclude -c $^ -o $@

