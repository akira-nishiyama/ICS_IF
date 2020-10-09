
# Component diagram

```plantuml
[ICS_IF_MAIN]
[ICS_IF_TX]
[ICS_IF_RX]
ICS_IF_MAIN --> ICS_IF_TX
ICS_IF_MAIN <-- ICS_IF_RX

(axi4-lite) <-r-> ICS_IF_MAIN
(bram) <-l-> ICS_IF_MAIN

ICS_IF_TX --> (uart_tx)
ICS_IF_RX <-- (uart_rx)

```

---

# Registers

(note:SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

## Control Signals(0x00)
```{.wavedrom}
{"reg" : [
  {"bits":1, "name": "st", "attr": ["rw","CH"] },
  {"bits":1, "name": "dn", "attr": ["r","CR"] },
  {"bits":1, "name": "idl", "attr": ["r"] },
  {"bits":1, "name": "rdy", "attr": ["r"] },
  {"bits":3 },
  {"bits":1, "name": "ars", "attr": ["rw"] },
  {"bits":24}
]}
```
initial:0

**st** = ap_start, see UG902  
**dn** = ap_done, see UG902  
**idl** = ap_idle, see UG902  
**rdy** = ap_ready, see UG902  
**ars** = auto_restart, see UG902  

---

## Global Interrupt Enable(0x04)

```{.wavedrom}
{"reg" : [
    {"bits":1, "name":"gie", "attr":["rw"]},
    {"bits":31}
]}
```

**gie** = Global Interrupt Enable, see UG902

---

## IP Interrupt Enable(0x08)

```{.wavedrom}
{"reg" : [
    {"bits":1, "name":"ch0", "attr":["rw"]},
    {"bits":1, "name":"ch1", "attr":["rw"]},
    {"bits":30}
]}
```
initial:0

**ch0** = IP Interrupt Enable Channel 0(ap_done), see UG902

**ch1** = IP Interrupt Enable Channel 1(ap_ready), see UG902

---

## IP Interrupt Status(0x0c)

```{.wavedrom}
{"reg" : [
    {"bits":1, "name":"ch0", "attr":["r","TOW"]},
    {"bits":1, "name":"ch1", "attr":["r","TOW"]},
    {"bits":30}
]}
```
initial:0

**ch0** = IP Interrupt Status Channel 0(ap_done), see UG902

**ch1** = IP Interrupt Status Channel 1(ap_ready), see UG902

---

## Baudrate Config(0x10)

```{.wavedrom}
{"reg" : [
    {"bits":3, "name":"period", "attr":["rw"]},
    {"bits":29}
]}
```
initial:0

**period** defines baudrate of ics.  
0:115200bps  
1:625000bps  
2:1152000bps  

---

## reserved(0x14)

---

## Number of Servos(0x18)

```{.wavedrom}
{"reg" : [
    {"bits":6, "name":"number_of_servos", "attr":["rw"]},
    {"bits":26}
]}
```
initial:0

**number_of_servos** defines number of servos that connected through this ics_if ip.  
   0:stop communication(not configured)  
1-32:start communication. cyclic fields are used upto specified number.  
  ex)set 1 uses only cyclic0_xx_0 field. set 2 uses cyclic0_xx_0 and   cyclic0_xx_1 fields.  
33- :stop communication(invalid)  

---

## reserved(0x1c)

---

## Cyclic0 config(0x20)
```{.wavedrom}
{"reg" : [
    {"bits":16, "name":"cyclic0_interval", "attr":["rw"]},
    {"bits":15},
    {"bits":1,  "name":"en", "attr":["rw"]}
]}
```
initial:0

**cyclic0_interval** defines cyclic0(position read/write) communication interval.  
write 1 for 0.65536 ms.

**en** is enable of cyclic0.  
0:disable cyclic0  
1:enable cyclic0  

---

## reserved(0x24)

---

## Cyclic1 config(0x28)
```{.wavedrom}
{"reg" : [
    {"bits":16, "name":"cyclic1_interval", "attr":["rw"]},
    {"bits":15},
    {"bits":1,  "name":"en", "attr":["rw"]}
]}
```
initial:0

**cyclic1_interval** defines cyclic1(current read) communication interval.  
write 1 for 0.65536 ms.  

**en** is enable of cyclic0.  
0:disable cyclic1  
1:enable cyclic1  

---

## reserved(0x2c)

---

## Cyclic2 config(0x30)
```{.wavedrom}
{"reg" : [
    {"bits":16, "name":"cyclic2_interval", "attr":["rw"]},
    {"bits":15},
    {"bits":1,  "name":"en", "attr":["rw"]}
]}
```
initial:0

**cyclic2_interval** defines cyclic2(temprature read) communication interval.  
write 1 for 0.65536 ms.  

**en** is enable of cyclic2.  
0:disable cyclic2  
1:enable cyclic2  

---

## reserved(0x34)

---

## Command Error Counter(0x38)
```{.wavedrom}
{"reg" : [
    {"bits":8, "name":"cmd_error_cnt", "attr":["r"]},
    {"bits":24}
]}
```
initial:0

**cmd_error_cnt** is a counter that counts up when invalid CMD_R detected.

---

## reserved(0x3c)

---
