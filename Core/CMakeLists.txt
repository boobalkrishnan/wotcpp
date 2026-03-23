project(WoTCore)

add_library(${PROJECT_NAME} 
            ./src/WoT.cpp
			./src/WoTAdapter.cpp
			./src/WoTServient.cpp
            ./src/WoTProperty.cpp
            ./src/WoTAction.cpp
            ./src/WoTEvent.cpp
            ./src/WoTThing.cpp
			./src/WoTTd.cpp)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
# target_include_directories(IP)

target_include_directories(${PROJECT_NAME}
	PUBLIC
        ./include
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		cpp-logger
		wot-binding-rpi
	)