SET(kipiplugin_timeadjust_PART_SRCS plugin_timeadjust.cpp timeadjustdialog.cpp)

KDE4_ADD_PLUGIN(kipiplugin_timeadjust ${kipiplugin_timeadjust_PART_SRCS})

TARGET_LINK_LIBRARIES(kipiplugin_timeadjust 
                      ${KDE4_KPARTS_LIBS} 
                      ${QT_AND_KDECORE_LIBS} 
                      ${KIPI_LIBRARIES} 
                      ${KEXIV2_LIBRARIES} 
                      kipiplugins
                     )

KDE4_INSTALL_ICONS(${ICON_INSTALL_DIR})

INSTALL(TARGETS kipiplugin_timeadjust DESTINATION ${PLUGIN_INSTALL_DIR})

INSTALL(FILES kipiplugin_timeadjust.desktop DESTINATION ${SERVICES_INSTALL_DIR})
