project (lvfs-core)

# Project header
project_header_default ("POSITION_INDEPENDENT_CODE:YES")

# 3rdparty
list (APPEND ${PROJECT_NAME}_LIBS ${EFC_LIB})
list (APPEND ${PROJECT_NAME}_LIBS ${LVFS_LIB})
#list (APPEND ${PROJECT_NAME}_LIBS kitemviews)
list (APPEND ${PROJECT_NAME}_LIBS ${QT_LIBRARIES})

# Sources
add_subdirectory (src)
QT4_WRAP_CPP (${PROJECT_NAME}_MOC_SOURCES ${${PROJECT_NAME}_HEADERS} OPTIONS "-nw")
list (APPEND ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_MOC_SOURCES})

# Target - lvfs-core
add_library (lvfs-core SHARED ${${PROJECT_NAME}_SOURCES})
target_link_libraries (lvfs-core ${${PROJECT_NAME}_LIBS})

# Documentation
add_documentation (lvfs-core 0.0.1 "LVFS core library")

# Install rules
install_header_files (lvfs-core "src/lvfs_core_INode.h:INode"
                                "src/lvfs_core_IView.h:IView"
                                "src/lvfs_core_IEditor.h:IEditor"
                                "src/lvfs_core_IMainView.h:IMainView"
                                "src/lvfs_core_INodeFactory.h:INodeFactory"
                                "src/lvfs_core_IViewFactory.h:IViewFactory"

                                "src/settings/lvfs_core_settings_Option.h:settings/Option"
                                "src/settings/lvfs_core_settings_IntOption.h:settings/IntOption"
                                "src/settings/lvfs_core_settings_StringOption.h:settings/StringOption"
                                "src/settings/lvfs_core_settings_Scope.h:settings/Scope"
                                "src/settings/lvfs_core_settings_Page.h:settings/Page"

                                "src/tools/models/lvfs_core_TreeModel.h:tools/models/TreeModel"
                                "src/tools/strings/strnatcmp.h:tools/strings/strnatcmp.h"
                                "src/tools/strings/readableints.h:tools/strings/readableints.h"
                                "src/tools/templates/mtp.h:tools/templates/mtp.h"
                                "src/tools/templates/uri.h:tools/templates/uri.h"
                                "src/tools/events/eventhandler.h:tools/events/EventHandler"
                                "src/tools/events/eventsourcebase.h:tools/events/EventSourceBase"
                                "src/tools/events/eventhandlerbase.h:tools/events/EventHandlerBase"
                                "src/tools/events/eventhandlerdefault.h:tools/events/EventHandlerDefault"
                                "src/tools/events/contextmenueventhandler.h:tools/events/ContextMenuEventHandler"
                                "src/tools/events/contextmenueventsource.h:tools/events/ContextMenuEventSource"
                                "src/tools/events/focuseventhandler.h:tools/events/FocusEventHandler"
                                "src/tools/events/focuseventsource.h:tools/events/FocusEventSource"
                                "src/tools/events/keyboardeventhandler.h:tools/events/KeyboardEventHandler"
                                "src/tools/events/keyboardeventsource.h:tools/events/KeyboardEventSource"
                                "src/tools/events/mouseeventhandler.h:tools/events/MouseEventHandler"
                                "src/tools/events/mouseeventsource.h:tools/events/MouseEventSource"

                                "src/tools/widgets/choosedialog/lvfs_core_ChooseDialog.h:tools/widgets/ChooseDialog"
                                "src/tools/widgets/intdialog/lvfs_core_IntDialog.h:tools/widgets/IntDialog"
                                "src/tools/widgets/nestedplaindialog/widgets/lvfs_core_BaseNestedWidget.h:tools/widgets/BaseNestedWidget"
                                "src/tools/widgets/nestedplaindialog/widgets/lvfs_core_NestedDialog.h:tools/widgets/NestedDialog"
                                "src/tools/widgets/nestedplaindialog/widgets/lvfs_core_NestedWidget.h:tools/widgets/NestedWidget"
                                "src/tools/widgets/nestedplaindialog/lvfs_core_NestedPlainDialog.h:tools/widgets/NestedPlainDialog"
                                "src/tools/widgets/stringdialog/lvfs_core_StringDialog.h:tools/widgets/StringDialog"
                                "src/tools/widgets/valuedialog/lvfs_core_ValueDialog.h:tools/widgets/ValueDialog"

                                "src/models/kde/lvfs_core_kde_INode.h:models/Kde/INode"

                                "src/models/lvfs_core_Node.h:models/Node"
                                "src/models/qt/lvfs_core_qt_INode.h:models/Qt/INode"
                                "src/models/qt/lvfs_core_qt_IView.h:models/Qt/IView"
                                "src/models/qt/lvfs_core_qt_Node.h:models/Qt/Node"
                                "src/models/qt/lvfs_core_qt_SortFilterModel.h:models/Qt/SortFilterModel"
                                "src/models/qt/lvfs_core_qt_StyledItemDelegate.h:models/Qt/StyledItemDelegate")
install_target (lvfs-core)
