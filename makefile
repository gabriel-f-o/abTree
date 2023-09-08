#Defines a recursive wildcard function.
#Use this function like: $(call rwildcard, <path_list>, <pattern_list>)
#e.g. $(call rwildcard, ., *.c *.h) returns all .c and .h files recursively in the current folder
#When using call, $(1) is replaced by the first parameter, $(2) the second... 
#
#$(wildcard $(1:%=%/*)concatenates /* to the end of every string in the list.
#Then, using the recursive call $(call rwildcard,$d,$2), we enter into each sub-directory (if it is a file, the call will do nothing).

#If everything is a file in this folder, each $(call rwildcard,$d,$2) will do nothing, and $(filter $(subst *,%,$2), $d) will execute
#$(subst *,%,$2) substitutes every * for % in the second argument e.g. $(subst *,%,*.c *.h) -> %.c %.h, then $(filter $(subst *,%,$2), $d) will match $d if is matches the pattern 
#E.g. $(filter %.c %.h, lib.h)) -> lib.h ; $(filter %.c %.h, lib.o)) -> <nothing> 
rwildcard=$(foreach d, $(wildcard $(1:%=%/*)), $(call rwildcard,$d,$2) $(filter $(subst *,%,$2), $d))

#Define a function that removes duplicates from a string
define uniq =
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef

#####################################################
###############			INPUT		  ###############
#####################################################

#Define a build directory
BUILD_DIR=build

#Defines 
DEFINES := 

#Directory / files to ignore. Directories / files can overlap
CSRC_IGNORE_RECURSIVE_DIR := 
CSRC_IGNORE_DIR := 
CSRC_IGNORE_FILE := 

#Directory / files to compile. Directories / files can overlap
CSRC_RECURSIVE_DIR := ./
CSRC_DIR := 
CSRC_FILE :=

#Directories to include. Directories can overlap but will only be passed to compiler if it has at least one .h inside
INC_RECURSIVE_DIR :=
INC_DIR := ./

#Compiler flags
CFLAGS := -g3 -Wno-address

#Linker flags
LDFLAGS := -g3 -Wno-address

#####################################################
###############			MAGIC		  ###############
#####################################################

#Gets all .c files to ignore recursively in the current directory, only in the directories, or single files, then remove duplicates and remove useles spacing
CSRC_IGNORE := $(call rwildcard, $(CSRC_IGNORE_RECURSIVE_DIR:%/=%), *.c)
CSRC_IGNORE += $(foreach d, $(CSRC_IGNORE_DIR:%/=%), $(wildcard $(d)/*.c))
CSRC_IGNORE += $(CSRC_IGNORE_FILE)
CSRC_IGNORE := $(CSRC_IGNORE:./%=%)
CSRC_IGNORE := $(call uniq, $(CSRC_IGNORE))
CSRC_IGNORE := $(CSRC_IGNORE:%=%)

#Gets all .c files recursively in the current directory, only in the directories, or single files, then remove duplicates and remove useles spacing
CSRC := $(call rwildcard, $(CSRC_RECURSIVE_DIR:%/=%), *.c)
CSRC += $(foreach d, $(CSRC_DIR:%/=%), $(wildcard $(d)/*.c))
CSRC += $(CSRC_FILE)
CSRC := $(CSRC:./%=%)
CSRC := $(call uniq, $(CSRC))
CSRC := $(CSRC:%=%)
CSRC := $(filter-out $(CSRC_IGNORE), $(CSRC))

#Gets all .h files recursively in the current directory, only in the directories, or single files, then remove duplicates and remove useles spacing
INC := $(call rwildcard, $(INC_RECURSIVE_DIR:%/=%), *.h)
INC += $(foreach d, $(INC_DIR:%/=%), $(wildcard $(d)/*.h))
INC := $(INC:./%=%)
INC := $(call uniq, $(INC))
INC := $(INC:%=%)

#Gets all .h files recursively in the current directory, only in the directories, or single files, then remove duplicates and remove useles spacing
INC_DIRS := $(foreach f, $(INC), $(dir $(f)))
INC_DIRS += $(INC_DIR)
INC_DIRS := $(call uniq, $(INC_DIRS))
INC_DIRS := $(INC_DIRS:%=%)

#For all strings in the list, concatenate the build directory at the beginning and .o at the end e.g. lib.c o/lib2.c -> build/lib.c.o build/o/lib2.c.o 
#Keeping the .c is useful if there are lib.c and lib.cpp in the same folder
COBJ := $(subst ../, .._/, $(CSRC))
COBJ := $(COBJ:%=$(BUILD_DIR)/%.o)

#Get the list of dependencies by changing .o to .d e.g. build/lib.c.o build/o/lib2.c.o	-> build/lib.c.d build/o/lib2.c.d
#This is important for later
CDEP := $(COBJ:%.o=%.d)

#$(info INC = $(INC))
#$(info CSRC = $(CSRC))
#$(info CSRC_IGNORE = $(CSRC_IGNORE))
#$(info INC_DIRS = $(INC_DIRS))
#$(info COBJ = $(COBJ))
#$(info CDEP = $(CDEP))

#Declare default targets
.PHONY : all clean clear
all: $(BUILD_DIR)/main.exe
	
#Declare the first target. Syntax is 
#target: dependency1 dependency2... dependencyN
#	command1
#	command2
#
#By default a target is a file. If this file exists and it is newer than all its dependencies, nothing will be done.
#If it should not be a file, (like clean) add it to .PHONY
#The tab is mandatory to inform the commands are inside the target. ALL UNTABED LINES OF ALL THE MAKEFILE (EVEN INCLUDES) ARE EXECUTED BEFORE THE TABED ONES (THIS IS VERY IMPORTANT)
#The final program depends on every .o file. Makefile will scan each .o dependency, and if the .o file is newer than all dependencies, it will do nothing and goes to the next
#If the a .o does not exist or a dependency was changed, makefile will jump to its target e.g. build/lib.c.o
#
#PS: When running make without arguments, it will execute the first target i.e. $(BUILD_DIR)/main.exe in this case
$(BUILD_DIR)/main.exe: $(COBJ)

ifneq ($(COBJ),)

#When we enter here, all .o files are done, so we have link them
#Second, script functions are executed (echo, gcc). When compiling, the console will print "Generating objlist" (see below), then "Linking" 
	@printf "Linking...\n"
	@gcc @$(BUILD_DIR)/objlist.list -o $(@) $(LDFLAGS) -Wall -std=c99

#First, all makefile standard commands are executed (commands like $(fn ...). Here, the order is wierd to illustrate this behavior.
#We print useful information and create the objlist.list file containing all .o dependencies
	$(info )
	$(info Generating objlist... )
	
	$(file >$(BUILD_DIR)/objlist.list) 
	$(foreach file, $(^), $(file >>$(BUILD_DIR)/objlist.list, "$(file)"))

else
	@printf "No files to compile...\n"

endif
	@printf "\nDone\n"

#Include all .d files generated by the compiler. the '-' is used to ignore if a file does not exist. In the first compilation, this line does nothing
#In all further compilations, all .d are included here, and all .o create a target that depends on the .h and .c ; e.g. build/lib.c.o : lib.c lib.h 
#Since this target does nothing, the dependencies are carried to $(BUILD_DIR)/%.c.o: %.c This is important to recompile the source file if lib.h changes
-include $(CDEP)

#declare a clear and clean target to remove the build folder if it exists. Use @: as a NOP command because otherwise the message "nothing to be done" will appear 
clean clear : 
	@printf "Clearing $(BUILD_DIR)...\n"
	
ifneq ($(wildcard $(BUILD_DIR)), )
	@rm -r $(BUILD_DIR)
endif

	@printf "\nDone\n"

#This directive enables second dependency expansion and works for every target from now on. That is why I kept at the end of the file.
#Normally, the dependencies are expanded only once, so to make lib.c -> $(BUILD_DIR)/%.c.o: %.c is expanded to build/lib.c.o : lib.c.
#However, if we want to manipulate the dependency name, it is impossible e.g.: to make .._/lib.c.o from ../lib.c -> 
#$(BUILD_DIR)/%.c.o: $(subst .._/, ../, %.c) is expanded to build/.._/lib.c.o : .._/lib.c, because %.c will be treated as a literal string, so $(subst .._/, ../, %.c) -> %.c
#with .SECONDEXPANSION enabled, we can force %.c to expand to the final dependency name before calling the function, we only need to escape the function first
#so to make build/.._/lib.c.o from ../lib.c : $(BUILD_DIR)/%.c.o: $$(subst .._/, ../, %.c) is expanded -> build/.._/lib.c.o: $(subst .._/, ../, .._/lib.c), then expanded again to -> build/.._/lib.c.o: ../lib.c
.SECONDEXPANSION:

#When searching for a target, this target will match every build/*.c.o, e.g. build/lib.c.o, build/o/lib2.c.o, one at a time
#By doing $(BUILD_DIR)/%.c.o: %.c we can extract the .c file name by removing the build/ and .o from its names e.g. : build/lib.c.o : lib.c ; build/o/lib.c.o : o/lib2.c
#We can use this to our advantage with the standard variables
#$@ : expands to the current target (e.g. build/lib.c.o)
#$< : expands to the first dependency (e.g. lib.c)
#$? : expands to all dependencies that are newer to the target
#$^ : expands to all dependencies
$(BUILD_DIR)/%.c.o: $$(subst .._/, ../, %.c)

#If directory does not exist, create it. @ at the beginning sileces this command. All shell commands are echoed by default
#The ifeq are evaluated during the first makefile pass, and the lines are kept accordingly
ifeq ($(SHELL), cmd)
	@IF NOT EXIST "$(dir $@)" (mkdir "$(dir $@)")
else
	@mkdir -p $(dir $@)
endif
	
#Compile .c file using -c flag to not link it. -MP and -MMD are used to create .d along with .o ; .d files create targets for each .o that depands on every .h inside the code
#This is necessary later

	@printf "Compiling $(<)\n"
	@gcc -c $(<) -o $(@) -MMD -MP $(INC_DIRS:%=-I%) $(CFLAGS) $(DEFINES) -Wall -std=c99