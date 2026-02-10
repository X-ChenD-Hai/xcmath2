#!/usr/bin/env python3
"""
Python test data generator for xcmath transform matrices.
Generates expected values for comparing with C++ implementation.
"""

import math

def radians(degrees):
    """Convert degrees to radians."""
    return degrees * math.pi / 180.0

def generate_rotation_data():
    """Generate test data for rotation matrices."""
    test_cases = []

    # 90 degree rotations
    angle_90 = 90.0
    rad_90 = radians(angle_90)

    # Expected 3x3 rotation matrices for 90 degrees
    test_cases.append({
        "name": "rotate_x_90_degrees",
        "angle": angle_90,
        "expected_3x3": [
            [1.0, 0.0, 0.0],
            [0.0, 0.0, -1.0],
            [0.0, 1.0, 0.0]
        ]
    })

    test_cases.append({
        "name": "rotate_y_90_degrees",
        "angle": angle_90,
        "expected_3x3": [
            [0.0, 0.0, 1.0],
            [0.0, 1.0, 0.0],
            [-1.0, 0.0, 0.0]
        ]
    })

    test_cases.append({
        "name": "rotate_z_90_degrees",
        "angle": angle_90,
        "expected_3x3": [
            [0.0, -1.0, 0.0],
            [1.0, 0.0, 0.0],
            [0.0, 0.0, 1.0]
        ]
    })

    # 45 degree rotations
    angle_45 = 45.0
    rad_45 = radians(angle_45)
    c_45 = math.cos(rad_45)
    s_45 = math.sin(rad_45)

    test_cases.append({
        "name": "rotate_x_45_degrees",
        "angle": angle_45,
        "expected_3x3": [
            [1.0, 0.0, 0.0],
            [0.0, c_45, -s_45],
            [0.0, s_45, c_45]
        ]
    })

    test_cases.append({
        "name": "rotate_y_45_degrees",
        "angle": angle_45,
        "expected_3x3": [
            [c_45, 0.0, s_45],
            [0.0, 1.0, 0.0],
            [-s_45, 0.0, c_45]
        ]
    })

    test_cases.append({
        "name": "rotate_z_45_degrees",
        "angle": angle_45,
        "expected_3x3": [
            [c_45, -s_45, 0.0],
            [s_45, c_45, 0.0],
            [0.0, 0.0, 1.0]
        ]
    })

    return test_cases

def generate_shear_data():
    """Generate test data for shear matrices."""
    # 3x3 shear factors: [sh_xy, sh_xz, sh_yx, sh_yz, sh_zx, sh_zy]
    test_cases = []

    # Simple xy shear
    test_cases.append({
        "name": "shear_xy",
        "factors": [1.0, 0.0, 0.0, 0.0, 0.0, 0.0],
        "expected_3x3": [
            [1.0, 1.0, 0.0],
            [0.0, 1.0, 0.0],
            [0.0, 0.0, 1.0]
        ]
    })

    # Combined shear
    test_cases.append({
        "name": "shear_xy_xz",
        "factors": [0.5, 1.0, 0.0, 0.0, 0.0, 0.0],
        "expected_3x3": [
            [1.0, 0.5, 1.0],
            [0.0, 1.0, 0.0],
            [0.0, 0.0, 1.0]
        ]
    })

    return test_cases

def generate_ortho_data():
    """Generate test data for orthographic projection matrices."""
    test_cases = []

    # Standard ortho: left=-1, right=1, bottom=-1, top=1, near=-1, far=1
    test_cases.append({
        "name": "ortho_standard",
        "params": {"left": -1.0, "right": 1.0, "bottom": -1.0, "top": 1.0, "near": -1.0, "far": 1.0},
        "expected_4x4": [
            [1.0, 0.0, 0.0, 0.0],
            [0.0, 1.0, 0.0, 0.0],
            [0.0, 0.0, -1.0, 0.0],
            [0.0, 0.0, 0.0, 1.0]
        ]
    })

    # Ortho with different bounds
    test_cases.append({
        "name": "ortho_custom",
        "params": {"left": 0.0, "right": 10.0, "bottom": 0.0, "top": 5.0, "near": -1.0, "far": 1.0},
        "expected_4x4": [
            [0.2, 0.0, 0.0, -1.0],
            [0.0, 0.4, 0.0, -1.0],
            [0.0, 0.0, -1.0, 0.0],
            [0.0, 0.0, 0.0, 1.0]
        ]
    })

    return test_cases

