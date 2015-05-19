ADD_DEFINITIONS(
	-D_LIB
)

IF ( BW_IS_TOOLS OR 
	 BW_IS_QT_TOOLS OR 
	 BW_IS_GENERIC_APP_TEST OR 
	 BW_IS_OFFLINEPROCESSOR )
	SET( BW_USE_CSTDMF_DLL ON )
	SET( BW_USE_CSTDMF_IN_DLL ON )

	ADD_DEFINITIONS( 
		-DCSTDMF_IMPORT
	)
ENDIF()

IF ( BW_USE_BWENTITY_DLL )
	SET( BW_USE_CSTDMF_IN_DLL ON )
	SET( BW_USE_BWENTITY_IN_DLL ON )
	ADD_DEFINITIONS( 
		-DBWENTITY_DLL
		-DBWENTITY_DLL_IMPORT
		-DCSTDMF_IMPORT
	)
ENDIF()

