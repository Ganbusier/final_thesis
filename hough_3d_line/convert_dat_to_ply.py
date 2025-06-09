#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
DAT to PLY Line Converter
Convert output line DAT files in the output folder to PLY line format
"""

import os
import glob
import re

def parse_endpoints_from_line(line):
    """
    Parse endpoints from a DAT file line
    
    Args:
        line (str): Line from DAT file
        
    Returns:
        tuple: ((x1, y1, z1), (x2, y2, z2)) or None if parsing fails
    """
    try:
        # Use regex to find endpoints pattern: endpoints=[(x1,y1,z1), (x2,y2,z2)]
        pattern = r'endpoints=\[\(([^)]+)\), \(([^)]+)\)\]'
        match = re.search(pattern, line)
        
        if match:
            # Parse first endpoint
            coords1 = match.group(1).split(',')
            x1, y1, z1 = float(coords1[0]), float(coords1[1]), float(coords1[2])
            
            # Parse second endpoint
            coords2 = match.group(2).split(',')
            x2, y2, z2 = float(coords2[0]), float(coords2[1]), float(coords2[2])
            
            return ((x1, y1, z1), (x2, y2, z2))
        else:
            return None
            
    except (ValueError, IndexError) as e:
        return None

def read_dat_file(dat_path):
    """
    Read DAT file and extract line segments
    
    Args:
        dat_path (str): Path to DAT file
        
    Returns:
        list: List of line segments, each as ((x1,y1,z1), (x2,y2,z2))
    """
    line_segments = []
    
    try:
        with open(dat_path, 'r', encoding='utf-8') as file:
            for line_num, line in enumerate(file, 1):
                line = line.strip()
                if line:  # Skip empty lines
                    endpoints = parse_endpoints_from_line(line)
                    if endpoints:
                        line_segments.append(endpoints)
                    else:
                        print(f"Warning: Could not parse line {line_num} in {dat_path}")
                        
    except Exception as e:
        print(f"Error reading DAT file {dat_path}: {str(e)}")
        raise
    
    return line_segments

def write_ply_lines_file(line_segments, ply_path):
    """
    Write line segments to PLY file format
    
    Args:
        line_segments (list): List of line segments
        ply_path (str): Output PLY file path
    """
    try:
        # Collect all unique vertices
        vertices = []
        vertex_map = {}  # Map from (x,y,z) to vertex index
        edges = []
        
        vertex_index = 0
        
        for segment in line_segments:
            point1, point2 = segment
            
            # Add first point if not already in vertices
            if point1 not in vertex_map:
                vertices.append(point1)
                vertex_map[point1] = vertex_index
                vertex_index += 1
            
            # Add second point if not already in vertices
            if point2 not in vertex_map:
                vertices.append(point2)
                vertex_map[point2] = vertex_index
                vertex_index += 1
            
            # Add edge (line segment)
            idx1 = vertex_map[point1]
            idx2 = vertex_map[point2]
            edges.append((idx1, idx2))
        
        # Write PLY file
        with open(ply_path, 'w', encoding='utf-8') as file:
            # Write PLY header
            file.write("ply\n")
            file.write("format ascii 1.0\n")
            file.write("comment Converted from DAT line segments\n")
            file.write(f"element vertex {len(vertices)}\n")
            file.write("property float x\n")
            file.write("property float y\n")
            file.write("property float z\n")
            file.write(f"element edge {len(edges)}\n")
            file.write("property list uchar int vertex_indices\n")
            file.write("end_header\n")
            
            # Write vertex data
            for x, y, z in vertices:
                file.write(f"{x} {y} {z}\n")
            
            # Write edge data
            for idx1, idx2 in edges:
                file.write(f"2 {idx1} {idx2}\n")
                
    except Exception as e:
        print(f"Error writing PLY file {ply_path}: {str(e)}")
        raise

def convert_dat_to_ply(dat_path, ply_path):
    """
    Convert DAT file to PLY line format
    
    Args:
        dat_path (str): Input DAT file path
        ply_path (str): Output PLY file path
    """
    try:
        # Read DAT file
        line_segments = read_dat_file(dat_path)
        
        if not line_segments:
            print(f"Warning: No valid line segments found in {os.path.basename(dat_path)}")
            return
        
        # Write PLY file
        write_ply_lines_file(line_segments, ply_path)
        
        print(f"Successfully converted: {os.path.basename(dat_path)} -> {os.path.basename(ply_path)} ({len(line_segments)} line segments)")
        
    except Exception as e:
        print(f"Conversion failed {os.path.basename(dat_path)}: {str(e)}")

def main():
    """Main function"""
    # Input and output directories
    input_dir = "output"
    output_dir = "output"
    
    # Find all DAT files in the output directory
    dat_files = glob.glob(os.path.join(input_dir, "*.dat"))
    
    if not dat_files:
        print(f"No DAT files found in {input_dir} directory")
        return
    
    print(f"Found {len(dat_files)} DAT files, starting conversion...")
    
    # Convert each DAT file
    for dat_path in dat_files:
        # Generate output filename
        base_name = os.path.splitext(os.path.basename(dat_path))[0]
        ply_path = os.path.join(output_dir, f"{base_name}.ply")
        
        # Execute conversion
        convert_dat_to_ply(dat_path, ply_path)
    
    print("\nConversion completed!")
    print(f"Results saved in '{output_dir}' directory")

if __name__ == "__main__":
    main() 