DEAL_II_INITIALIZE_CACHED_VARIABLES()
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF ("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF ("${isSystemDir}" STREQUAL "-1")


# Set the name of the project and target:
SET(TARGET "rpt_cell_reconstruction_3d")

INCLUDE_DIRECTORIES(
        lethe
        ${CMAKE_SOURCE_DIR}/include/
)

ADD_EXECUTABLE(rpt_cell_reconstruction_3d rpt_cell_reconstruction_3d.cc)
DEAL_II_SETUP_TARGET(rpt_cell_reconstruction_3d)
TARGET_LINK_LIBRARIES(rpt_cell_reconstruction_3d lethe-core lethe-rpt)

install(TARGETS rpt_cell_reconstruction_3d RUNTIME DESTINATION bin)
