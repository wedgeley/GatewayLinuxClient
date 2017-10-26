# ====================================================
#
# 	Make file for libGatewayClient
#
# 	Usage:
#
#		Debug build:
#			make
#
#		Release build:
#			make target=Release
#
#
# ===================================================

# Compiler flags
# if target variable is empty, setting debug build mode
ifeq ($(target),Release)
   CFLAGS = -O2 -Wall -Iinclude
else
   target = Debug
   CFLAGS = -Wall -g -Iinclude
endif

# All object files in the project
objects = \
	obj/$(target)/aes/Rijndael.o \
	obj/$(target)/gway_api.o \
	obj/$(target)/gway_errors.o \
	obj/$(target)/json/jsoncpp.o \
	obj/$(target)/src/Encryptor.o \
	obj/$(target)/src/GatewayClient.o \
	obj/$(target)/src/JsonConverter.o \
	obj/$(target)/src/MessageBuffer.o

# All directories we need to create
directories = \
	bin/$(target) \
	obj/$(target)/aes \
	obj/$(target)/json \
	obj/$(target)/src

# Create directories
$(shell mkdir -p $(directories))

# Main target
All : bin/$(target)/libGatewayClient.a

# Link
bin/$(target)/libGatewayClient.a : $(objects)
	ar -r -s $@ $^

# Compilation
obj/$(target)/aes/%.o : aes/%.cpp
	g++ $(CFLAGS) -c $^ -o $@

obj/$(target)/%.o : %.cpp
	g++ $(CFLAGS) -c $^ -o $@

obj/$(target)/json/%.o : json/%.cpp
	g++ $(CFLAGS) -c $^ -o $@

obj/$(target)/src/%.o : src/%.cpp
	g++ $(CFLAGS) -c $^ -o $@

# Header file dependency
obj/$(target)/aes/Rijndael.o : aes/Rijndael.h
obj/$(target)/gway_api.o : include/gway_errors.h
obj/$(target)/json/jsoncpp.o : json/json.h json/json-forwards.h
obj/$(target)/src/Encryptor.o : include/Encryptor.h aes/Rijndael.h base64/base64.h include/gway_errors.h
obj/$(target)/src/GatewayClient.o : include/GatewayClient.h include/JsonConverter.h json/json.h include/MessageBuffer.h include/gway_errors.h
obj/$(target)/src/JsonConverter.o : include/JsonConverter.h json/json.h include/gway_errors.h
obj/$(target)/src/MessageBuffer.o : include/MessageBuffer.h json/json.h include/gway_errors.h


