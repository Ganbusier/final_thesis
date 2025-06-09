#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
TXT to PLY Point Cloud Converter
Convert TXT point cloud files in the output folder to PLY format
"""

import os
import glob

def read_txt_file(txt_path):
    """
    Read TXT file and extract point coordinates
    
    Args:
        txt_path (str): Path to TXT file
        
    Returns:
        list: List of (x, y, z) coordinates
    """
    points = []
    
    try:
        with open(txt_path, 'r', encoding='utf-8') as file:
            for line_num, line in enumerate(file, 1):
                line = line.strip()
                if line:  # Skip empty lines
                    try:
                        # Split by comma and convert to float
                        coords = line.split(',')
                        if len(coords) >= 3:
                            x, y, z = float(coords[0]), float(coords[1]), float(coords[2])
                            points.append((x, y, z))
                    except ValueError:
                        print(f"Warning: Skip invalid line {line_num} in {txt_path}: {line}")
                        continue
    except Exception as e:
        print(f"Error reading TXT file {txt_path}: {str(e)}")
        raise
    
    return points

def write_ply_file(points, ply_path):
    """
    Write points to PLY file format
    
    Args:
        points (list): List of (x, y, z) coordinates
        ply_path (str): Output PLY file path
    """
    try:
        with open(ply_path, 'w', encoding='utf-8') as file:
            # Write PLY header
            file.write("ply\n")
            file.write("format ascii 1.0\n")
            file.write("comment Converted from TXT point cloud\n")
            file.write(f"element vertex {len(points)}\n")
            file.write("property float x\n")
            file.write("property float y\n")
            file.write("property float z\n")
            file.write("end_header\n")
            
            # Write vertex data
            for x, y, z in points:
                file.write(f"{x} {y} {z}\n")
                
    except Exception as e:
        print(f"Error writing PLY file {ply_path}: {str(e)}")
        raise

def convert_txt_to_ply(txt_path, ply_path):
    """
    Convert TXT file to PLY format
    
    Args:
        txt_path (str): Input TXT file path
        ply_path (str): Output PLY file path
    """
    try:
        # Read TXT file
        points = read_txt_file(txt_path)
        
        if not points:
            print(f"Warning: No valid points found in {os.path.basename(txt_path)}")
            return
        
        # Write PLY file
        write_ply_file(points, ply_path)
        
        print(f"Successfully converted: {os.path.basename(txt_path)} -> {os.path.basename(ply_path)} ({len(points)} points)")
        
    except Exception as e:
        print(f"Conversion failed {os.path.basename(txt_path)}: {str(e)}")

def main():
    """Main function"""
    # Input and output directories
    input_dir = "output"
    output_dir = "output"
    
    # Create output directory
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"Created output directory: {output_dir}")
    
    # Find all TXT files in the output directory
    txt_files = glob.glob(os.path.join(input_dir, "*.txt"))
    
    if not txt_files:
        print(f"No TXT files found in {input_dir} directory")
        return
    
    print(f"Found {len(txt_files)} TXT files, starting conversion...")
    
    # Convert each TXT file
    for txt_path in txt_files:
        # Generate output filename
        base_name = os.path.splitext(os.path.basename(txt_path))[0]
        ply_path = os.path.join(output_dir, f"{base_name}.ply")
        
        # Execute conversion
        convert_txt_to_ply(txt_path, ply_path)
    
    print("\nConversion completed!")
    print(f"Results saved in '{output_dir}' directory")

if __name__ == "__main__":
    main() 