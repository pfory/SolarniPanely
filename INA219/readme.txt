float getBusVoltage_V(void);  

Reads the voltage between GND and V-.  This is the total voltage seen by the circuit under test.  (Supply voltage - shunt voltage).  

The return value is in Volts.

float getShuntVoltage_mV(void);  

Reads the voltage between V- and V+.  This is the measured voltage drop across the shunt resistor.  

The return value is in Milivolts.

float getCurrent_mA(void);

Reads the current, derived via Ohms Law from the measured shunt voltage.

The return value is in Milliamps.