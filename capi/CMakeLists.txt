if (NOT BUILD_CAPI)
  return()
endif()

add_subdirectory(base)
add_subdirectory(link_bundle)
add_subdirectory(link_debug)
add_subdirectory(link_remote_twincat)
add_subdirectory(link_remote_soem)
add_subdirectory(link_simulator)
add_subdirectory(link_soem)
add_subdirectory(link_twincat)
add_subdirectory(gain_holo)
add_subdirectory(modulation_audio_file)
add_subdirectory(extra_geometry_viewer)
add_subdirectory(extra_simulator)
add_subdirectory(example)
