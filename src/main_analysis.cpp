#include <easy3d/core/graph.h>
#include <easy3d/fileio/graph_io.h>

#include "analysis.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    LOG(ERROR) << "Usage: " << argv[0]
               << " <estimated_ply_file> <ground_truth_ply_file>";
    return 1;
  }

  std::string estimatedFile = argv[1];
  std::string groundTruthFile = argv[2];

  // Load estimated file and ground truth file as graphs
  easy3d::Graph* estimatedGraph = easy3d::GraphIO::load(estimatedFile);
  easy3d::Graph* groundTruthGraph = easy3d::GraphIO::load(groundTruthFile);

  // TODO: Compare the two graphs
  // TODO: Log the results

  return 0;
}
