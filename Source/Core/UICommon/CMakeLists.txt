set(SRCS  UICommon.cpp)

if(NOT LIBRETRO)
  set(SRCS ${SRCS}
    CommandLineParse.cpp
    Disassembler.cpp
    USBUtils.cpp
  )
  if(USE_X11)
    set(SRCS ${SRCS} X11Utils.cpp)
  endif()

  set(LIBS common cpp-optparse)
endif()
if(LIBUSB_FOUND)
  set(LIBS ${LIBS} ${LIBUSB_LIBRARIES})
endif()

add_dolphin_library(uicommon "${SRCS}" "${LIBS}")

if(ENABLE_LLVM)
  find_package(LLVM CONFIG QUIET)
  if(LLVM_FOUND AND TARGET LLVM)
    message(STATUS "LLVM found, enabling LLVM support in disassembler")
    target_compile_definitions(uicommon PRIVATE HAVE_LLVM)
    target_link_libraries(uicommon PRIVATE LLVM)
    target_include_directories(uicommon PRIVATE ${LLVM_INCLUDE_DIRS})
  endif()
endif()
