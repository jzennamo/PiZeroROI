# basic source code CMakeLists.txt
#include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
#include_directories( $ENV{NUTOOLS_INC} )

art_make( 
	  MODULE_LIBRARIES
			ub_PiZeroROI
#			Geometry
#	  	        Geometry_service
#		        Utilities
#	       	        TimeService_service
#	                uboone_Geometry
#                      OpticalDetectorData
#			OpticalDetector
 #                     RawData
#			RecoBase
#			Simulation
#			SimulationBase
                        ${ART_FRAMEWORK_CORE}
			${ART_FRAMEWORK_IO_SOURCES}
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
                        ${ROOT_XMLIO}
                        ${ROOT_GDML}
                        ${ROOT_BASIC_LIB_LIST}
			${SIMULATIONBASE}
)							   

install_headers()
install_source()
install_fhicl()
