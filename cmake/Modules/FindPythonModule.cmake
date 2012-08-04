# Function to locate a python module.
# The location is placed into the variable PY_MODULE,
# where MODULE is the uppercase name of the module.

FUNCTION (FIND_PYTHON_MODULE module)
    # Define the uppercase module name
    STRING (TOUPPER ${module} module_upper)
    # Only check if the name hasn't been defined already
    IF (NOT PY_${module_upper})
        # Mark required if it is required
        IF (ARGC GREATER 1 AND ARGV1 STREQUAL "REQUIRED")
            SET (PY_${module}_FIND_REQUIRED TRUE)
        ENDIF (ARGC GREATER 1 AND ARGV1 STREQUAL "REQUIRED")
        # A module's location is usually a directory, but for binary modules
        # it's a .so file.
        EXECUTE_PROCESS (COMMAND 
            "${PYTHON_EXECUTABLE}" "-c"
            "import re, ${module}; print re.compile('/__init__.py.*').sub('',${module}.__file__)"
            RESULT_VARIABLE _${module}_status 
            OUTPUT_VARIABLE _${module}_location
            ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
        # If found, define the variable
        IF (NOT _${module}_status)
            SET (PY_${module_upper} ${_${module}_location} CACHE STRING 
                "Location of Python module ${module}")
        ENDIF (NOT _${module}_status)
    ENDIF (NOT PY_${module_upper})
    # Alert user to status
    FIND_PACKAGE_HANDLE_STANDARD_ARGS (PY_${module}
                                       DEFAULT_MSG PY_${module_upper})
ENDFUNCTION (FIND_PYTHON_MODULE)
