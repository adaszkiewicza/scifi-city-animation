# LAB4

LAB4 for computer graphics course. Object files are stolen form somewhere,
but I made sure they have proper licences (by stolen I mean I don't remember their origin, 
they were free). City not rendering properly - the *.obj file probably has incorrect normals.

## Installation

```bash
mkdir build
cd build
cmake ..
make
```

## Execution

To run execute (make sure you are in `build` directory):

```bash
./lab4

```

## Usage

You can manipulate the animation by pressing following keys:

### F

Fog on/off.

### C

Switch to next camera. There is a total of 4 - static camera, freecam (you can move this camera with
arrow keys and change it's target with mouse), camera following the ship and TPP camera.

### W

Wires on/off (polygon edges).

### A

Start/stop ship's movement.

### M

Skip day cycle to midnight.

### N

Skip day cycle to noon.

### V

Ship vibrations on/off.

### 1 - 8 (num keys)

Change the target of spotlights on the ship. 1 - left, 2 - right, 3 - up, 4 - down.
Keys through 4 to 8 behave in the same fashion.

### S

Switch between different shading modes: Phong, Gouraud and flat.

