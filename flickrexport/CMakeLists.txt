ADD_DEFINITIONS (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

SET(kipiplugin_flickrexport_PART_SRCS
    plugin_flickrexport.cpp
    flickrwindow.cpp
    login.cpp
    flickrtalker.cpp
    flickrwidget.cpp
    mpform.cpp
    flickrviewitem.cpp
   )

KDE4_ADD_UI_FILES(kipiplugin_flickrexport_PART_SRCS flickralbumdialog.ui)

KDE4_ADD_PLUGIN(kipiplugin_flickrexport ${kipiplugin_flickrexport_PART_SRCS})

TARGET_LINK_LIBRARIES(kipiplugin_flickrexport
                      kipiplugins
                      ${KIPI_LIBRARIES}
                      ${KEXIV2_LIBRARIES}
                      ${KDCRAW_LIBRARIES}
                      ${KDE4_KDE3SUPPORT_LIBS}
                      ${KDE4_KIO_LIBS}
                      ${KDE4_KDEUI_LIBS}
                     )

INSTALL(TARGETS kipiplugin_flickrexport DESTINATION ${PLUGIN_INSTALL_DIR})
INSTALL(FILES kipiplugin_flickrexport.desktop DESTINATION ${SERVICES_INSTALL_DIR})
