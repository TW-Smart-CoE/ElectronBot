cmake_minimum_required(VERSION 3.19)
project(ElectronBotSDK)


add_subdirectory(./ElectronBotSDK-LowLevel)
add_subdirectory(./ElectronBotSDK-Player)
add_subdirectory(./ElectronBotSDK-UnityBridge)


install(
    TARGETS ElectronBotSDK-LowLevel Sample_LowLevel ElectronBotSDK-Player Sample_Player ElectronBotSDK-UnityBridge
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)