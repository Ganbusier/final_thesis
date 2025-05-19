#include <gco/GCoptimization.h>
#include <CGAL/Bbox_3.h>
#include <easy3d/viewer/viewer.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <energyMinimization/energyMinimization.h>
#include <graph/graph.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file_path>" << std::endl;
        return -1;
    }

    std::string inputFilePath = argv[1];

    auto pointCloud = easy3d::PointCloudIO::load(inputFilePath);

    graph::Graph graph(pointCloud, 10, 2.0f);
    graph.buildMixGraph();
    auto mixGraph = graph.getMixGraph();
    graph.buildDualGraph(mixGraph);
    auto dualGraph = graph.getDualGraph();

    energyMinimization::EnergyMinimization energyMinimization(
        dualGraph->n_vertices(), dualGraph->n_edges(), mixGraph, pointCloud);
    energyMinimization.setDataTerm();
    energyMinimization.setSmoothnessTerm();
    energyMinimization.optimize();
    energyMinimization.getResults();
    energyMinimization.saveResults(inputFilePath);

    return 0;
}