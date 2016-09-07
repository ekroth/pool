macro(kke_factory_registration
name
base
params
classes
class_headers
namespace
)
	set(output "${CMAKE_BINARY_DIR}/Factory_Register_${name}.hpp")
	file(WRITE ${output} "/*\n\tAutomatically generated by UseFactory.cmake\n*/\n\n")
	file(APPEND ${output} "#ifndef KKE_FACTORY_REGISTER_${name}\n")
	file(APPEND ${output} "#define KKE_FACTORY_REGISTER_${name}\n\n")

	# Add includes
	foreach(c ${class_headers})
		file(APPEND ${output} "#include \"${c}\"\n")
	endforeach()

	set(factory_name "Factory<${base}")
	set(factory_params_i 0)
	set(factory_params_types)
	set(factory_params_vars)

	foreach (p ${params})
		set(factory_name "${factory_name}, ${p}")

		set(factory_param_name "var${factory_params_i}")
		if (${factory_params_i} EQUAL 0)
			set(factory_params_types "${p} ${factory_param_name}")
			set(factory_params_vars "${factory_param_name}")
		else()
			set(factory_params_types "${factory_params_types}, ${p} ${factory_param_name}")
			set(factory_params_vars "${factory_params_vars}, ${factory_param_name}")
		endif()

		math(EXPR factory_params_i "${factory_params_i} + 1")
	endforeach()
	set(factory_name "${factory_name}>")

	# Add registrations
	file(APPEND ${output} "namespace ${namespace}\n{")
	file(APPEND ${output} "\n\tvoid Factory_Register_${name}()\n\t{\n")
	foreach(c ${classes})
		file(APPEND ${output} "\t\t${factory_name}::Instance().Register(\"${c}\", [](${factory_params_types}) { return new ${c}(${factory_params_vars}); });\n")
	endforeach()

	file(APPEND ${output} "\t}\n}\n\n#endif // KKE_FACTORY_REGISTER_${name}\n")

	set(output)
	set(factory_name)
	set(factory_params_i)
	set(factory_params_types)
	set(factory_params_vars)
endmacro()

macro(kke_factory_register_all
names
namespace)

	# Header
	set(output_h "${CMAKE_BINARY_DIR}/Factory_Register_All.hpp")
	
	file(WRITE ${output_h} "/*\n\tAutomatically generated by UseFactory.cmake\n*/\n\n")
	file(APPEND ${output_h} "#ifndef KKE_FACTORY_REGISTER_ALL\n")
	file(APPEND ${output_h} "#define KKE_FACTORY_REGISTER_ALL\n\n")

	file(APPEND ${output_h} "namespace ${namespace}\n{\n")
	file(APPEND ${output_h} "\tvoid Factory_Register_All();")
	file(APPEND ${output_h} "\n}\n\n#endif // KKE_FACTORY_REGISTER_ALL\n")

	set(output_h)

	# Source
	set(output_s "${CMAKE_BINARY_DIR}/Factory_Register_All.cpp")
	file(WRITE ${output_s} "/*\n\tAutomatically generated by UseFactory.cmake\n*/\n\n")
	
	file(APPEND ${output_s} "#include \"${CMAKE_BINARY_DIR}/Factory_Register_All.hpp\"\n")
	foreach(n ${names})
		file(APPEND ${output_s} "#include \"${CMAKE_BINARY_DIR}/Factory_Register_${n}.hpp\"\n")
	endforeach()

	file(APPEND ${output_s} "\nvoid ${namespace}::Factory_Register_All()\n{")

	foreach(n ${names})
		file(APPEND ${output_s} "\n\t${namespace}::Factory_Register_${n}();")
	endforeach()

	file(APPEND ${output_s} "\n}")

	set(KKE_FACTORY_SOURCE ${output_s})
	set(output_s)
endmacro()