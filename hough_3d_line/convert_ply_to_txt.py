#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
PLY File Converter
Convert point cloud PLY files in the input folder to the TXT format (comma-separated coordinates)
"""

import os
import glob

def read_ply_file(ply_path):
    """
    Read PLY file and extract vertex coordinates
    
    Args:
        ply_path (str): Path to PLY file
        
    Returns:
        list: List of (x, y, z) coordinates
    """
    vertices = []
    
    # Try different encoding methods
    encodings = ['utf-8', 'latin-1', 'cp1252']
    file_content = None
    
    for encoding in encodings:
        try:
            with open(ply_path, 'r', encoding=encoding) as file:
                file_content = file.readlines()
                break
        except UnicodeDecodeError:
            continue
    
    if file_content is None:
        raise ValueError(f"Cannot read file {ply_path}, tried all encoding methods")
    
    lines = file_content
    
    # Find vertex count and header end position
    vertex_count = 0
    header_end = False
    start_index = 0
    
    for i, line in enumerate(lines):
        line = line.strip()
        
        if line.startswith('element vertex'):
            vertex_count = int(line.split()[-1])
            
        if line == 'end_header':
            header_end = True
            start_index = i + 1
            break
    
    if not header_end:
        raise ValueError(f"PLY file header end marker not found: {ply_path}")
        
    # Read vertex data
    for i in range(start_index, min(start_index + vertex_count, len(lines))):
        line = lines[i].strip()
        if line:  # Skip empty lines
            coords = line.split()
            if len(coords) >= 3:
                try:
                    x, y, z = float(coords[0]), float(coords[1]), float(coords[2])
                    vertices.append((x, y, z))
                except ValueError:
                    continue
    
    return vertices

def convert_ply_to_txt(ply_path, output_path):
    """
    Convert PLY file to TXT format
    
    Args:
        ply_path (str): Input PLY file path
        output_path (str): Output TXT file path
    """
    try:
        # Read PLY file
        vertices = read_ply_file(ply_path)
        
        # Write TXT file
        with open(output_path, 'w', encoding='utf-8') as file:
            for x, y, z in vertices:
                # Use the same format as example.txt: 8 decimal places
                file.write(f"{x:.8f},{y:.8f},{z:.8f}\n")
        
        print(f"Successfully converted: {os.path.basename(ply_path)} -> {os.path.basename(output_path)} ({len(vertices)} vertices)")
        
    except Exception as e:
        print(f"Conversion failed {os.path.basename(ply_path)}: {str(e)}")

def main():
    """Main function"""
    # Input and output directories
    input_dir = "input"
    output_dir = "input"
    
    # Create output directory
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Find all PLY files
    ply_files = glob.glob(os.path.join(input_dir, "*.ply"))
    
    if not ply_files:
        print(f"No PLY files found in {input_dir} directory")
        return
    
    print(f"Found {len(ply_files)} PLY files, starting conversion...")
    
    # Convert each PLY file
    for ply_path in ply_files:
        # Generate output filename
        base_name = os.path.splitext(os.path.basename(ply_path))[0]
        output_path = os.path.join(output_dir, f"{base_name}.txt")
        
        # Execute conversion
        convert_ply_to_txt(ply_path, output_path)
    
    print("\nConversion completed!")
    print(f"Results saved in '{output_dir}' directory")

if __name__ == "__main__":
    main() 