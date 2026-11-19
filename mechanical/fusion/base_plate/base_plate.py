"""Fusion script: first sketch of the robot's base plate.

Creates one sketch on the XY plane and extrudes the plate:
  - the plate outline, sized to cover both TT motors underneath it,
  - M3 clearance holes for standoffs, inside the motors so screws don't hit them,
  - a cable hole on the centre line,
  - construction outlines (dashed, not part of the plate) of the two
    TT motors and wheels, to check track width and clearances.

Coordinates, seen from above:
  X = along the wheel axle (left/right), centred on the robot
  Y = forward/back; the axle lies on Y = 0
The motors sit under the plate, flush with its side edges, with their bodies
pointing backwards from the axle. The wheels sit outside the plate.

Values marked MEASURE are typical sizes, not measured from your parts.
Measure your own parts with calipers and update them before printing.
To change the design, edit the numbers below and run the script again
in a new design.
"""

import traceback

import adsk.core
import adsk.fusion

# ---------------------------------------------------------------- plate (mm)
PLATE_WIDTH = 100.0      # along the axle; the motors sit under its side edges
PLATE_FRONT = 30.0       # how far the plate reaches in front of the axle
PLATE_MARGIN_BACK = 3.0  # extra plate behind the end of the motors
PLATE_THICKNESS = 3.0

# ---------------------------------------------------------------- holes (mm)
M3_CLEARANCE = 3.3       # from your hole test print
STANDOFF_INSET = 6.0     # hole centre distance from the motors and plate edges
CABLE_HOLE_DIAMETER = 12.0

# ------------------------------------------- TT motor, seen from above (mm)
MOTOR_LENGTH = 70.0          # MEASURE: gearbox + motor, along Y
MOTOR_WIDTH = 22.5           # MEASURE: along the shaft (X)
MOTOR_SHAFT_FROM_END = 11.0  # MEASURE: shaft centre to the gearbox's front end

# ------------------------------------------------ TT wheel, from above (mm)
WHEEL_DIAMETER = 66.0    # MEASURE
WHEEL_WIDTH = 26.0       # MEASURE
WHEEL_GAP = 3.0          # gap between motor and wheel


def mm(value):
    """The Fusion API works in centimetres."""
    return value / 10.0


def point(x, y):
    return adsk.core.Point3D.create(mm(x), mm(y), 0)


def rectangle(sketch, x0, y0, x1, y1, construction=False):
    lines = sketch.sketchCurves.sketchLines.addTwoPointRectangle(point(x0, y0), point(x1, y1))
    for i in range(lines.count):
        lines.item(i).isConstruction = construction
    return lines


def circle(sketch, x, y, diameter):
    return sketch.sketchCurves.sketchCircles.addByCenterRadius(point(x, y), mm(diameter / 2))


def run(context):
    ui = None
    try:
        app = adsk.core.Application.get()
        ui = app.userInterface
        design = adsk.fusion.Design.cast(app.activeProduct)
        if not design:
            ui.messageBox("Open a design in the Design workspace first.")
            return
        root = design.rootComponent

        sketch = root.sketches.add(root.xYConstructionPlane)
        sketch.name = "base_plate"

        # Layout along Y: motor front (shaft end) and back, then the plate.
        motor_front = MOTOR_SHAFT_FROM_END
        motor_back = MOTOR_SHAFT_FROM_END - MOTOR_LENGTH
        plate_front = PLATE_FRONT
        plate_back = motor_back - PLATE_MARGIN_BACK
        half_w = PLATE_WIDTH / 2

        # Plate outline
        rectangle(sketch, -half_w, plate_back, half_w, plate_front)

        # Standoff holes, inboard of the motors so their screws clear them
        motor_inner = half_w - MOTOR_WIDTH
        hx = motor_inner - STANDOFF_INSET
        for sx in (-1, 1):
            for y in (plate_front - STANDOFF_INSET, plate_back + STANDOFF_INSET):
                circle(sketch, sx * hx, y, M3_CLEARANCE)

        # Cable hole, centred between the motors, halfway down the plate
        circle(sketch, 0, (plate_front + plate_back) / 2, CABLE_HOLE_DIAMETER)

        # Motors and wheels (construction outlines), mirrored left/right
        wheel_outer = half_w + WHEEL_GAP + WHEEL_WIDTH
        for side in (-1, 1):
            rectangle(sketch, side * motor_inner, motor_back, side * half_w, motor_front,
                      construction=True)
            r = WHEEL_DIAMETER / 2
            rectangle(sketch, side * (half_w + WHEEL_GAP), -r, side * wheel_outer, r,
                      construction=True)

        # Axle line (construction), across the full width of the wheels
        axle = sketch.sketchCurves.sketchLines.addByTwoPoints(point(-wheel_outer, 0),
                                                              point(wheel_outer, 0))
        axle.isConstruction = True

        # Extrude the plate: the largest profile is the outline minus the holes.
        profiles = [sketch.profiles.item(i) for i in range(sketch.profiles.count)]
        plate = max(profiles, key=lambda p: p.areaProperties().area)
        extrudes = root.features.extrudeFeatures
        extrude_input = extrudes.createInput(plate, adsk.fusion.FeatureOperations.NewBodyFeatureOperation)
        extrude_input.setDistanceExtent(False, adsk.core.ValueInput.createByReal(mm(PLATE_THICKNESS)))
        extrude = extrudes.add(extrude_input)
        extrude.bodies.item(0).name = "base_plate"

        depth = plate_front - plate_back
        ui.messageBox(
            f"Base plate created: {PLATE_WIDTH:g} × {depth:g} × {PLATE_THICKNESS:g} mm.\n"
            f"Width over the wheels: {2 * wheel_outer:g} mm.\n\n"
            "Dashed outlines show the motors and wheels. Values marked MEASURE "
            "in the script are typical sizes; check them against your parts."
        )
    except Exception:
        if ui:
            ui.messageBox(f"Failed:\n{traceback.format_exc()}")
