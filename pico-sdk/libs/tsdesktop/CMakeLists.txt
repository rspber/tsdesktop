add_library(tsdesktop INTERFACE)

target_include_directories(tsdesktop INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop)

target_sources(tsdesktop INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/Container.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/Button.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/TextButton.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/ValueButton.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/DecorTextButton.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/CheckBox.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/RadioButton.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/Editable.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/Scroller.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/FieldSet.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/GfxObject.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tsdesktop/Canvas.cpp
)
