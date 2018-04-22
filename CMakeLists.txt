cmake_minimum_required(VERSION 3.8)
project(ld41)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

include_directories(/home/loic/Programmation/libs/fluffy/include /home/loic/Programmation/libs/fluffy/extlibs/jsoncpp)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp main.cpp Application.cpp Application.hpp typedefs.hpp systems/RenderSystem.cpp systems/RenderSystem.hpp events.hpp components/Body.hpp components/Drawable.hpp scene/LayerNode.hpp states/GameState.cpp states/GameState.hpp World.cpp World.hpp components/Camera.hpp components/PlayerControlled.hpp components/SpeedComponent.hpp systems/PlayerControlledSystem.cpp systems/PlayerControlledSystem.hpp PlayerController.cpp PlayerController.hpp systems/PhysicsSystem.cpp systems/PhysicsSystem.hpp collisions.hpp Animation.cpp Animation.hpp systems/AnimationSystem.cpp systems/AnimationSystem.hpp components/PlayerAnimation.hpp components/BallHolder.hpp systems/BallHolderSystem.hpp AIController.cpp AIController.hpp components/AIControlled.hpp systems/AIControlledSystem.cpp systems/AIControlledSystem.hpp components/Ball.hpp components/Team.hpp)
add_executable(ld41 ${SOURCE_FILES})
target_link_libraries(ld41 /home/loic/Programmation/libs/fluffy/cmake-build-debug/libfluffy.so sfml-system sfml-window sfml-graphics sfml-audio)
