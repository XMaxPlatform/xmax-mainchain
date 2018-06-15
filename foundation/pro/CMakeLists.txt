set(current_lib_name pro)


set(H_FILES)
set(S_FILES)

function(list_source ref_hpps ref_cpps hpps cpps code_scope)

	if( WIN32 )
		set(hscope "include\\${code_scope}")
		set(sscope "${current_lib_name}\\${code_scope}")
	else()
		set(hscope "include/${code_scope}")
		set(sscope "${current_lib_name}/${code_scope}")
	endif()


	file(GLOB hfiles "include/${current_lib_name}/${code_scope}/*.hpp")
	file(GLOB sfiles "${code_scope}/*.cpp")

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

endfunction(list_source)


list_source(H_FILES S_FILES "${H_FILES}" "${S_FILES}" types)
list_source(H_FILES S_FILES "${H_FILES}" "${S_FILES}" log)
list_source(H_FILES S_FILES "${H_FILES}" "${S_FILES}" utils)

message( STATUS "H_FILES:${H_FILES}")
message( STATUS "S_FILES:${S_FILES}")

add_library( ${current_lib_name} ${S_FILES} ${H_FILES} )
target_link_libraries( ${current_lib_name})
target_include_directories( ${current_lib_name} PUBLIC "include" "${CMAKE_CURRENT_SOURCE_DIR}" )
set_target_properties( ${current_lib_name} PROPERTIES PUBLIC_HEADER "${H_FILES}" FOLDER "1.Foundation")





