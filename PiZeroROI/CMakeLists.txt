# basic source code CMakeLists.txt
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

art_make( LIBRARY_NAME  ub_PiZeroROI
#	SOURCE PiZeroROI.cxx
	LIB_LIBRARIES
#                 ${SIMULATIONBASE}
#                 ${LARRECO_LIB}
                 ${LARDATA_LIB}
                 ${ART_FRAMEWORK_CORE}
                 ${ART_FRAMEWORK_PRINCIPAL}
                 ${ART_FRAMEWORK_SERVICES_REGISTRY}
                 ${ART_FRAMEWORK_SERVICES_OPTIONAL}
                 ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
                 ${ART_PERSISTENCY_COMMON}
                 ${ART_PERSISTENCY_PROVENANCE}
                 ${ART_UTILITIES}
                 ${MF_MESSAGELOGGER}
  	  	 ${MF_UTILITIES}
                 ${FHICLCPP}
                 ${CETLIB}
                 ${ROOT_GEOM}
 #                ${ROOT_XMLIO}
 #                ${ROOT_GDML}
                 ${ROOT_BASIC_LIB_LIST}

)

install_headers()
install_source()
