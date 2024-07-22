# simple-esp32-fpga-digital-adder
 adder logic implemented in fpga, digital read/write done by esp32

adder implemented on fpga
adder ckt:
input: a_in, b_in, c_in - P5: 1,3,5 (FPGA_IOR2A, FPGA_IOR11A, FPGA_IOR15A)
output: sum, carry - P5: 2, 4 (FPGA_IOR2B, FPGA_IOR11B)

adder tb on esp32
tb ckt:
input: sum, carry - P6: 2,4 (GPIO 21,17)
output: a_in, b_in, c_in - P6: 1,3,5 (GPIO 18, 16, 12)


Simulating ckt manually:

write at esp32 usb_serial, 

"a_in=0"
"a_in=1"
"b_in=0"
"b_in=1"
"c_in=0"
"c_in=1"
