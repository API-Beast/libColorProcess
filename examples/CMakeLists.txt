add_executable(palette_operations palette_operations.cpp HtmlExporter.cpp HtmlExporter.h)
target_link_libraries(palette_operations libColorTool)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/style.css DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/palette.gpl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})