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

  // Analyze the two graphs
  myAnalysis::Analysis analysis(estimatedGraph, groundTruthGraph);
  analysis.analyze();

  // Log the results
  myAnalysis::AnalysisResults results = analysis.getAnalysisResults();
  LOG(INFO) << "Min distance: " << results.minDistance;
  LOG(INFO) << "Max distance: " << results.maxDistance;
  LOG(INFO) << "Min angle: " << results.minAngle;
  LOG(INFO) << "Max angle: " << results.maxAngle;
  LOG(INFO) << "Mean distance: " << results.meanDistance;
  LOG(INFO) << "Mean angle: " << results.meanAngle;
  LOG(INFO) << "Median distance: " << results.medianDistance;
  LOG(INFO) << "Median angle: " << results.medianAngle;
  LOG(INFO) << "Std distance: " << results.stdDistance;
  LOG(INFO) << "Std angle: " << results.stdAngle;
  LOG(INFO) << "RMSE of mean distances: " << results.RMSEofMeanDistances;
  LOG(INFO) << "RMSE of mean angles: " << results.RMSEofMeanAngles;
  LOG(INFO) << "Unmatched estimated edges count: " << results.unmatchedEstimatedEdgesCount;
  LOG(INFO) << "Unmatched ground truth edges count: " << results.unmatchedGroundTruthEdgesCount;

  return 0;
}
