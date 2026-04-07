# Sample Datasheet: EIL Temperature and Vibration Advisory Model

## 1. Purpose
This sample Embedded Intelligence Layer (EIL) monitors temperature, vibration, and a cover-open status bit. It is intended to run alongside host firmware and provide advisory outputs without replacing deterministic control logic.

## 2. Intended Use
The model is suitable for a demo or starter integration in:
- package monitoring
- motor or enclosure monitoring
- industrial controller health advisory
- thermal drift warning systems

## 3. Inputs
### temperature_c
- Type: numeric
- Unit: C
- Typical range: 0 to 80
- Description: decoded temperature channel supplied by host firmware

### vibration_g
- Type: numeric
- Unit: g
- Typical range: 0.0 to 2.0
- Description: decoded vibration or shock magnitude supplied by host firmware

### cover_open
- Type: bit
- States: OPEN / CLOSED
- Description: protective cover or tamper state supplied by host firmware

## 4. Outputs
### anomaly_score
Normalized score from 0.0 to 1.0 representing deviation from expected operating behavior.

### advisory_score
Normalized score from 0.0 to 1.0 representing a simple trained risk estimate from the current input values.

### alert_state
- 0 = Normal
- 1 = Warning
- 2 = Fail

### reason_code
- NORMAL
- THERMAL_DRIFT
- VIBRATION_HIGH
- ACCESS_OPEN

## 5. Thresholds
- Warning threshold: 0.50
- Fail threshold: 0.80

## 6. Integration Model
The host firmware owns all control decisions. The EIL only evaluates already-decoded signals and returns advisory outputs. Typical firmware behavior:
1. decode raw sensors
2. populate an InputFrame
3. call `evaluate()`
4. log or display alert and reason
5. merge EIL outputs with deterministic rules for final action

## 7. Safety Position
This sample EIL is advisory only. It is not intended to directly actuate hardware without deterministic firmware review.

## 8. Resource Profile
This sample is intentionally small:
- no hidden neural layers
- no external runtime dependency
- simple float math
- portable C++ structure for MCU or MPU projects
