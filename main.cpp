#include <fstream>
#include <iostream>
#include <vector>

void ReadTreeRec(std::istream &in, std::vector<std::vector<int>> &tree, int v) {
  for (char command; in >> command && command != '1';) {
    int u = tree.size();
    tree.emplace_back();  //create in end
    tree[v].push_back(u); //create and push
    tree[u].push_back(v);
    ReadTreeRec(in, tree, u);
  }
}

std::vector<std::vector<int>> ReadTree(std::istream &in) {
  std::vector<std::vector<int>> tree(1);
  ReadTreeRec(in, tree, 0);
  return std::move(tree);
}

void CountMaxDistanceRec(const std::vector<std::vector<int>> &tree, std::vector<int> &distances, int p, int v) {
  for (auto u : tree[v]) {
    if (u != p) {
      distances[u] = distances[v] + 1;
      CountMaxDistanceRec(tree, distances, v, u);
    }
  }
}

int FindVertexWithMaxDistance(const std::vector<int> &distances) {
  int u = 0;
  for (int v = 0; v < distances.size(); v++) {
    if (distances[v] > distances[u]) {
      u = v;
    }
  }
  return u;
}

int CountDiameterLength(std::vector<std::vector<int>> &tree) {
  int vertices_count = tree.size();
  std::vector<int> distances(vertices_count);
  CountMaxDistanceRec(tree, distances, -1, 0);//potom
  int new_root = FindVertexWithMaxDistance(distances);
  distances[new_root] = 0;
  CountMaxDistanceRec(tree, distances, -1, new_root);
  return distances[FindVertexWithMaxDistance(distances)];
}

int main() {
  std::ifstream fin("input.txt");
  int n;
  fin >> n;
  auto tree = ReadTree(fin);
  fin.close();
  int diameter = CountDiameterLength(tree);
  int radius = (diameter + 1) / 2;
  int center = (diameter) % 2 + 1;
  std::ofstream fout("output.txt");
  fout << center << ' ' << radius << ' ' << diameter << std::endl;
  fout.close();
  return 0;
}
