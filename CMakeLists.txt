cmake_minimum_required(VERSION 3.16)
project(RTBE LANGUAGES C)
find_package(RaeptorCogs CONFIG REQUIRED)
find_package(Cog CONFIG REQUIRED)
CogCreate(${PROJECT_NAME}
  src/RaeptorCogs/${PROJECT_NAME}/rtbe.c
)
CogLoad("../ExampleCog" ExampleCog)
CogLoad("../PrimeMover" PrimeMover)
CogLoad("../RaylibCog" RaylibCog)

CogStarterSetEntryPoint(PrimeMover)
CogStarterSetType(Static)
# CogStarterAddCogs(ExampleCog)
CogStarterAddCogs(${PROJECT_NAME})
CogStarterAddCogs(RaylibCog)
CogStarterCreate()
