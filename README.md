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
- - создается переменная ```u``` равная **размеру списка смежности**
