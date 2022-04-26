# Нахождение центра, радиуса и диаметра дерева, заданного двоичным кодом
Алгоритм позволяет найти количество вершин, образующих центр, диаметр и радиус дерева, заданного **двоичным кодом**
## Постановка задачи
Исходные данные берутся из файла **[input.txt](https://github.com/alexvilno/calc_tree/blob/ver_1.0/cmake-build-debug/input.txt)**.  В первой строке записано одно число **n** &mdash; количество вершин в дереве. Во второй строке располагается двоичный код. Нужно записать **три числа** в файл **[output.txt](https://github.com/alexvilno/calc_tree/blob/ver_1.0/cmake-build-debug/output.txt)** количество вершин, образующих центр, радиус и диаметр.
## Пример решённой задачи
Для входных данных
```
4
001101
```
Корректным решением является:
```
2 2 3
```

## Краткий алгоритм
- Создаем список смежности из двоичного кода
- Считаем диаметр
- Считаем радиус
- Находим центровые вершины

## Создание списка смежности из двоичного кода
Для этого используем функцию
```cpp
std::vector<std::vector<int>> ReadTree(std::istream &in) {
  std::vector<std::vector<int>> tree(1);
  ReadTreeRec(in, tree, 0);
  return std::move(tree);
}
```
Она принимает в качестве аргумента входящий поток ```std::istream &in``` и создает список смежности на базе ```std::vector<std::vector<int>>``` первая его координата &mdash; рассматриваемая вершина, вторая &mdash вершина, которая ей смежна.
Например список смежности для дерева, заданного двоичным кодом ```0011``` будет выглядеть так:

![image](https://sun1-89.userapi.com/s/v1/ig2/6vg5WnpLNt93ctXxJUkw77DsPby0VhF6NKNYAf0WNTWO17T_47_Js4YrXWODDslN29qcS3jI0wC_ccJ3WZXsxphh.jpg?size=366x243&quality=96&type=album)

То есть по сути, наш **список смежности** выглядит так:

```tree[c][0] = v``` &mdash; вершина c смежна вершине v

Создается он следующим образом:

Вызывается **рекурсивная функция** 
```cpp
void ReadTreeRec(std::istream &in, std::vector<std::vector<int>> &tree, int v) {
  ...
}
```
Кроме входящего потока, она принимает, собственно, список смежности, который требуется заполнить и параметр ```int v``` &mdash; индекс вершины

Работает он так:
```cpp
void ReadTreeRec(std::istream &in, std::vector<std::vector<int>> &tree, int v) {
  for (char command; in >> command && command != '1';) {
    int u = tree.size();
    tree.emplace_back();  //create in end
    tree[v].push_back(u); //create and push
    tree[u].push_back(v);
    ReadTreeRec(in, tree, u);
  }
}
```
- Из входящего потока считывается **двоичный код**
- Если считываемый символ ```command != '1'``` тогда
  - создается переменная ```u``` равная **текущему размеру списка смежности** &mdash; ```tree.size()```
  - создается очередная вершина с индексом ```v```
  - в список смежности ```tree[v].push_back(u)``` кладется смежная с ней ```u```
  - для ```u``` делается то же самое:  ```tree[u].push_back(v)```
  - рекурсивно вызывается эта же функция, но уже от вершины с индексом ```u```

Довольно логично, что каждую инициализацию переменная ```u = tree.size()``` будет равна индексу очередной вершины в списке смежности

Когда мы получили список смежности, возвращаем его без копирования ```return std::move(tree)```

## Считаем диаметр

Теперь когда мы имеем список смежности для дерева, мы можем посчитать **диаметр** дерева.

Считать будем следующим образом:
- найдем самое **большое** расстояние от корня дерева до крайней вершины
- найдем самое **большое** расстояние от этой вершины до других вершин 

Функция, возвращающая **диаметр** дерева выглядит так:
```cpp
int CountDiameterLength(std::vector<std::vector<int>> &tree) {
  int vertices_count = tree.size();
  std::vector<int> distances(vertices_count);
  CountMaxDistanceRec(tree, distances, -1, 0);
  int new_root = FindVertexWithMaxDistance(distances);
  distances[new_root] = 0;
  CountMaxDistanceRec(tree, distances, -1, new_root);
  return distances[FindVertexWithMaxDistance(distances)];
}
```

В качестве аргумента она принимает ссылку на список смежности, созданный ранее

- ```int vertices_count = tree.size()``` запоминаем количество вершин и создаем массив такого размера ```std::vector<int> distances(vertices_count)```
- заполним этот массив **расстояниями** от корня дерева с помощью функции ```void CountMaxDistanceRec(const std::vector<std::vector<int>> &tree, std::vector<int> &distances, int p, int v)``` &mdash; она будет описана ниже
- от нового "нового корня" (т.е. от той вершины, расстояние до которой будет максимально) также запишем новые **расстояния**
- вернем максимальное расстояние, находящееся в массиве **расстояний** по индексу, возвращаемому функцией ```FindVertexWithMaxDistance(distances)``` (реализациия её тривиальна, описываться не будет)
 
Как работает ```void CountMaxDistanceRec(const std::vector<std::vector<int>> &tree, std::vector<int> &distances, int p, int v)``` ? 
 
Она принимает ссылку на список смежности, ссылку на массив **расстояний** индекс родительского элемента (для корня это -1) и индекс элемента, от которого считаются расстояния
- Проходимся по всем вершинам, кроме родительской
- Считаем рекурсивно расстояния до всех других вершин

То есть грубо говоря, мы прошлись **DFS** два раза

## Считаем радиус
Делаем это по формуле ```radius = (diameter + 1) / 2```  

## Считаем количество центров
По формуле ```center = (diameter) % 2 + 1```

## Тесты коррекности

Используем сайт [graphonline](https://graphonline.ru/)


**Тест 1**

![image](https://graphonline.ru/tmp/saved/EG/EGBFwxsuWacnWvBw.png) 

Входные данные: 
```
13
00011100011011000111
```
Выходные данные:
```
1 3 6
```
![image](https://graphonline.ru/tmp/saved/ot/otvXsnevaLOGGkgd.png)

**Тест 2**

![image](https://graphonline.ru/tmp/saved/eP/ePxAgEEbaPHnLYDs.png) 

Входные данные: 
```
7
000101101011
```
Выходные данные:
```
2 2 3
```
![image](https://graphonline.ru/tmp/saved/qZ/qZefgaLICjVOHyvM.png)

**Тест 3**

![image](https://graphonline.ru/tmp/saved/zm/zmSWwbLsykBCMdDw.png) 

Входные данные: 
```
12
0001010110110001011011
```
Выходные данные:
```
1 3 6
```
![image](https://graphonline.ru/tmp/saved/me/mezUQkZgaCGLutsw.png)

**Тест 4**

![image](https://graphonline.ru/tmp/saved/rR/rRigwOjWuvSfZwNB.png) 

Входные данные: 
```
10
001000101101100111
```
Выходные данные:
```
2 3 5
```
![image](https://graphonline.ru/tmp/saved/xw/xweeEnWbHODjkLtb.png)

**Тест 5**

![image](https://graphonline.ru/tmp/saved/cd/cdjewjYXQJBYNatV.png) 

Входные данные: 
```
5
00001111
```
Выходные данные:
```
1 2 4
```
![image](https://graphonline.ru/tmp/saved/qh/qhAaVsilvoFQJnKL.png)

## Тесты производительности

Для тестов производительности есть функция, возвращающая случайное дерево из n вершин в виде **двоичного кода**
Вот ее описание:
```cpp
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
```

| Количество | Время     |
|          -:|         -:|
|        100 |  0.003927 |
|        200 |  0.015992 |
|        300 |  0.034656 |
|        400 |  0.063029 |
|        500 |  0.099495 |
|        600 |  0.146084 |
|        700 |  0.200686 |
|        800 |  0.267820 |
|        900 |  0.342634 |
|       1000 |  0.419897 |
|       2000 |  1.805504 |
|       3000 |  4.409858 |
|       4000 |  7.823605 |
|       5000 | 12.455837 |
|       6000 | 18.458873 |
|       7000 | 25.120442 |
|       8000 | 33.252308 |
|       9000 | 42.337381 |
|      10000 | 52.894956 |
