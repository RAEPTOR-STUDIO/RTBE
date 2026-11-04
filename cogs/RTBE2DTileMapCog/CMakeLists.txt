cmake_minimum_required(VERSION 3.16)
project(RTBE2DTileMapCog LANGUAGES C)
find_package(RaeptorCogs CONFIG REQUIRED)
find_package(Cog CONFIG REQUIRED)
CogCreate(${PROJECT_NAME}
  src/RaeptorCogs/${PROJECT_NAME}/cog.c
)
CogLoad("../../../CoreCogs/GraphicsCogs/Platform/WindowCog" WindowCog)
CogLoad("../../../CoreCogs/GraphicsCogs/UI/PannelCog" PannelCog)
CogLoad("../../../CoreCogs/GraphicsCogs/UI/GraphicsCog" GraphicsCog)
CogLoad("../../../CoreCogs/GraphicsCogs/UI/RHICog" RHICog)
CogLoad("../../../CoreCogs/GraphicsCogs/UI/TextCog" TextCog)
CogLoad("../../../CoreCogs/IOCogs/InputCog" InputCog)
CogLoad("../../" RTBE)

CogRequireCogsDomains(${PROJECT_NAME}
  WindowCog
  PannelCog
  GraphicsCog
  RHICog
  TextCog
  InputCog
  RTBE
)
