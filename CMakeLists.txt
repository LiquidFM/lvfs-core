project (lvfs-core)

# Project header
project_cxx_library_header_default ()

# 3rdparty
list (APPEND ${PROJECT_NAME}_LIBS brolly)
list (APPEND ${PROJECT_NAME}_LIBS efc)
list (APPEND ${PROJECT_NAME}_LIBS lvfs)
list (APPEND ${PROJECT_NAME}_LIBS kitemviews)

if (WIN32)
    find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml REQUIRED)
else (UNIX)
    find_package (Qt4 4.7.1 COMPONENTS QtCore QtGui QtXml QtDbus REQUIRED)
endif ()
include (${QT_USE_FILE})
list (APPEND ${PROJECT_NAME}_LIBS ${QT_LIBRARIES})
add_definitions (${QT_DEFINITIONS} -DQT_NO_CAST_FROM_ASCII)

# Sources
add_subdirectory (src)

# Target - lvfs-core
add_library(lvfs-core SHARED ${${PROJECT_NAME}_SOURCES})
target_link_libraries (lvfs-core ${${PROJECT_NAME}_LIBS})

# Version
build_version_file (lvfs-core 0.0.1)

# Documentation
add_documentation ("LVFS-Core" 0.0.1 "LVFS core library")

# Install rules
install_header_files (lvfs-core "src/lvfs_core_INode.h:INode"
                                "src/lvfs_core_IView.h:IView"
                                "src/lvfs_core_INodeFactory.h:INodeFactory"
                                "src/lvfs_core_IViewFactory.h:IViewFactory"
                                "src/models/kde/lvfs_core_kde_INode.h:models/Kde/INode"
                                "src/models/qt/lvfs_core_qt_INode.h:models/Qt/INode"
                                "src/models/qt/lvfs_core_qt_Node.h:models/Qt/Node"
                                "src/models/qt/lvfs_core_qt_ModelNode.h:models/Qt/ModelNode")
install_target (lvfs-core)
