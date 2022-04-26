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
