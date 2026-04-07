# User Guide: Integrating the Sample EIL

## Overview
This guide shows how to integrate the sample Embedded Intelligence Layer (EIL) into an existing firmware application. The EIL does not replace your firmware. It adds a small advisory model on top of it.

## Project Files
- `model.config.json` - EIL model description
- `include/eil_types.hpp` - input and output types
- `include/eil_model.hpp` - EIL class interface
- `src/eil_model.cpp` - EIL implementation
- `src/main.cpp` - simple host-side example
- `docs/SAMPLE_DATASHEET.md` - model behavior reference

## Integration Steps
### 1. Copy the model files
Copy `include/eil_types.hpp`, `include/eil_model.hpp`, and `src/eil_model.cpp` into your firmware or application repository.

### 2. Decode your real signals first
The EIL expects host firmware to provide already-decoded values. For example:
- ADC or sensor driver converts raw temperature to `temperature_c`
- vibration algorithm or driver provides `vibration_g`
- GPIO or protocol state provides `cover_open`

### 3. Populate the input frame
Create an `eil::InputFrame` and fill it with your current system values.

### 4. Run the model
Call `eil.evaluate(input_frame)` once per loop, once per task cycle, or at your chosen monitoring cadence.

### 5. Use the results
Use the returned `OutputFrame` to:
- log warnings
- display advisory status to the user
- trigger deterministic safety checks
- store reason codes for diagnostics

## Minimal Host Example
Use this pattern in your application loop:

1. Read sensors
2. Convert to engineering units
3. Fill `InputFrame`
4. Call `evaluate()`
5. Merge `alert_state` and `reason_code` into your normal firmware policy

## Example Integration Policy
- If `alert_state == Normal`, continue normal operation
- If `alert_state == Warning`, increase monitoring or warn the user
- If `alert_state == Fail`, raise a deterministic alarm and let firmware decide the response

## Build Example
From the project root:

```bash
cmake -S . -B build
cmake --build build
./build/eil_sample
```

## Porting Notes
- replace stdout printing with UART, logging, or UI code
- replace float math if fixed-point is preferred
- call the EIL from a task, timer callback, or control supervisor loop
- thresholds can be moved to configuration or generated code

## Key Principle
The EIL is a parallel intelligence layer. It improves visibility and early warning, but the firmware remains authoritative.
