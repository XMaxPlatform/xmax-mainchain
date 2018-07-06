set(current_lib_name unitedb)


set(H_FILES)
set(S_FILES)

function(list_root ref_hpps ref_cpps hpps cpps)

	if( WIN32 )
		set(hscope "include")
		set(sscope "${current_lib_name}")
	else()
		set(hscope "include")
		set(sscope "${current_lib_name}")
	endif()


	file(GLOB hfiles "include/${current_lib_name}/*.hpp")
	file(GLOB sfiles "*.cpp")

	source_group( 
		${hscope}
		FILES 
		${hfiles}
	)
	
	source_group( 
		${sscope}
		FILES 
		${sfiles}
	)
	set(newhpp "${hpps};${hfiles}")
	set(${ref_hpps} ${newhpp} PARENT_SCOPE)

	set(newcpp "${cpps};${sfiles}")
	set(${ref_cpps} ${newcpp} PARENT_SCOPE)

endfunction(list_root)


list_root(H_FILES S_FILES "${H_FILES}" "${S_FILES}")


add_library( ${current_lib_name} ${S_FILES} ${H_FILES} )

target_link_libraries( ${current_lib_name} 
	${Boost_LIBRARIES} 
	${xmax_core_lib_name}
)

target_include_directories( ${current_lib_name} PUBLIC "include" "${Boost_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" )

set_target_properties( ${current_lib_name} PROPERTIES PUBLIC_HEADER "${H_FILES}" FOLDER "1.Foundation")