def generate_frustum_data():
    """Generate test data for frustum projection matrices."""
    test_cases = []

    # Standard frustum: symmetric view
    test_cases.append({
        "name": "frustum_standard",
        "params": {"left": -1.0, "right": 1.0, "bottom": -1.0, "top": 1.0, "near": 1.0, "far": 10.0},
        "expected_4x4": [
            [1.0, 0.0, 0.0, 0.0],
            [0.0, 1.0, 0.0, 0.0],
            [0.0, 0.0, -1.3636, -2.2222],
            [0.0, 0.0, -1.0, 0.0]
        ]
    })

    # Asymmetric frustum
    test_cases.append({
        "name": "frustum_asymmetric",
        "params": {"left": -2.0, "right": 1.0, "bottom": -1.0, "top": 1.0, "near": 1.0, "far": 100.0},
        "expected_4x4": [
            [0.6667, 0.0, 0.0, 0.3333],
            [0.0, 1.0, 0.0, 0.0],
            [0.0, 0.0, -1.0202, -2.0404],
            [0.0, 0.0, -1.0, 0.0]
        ]
    })

    return test_cases

def generate_look_at_data():
    """Generate test data for look_at matrices."""
    test_cases = []

    # Simple look_at: eye at origin, looking at +Z
    eye = [0.0, 0.0, 0.0]
    center = [0.0, 0.0, 1.0]
    up = [0.0, 1.0, 0.0]

    # Forward = (0, 0, 1) normalized
    # Right = up × forward = (1, 0, 0) normalized
    # NewUp = forward × right = (0, 1, 0)
    test_cases.append({
        "name": "look_at_origin",
        "eye": eye,
        "center": center,
        "up": up,
        "expected_4x4": [
            [1.0, 0.0, 0.0, 0.0],
            [0.0, 1.0, 0.0, 0.0],
            [0.0, 0.0, -1.0, 0.0],
            [0.0, 0.0, 0.0, 1.0]
        ]
    })

    # Eye at (0, 0, 5), looking at origin
    eye = [0.0, 0.0, 5.0]
    center = [0.0, 0.0, 0.0]
    up = [0.0, 1.0, 0.0]
    forward = [0.0, 0.0, -1.0]
    right = [1.0, 0.0, 0.0]
    new_up = [0.0, 1.0, 0.0]

    test_cases.append({
        "name": "look_at_eye_z5",
        "eye": eye,
        "center": center,
        "up": up,
        "expected_4x4": [
            [1.0, 0.0, 0.0, 0.0],
            [0.0, 1.0, 0.0, 0.0],
            [0.0, 0.0, -1.0, 0.0],
            [0.0, 0.0, 0.0, 1.0]
        ]
    })

    return test_cases

def format_float(f, precision=6):
    """Format float with given precision."""
    return round(f, precision)

def format_matrix(matrix, name, precision=6):
    """Format matrix as C++ initializer list."""
    lines = []
    lines.append(f"// {name}")
    lines.append("{")
    for i, row in enumerate(matrix):
        formatted_row = ", ".join([format_float(x, precision) for x in row])
        if i < len(matrix) - 1:
            lines.append(f"    {{{formatted_row}}},")
        else:
            lines.append(f"    {{{formatted_row}}}")
    lines.append("}")
    return "\n".join(lines)

def main():
    print("=== Rotation Matrix Test Data ===")
    for tc in generate_rotation_data():
        print(f"\n// {tc['name']}")
        print(f"angle = {tc['angle']} degrees")
        print(format_matrix(tc['expected_3x3'], tc['name']))

    print("\n\n=== Shear Matrix Test Data ===")
    for tc in generate_shear_data():
        print(f"\n// {tc['name']}")
        print(f"factors = [{', '.join(map(str, tc['factors']))}]")
        print(format_matrix(tc['expected_3x3'], tc['name']))

    print("\n\n=== Ortho Matrix Test Data ===")
    for tc in generate_ortho_data():
        print(f"\n// {tc['name']}")
        p = tc['params']
        print(f"left={p['left']}, right={p['right']}, bottom={p['bottom']}, top={p['top']}, near={p['near']}, far={p['far']}")
        print(format_matrix(tc['expected_4x4'], tc['name']))

    print("\n\n=== Frustum Matrix Test Data ===")
    for tc in generate_frustum_data():
        print(f"\n// {tc['name']}")
        p = tc['params']
        print(f"left={p['left']}, right={p['right']}, bottom={p['bottom']}, top={p['top']}, near={p['near']}, far={p['far']}")
        print(format_matrix(tc['expected_4x4'], tc['name']))

    print("\n\n=== LookAt Matrix Test Data ===")
    for tc in generate_look_at_data():
        print(f"\n// {tc['name']}")
        print(f"eye=({', '.join(map(str, tc['eye']))}), center=({', '.join(map(str, tc['center']))}), up=({', '.join(map(str, tc['up']))})")
        print(format_matrix(tc['expected_4x4'], tc['name']))

if __name__ == "__main__":
    main()
