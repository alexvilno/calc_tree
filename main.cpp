#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <random>

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

std::string tree_generator(int v_count) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(0, 1);

  std::string generated = "";
  int count_0 = 0;
  int count_1 = 0;

  for (int i = 0; i < v_count * 2; ++i) {
    int x = distribution(generator);

    if (count_0 >= v_count) {
      x = 1;
      generated += x + 48;
      continue;
    }
    if (x == 1 && count_1 < count_0) {
      generated += x + 48;
      ++count_1;
    } else if (x == 1 && count_1 >= count_0) {
      x = 0;
      generated += x + 48;
      ++count_0;
    } else {
      generated += x + 48;
      x == 1 ? ++count_1 : ++count_0;
    }
  }

  return generated;
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
