# /Pylovara/System/Kernel/Core/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(PylovaraKernel VERSION 2.9 LANGUAGES C)

include_directories(
    ${CMAKE_SOURCE_DIR}/include   # Öffentliche API, Priorität 1
    ${CMAKE_SOURCE_DIR}/Logic     # Lexer-Header
    ${CMAKE_SOURCE_DIR}/Lib       # Für Legacy (später entfernen)
    ${CMAKE_SOURCE_DIR}/Lab       # Spätere Testphasen
)

# Quellen sammeln
set(SRC_LIB
    Lib/mcs_action.c
    Lib/mcs_argument.c
    Lib/mcs_feed.c
    Lib/mcs_protein.c
    Lib/mcs_proton.c
    Lib/mcs_warp.c
    Lib/mcs_wahrheiten.c
    Lib/mcs_transaktion.c
    Lib/mcs_eingaben.c
    Lib/mcs_runner.c
    Lib/mcs_semantics.c
    Logic/mcs_lexer.c
)

set(SRC_LOGIC
    Logic/mcs_lexer.c
)

# Bibliothek bauen (statisch)
#add_library(mcs_core STATIC ${SRC_LIB} ${SRC_LOGIC})
add_library(mcs_core STATIC
    Lib/mcs_action.c
    Lib/mcs_argument.c
    Lib/mcs_feed.c
    Lib/mcs_protein.c
    Lib/mcs_proton.c
    Lib/mcs_warp.c
    Lib/mcs_wahrheiten.c
    Lib/mcs_semantics.c
    Lib/mcs_operatoren.c   # ← VOR mcs_transaktion.c verschoben
    Lib/mcs_transaktion.c
    Lib/mcs_eingaben.c
    Lib/mcs_cmd_register.c   # ← NEU — vor runner
    Lib/mcs_runner.c
    Logic/mcs_lexer.c
)

# Test-Programme
add_executable(test_lexer Lab/test_lexer.c)
target_link_libraries(test_lexer mcs_core)

add_executable(test_action Lab/test_action.c)
target_link_libraries(test_action mcs_core)

add_executable(test_if Lab/test_if.c)
target_link_libraries(test_if mcs_core)

add_executable(test_trans Lab/test_trans.c)
target_link_libraries(test_trans mcs_core)

# Optional: Hauptprogramm
# add_executable(pylovara_kernel main.c)
# target_link_libraries(pylovara_kernel mcs_core)

add_executable(mcs main.c)
target_link_libraries(mcs mcs_core)




