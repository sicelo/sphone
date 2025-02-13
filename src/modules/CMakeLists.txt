set(MODULE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../modapi)

add_library(test SHARED test.c)
target_link_libraries(test ${COMMON_LIBRARIES})
target_include_directories(test SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(test PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS test DESTINATION ${SPHONE_MODULE_DIR})

add_library(glibloop SHARED glibloop.c)
target_link_libraries(glibloop ${COMMON_LIBRARIES})
target_include_directories(glibloop SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(glibloop PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS glibloop DESTINATION ${SPHONE_MODULE_DIR})

if(DEFINED Qt${QT_VERSION_MAJOR}_FOUND)
	add_library(qtloop SHARED qtloop.cpp)
	target_link_libraries(qtloop ${COMMON_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets)
	target_include_directories(qtloop SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
	target_include_directories(qtloop PRIVATE ${MODULE_INCLUDE_DIRS})
	install(TARGETS qtloop DESTINATION ${SPHONE_MODULE_DIR})
endif(DEFINED Qt${QT_VERSION_MAJOR}_FOUND)

add_library(manager SHARED manager.c)
target_link_libraries(manager ${COMMON_LIBRARIES})
target_include_directories(manager SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(manager PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS manager DESTINATION ${SPHONE_MODULE_DIR})

add_library(external-exec SHARED external-exec.c)
target_link_libraries(external-exec ${COMMON_LIBRARIES})
target_include_directories(external-exec SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(external-exec PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS external-exec DESTINATION ${SPHONE_MODULE_DIR})

if(DEFINED EBOOK_LIBRARIES AND DEFINED EBOOKC_LIBRARIES)
	add_library(contacts-evolution SHARED contacts-evolution.c)
	target_link_libraries(contacts-evolution ${COMMON_LIBRARIES} ${EBOOK_LIBRARIES} ${EBOOKC_LIBRARIES})
	target_include_directories(contacts-evolution SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS} ${EBOOK_INCLUDE_DIRS} ${EBOOKC_INCLUDE_DIRS})
	target_include_directories(contacts-evolution PRIVATE ${MODULE_INCLUDE_DIRS})
	install(TARGETS contacts-evolution DESTINATION ${SPHONE_MODULE_DIR})
endif(DEFINED EBOOK_LIBRARIES AND DEFINED EBOOKC_LIBRARIES)

if(DEFINED GSTREAMER_LIBRARIES)
	add_library(playback-gstreamer SHARED playback-gstreamer.c)
	target_link_libraries(playback-gstreamer ${COMMON_LIBRARIES} ${GSTREAMER_LIBRARIES})
	target_include_directories(playback-gstreamer SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS} ${GSTREAMER_INCLUDE_DIRS})
	target_include_directories(playback-gstreamer PRIVATE ${MODULE_INCLUDE_DIRS})
	install(TARGETS playback-gstreamer DESTINATION ${SPHONE_MODULE_DIR})
endif(DEFINED GSTREAMER_LIBRARIES)

if(DEFINED PULSE_LIBRARIES)
	add_library(route-pulseaudio SHARED route-pulseaudio.c)
	target_link_libraries(route-pulseaudio ${COMMON_LIBRARIES} ${PULSE_LIBRARIES})
	target_include_directories(route-pulseaudio SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS} ${PULSE_INCLUDE_DIRS})
	target_include_directories(route-pulseaudio PRIVATE ${MODULE_INCLUDE_DIRS})
	install(TARGETS route-pulseaudio DESTINATION ${SPHONE_MODULE_DIR})
endif(DEFINED PULSE_LIBRARIES)

add_library(rtconf-ini SHARED rtconf-ini.c)
target_link_libraries(rtconf-ini ${COMMON_LIBRARIES})
target_include_directories(rtconf-ini PRIVATE ${COMMON_INCLUDE_DIRS} ${MODULE_INCLUDE_DIRS})
install(TARGETS rtconf-ini DESTINATION ${SPHONE_MODULE_DIR})

if(DEFINED LIBPROFILE_LIBRARIES)
	add_library(rtconf-libprofile SHARED rtconf-libprofile.c)
	target_link_libraries(rtconf-libprofile ${COMMON_LIBRARIES} ${LIBPROFILE_LIBRARIES})
	target_include_directories(rtconf-libprofile SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS} ${LIBPROFILE_INCLUDE_DIRS})
	target_include_directories(rtconf-libprofile PRIVATE ${MODULE_INCLUDE_DIRS})
	install(TARGETS rtconf-libprofile DESTINATION ${SPHONE_MODULE_DIR})
endif(DEFINED LIBPROFILE_LIBRARIES)

add_library(sphone-mce SHARED sphone-mce.c)
target_link_libraries(sphone-mce ${COMMON_LIBRARIES})
target_include_directories(sphone-mce SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(sphone-mce PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS sphone-mce DESTINATION ${SPHONE_MODULE_DIR})

add_library(comm-ofono SHARED comm-ofono.c)
target_link_libraries(comm-ofono ${COMMON_LIBRARIES})
target_include_directories(comm-ofono SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(comm-ofono PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS comm-ofono DESTINATION ${SPHONE_MODULE_DIR})

add_library(comm-error-gtk SHARED comm-error-gtk.c)
target_link_libraries(comm-error-gtk ${COMMON_LIBRARIES})
target_include_directories(comm-error-gtk SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(comm-error-gtk PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS comm-error-gtk DESTINATION ${SPHONE_MODULE_DIR})

add_library(contacts-ui-exec SHARED contacts-ui-exec.c)
target_link_libraries(contacts-ui-exec ${COMMON_LIBRARIES})
target_include_directories(contacts-ui-exec SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(contacts-ui-exec PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS contacts-ui-exec DESTINATION ${SPHONE_MODULE_DIR})

add_library(commtest SHARED commtest.c)
target_link_libraries(commtest ${COMMON_LIBRARIES})
target_include_directories(commtest SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS})
target_include_directories(commtest PRIVATE ${MODULE_INCLUDE_DIRS})
install(TARGETS commtest DESTINATION ${SPHONE_MODULE_DIR})

if(DEFINED RTCOM_LIBRARIES)
	add_library(store-rtcom SHARED store-rtcom.c)
	target_link_libraries(store-rtcom ${COMMON_LIBRARIES} ${RTCOM_LIBRARIES})
	target_include_directories(store-rtcom SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS} ${RTCOM_INCLUDE_DIRS})
	target_include_directories(store-rtcom PRIVATE ${MODULE_INCLUDE_DIRS})
	install(TARGETS store-rtcom DESTINATION ${SPHONE_MODULE_DIR})
endif(DEFINED RTCOM_LIBRARIES)

if(DEFINED ABOOK_LIBRARIES AND DEFINED EBOOK_LIBRARIES AND DEFINED EBOOKC_LIBRARIES)
	add_library(contacts-ui-abook SHARED contacts-ui-abook.c)
	target_link_libraries(contacts-ui-abook ${COMMON_LIBRARIES} ${ABOOK_LIBRARIES} ${EBOOK_LIBRARIES} ${EBOOKC_LIBRARIES})
	target_include_directories(contacts-ui-abook SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS} ${ABOOK_INCLUDE_DIRS} ${EBOOK_INCLUDE_DIRS} ${EBOOKC_INCLUDE_DIRS})
	target_include_directories(contacts-ui-abook PRIVATE ${MODULE_INCLUDE_DIRS})
	install(TARGETS contacts-ui-abook DESTINATION ${SPHONE_MODULE_DIR})
endif(DEFINED ABOOK_LIBRARIES AND DEFINED EBOOK_LIBRARIES AND DEFINED EBOOKC_LIBRARIES)

if(DEFINED NOTIFY_LIBRARIES)
	add_library(notify-libnotify SHARED notify-libnotify.c)
	target_link_libraries(notify-libnotify ${COMMON_LIBRARIES} ${NOTIFY_LIBRARIES})
	target_include_directories(notify-libnotify SYSTEM PRIVATE ${COMMON_INCLUDE_DIRS} ${NOTIFY_INCLUDE_DIRS})
	target_include_directories(notify-libnotify PRIVATE ${MODULE_INCLUDE_DIRS})
	install(TARGETS notify-libnotify DESTINATION ${SPHONE_MODULE_DIR})
endif(DEFINED NOTIFY_LIBRARIES)

add_subdirectory(gui)
