################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DrumInstruments.cpp \
../DrumPattern.cpp \
../MTrkEvent.cpp \
../MidiFileData.cpp \
../MidiFileHdr.cpp \
../MidiSection.cpp \
../main.cpp 

OBJS += \
./DrumInstruments.o \
./DrumPattern.o \
./MTrkEvent.o \
./MidiFileData.o \
./MidiFileHdr.o \
./MidiSection.o \
./main.o 

CPP_DEPS += \
./DrumInstruments.d \
./DrumPattern.d \
./MTrkEvent.d \
./MidiFileData.d \
./MidiFileHdr.d \
./MidiSection.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


