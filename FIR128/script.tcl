############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
## Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
############################################################
open_project hw1_fir128
set_top fir128
add_files fir.cpp
add_files fir.h
add_files input.dat
add_files -tb fir_test.cpp -cflags "-Wno-unknown-pragmas"
add_files -tb out.gold.dat -cflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xc7z020clg400-1}
create_clock -period 10 -name default
config_cosim -tool xsim
config_export -format ip_catalog -output D:/Coursework/UCSD_WES/Hardware_for_Embed_Sys/project1/fir128/hw1_fir128 -rtl verilog
source "./hw1_fir128/solution1/directives.tcl"
csim_design -clean
csynth_design
cosim_design
export_design -rtl verilog -format ip_catalog -output D:/Coursework/UCSD_WES/Hardware_for_Embed_Sys/project1/fir128/hw1_fir128
