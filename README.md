# Simple ESP32-FPGA Digital Adder

**Hardware Used:**  
- Lilygo T-FPGA Development board  
- T-FPGA Shield

**Software Used for FPGA prototyping:**  
- GOWIN V1.9.10

**Software Used for ESP32 programming:**  
- Arduino IDE V1.8.19

---

### Adder Logic Implemented in FPGA

**Adder Circuit:**  
- **Inputs:**  
  - `a_in`, `b_in`, `c_in`  
  - Pin assignments (P5):  
    - `a_in`: P5:1 (FPGA_IOR2A)  
    - `b_in`: P5:3 (FPGA_IOR11A)  
    - `c_in`: P5:5 (FPGA_IOR15A)  

- **Outputs:**  
  - `sum`, `carry`  
  - Pin assignments (P5):  
    - `sum`: P5:2 (FPGA_IOR2B)  
    - `carry`: P5:4 (FPGA_IOR11B)  

---

### Testbench on ESP32

**Testbench Circuit:**  
- **Inputs:**  
  - `sum`, `carry`  
  - Pin assignments (P6):  
    - `sum`: P6:2 (GPIO 21)  
    - `carry`: P6:4 (GPIO 17)  

- **Outputs:**  
  - `a_in`, `b_in`, `c_in`  
  - Pin assignments (P6):  
    - `a_in`: P6:1 (GPIO 18)  
    - `b_in`: P6:3 (GPIO 16)  
    - `c_in`: P6:5 (GPIO 12)  

---

### Simulating Circuit Auto mode
- `Each of the a_in, b_in and c_in are toggled with accurate timing manipulation using millis() (millis is a lifesaver :)`
