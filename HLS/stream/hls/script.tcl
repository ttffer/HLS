############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
############################################################
open_project stream_int_test_rom
set_top top
add_files stream_int_test_rom/lrsw4.cpp
add_files stream_int_test_rom/lrsw2.cpp
add_files stream_int_test_rom/lrsw0.cpp
add_files stream_int_test_rom/lrsb4.cpp
add_files stream_int_test_rom/lrsb2.cpp
add_files stream_int_test_rom/lrsb0.cpp
add_files stream_int_test_rom/int_stream.h
add_files stream_int_test_rom/int_stream.cpp
add_files -tb stream_int_test_rom/tb.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xc7z020-clg400-1}
create_clock -period 10 -name default
config_export -format ip_catalog -rtl verilog
source "./stream_int_test_rom/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -rtl verilog -format ip_catalog
