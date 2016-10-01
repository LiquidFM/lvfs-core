# - Find lvfs-core
# Find the lvfs-core library
#
#  This module defines the following variables:
#     LVFS_CORE_FOUND   - True if LVFS_CORE_INCLUDE is found
#     LVFS_CORE_INCLUDE - where to find header files
#     LVFS_CORE_LIB     - the library files


if (NOT LVFS_CORE_FOUND)
    find_path (LVFS_CORE_INCLUDE
               NAMES "lvfs-core/INode"
               PATH_SUFFIXES include
               DOC "Lvfs-Core include directory")

    find_library (LVFS_CORE_LIB
                  NAMES lvfs-core
                  DOC "Lvfs-Core library file")

    # handle the QUIETLY and REQUIRED arguments and set LVFS_CORE_FOUND to TRUE if all listed variables are TRUE
    include (FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS (Lvfs-Core DEFAULT_MSG LVFS_CORE_INCLUDE LVFS_CORE_LIB)

    mark_as_advanced (LVFS_CORE_INCLUDE LVFS_CORE_LIB)
endif ()
