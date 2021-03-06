ADD_DEFINITIONS(-DQT3_SUPPORT 
                -DQT3_SUPPORT_WARNINGS
               )

SET(kipiplugin_advancedslideshow_PART_SRCS common.cpp
                                   kbeffect.cpp 
                                   screenproperties.cpp
                                   slideshowkb.cpp 
                                   imageloadthread.cpp
                                   listimageitems.cpp
                                   maindialog.cpp
                                   captiondialog.cpp
                                   soundtrackdialog.cpp
                                   advanceddialog.cpp 
                                   playbackwidget.cpp
                                   slideplaybackwidget.cpp
                                   slideshow.cpp 
                                   slideshowconfig.cpp 
                                   slideshowgl.cpp 
                                   slideshowloader.cpp
                                   listsounditems.cpp
                                   plugin_advancedslideshow.cpp 
   )

KDE4_ADD_UI_FILES(kipiplugin_advancedslideshow_PART_SRCS 
                  maindialog.ui 
                  captiondialog.ui 
                  advanceddialog.ui
                  soundtrackdialog.ui
                  slideplaybackwidget.ui
                  playbackwidget.ui
                 )

KDE4_ADD_PLUGIN(kipiplugin_advancedslideshow ${kipiplugin_advancedslideshow_PART_SRCS})

TARGET_LINK_LIBRARIES(kipiplugin_advancedslideshow 
                      ${KDE4_KPARTS_LIBS} 
                      ${QT_AND_KDECORE_LIBS} 
                      ${QT_QTOPENGL_LIBRARY}
                      ${OPENGL_LIBRARIES} 
                      ${KIPI_LIBRARIES} 
                      ${KEXIV2_LIBRARIES} 
                      ${KDCRAW_LIBRARIES} 
                      ${KDE4_KDE3SUPPORT_LIBS}
                      ${KDE4_PHONON_LIBRARY}

                      # For Linux only.
                      ${X11_LIBRARIES}
                      ${X11_Xrandr_LIB}
                      
                      kipiplugins
                     )

KDE4_INSTALL_ICONS(${ICON_INSTALL_DIR})

INSTALL(FILES KIPIicon.svg DESTINATION ${DATA_INSTALL_DIR}/kipiplugin_advancedslideshow)
INSTALL(TARGETS kipiplugin_advancedslideshow DESTINATION ${PLUGIN_INSTALL_DIR})
INSTALL(FILES kipiplugin_advancedslideshow.desktop DESTINATION ${SERVICES_INSTALL_DIR})
