import numpy as np
import open3d as o3d
import logging

def obj_to_ply(obj_file, ply_file):
    """convert obj file containing bspline curves to ply file with curve endpoints and edges
    
    args:
        obj_file: str, input obj file path
        ply_file: str, output ply file path
    """
    try:
        # read obj file
        vertices = []
        curve_edges = []
        
        with open(obj_file, 'r') as f:
            for line in f:
                if line.startswith('v '):  # vertex
                    # parse vertex coordinates
                    coords = line[2:].strip().split()
                    vertex = [float(x) for x in coords]
                    vertices.append(vertex)
                elif line.startswith('curv '):  # curve definition
                    # parse curve indices
                    parts = line[5:].strip().split()
                    # In the obj file, the format of the curv line is:
                    # curv [start_param] [end_param] [vertex_index1] [vertex_index2]
                    # so the vertex indices are in the last two elements of parts
                    if len(parts) >= 4:  # ensure we have enough parts
                        vertex_indices = [int(x) - 1 for x in parts[-2:]]  # obj indices start from 1
                        curve_edges.append(vertex_indices)
        
        unique_vertices = []
        unique_indices = {}
        edges = []
        
        # create a list of unique vertices and a mapping of vertex indices
        for edge in curve_edges:
            start_idx = edge[0]
            end_idx = edge[1]
            
            # ensure the indices are within the valid range
            if start_idx < 0 or start_idx >= len(vertices) or end_idx < 0 or end_idx >= len(vertices):
                logging.warning(f"skip invalid edge: {edge}, vertex indices out of range")
                continue
                
            start_point = vertices[start_idx]
            end_point = vertices[end_idx]
            
            # check if the start point already exists
            start_point_tuple = tuple(start_point)
            if start_point_tuple not in unique_indices:
                unique_indices[start_point_tuple] = len(unique_vertices)
                unique_vertices.append(start_point)
            
            # check if the end point already exists
            end_point_tuple = tuple(end_point)
            if end_point_tuple not in unique_indices:
                unique_indices[end_point_tuple] = len(unique_vertices)
                unique_vertices.append(end_point)
            
            # add edge, using new vertex indices
            edges.append([unique_indices[start_point_tuple], unique_indices[end_point_tuple]])
        
        # save as ASCII ply file
        with open(ply_file, 'w') as f:
            # write header
            f.write("ply\n")
            f.write("format ascii 1.0\n")
            f.write(f"element vertex {len(unique_vertices)}\n")
            f.write("property float x\n")
            f.write("property float y\n")
            f.write("property float z\n")
            f.write(f"element edge {len(edges)}\n")
            f.write("property list uchar int vertex_indices\n")
            f.write("end_header\n")
            
            # write vertices
            for vertex in unique_vertices:
                f.write(f"{vertex[0]} {vertex[1]} {vertex[2]}\n")
            
            # write edges
            for edge in edges:
                f.write(f"2 {edge[0]} {edge[1]}\n")
        
        logging.info(f"convert {obj_file} to {ply_file}")
        logging.info(f"  original vertex count: {len(vertices)}")
        logging.info(f"  unique vertex count: {len(unique_vertices)}")
        logging.info(f"  edge count: {len(edges)}")
        
    except Exception as e:
        logging.error(f"convert obj to ply failed: {str(e)}")
        raise

if __name__ == "__main__":
    # set up logging
    logging.basicConfig(level=logging.INFO,
                       format='%(asctime)s - %(levelname)s - %(message)s')
    
    # example usage
    obj_file = "./pointclouds/pylon9_lines.obj"
    ply_file = "./groundtruth/pylon9_lines.ply"
    obj_to_ply(obj_file, ply_file)
