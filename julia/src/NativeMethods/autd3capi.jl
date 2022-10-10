# This file was automatically generated from header file

module autd3capi

function get_bin_path()
if Sys.iswindows()
    return "win-x64"
elseif Sys.isapple()
    return "macos-universal"
elseif Sys.islinux()
    return "linux-x64"
end
end

function get_lib_ext()
if Sys.iswindows()
    return ".dll"
elseif Sys.isapple()
    return ".dylib"
elseif Sys.islinux()
    return ".so"
end
end

function get_lib_prefix()
if Sys.iswindows()
    return ""
else
    return "lib"
end
end

const _dll = joinpath(@__DIR__, get_bin_path(), "bin", get_lib_prefix() * "autd3capi" * get_lib_ext())

autd_get_last_error(error) = ccall((:AUTDGetLastError, _dll), Int32, (Ptr{UInt8}, ), error);
autd_create_controller(out) = ccall((:AUTDCreateController, _dll), Cvoid, (Ref{Ptr{Cvoid}}, ), out);
autd_open_controller(handle, link) = ccall((:AUTDOpenController, _dll), Bool, (Ptr{Cvoid}, Ptr{Cvoid}, ), handle, link);
autd_add_device(handle, x, y, z, rz1, ry, rz2) = ccall((:AUTDAddDevice, _dll), Int32, (Ptr{Cvoid}, Float64, Float64, Float64, Float64, Float64, Float64, ), handle, x, y, z, rz1, ry, rz2);
autd_add_device_quaternion(handle, x, y, z, qw, qx, qy, qz) = ccall((:AUTDAddDeviceQuaternion, _dll), Int32, (Ptr{Cvoid}, Float64, Float64, Float64, Float64, Float64, Float64, Float64, ), handle, x, y, z, qw, qx, qy, qz);
autd_close(handle) = ccall((:AUTDClose, _dll), Int32, (Ptr{Cvoid}, ), handle);
autd_clear(handle) = ccall((:AUTDClear, _dll), Int32, (Ptr{Cvoid}, ), handle);
autd_synchronize(handle) = ccall((:AUTDSynchronize, _dll), Int32, (Ptr{Cvoid}, ), handle);
autd_free_controller(handle) = ccall((:AUTDFreeController, _dll), Cvoid, (Ptr{Cvoid}, ), handle);
autd_is_open(handle) = ccall((:AUTDIsOpen, _dll), Bool, (Ptr{Cvoid}, ), handle);
autd_get_force_fan(handle) = ccall((:AUTDGetForceFan, _dll), Bool, (Ptr{Cvoid}, ), handle);
autd_get_reads_fpga_info(handle) = ccall((:AUTDGetReadsFPGAInfo, _dll), Bool, (Ptr{Cvoid}, ), handle);
autd_get_check_trials(handle) = ccall((:AUTDGetCheckTrials, _dll), Int32, (Ptr{Cvoid}, ), handle);
autd_get_send_interval(handle) = ccall((:AUTDGetSendInterval, _dll), Int32, (Ptr{Cvoid}, ), handle);
autd_set_reads_fpga_info(handle, reads_fpga_info) = ccall((:AUTDSetReadsFPGAInfo, _dll), Cvoid, (Ptr{Cvoid}, Bool, ), handle, reads_fpga_info);
autd_set_check_trials(handle, trials) = ccall((:AUTDSetCheckTrials, _dll), Cvoid, (Ptr{Cvoid}, Int32, ), handle, trials);
autd_set_send_interval(handle, interval) = ccall((:AUTDSetSendInterval, _dll), Cvoid, (Ptr{Cvoid}, Int32, ), handle, interval);
autd_set_force_fan(handle, force) = ccall((:AUTDSetForceFan, _dll), Cvoid, (Ptr{Cvoid}, Bool, ), handle, force);
autd_get_sound_speed(handle) = ccall((:AUTDGetSoundSpeed, _dll), Float64, (Ptr{Cvoid}, ), handle);
autd_set_sound_speed(handle, sound_speed) = ccall((:AUTDSetSoundSpeed, _dll), Cvoid, (Ptr{Cvoid}, Float64, ), handle, sound_speed);
autd_get_trans_frequency(handle, device_idx, local_trans_idx) = ccall((:AUTDGetTransFrequency, _dll), Float64, (Ptr{Cvoid}, Int32, Int32, ), handle, device_idx, local_trans_idx);
autd_set_trans_frequency(handle, device_idx, local_trans_idx, frequency) = ccall((:AUTDSetTransFrequency, _dll), Cvoid, (Ptr{Cvoid}, Int32, Int32, Float64, ), handle, device_idx, local_trans_idx, frequency);
autd_get_trans_cycle(handle, device_idx, local_trans_idx) = ccall((:AUTDGetTransCycle, _dll), UInt16, (Ptr{Cvoid}, Int32, Int32, ), handle, device_idx, local_trans_idx);
autd_set_trans_cycle(handle, device_idx, local_trans_idx, cycle) = ccall((:AUTDSetTransCycle, _dll), Cvoid, (Ptr{Cvoid}, Int32, Int32, UInt16, ), handle, device_idx, local_trans_idx, cycle);
autd_get_wavelength(handle, device_idx, local_trans_idx) = ccall((:AUTDGetWavelength, _dll), Float64, (Ptr{Cvoid}, Int32, Int32, ), handle, device_idx, local_trans_idx);
autd_get_attenuation(handle) = ccall((:AUTDGetAttenuation, _dll), Float64, (Ptr{Cvoid}, ), handle);
autd_set_attenuation(handle, attenuation) = ccall((:AUTDSetAttenuation, _dll), Cvoid, (Ptr{Cvoid}, Float64, ), handle, attenuation);
autd_get_fpga_info(handle, out) = ccall((:AUTDGetFPGAInfo, _dll), Bool, (Ptr{Cvoid}, Array{UInt8,1}, ), handle, out);
autd_update_flags(handle) = ccall((:AUTDUpdateFlags, _dll), Int32, (Ptr{Cvoid}, ), handle);
autd_num_devices(handle) = ccall((:AUTDNumDevices, _dll), Int32, (Ptr{Cvoid}, ), handle);
autd_trans_position(handle, device_idx, local_trans_idx, x, y, z) = ccall((:AUTDTransPosition, _dll), Cvoid, (Ptr{Cvoid}, Int32, Int32, Ref{Float64}, Ref{Float64}, Ref{Float64}, ), handle, device_idx, local_trans_idx, x, y, z);
autd_trans_x_direction(handle, device_idx, local_trans_idx, x, y, z) = ccall((:AUTDTransXDirection, _dll), Cvoid, (Ptr{Cvoid}, Int32, Int32, Ref{Float64}, Ref{Float64}, Ref{Float64}, ), handle, device_idx, local_trans_idx, x, y, z);
autd_trans_y_direction(handle, device_idx, local_trans_idx, x, y, z) = ccall((:AUTDTransYDirection, _dll), Cvoid, (Ptr{Cvoid}, Int32, Int32, Ref{Float64}, Ref{Float64}, Ref{Float64}, ), handle, device_idx, local_trans_idx, x, y, z);
autd_trans_z_direction(handle, device_idx, local_trans_idx, x, y, z) = ccall((:AUTDTransZDirection, _dll), Cvoid, (Ptr{Cvoid}, Int32, Int32, Ref{Float64}, Ref{Float64}, Ref{Float64}, ), handle, device_idx, local_trans_idx, x, y, z);
autd_get_firmware_info_list_pointer(handle, out) = ccall((:AUTDGetFirmwareInfoListPointer, _dll), Int32, (Ptr{Cvoid}, Ref{Ptr{Cvoid}}, ), handle, out);
autd_get_firmware_info(p_firm_info_list, index, info) = ccall((:AUTDGetFirmwareInfo, _dll), Cvoid, (Ptr{Cvoid}, Int32, Ptr{UInt8}, ), p_firm_info_list, index, info);
autd_free_firmware_info_list_pointer(p_firm_info_list) = ccall((:AUTDFreeFirmwareInfoListPointer, _dll), Cvoid, (Ptr{Cvoid}, ), p_firm_info_list);
autd_gain_null(gain) = ccall((:AUTDGainNull, _dll), Cvoid, (Ref{Ptr{Cvoid}}, ), gain);
autd_gain_grouped(gain, handle) = ccall((:AUTDGainGrouped, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Ptr{Cvoid}, ), gain, handle);
autd_gain_grouped_add(grouped_gain, device_id, gain) = ccall((:AUTDGainGroupedAdd, _dll), Cvoid, (Ptr{Cvoid}, Int32, Ptr{Cvoid}, ), grouped_gain, device_id, gain);
autd_gain_focus(gain, x, y, z, amp) = ccall((:AUTDGainFocus, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Float64, Float64, Float64, Float64, ), gain, x, y, z, amp);
autd_gain_bessel_beam(gain, x, y, z, n_x, n_y, n_z, theta_z, amp) = ccall((:AUTDGainBesselBeam, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Float64, Float64, Float64, Float64, Float64, Float64, Float64, Float64, ), gain, x, y, z, n_x, n_y, n_z, theta_z, amp);
autd_gain_plane_wave(gain, n_x, n_y, n_z, amp) = ccall((:AUTDGainPlaneWave, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Float64, Float64, Float64, Float64, ), gain, n_x, n_y, n_z, amp);
autd_gain_custom(gain, amp, phase, size) = ccall((:AUTDGainCustom, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Array{Float64,1}, Array{Float64,1}, UInt64, ), gain, amp, phase, size);
autd_delete_gain(gain) = ccall((:AUTDDeleteGain, _dll), Cvoid, (Ptr{Cvoid}, ), gain);
autd_modulation_static(mod, amp) = ccall((:AUTDModulationStatic, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Float64, ), mod, amp);
autd_modulation_sine(mod, freq, amp, offset) = ccall((:AUTDModulationSine, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Int32, Float64, Float64, ), mod, freq, amp, offset);
autd_modulation_sine_squared(mod, freq, amp, offset) = ccall((:AUTDModulationSineSquared, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Int32, Float64, Float64, ), mod, freq, amp, offset);
autd_modulation_sine_legacy(mod, freq, amp, offset) = ccall((:AUTDModulationSineLegacy, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Float64, Float64, Float64, ), mod, freq, amp, offset);
autd_modulation_square(mod, freq, low, high, duty) = ccall((:AUTDModulationSquare, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Int32, Float64, Float64, Float64, ), mod, freq, low, high, duty);
autd_modulation_custom(mod, buffer, size, freq_div) = ccall((:AUTDModulationCustom, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Array{UInt8,1}, UInt64, UInt32, ), mod, buffer, size, freq_div);
autd_modulation_sampling_frequency_division(mod) = ccall((:AUTDModulationSamplingFrequencyDivision, _dll), UInt32, (Ptr{Cvoid}, ), mod);
autd_modulation_set_sampling_frequency_division(mod, freq_div) = ccall((:AUTDModulationSetSamplingFrequencyDivision, _dll), Cvoid, (Ptr{Cvoid}, UInt32, ), mod, freq_div);
autd_modulation_sampling_frequency(mod) = ccall((:AUTDModulationSamplingFrequency, _dll), Float64, (Ptr{Cvoid}, ), mod);
autd_delete_modulation(mod) = ccall((:AUTDDeleteModulation, _dll), Cvoid, (Ptr{Cvoid}, ), mod);
autd_point_stm(out) = ccall((:AUTDPointSTM, _dll), Cvoid, (Ref{Ptr{Cvoid}}, ), out);
autd_gain_stm(out, handle) = ccall((:AUTDGainSTM, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Ptr{Cvoid}, ), out, handle);
autd_point_stm_add(stm, x, y, z, shift) = ccall((:AUTDPointSTMAdd, _dll), Bool, (Ptr{Cvoid}, Float64, Float64, Float64, UInt8, ), stm, x, y, z, shift);
autd_gain_stm_add(stm, gain) = ccall((:AUTDGainSTMAdd, _dll), Bool, (Ptr{Cvoid}, Ptr{Cvoid}, ), stm, gain);
autd_get_gain_stm_mode(stm) = ccall((:AUTDGetGainSTMMode, _dll), UInt16, (Ptr{Cvoid}, ), stm);
autd_set_gain_stm_mode(stm, mode) = ccall((:AUTDSetGainSTMMode, _dll), Cvoid, (Ptr{Cvoid}, UInt16, ), stm, mode);
autdstm_set_frequency(stm, freq) = ccall((:AUTDSTMSetFrequency, _dll), Float64, (Ptr{Cvoid}, Float64, ), stm, freq);
autdstm_frequency(stm) = ccall((:AUTDSTMFrequency, _dll), Float64, (Ptr{Cvoid}, ), stm);
autdstm_sampling_frequency(stm) = ccall((:AUTDSTMSamplingFrequency, _dll), Float64, (Ptr{Cvoid}, ), stm);
autdstm_sampling_frequency_division(stm) = ccall((:AUTDSTMSamplingFrequencyDivision, _dll), UInt32, (Ptr{Cvoid}, ), stm);
autdstm_set_sampling_frequency_division(stm, freq_div) = ccall((:AUTDSTMSetSamplingFrequencyDivision, _dll), Cvoid, (Ptr{Cvoid}, UInt32, ), stm, freq_div);
autd_delete_stm(stm) = ccall((:AUTDDeleteSTM, _dll), Cvoid, (Ptr{Cvoid}, ), stm);
autd_stop(handle) = ccall((:AUTDStop, _dll), Int32, (Ptr{Cvoid}, ), handle);
autd_create_silencer(out, step, cycle) = ccall((:AUTDCreateSilencer, _dll), Cvoid, (Ref{Ptr{Cvoid}}, UInt16, UInt16, ), out, step, cycle);
autd_delete_silencer(config) = ccall((:AUTDDeleteSilencer, _dll), Cvoid, (Ptr{Cvoid}, ), config);
autd_send(handle, header, body) = ccall((:AUTDSend, _dll), Int32, (Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Cvoid}, ), handle, header, body);
autd_set_mod_delay(handle, device_idx, local_trans_idx, delay) = ccall((:AUTDSetModDelay, _dll), Cvoid, (Ptr{Cvoid}, Int32, Int32, UInt16, ), handle, device_idx, local_trans_idx, delay);
autd_create_mod_delay_config(out) = ccall((:AUTDCreateModDelayConfig, _dll), Cvoid, (Ref{Ptr{Cvoid}}, ), out);
autd_delete_mod_delay_config(config) = ccall((:AUTDDeleteModDelayConfig, _dll), Cvoid, (Ptr{Cvoid}, ), config);
autd_create_amplitudes(out, amp) = ccall((:AUTDCreateAmplitudes, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Float64, ), out, amp);
autd_delete_amplitudes(amplitudes) = ccall((:AUTDDeleteAmplitudes, _dll), Cvoid, (Ptr{Cvoid}, ), amplitudes);
autd_set_mode(handle, mode) = ccall((:AUTDSetMode, _dll), Cvoid, (Ptr{Cvoid}, UInt8, ), handle, mode);
autd_software_stm(out) = ccall((:AUTDSoftwareSTM, _dll), Cvoid, (Ref{Ptr{Cvoid}}, ), out);
autd_software_stm_set_strategy(stm, strategy) = ccall((:AUTDSoftwareSTMSetStrategy, _dll), Cvoid, (Ptr{Cvoid}, UInt8, ), stm, strategy);
autd_software_stm_add(stm, gain) = ccall((:AUTDSoftwareSTMAdd, _dll), Cvoid, (Ptr{Cvoid}, Ptr{Cvoid}, ), stm, gain);
autd_software_stm_start(handle, stm, cnt) = ccall((:AUTDSoftwareSTMStart, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Ptr{Cvoid}, Ptr{Cvoid}, ), handle, stm, cnt);
autd_software_stm_finish(cnt, handle) = ccall((:AUTDSoftwareSTMFinish, _dll), Cvoid, (Ref{Ptr{Cvoid}}, Ptr{Cvoid}, ), cnt, handle);
autd_software_stm_set_frequency(stm, freq) = ccall((:AUTDSoftwareSTMSetFrequency, _dll), Float64, (Ptr{Cvoid}, Float64, ), stm, freq);
autd_software_stm_frequency(stm) = ccall((:AUTDSoftwareSTMFrequency, _dll), Float64, (Ptr{Cvoid}, ), stm);
autd_software_stm_period(stm) = ccall((:AUTDSoftwareSTMPeriod, _dll), UInt64, (Ptr{Cvoid}, ), stm);
autd_software_stm_sampling_frequency(stm) = ccall((:AUTDSoftwareSTMSamplingFrequency, _dll), Float64, (Ptr{Cvoid}, ), stm);
autd_software_stm_sampling_period(stm) = ccall((:AUTDSoftwareSTMSamplingPeriod, _dll), UInt64, (Ptr{Cvoid}, ), stm);
autd_software_stm_set_sampling_period(stm, period) = ccall((:AUTDSoftwareSTMSetSamplingPeriod, _dll), Cvoid, (Ptr{Cvoid}, UInt64, ), stm, period);
autd_delete_software_stm(stm) = ccall((:AUTDDeleteSoftwareSTM, _dll), Cvoid, (Ptr{Cvoid}, ), stm);
end